#include "http_file_service.hpp"

// Return a reasonable mime type based on the extension of a file.
beast::string_view mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/octet-stream";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
string path_cat(beast::string_view base, beast::string_view path){
    if(base.empty())
        return string(path);
    string result(base);

    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}


void session::handle_request(shared_ptr<http::request_parser<http::empty_body>> parser_ptr){
    cout << "handle_request empty body parser\n";
    // Returns a bad request response
    http::request<http::empty_body>& req = parser_ptr->get();
    auto const bad_request =
    [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Returns a forbidden error response
    auto const forbidden_error =
    [&req](beast::string_view what)
    {
        http::response<http::string_body> res{http::status::forbidden, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return do_write(bad_request("Illegal request-target"));

    // Build the path to the requested file
    std::string path = path_cat(*doc_root_, req.target());
    if(req.target().back() == '/')
        path.append("index.html");

    // Make sure we can handle the method
    if( req.method() == http::verb::head ||
        req.method() == http::verb::get){
        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);
        // Handle the case where the file doesn't exist
        if(ec == beast::errc::no_such_file_or_directory)
            return do_write(not_found(req.target()));

        // Handle an unknown error
        if(ec)
            return do_write(server_error(ec.message()));

        auto const size = body.size();

        if (req.method() == http::verb::head){
            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            return do_write(std::move(res));
        }else{
            // Respond to GET request
            http::response<http::file_body> res{
                std::piecewise_construct,
                std::make_tuple(std::move(body)),
                std::make_tuple(http::status::ok, req.version())};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(path));
            res.content_length(size);
            res.prepare_payload();
            res.keep_alive(req.keep_alive());
            return do_write(std::move(res));
        }

    }else if (req.method() == http::verb::post){
        cout << "post req.path=[" << path << "]\n";
        if (!req.target().compare("/echo")){
            shared_ptr<http::request_parser<http::string_body>> new_parser_ptr 
                = make_shared<http::request_parser<http::string_body>>(std::move(*parser_ptr));
            return do_read(new_parser_ptr);
        }else if(!req.target().compare("/upload")){
            // Attempt to open the file
            beast::error_code ec;
            http::file_body::value_type body;
            body.open(path.c_str(), beast::file_mode::write_new, ec);
            // Handle an unknown error
            if(ec){
                return do_write(forbidden_error(ec.message()));
            }else{
                shared_ptr<http::request_parser<http::file_body>> new_parser_ptr 
                    = make_shared<http::request_parser<http::file_body>>(std::move(*parser_ptr));
                return do_read(new_parser_ptr);
            }
        }else{
            return do_write(bad_request("Illegal post request target"));
        }

    }else{
        return do_write(bad_request("Unknown HTTP-method"));
    }
}

void session::handle_request(shared_ptr<http::request_parser<http::file_body>> parser_ptr){
    cout << "handle_request file body parser\n";

    // Returns a bad request response
    http::request<http::file_body> req = parser_ptr->release();
    http::response<http::empty_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/octet-stream");
    res.content_length(req.body().size());
    res.keep_alive(req.keep_alive());
    return do_write(std::move(res));
}

void session::handle_request(shared_ptr<http::request_parser<http::string_body>> parser_ptr){
    cout << "handle_request string body parser\n";

    // Returns a bad request response
    http::request<http::string_body> req = parser_ptr->release();
    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req.keep_alive());
    res.body() = req.body();
    res.prepare_payload();
    return do_write(std::move(res));
}

void session::do_read(shared_ptr<http::request_parser<http::file_body>> parser_ptr){
    cout << "read file body\n";
    auto self = shared_from_this();
     http::async_read(stream_, buffer_, *parser_ptr,
        [self,parser_ptr](const beast::error_code &ec, size_t bytes_transferred){
            boost::ignore_unused(bytes_transferred);
            // This means they closed the connection
            if(ec == http::error::end_of_stream)
                return self->do_close();

            if(ec)
                return fail(ec, "read");

            return self->handle_request(parser_ptr);
        }
     );
}

void session::do_read(shared_ptr<http::request_parser<http::string_body>> parser_ptr){
    cout << "read string body\n";
    auto self = shared_from_this();
     http::async_read(stream_, buffer_, *parser_ptr,
        [self, parser_ptr](const beast::error_code &ec, size_t bytes_transferred){
            boost::ignore_unused(bytes_transferred);
            // This means they closed the connection
            if(ec == http::error::end_of_stream)
                return self->do_close();

            if(ec)
                return fail(ec, "read");

            return self->handle_request(parser_ptr);
        }
     );
}


void session::do_read(shared_ptr<http::request_parser<http::empty_body>> parser_ptr){
    cout << "read empty body\n";
    auto self = shared_from_this();
    http::async_read_header(
        stream_, buffer_, *parser_ptr,
        [self, parser_ptr](const beast::error_code &ec, size_t bytes_transferred){
            boost::ignore_unused(bytes_transferred);
            // This means they closed the connection
            if(ec == http::error::end_of_stream)
                return self->do_close();

            if(ec)
                return fail(ec, "read");

            self->handle_request(parser_ptr);
        });
}

void session::run(){
    hdr_parser_ptr_ = make_shared<http::request_parser<http::empty_body>>();
    do_read(hdr_parser_ptr_);

}

template<bool isRequest, class Body, class Fields>
void session::do_write(http::message<isRequest, Body, Fields>&& msg){
        auto self = shared_from_this();

        // The lifetime of the message has to extend
        // for the duration of the async operation so
        // we use a shared_ptr to manage it.
        auto res = std::make_shared<
            http::message<isRequest, Body, Fields>>(std::move(msg));

        bool close = res->need_eof();
        auto l = [self, close, res, this](beast::error_code ec, std::size_t bytes_transferred){
            boost::ignore_unused(bytes_transferred);
            if(ec)
                return fail(ec, "write");

            if(close){
                // This means we should close the connection, usually because
                // the response indicated the "Connection: close" semantic.
                return self->do_close();
            }else{
                // Read another request
                hdr_parser_ptr_ = make_shared<http::request_parser<http::empty_body>>();
                self->do_read(hdr_parser_ptr_);
            }
        };
        // Write the response
        http::async_write(stream_,*res, l);
}

void session::do_close()
{
    // Send a TCP shutdown
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
}


int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 4)
    {
        std::cerr <<
            "Usage: http_file_service <address> <port> <doc_root>\n" <<
            "Example:\n" <<
            "    http_file_service 0.0.0.0 8080 .\n";
        return EXIT_FAILURE;
    }
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);

    // The io_context is required for all I/O
    net::io_context ioc;

    // Create and launch a listening port
    std::make_shared<listener>(
        ioc,
        tcp::endpoint{address, port},
        doc_root)->run();

    ioc.run();

    return EXIT_SUCCESS;
}