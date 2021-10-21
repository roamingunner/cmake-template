#include "http_uploader.hpp"
#include <argp.h>
#include <ctime>
#include <memory>
#include <stdlib.h>


void http_uploader::async_go(const string& url, const string& uploadfile, http_upload_callback cb) {
    LUrlParser::ParseURL url_parse = LUrlParser::ParseURL::parseURL(url);

    if (!url_parse.isValid()) {
        cerr << "url is invalid!\n";
        return ;
    }

    //url_parse miss "/" header
    if (url_parse.path_.empty()){
        url_parse.path_ = "/";
    }else{
        url_parse.path_.insert(0,"/");
    }

    if (url_parse.port_.empty()) {
        if (!url_parse.scheme_.compare("https")) {
            url_parse.port_ = "433";
        } else if (!url_parse.scheme_.compare("http")) {
            url_parse.port_ = "80";
        }
    }

    uploadfile_ = uploadfile;
    ul_url_ = url;
    ul_host_ = url_parse.host_;
    ul_target_ = url_parse.path_;
    cb_ = cb;

    resolver_.async_resolve(
        url_parse.host_, url_parse.port_,
        [this](const boost::system::error_code &ec, tcp::resolver::results_type results) {
            if (ec) {
                fail(ec, "resolve failed");
                if (cb_){
                    cb_(ec,ul_url_);
                }
            } else {
                connect_to_remote(results);
            }
        });
}

void http_uploader::connect_to_remote(tcp::resolver::results_type &results) {
    cout << "Connecting ...\n";
    stream_.async_connect(results, 
    [this](const boost::system::error_code &ec, tcp::resolver::results_type::endpoint_type) {
        if (ec) {
            fail(ec, "connect to remote failed");
            if (cb_){
                cb_(ec,ul_url_);
            }
        } else {
            send_request();
        }
    });
}

void http_uploader::send_request() {
    // Send HTTP(S) request using beast
    auto req = make_shared<http::request<http::file_body>>();
    req->version(11);
    req->method(http::verb::post);
    req->target(ul_target_);
    req->set(http::field::host, ul_host_);
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    
    beast::error_code ec;
    req->body().open(uploadfile_.c_str(), beast::file_mode::read, ec);
    if (ec){
        fail(ec,"open file");
        return ;
    }
    req->prepare_payload();
    cout << "send request ...\n";
    http::async_write(stream_, *req,
                      [this,req](const beast::error_code &ec, std::size_t) {
                        cout << "send request cb...\n";
                        if (ec) {
                            fail(ec, "send request fail");
                            if (cb_){
                                cb_(ec,ul_url_);
                            }
                        } else {
                            recive_respone();
                        }
                      });
}

void http_uploader::recive_respone(){
    shared_ptr<http::request<http::string_body>> rsp = 
        make_shared<http::request<http::string_body>>();
    http::async_read(stream_, buffer_, *rsp,
    [this, rsp](const beast::error_code &ec, std::size_t bytes_transferred){
        boost::ignore_unused(bytes_transferred);
        if (ec) {
            fail(ec, "recive respone failed");
            if (cb_){
                cb_(ec,ul_url_);
            }
        }

    });
}

//################### test code



struct hd_config{
    char *args[2];
};



const char *argp_program_version = "http_uploader 1.0";
const char *argp_program_bug_address = "leo.qin <103922926@qq.com>";

/* Program documentation. */
static char doc[] = "http_uploader -- a sample http uploader";

/* A description of the arguments we accept. */
static char args_doc[] = "URL file [option]";

/* The options we understand. */
static struct argp_option options[] = {{"verbose", 'v', 0, 0, "Produce verbose output"},
                                       {0}};

/* struct argp_option定义
struct argp_option
{
const char *name; option名称，用作long_option
int key;  option的key，用作short_option
const char *arg;
如果该option需要跟参数，那么可通过arg字段给参数定一个名称。可以在usage显示出来，同时在parser里才能通过arg拿到option参数
int flags; 一般不写
const char *doc; 参数说明
int group; 参数分组，一般不写
};
*/

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    /* Get the input argument from argp_parse, which we
        know is a pointer to our arguments structure. */
    struct hd_config *arguments = static_cast<hd_config *>(state->input);

    switch (key) {
    case 'v':
        argp_usage(state);
        break;
    // ARGP定义了一些特殊的key
    // ARGP_KEY_ARG用于接收没有option的参数
    // ARGP_KEY_END在参数粘贴完成后会调用
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
            /* Too many arguments. */
            argp_usage(state);
        arguments->args[state->arg_num] = arg;
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 1)
            /* Not enough arguments. */
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

void ul_callback(const boost::system::error_code ec, const string& url){
    if (ec){
        cout << "upload " << url << " failed:" << ec.message() << endl;
    }else{
        cout << "upload " << url << " finished\n";
    }
}

int main(int argc, char **argv) {
    /* code */
    struct hd_config conf;
    
    argp_parse (&argp, argc, argv, 0, 0, &conf);

    net::io_context ioc;
    http_uploader ul(ioc);
    
    ul.async_go(conf.args[0],conf.args[1],ul_callback);

    ioc.run();
    
    return 0;
}
