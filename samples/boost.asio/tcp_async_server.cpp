/**
 * @file tcp_async_server.cpp
 * @author leo.qin (103922926@qq.com)
 * @brief
 * @version 0.1
 * @date 2021-01-28
 *
 * @copyright Copyright (c) 2021
 *
 */


#include <argp.h>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdbool.h>
#include "aixlog.hpp"
#include "revision.h"
#include "xxx_protocol.h"

using boost::asio::ip::tcp;

/* -------------------- program praser--------------------*/
const char *argp_program_version = VCS_FULL_HASH;
const char *argp_program_bug_address = "<xxxxx@xx.com>";
static char doc[] = "a tcp async server sample.";
static char args_doc[] = "[port]";
static struct argp_option options[] = {
    {"log_level", 'l', "LEVEL", 0, "set log level:trace,debug,info,notice,warning.(default:notice)"},
    {"log_sink", 'L', "SINK", 0, "set log sink:console,syslog,/path/to_file. (default:console)"},
    {"daemon", 'd', 0, 0, "run as a daemon process"},
    {0}};

struct arguments {
    const char *log_level;
    const char *log_sink;
    uint16_t port;
    bool daemon;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = reinterpret_cast<struct arguments *>(state->input);

    switch (key) {
    case 'd':
        arguments->daemon = true;
        break;
    case 'l':
        arguments->log_level = arg;
        break;
    case 'L':
        arguments->log_sink = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            /* Too many arguments. */
            argp_usage(state);
        arguments->port = atoi(arg);
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
/* --------------------------------------------------------*/

/* -------------------- daemonize --------------------*/
static void daemonize() {
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    /*TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }
}
/* --------------------------------------------------------*/

/* -------------------- tcp async server --------------------*/


class session : public std::enable_shared_from_this<session> {
  public:
    session(tcp::socket socket) : socket_(std::move(socket)) {}
    ~session(){
        LOG(DEBUG) << "session will be closed\n" ;
    }
    void start() { do_read_header(); }

  private:
    void do_read_header() {
        auto self(shared_from_this());
		async_read(socket_,boost::asio::buffer(recv_buff_, sizeof(xxx_protocol)),boost::asio::transfer_all(),
			[this, self](boost::system::error_code ec, std::size_t length){
				if (ec){
					LOG(ERROR) << "read header failed. Session is destructed. " << ec.message() << std::endl;
				}else{
					struct xxx_protocol* header = reinterpret_cast<struct xxx_protocol*>(recv_buff_);
					uint16_t magic = ntohs(header->magic);
					LOG(DEBUG) << "read bytes:" << length << std::endl;

					if (XXX_PROTOCOL_MAGIC == magic){
						uint32_t payload_length = ntohl(header->payload_length);
						if (payload_length){
							do_read_payload(payload_length);
						}else{
							dispatch_msg();
						}
					}else{
						LOG(ERROR) << "header magic mismatch. Session is destructed.\n" ;
					}
				}
			}
		);
    }

	void do_read_payload(uint32_t bytes_to_read){
        auto self(shared_from_this());
		async_read(socket_,boost::asio::buffer(recv_buff_ + sizeof(struct xxx_protocol), bytes_to_read),boost::asio::transfer_all(),
			[this, self](boost::system::error_code ec, std::size_t length){
				if (ec){
					LOG(ERROR) << "read payload failed. Session is destructed. " << ec.message() << std::endl;
				}else{
					LOG(DEBUG) << "read bytes:" << length << std::endl;
					dispatch_msg();
				}
			}
		);
	}

	void dispatch_msg(){
		struct xxx_protocol* header = reinterpret_cast<struct xxx_protocol*>(recv_buff_);
		uint16_t magic = ntohs(header->magic);
		uint16_t type = ntohs(header->type);
		uint32_t payload_length = ntohl(header->payload_length);
		char *payload = header->payload;

		switch (type)
		{
		case XXX_PROTOOCL_VERSION:
			/* code */
			{
				struct xxx_protocol resp_hdr;
				resp_hdr.magic = htons(magic);
				resp_hdr.type = htons(type);
				resp_hdr.payload_length = htonl(strlen(VCS_FULL_HASH));
				do_write(reinterpret_cast<const char *>(&resp_hdr),sizeof(struct xxx_protocol));
				do_write(reinterpret_cast<const char *>(argp_program_version),strlen(VCS_FULL_HASH));
			}
			break;
		case XXX_PROTOOCL_ECHO:
			/* code */
			{
				struct xxx_protocol resp_hdr;
				resp_hdr.magic = htons(magic);
				resp_hdr.type = htons(type);
				resp_hdr.payload_length = htonl(payload_length);
				do_write(reinterpret_cast<char *>(&resp_hdr),sizeof(struct xxx_protocol));
				do_write(payload,payload_length);
			}
			break;		
		default:
			break;
		}

		//consume buffer
		memset(recv_buff_,0,max_length);

		//pending read operation
		do_read_header();
	}

    void do_write(const char *buff, std::size_t length) {
		//buff可能是其他数据结构串行化的结果，该结果可能在栈上。异步发送必须保证数据在回调函数调用前，数据都是有效的，所以这里把数据拷贝到堆上
        auto self(shared_from_this());
		char *send_buff = new char[length]();
		memcpy(send_buff, buff, length);

        async_write(socket_, boost::asio::buffer(buff, length),
            [this, self, send_buff](boost::system::error_code ec, std::size_t length) {
                if (ec) {
					LOG(ERROR) << "write failed. " << ec.message() << std::endl;
                }
				LOG(DEBUG) << "write bytes:" << length << std::endl;
				//回收发送buff
				delete []send_buff;
            });
    }

    tcp::socket socket_;
    enum { max_length = 4096 };
    uint8_t recv_buff_[max_length];
};

class server {
  public:
    server(boost::asio::io_context &io_context, short port) :
        acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        LOG(NOTICE) << "start listen " << port << std::endl;
        do_accept();
    }

  private:
    void do_accept() {
        LOG(DEBUG) << "wait for client...\n";
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<session>(std::move(socket))->start();
				do_accept();
            }else{
				LOG(ERROR) << "accept failed. " << ec.message() << std::endl;
			}
        });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[]) {
    struct arguments args;

    // setup default arguments
    args.daemon = false;
    args.log_level = "notice";
    args.log_sink = "console";
    args.port = 12021;
    // parse program args
    argp_parse(&argp, argc, argv, 0, 0, &args);

    // setup aixlog
    auto logl = AixLog::Severity::notice;
    if (!strcmp(args.log_level, "trace")) {
        logl = AixLog::Severity::trace;
    } else if (!strcmp(args.log_level, "debug")) {
        logl = AixLog::Severity::debug;
    } else if (!strcmp(args.log_level, "info")) {
        logl = AixLog::Severity::info;
    } else if (!strcmp(args.log_level, "notice")) {
        logl = AixLog::Severity::notice;
    } else if (!strcmp(args.log_level, "warning")) {
        logl = AixLog::Severity::warning;
    }
    if (!strcmp(args.log_sink, "console")) {
        auto sink = std::make_shared<AixLog::SinkCout>(logl);
        AixLog::Log::init({sink});
    } else if (!strcmp(args.log_level, "syslog")) {
        //
    } else {
        auto sink = std::make_shared<AixLog::SinkFile>(logl, args.log_sink);
        AixLog::Log::init({sink});
    }

    if (args.daemon) {
        daemonize();
    }

    try {
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, argv](boost::system::error_code /*ec*/, int /*signo*/) {
            LOG(NOTICE) << argv[0] << " stop ...\n";
            io_context.stop();
        });

        server s(io_context, args.port);
        io_context.run();

    } catch (std::exception &e) {
        LOG(ERROR) << "Exception: " << e.what() << "\n";
    }

    return 0;
}