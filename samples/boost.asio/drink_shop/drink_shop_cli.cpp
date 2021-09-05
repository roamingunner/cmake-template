#include <argp.h>
#include <stdint.h>
#include <string>
#include <json/json.h>
#include <iostream>
#include <boost/asio.hpp>
#include "drink_shop_protocol.hpp"

using namespace std;
using namespace boost::asio;
const char *argp_program_version =
"1.0";
const char *argp_program_bug_address =
"<xxxxxx@xxx.com>";

/* Program documentation. */
static char doc[] =
"drink_shop_cli";

/* A description of the arguments we accept. */
static char args_doc[] = "<host> <port>";

/* The options we understand. */
static struct argp_option options[] = {
{"verbose",  'v', 0,      0,  "Produce verbose output" },
{"type", 't', "STRING", 0,  "set type" },
{"size", 's', "STRING", 0,  "set size" },
{"packed", 'p', 0, 0,  "set packed" },
{ 0 }
};

struct drink_shop_cli_cfg
{
    char *args[2];  /* arg1:host ip & arg2:host port */
    string type;
    string size;
    bool packed;
};

/* struct argp_option定义
struct argp_option
{
const char *name; option名称，用作long_option
int key;  option的key，用作short_option
const char *arg; 如果该option需要跟参数，那么可通过arg字段给参数定一个名称。可以在usage显示出来，同时在parser里才能通过arg拿到option参数
int flags; 一般不写
const char *doc; 参数说明
int group; 参数分组，一般不写
};
*/

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
struct drink_shop_cli_cfg *arguments = reinterpret_cast<struct drink_shop_cli_cfg *>(state->input);
switch (key)
    {
    case 'p':
        arguments->packed = true;
        break;
    case 't':
        arguments->type = arg;
        break;
    case 'v':
        cout << argp_program_version << endl;
        exit(0);
        //never reach
        break;
    case 's':
        arguments->size = arg;
        break;
    //ARGP定义了一些特殊的key
    //ARGP_KEY_ARG用于接收没有option的参数
    //ARGP_KEY_END在参数粘贴完成后会调用
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
            /* Too many arguments. */
            argp_usage (state);

        arguments->args[state->arg_num] = arg;
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 2)
            /* Not enough arguments. */
            argp_usage (state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv)
{
    enum {BUFFER_SIZE = 1024};
    /* Parse our arguments; every option seen by parse_opt will
        be reflected in arguments. */
    struct drink_shop_cli_cfg cfg;
    cfg.type = "milk";
    cfg.size = "big";
    cfg.packed = false;
    argp_parse (&argp, argc, argv, 0, 0, &cfg);

    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    io_context ioc;
    ip::tcp::resolver resolver(ioc);
    ip::tcp::socket sock(ioc);
    boost::asio::connect(sock,resolver.resolve(cfg.args[0],cfg.args[1]));

    try
    {

        drink_shop_protocol h1(DRINK_SHOP_PROTO_CMD_VERSION,0,0,nullptr);
        memset(send_buffer,0,BUFFER_SIZE);
        h1.serialize_pkg_to_buffer(send_buffer);
        boost::asio::write(sock,boost::asio::buffer(send_buffer,h1.package_size()));
        memset(recv_buffer,0,BUFFER_SIZE);
        sock.read_some(boost::asio::buffer(recv_buffer,BUFFER_SIZE));
        if (h1.parse_from_buffer(recv_buffer)){
            cout << "parse reply failed\n";
        }else{
            cout << "drink shop version:" << h1.body() << endl;
        }

        Json::Value obj;
        obj["type"] = cfg.type;
        obj["size"] = cfg.size;
        obj["packed"] = cfg.packed;

        string obj_str = obj.toStyledString();
        drink_shop_protocol h2(DRINK_SHOP_PROTO_CMD_ORDER,0,obj_str.length(),obj_str.data());
        memset(send_buffer,0,BUFFER_SIZE);
        h2.serialize_pkg_to_buffer(send_buffer);
        boost::asio::write(sock,boost::asio::buffer(send_buffer,h2.package_size()));
        memset(recv_buffer,0,BUFFER_SIZE);
        sock.read_some(boost::asio::buffer(recv_buffer,BUFFER_SIZE));
        if (h2.parse_from_buffer(recv_buffer)){
            cout << "parse reply failed\n";
        }else{
            cout << "order:" << h2.body() << endl;
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    sock.close();


    return 0;
}
