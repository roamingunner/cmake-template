#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <json/json.h>
#include <functional>
#include "drink_shop_receptionist.hpp"
#include "drink_shop_protocol.hpp"
#include "drink_shop_transaction.hpp"
#include "drink_shop_config.hpp"

void session::read_header(){
    auto self(shared_from_this());
    memset(recv_buffer,0,INBUF_SIZE);
    drink_shop_protocol hdr;
    async_read(socket_,boost::asio::buffer(recv_buffer,hdr.header_size()),boost::asio::transfer_all(),
    [this,self](boost::system::error_code ec, size_t length){
        if (ec){
            LOG(ERROR) << "session::read header failed:" << ec.message() << endl;
            pkg_handle_(rid_, ec.value(),nullptr,0);
        }else{
            drink_shop_protocol h;
            if (h.parse_from_buffer(recv_buffer)){
                LOG(ERROR) << "session::read header parse failed:" << endl;
                pkg_handle_(rid_, -EINVAL,nullptr,0);
            }else if(h.body_length()){
                if (h.body_length() < (INBUF_SIZE - h.header_size())){
                    read_body(recv_buffer + h.header_size(),h.body_length());
                }else{
                    LOG(ERROR) << "session::body large than rest buffer\n" << endl;
                    pkg_handle_(rid_, -ENOMEM,nullptr,0);
                }
            }else{
                pkg_handle_(rid_,0,recv_buffer,h.header_size());
                read_header();
            }
        }
    }
    );
}

void session::read_body(char *ptr, const size_t length){
    auto self(shared_from_this());
    async_read(socket_,boost::asio::buffer(ptr,length),boost::asio::transfer_all(),
    [this,self,ptr](boost::system::error_code ec, size_t length){
        if (ec){
            LOG(ERROR) << "session::read body failed:" << ec.message() << endl;
            pkg_handle_(rid_, ec.value(),nullptr,0);
        }else{
            size_t total_length = (ptr - recv_buffer) + length;
            pkg_handle_(rid_, 0,recv_buffer,total_length);
            read_header();
        }
    }
    );
}

int session::send_package(char *buf, size_t length){
    auto self(shared_from_this());
    LOG(DEBUG) << "session send_package\n";
    async_write(socket_, boost::asio::buffer(buf, length),
    [self,buf](boost::system::error_code ec, size_t length){
        if (ec){
            LOG(ERROR) << "send package failed:" << ec.message() << endl;
        }
        LOG(DEBUG) << "session send_package complete\n";
        delete []buf;
    }
    );
}

int drink_shop_receptionist::add_session(ip::tcp::socket socket){
    mtx_.lock();
    uint32_t rid = rid_iterator_++;
    try
    {
        sessions_[rid] = make_shared<session>(rid,std::move(socket),std::bind(&drink_shop_receptionist::on_pacakge,this,
            placeholders::_1,
            placeholders::_2,
            placeholders::_3,
            placeholders::_4));
        sessions_[rid]->start();
        LOG(NOTICE) << "add session:" << rid << endl;
    }
    catch(const exception& e)
    {
        LOG(ERROR) << e.what() << '\n';
    }
    mtx_.unlock();
}

void drink_shop_receptionist::del_session(uint32_t rid){
    LOG(NOTICE) << "del session:" << rid << endl;
    sessions_.erase(rid);
}

void drink_shop_receptionist::do_accept(){
    LOG(NOTICE) << "drink_shop_receptionist accept\n";
    acceptor_.async_accept(
        [this](boost::system::error_code ec, ip::tcp::socket socket){
            if (!ec){
                add_session(std::move(socket));
                do_accept();
            }else{
                LOG(ERROR) << "accept failed:" << ec.message() << endl;
            }
        }
    );
}

