#ifndef _HTTP_UPLOADER_H_
#define _HTTP_UPLOADER_H_
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "LUrlParser/LUrlParser.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using namespace std;

typedef void (*http_upload_callback)(const boost::system::error_code ec, const string& url);

class http_uploader
{
private:
    /* data */
    beast::tcp_stream stream_;
    boost::asio::ip::tcp::resolver resolver_;
    beast::flat_buffer buffer_;
    string ul_url_;
    string ul_host_;
    string uploadfile_;
    string ul_target_;
    http_upload_callback cb_;

private:
    void send_request();
    void recive_header();
    void recive_respone();
    template<class Body, class Allocator>
    void recive_body(std::shared_ptr<http::response_parser<Body, Allocator>> rsp_praser);
    void connect_to_remote( tcp::resolver::results_type &results);
    void fail(beast::error_code ec, char const* what){
        cerr << what << ": " << ec.message() << "\n";
    }
public:
    http_uploader(net::io_context& ioc):stream_(ioc),resolver_(ioc){}
    ~http_uploader(){}
    void async_go(const string& url, const string& uploadfile, http_upload_callback cb = nullptr);
};


#endif