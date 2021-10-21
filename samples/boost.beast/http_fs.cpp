#include "http_fs.hpp"

void fail(beast::error_code ec, char const* what){
    LOG(ERROR) <<  what << ": " << ec.message() << "\n";
}

#if 0
{//client
    if (!std::strcmp("upload", mode)) {
        file_target_ = file_target;
        file_destination_ = "/" + std::string(file_destination);
        // Set up an HTTP PUT request message
        //req_.method(http::verb::put);
        //req_.target(file_destination_);
        //req_.version(version);
        //req_.set(http::field::host, host);
        //req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        //// experimental
        //req_.set(http::field::content_type, "text/plain");
        //beast::string_param string_param("bla bla bla");
        //req_.set(http::field::body, string_param);
        //req_.prepare_payload();

        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(file_target_.c_str(), beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if (ec == beast::errc::no_such_file_or_directory)
            std::cout << file_target_.c_str() << " not found" << std::endl;

        // Handle an unknown error
        if (ec)
            std::cout << "Error: " << ec.message() << std::endl;

        // Cache the size since we need it after the move
        auto const size = body.size();

        //https://github.com/boostorg/beast/issues/1304
        req_for_upload_.method(http::verb::put);
        req_for_upload_.target(file_destination_);
        req_for_upload_.version(version);
        req_for_upload_.set(http::field::host, host);
        req_for_upload_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req_for_upload_.set(http::field::content_type, "text/plain");
        req_for_upload_.content_length(size);
        req_for_upload_.body() = std::move(body);
        req_for_upload_.prepare_payload();
    }
        
}

{//server


    // Respond to a PUT request
    if (req.method() == http::verb::put) {
        std::cout << "-> PUT message received" << std::endl;
        // used with experimental
        //std::cout << "-> Body: " << req[http::field::body] << std::endl;
        std::cout << "-> File: " << req.target() << std::endl;
        // https://github.com/boostorg/beast/issues/819
        std::cout << "-> Message: " << std::endl;
        std::cout << req << std::endl;

        // Store the received file on disk
        std::string file_destination_ = static_cast<std::string>(req.target());
        // remove the forward slash
        file_destination_.erase(0, 1);
        boost::filesystem::path p{ file_destination_ };
        boost::filesystem::ofstream ofs{ p };
        // remove all the \r-characters (return) from the payload
        boost::erase_all(req.body(), "\r");
        ofs << req.body();

        http::response<http::string_body> res{
            http::status::ok, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }
}
#endif

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
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(beast::string_view base, beast::string_view path){
    if(base.empty())
        return std::string(path);
    std::string result(base);

    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    return result;
}

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template<class Body, class Allocator, class Send>
void handle_request(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
{
    // Returns a bad request response
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


    // Make sure we can handle the method
    if( req.method() != http::verb::head &&
        req.method() != http::verb::get &&
        req.method() != http::verb::put &&
        req.method() != http::verb::post)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/')
        path.append("index.html");


    if (req.method() == http::verb::head){
        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);
        // Handle the case where the file doesn't exist
        if(ec == beast::errc::no_such_file_or_directory)
            return send(not_found(req.target()));

        // Handle an unknown error
        if(ec)
            return send(server_error(ec.message()));

        http::response<http::empty_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }else if (req.method() == http::verb::get){
        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);
        // Handle the case where the file doesn't exist
        if(ec == beast::errc::no_such_file_or_directory)
            return send(not_found(req.target()));

        // Handle an unknown error
        if(ec)
            return send(server_error(ec.message()));

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
        return send(std::move(res));
    }else if(req.method() == http::verb::put){
        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::write_new, ec);
        // Handle an unknown error
        if(ec)
            return send(forbidden_error(ec.message()));

        // Respond to PUT request
        http::response<http::file_body> res{
            std::piecewise_construct,
            std::make_tuple(std::move(body)),
            std::make_tuple(http::status::ok, req.version())};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.prepare_payload();
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }else{

        http::response<http::string_body> res{http::status::ok, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }


    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if(ec == beast::errc::no_such_file_or_directory)
        return send(not_found(req.target()));

    // Handle an unknown error
    if(ec)
        return send(server_error(ec.message()));

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if(req.method() == http::verb::head){
        http::response<http::empty_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }else if(req.method() == http::verb::get){
        // Respond to GET request
        http::response<http::file_body> res{
            std::piecewise_construct,
            std::make_tuple(std::move(body)),
            std::make_tuple(http::status::ok, req.version())};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }else {
        // Respond to a PUT request
        std::cout << "-> PUT message received" << std::endl;
        // used with experimental
        //std::cout << "-> Body: " << req[http::field::body] << std::endl;
        std::cout << "-> File: " << req.target() << std::endl;
        // https://github.com/boostorg/beast/issues/819
        std::cout << "-> Message: " << std::endl;
        std::cout << req << std::endl;

        // Store the received file on disk
        std::string file_destination_ = static_cast<std::string>(req.target());
        // remove the forward slash
        file_destination_.erase(0, 1);
        boost::filesystem::path p{ file_destination_ };
        boost::filesystem::ofstream ofs{ p };
        // remove all the \r-characters (return) from the payload
        boost::erase_all(req.body(), "\r");
        ofs << req.body();

        http::response<http::string_body> res{
            http::status::ok, req.version() };
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }


}

//------------------------------------------------------------------------------



listener::listener(net::io_context& ioc, tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root)
    : ioc_(ioc) , acceptor_(net::make_strand(ioc)), doc_root_(doc_root){
    beast::error_code ec;
    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if(ec){
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if(ec){
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);if(ec)
    {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(
        net::socket_base::max_listen_connections, ec);
    if(ec){
        fail(ec, "listen");
        return;
    }
}

void listener::run() {
    do_accept();
}

void listener::do_accept() {
    // The new connection gets its own strand
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(
            &listener::on_accept,
            shared_from_this()));
}

void listener::on_accept(beast::error_code ec, tcp::socket socket){
    if(ec){
        fail(ec, "accept");
    }else{
        // Create the session and run it
        std::make_shared<session>(std::move(socket),doc_root_)->run();
    }

    // Accept another connection
    do_accept();
}


void session::run(){
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    net::dispatch(stream_.get_executor(),
                    beast::bind_front_handler(
                        &session::do_read,
                        shared_from_this()));
}

void session::do_read()
{
    // Make the request empty before reading,
    // otherwise the operation behavior is undefined.
    req_ = {};

    // Set the timeout.
    stream_.expires_after(std::chrono::seconds(30));

    // Read a request
    http::async_read(stream_, buffer_, req_,
        beast::bind_front_handler(
            &session::on_read,
            shared_from_this()));
}

void session::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == http::error::end_of_stream)
        return do_close();

    if(ec)
        return fail(ec, "read");

    // Send the response
    handle_request(*doc_root_, std::move(req_), lambda_);
}

void session::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    if(close)
    {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return do_close();
    }

    // We're done with the response so delete it
    res_ = nullptr;

    // Read another request
    do_read();
}

void session::do_close() {
    // Send a TCP shutdown
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    // At this point the connection is closed gracefully
}