void drink_shop_receptionist::on_pacakge(uint32_t rid, int32_t result, char *buffer, size_t length){
    //remove unable session
    if (result){
        return del_session(rid);
    }
    LOG(DEBUG) << "drink_shop_receptionist::on_pacakge\n";
    drink_shop_protocol h;
    h.parse_from_buffer(buffer);
    switch (h.command())
    {
        case DRINK_SHOP_PROTO_CMD_VERSION:
        {
            /* code */
            LOG(DEBUG) << "command:DRINK_SHOP_PROTO_CMD_VERSION\n";

            Json::Value obj;
            configuration *conf = configuration::get_instance();
            obj["version"] = conf->version();
            string obj_str = obj.toStyledString();
            drink_shop_protocol rh(h.command(),0,obj_str.length(),obj_str.data());
            char *buffer = new char[rh.package_size()];
            if (!buffer){
                LOG(ERROR) << "alloc replay memory failed\n";
            }else{
                rh.serialize_pkg_to_buffer(buffer);
                sessions_[rid]->send_package(buffer,rh.package_size());
            }
            break;
        }

        case DRINK_SHOP_PROTO_CMD_ORDER:
        {
            LOG(DEBUG) << "command:DRINK_SHOP_PROTO_CMD_ORDER\n";
            Json::Value obj;
            Json::CharReaderBuilder builder;
            const unique_ptr<Json::CharReader> reader(builder.newCharReader());
            JSONCPP_STRING err;
            transacation *t = nullptr;
            if (false == reader->parse(h.body(),h.body() + h.body_length(),&obj,&err)){
                LOG(ERROR) << "parse body failed\n";
                drink_shop_protocol rh(h.command(),-EINVAL,0,nullptr);
                char *buffer = new char[rh.package_size()];
                if (!buffer){
                    LOG(ERROR) << "alloc replay memory failed\n";
                }else{
                    memset(buffer,0,h.package_size());
                    rh.serialize_pkg_to_buffer(buffer);
                    sessions_[rid]->send_package(buffer,rh.package_size());
                }
            }else{
                try
                {
                    t = new transacation(rid,obj["type"].asString(),obj["size"].asString(),obj["packed"].asBool());
                    t->add_stamp("receptionist");
                    entry_->handle(t);
                }
                catch(const exception& e)
                {
                    LOG(ERROR) << "transacation create failed:" << e.what() << '\n';
                }
            }
            break;
        }
        default:
            {
                /* code */
                LOG(DEBUG) << "command:others\n";
                drink_shop_protocol rh(rh.command(),-EINVAL,0,nullptr);
                char *buffer = new char[rh.package_size()];
                if (!buffer){
                    LOG(ERROR) << "alloc replay memory failed\n";
                }else{
                    rh.serialize_pkg_to_buffer(buffer);
                    sessions_[rid]->send_package(buffer,rh.package_size());
                }
                break;
            }
    }
}

void drink_shop_receptionist::handle(transacation* t) {
    LOG(DEBUG) << "drink_shop_receptionist handle\n"; 
    Json::Value obj;
    obj["type"] = t->type();
    obj["size"] = t->size();
    obj["packed"] = t->is_packed();
    vector<string>& stamps = t->stamps();
    obj["stamps"] = Json::arrayValue;

    for (int i = 0; i < stamps.size(); i++){
        obj["stamps"].append(stamps[i]);
    }

    string obj_str = obj.toStyledString();
    drink_shop_protocol h(DRINK_SHOP_PROTO_CMD_ORDER,0,obj_str.length(),obj_str.data());
    
    char *send_buffer = new char[h.package_size()];
    if (!send_buffer){
        LOG(ERROR) << "receptionist alloc buffer failed! alloc size " << h.package_size() << endl;
    }else{
        memset(send_buffer,0,h.package_size());
        h.serialize_pkg_to_buffer(send_buffer);
        sessions_[t->rid()]->send_package(send_buffer,h.package_size());
    }
    sessions_.erase(t->rid());
    return abstract_handler::handle(t);
}