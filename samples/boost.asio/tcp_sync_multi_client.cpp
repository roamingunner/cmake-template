/**
 * @file tcp_sync_multi_client.cpp
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
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdbool.h>
#include <thread>
#include "aixlog.hpp"
#include "revision.h"
#include "xxx_protocol.h"
using boost::asio::ip::tcp;

enum { max_length = 512 };

/* -------------------- program praser--------------------*/
const char *argp_program_version = VCS_FULL_HASH;
const char *argp_program_bug_address = "<xxxxx@xx.com>";
static char doc[] = "a tcp sync client sample.";
static char args_doc[] = "[ip] [port] [message]";
static struct argp_option options[] = {
    {"log_level", 'l', 0, 0, "set log level:trace,debug,info,notice,warning.(default:notice)"},
    {"log_sink", 'L', 0, 0, "set log sink:console,syslog,/path/to_file. (default:console)"},
    {"threads", 't', "NUMBER", 0, "set concurrent threads"},
    {0}};

struct arguments {
    char *args[3];  // ip address & port & message
    const char *log_level;
    const char *log_sink;
    uint16_t threads_num;

};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = reinterpret_cast<struct arguments *>(state->input);

    switch (key) {
    case 'l':
        arguments->log_level = arg;
        break;
    case 't':
        arguments->threads_num = atoi(arg);
        break;
    case 'L':
        arguments->log_sink = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 3)
            /* Too many arguments. */
            argp_usage(state);

        arguments->args[state->arg_num] = arg;

        break;
    case ARGP_KEY_END:
        if (state->arg_num < 3)
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

void talk_to_server(int id, struct arguments& args){
    try {
        char send_buf[max_length] = {0};
        char recv_buf[max_length] = {0};
        struct xxx_protocol *hdr = nullptr;
        boost::asio::io_context io_context;
        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s, resolver.resolve(args.args[0],args.args[1]));

        //get version
        hdr = reinterpret_cast<struct xxx_protocol *>(send_buf);
        hdr->magic = htons(XXX_PROTOCOL_MAGIC);
        hdr->type = htons(XXX_PROTOOCL_VERSION);
        hdr->payload_length = 0;
        boost::asio::write(s, boost::asio::buffer(send_buf, sizeof(struct xxx_protocol)));

        //read header
        //boost::asio::read返回条件是buffer满或者错误产生。如果想读取一些数据就返回，可以用 ip::tcp::socket的member function
        boost::asio::read(s, boost::asio::buffer(recv_buf, sizeof(struct xxx_protocol)));
        hdr = reinterpret_cast<struct xxx_protocol *>(recv_buf);
        //read payload
        boost::asio::read(s, boost::asio::buffer(recv_buf + sizeof(struct xxx_protocol), ntohl(hdr->payload_length))); 
        std::string svr_version(hdr->payload,ntohl(hdr->payload_length));
        LOG(NOTICE) << "thread:" << id <<" get server version:" << svr_version << std::endl;


        //send echo message
        memset(send_buf,0,max_length);
        memset(recv_buf,0,max_length);
        hdr = reinterpret_cast<struct xxx_protocol *>(send_buf);
        hdr->magic = htons(XXX_PROTOCOL_MAGIC);
        hdr->type = htons(XXX_PROTOOCL_ECHO);
        hdr->payload_length = htonl(strlen(args.args[2]));
        memcpy(hdr->payload, args.args[2],strlen(args.args[2]));
        boost::asio::write(s, boost::asio::buffer(send_buf, sizeof(struct xxx_protocol) + strlen(args.args[2])));

        //read header
        boost::asio::read(s, boost::asio::buffer(recv_buf, sizeof(struct xxx_protocol)));
        hdr = reinterpret_cast<struct xxx_protocol *>(recv_buf);
        //read payload
        boost::asio::read(s, boost::asio::buffer(recv_buf + sizeof(struct xxx_protocol), ntohl(hdr->payload_length))); 
        std::string recv_message(hdr->payload,ntohl(hdr->payload_length));
        LOG(NOTICE) << "thread:" << id << " get message from server: " << recv_message << std::endl;
    } catch (std::exception &e) {
        LOG(ERROR) << "Exception: " << e.what() << "\n";
    }
}

int main(int argc, char *argv[]) {
    struct arguments args;

    // setup default arguments
    args.log_level = "notice";
    args.log_sink = "console";
    args.threads_num = 1;
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

 
    std::vector<std::thread> threads;
    for (int i = 0; i < args.threads_num ; i++){
        threads.emplace_back([i,&args](){talk_to_server(i,args);});
    }
    for (auto& t : threads){
        t.join();
    }





    return 0;
}