#include <argp.h>
#include <iostream>
#include "drink_shop_config.hpp"

const char *argp_program_version =
  "drink_shop 1.0";
const char *argp_program_bug_address =
  "<103922926@qq.com>";

/* Program documentation. */
static char doc[] =
  "drink_shop";

/* A description of the arguments we accept. */
static char args_doc[] = "null";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"port", 'p', "NUMBER", 0,  "set listen port" },
  {"bind_ip", 'b', "STRING", 0,  "bind ip address" },
  {"log_level", 'l', "LEVEL", 0, "set log level:trace,debug,info,notice,warning.(default:notice)"},
  {"log_sink", 'L', "SINK", 0, "set log sink:console,syslog,/path/to_file. (default:console)"},
  {"deamon", 'd', 0, 0,  "deamon mode" },
  { 0 }
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
  configuration* conf = configuration::get_instance();
  switch (key)
    {
    case 'p':
      conf->set_port(atoi(arg));
      break;
    case 'b':
      conf->set_bind_ip(arg);
      break;
    case 'v':
      cout << conf->version() << endl;
      exit(0);
      //never reach
      break;
    case 'l':
      conf->set_log_level(arg);
      break;
    case 'L':
      conf->set_log_sink(arg);
      break;
    case 'd':
      conf->set_deamon(true);
      break;
    //ARGP定义了一些特殊的key
    //ARGP_KEY_ARG用于接收没有option的参数
    //ARGP_KEY_END在参数粘贴完成后会调用
    case ARGP_KEY_ARG:
        argp_usage (state);
      break;

    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

const uint16_t configuration::port() const{
    return port_;
}
void configuration::set_port(const uint16_t port){
    port_ = port;
}
void configuration::set_bind_ip(const string& bind_ip){
    bind_ip_ = bind_ip;
}
const string configuration::bind_ip() const {
    return bind_ip_;
}
const string configuration::log_level() const{
    return log_level_;
}
void configuration::set_log_level(const string& log_level){
    log_level_ = log_level;
}
const string configuration::log_sink() const{
    return log_sink_;
}
void configuration::set_log_sink(const string& log_sink){
    log_sink_ = log_sink;
}
const bool configuration::deamon() const{
    return port_;
}
void configuration::set_deamon(const bool deamon){
    deamon_ = deamon;
}
const char *configuration::version(){
    return argp_program_version;
}
int configuration::parse_from_commandline(int argc, char **argv){
    /* Our argp parser. */
    struct argp argp = { options, parse_opt, nullptr, doc };
    return argp_parse (&argp, argc, argv, 0, 0, nullptr);
}

configuration* configuration::instance = nullptr;

/**
 * Static methods should be defined outside the class.
 */
configuration* configuration::get_instance(const uint16_t port, const string log_sink, const string log_level, const bool deamon)
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(instance ==nullptr){
        instance  = new configuration(port, log_sink, log_level, deamon);
    }
    return instance;
}

configuration* configuration::get_instance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(instance ==nullptr){
        instance  = new configuration();
    }
    return instance;
}


