#ifndef _HTTP_DOWNLOADER_H_
#define _HTTP_DOWNLOADER_H_
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "LUrlParser/LUrlParser.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using namespace std;

struct hd_config{
    string url;
    string output_path;
    bool dump_header;
};

class http_downloader
{
private:
    /* data */
    hd_config config_;
    LUrlParser::ParseURL url_;
    beast::tcp_stream stream_;
    boost::asio::ip::tcp::resolver resolver_;
    beast::flat_buffer buffer_;
private:
    void send_request();
    void recive_header();
    template<class Body, class Allocator>
    void recive_body(std::shared_ptr<http::response_parser<Body, Allocator>> rsp_praser);
    void connect_to_remote(tcp::resolver::results_type& results);
    void fail(beast::error_code ec, char const* what){
        cerr << what << ": " << ec.message() << "\n";
    }
public:
    http_downloader(net::io_context& ioc, const hd_config& config):
        config_(config),url_(LUrlParser::ParseURL::parseURL(config_.url)),stream_(ioc),resolver_(ioc){}
    ~http_downloader(){}

    void go();
};


#endif