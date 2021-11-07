#ifndef _HTTP_FILE_SERVICE_
#define _HTTP_FILE_SERVICE_
#include <string>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using namespace std;


void fail(beast::error_code ec, char const* what){
    cerr <<  what << ": " << ec.message() << "\n";
}

// Handles an HTTP server connection
class session : public enable_shared_from_this<session>
{
private:
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    shared_ptr<string const> doc_root_;
    shared_ptr<http::request_parser<http::empty_body>> hdr_parser_ptr_;

public:
    // Take ownership of the stream
    session(tcp::socket&& socket, shared_ptr<string const> const& doc_root)
        : stream_(move(socket)), doc_root_(doc_root){}

    void run();
    void do_read(shared_ptr<http::request_parser<http::empty_body>> parser_ptr);
    void do_read(shared_ptr<http::request_parser<http::file_body>> parser_ptr);
    void do_read(shared_ptr<http::request_parser<http::string_body>> parser_ptr);


    template<bool isRequest, class Body, class Fields>
    void do_write(http::message<isRequest, Body, Fields>&& msg);
    void handle_request(shared_ptr<http::request_parser<http::empty_body>> parser_ptr);
    void handle_request(shared_ptr<http::request_parser<http::file_body>> parser_ptr);
    void handle_request(shared_ptr<http::request_parser<http::string_body>> parser_ptr);
    void do_close();
};


class listener : public enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    shared_ptr<string const> doc_root_;

public:
    listener(net::io_context& ioc, tcp::endpoint endpoint, shared_ptr<string const> const& doc_root)
    : ioc_(ioc),acceptor_(net::make_strand(ioc)),doc_root_(doc_root)
    {
        beast::error_code ec;
        // Open the acceptor
        acceptor_.open(endpoint.protocol());

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true));

        // Bind to the server address
        acceptor_.bind(endpoint);

        // Start listening for connections
        acceptor_.listen(net::socket_base::max_listen_connections);
    }
    // Start accepting incoming connections
    void run(){
        do_accept();
    }
private:
    void do_accept(){
        // The new connection gets its own strand
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }
    void on_accept(beast::error_code ec, tcp::socket socket){
        if(ec){
            fail(ec, "accept");
        }else{
            // Create the session and run it
            make_shared<session>(move(socket),doc_root_)->run();
        }

        // Accept another connection
        do_accept();
    }
};




#endif