#ifndef _DRINK_SHOP_RECEPTIONIST_H_
#define _DRINK_SHOP_RECEPTIONIST_H_
#include <string>
#include <memory>
#include <mutex>
#include <functional>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/socket_base.hpp>
#include "drink_shop_transaction.hpp"
#include "drink_shop_handler.hpp"
#include "aixlog.hpp"
using namespace std;
using namespace boost::asio;

typedef function<void (uint32_t rid, int32_t result, char *buffer, size_t length)> package_callback;
class session : public enable_shared_from_this<session>
{
private:
    /* data */
    enum {INBUF_SIZE = 4096};
    uint32_t rid_;
    ip::tcp::socket socket_;
    package_callback pkg_handle_;
    char recv_buffer[INBUF_SIZE];
private:
    void read_header();
    void read_body(char *ptr, const size_t body_length);
public:
    session(uint32_t rid, ip::tcp::socket sock, package_callback h):rid_(rid),socket_(std::move(sock)),pkg_handle_(h){
        LOG(DEBUG) << "session " << rid_ << " construct\n";
    }
    ~session() {
        LOG(DEBUG) << "session " << rid_ << " destory\n";
    }
    void start(){
        read_header();
    }
    int send_package(char *buffer, size_t length);

};
class drink_shop_receptionist :public abstract_handler
{
private:
    /* data */
    uint32_t rid_iterator_;
    ip::tcp::acceptor acceptor_;
    handler *entry_;
    mutex mtx_;
    map<uint32_t,shared_ptr<session>> sessions_;
private:
    void on_pacakge(uint32_t rid, int32_t result, char *buffer, size_t length);
    int add_session(ip::tcp::socket sock);
    void del_session(uint32_t rid);
public:
    drink_shop_receptionist(io_context& ioc, ip::tcp::endpoint& ep)
        :abstract_handler(RECEPTIONIST_HANDLER_ID),rid_iterator_(0),acceptor_(ioc,ep),entry_(nullptr){}
    ~drink_shop_receptionist() {}
    void do_accept();
    void handle(transacation* t) override;
    void set_entry_handle(handler * h){
        entry_ = h;
    }
};

#endif