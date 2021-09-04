#ifndef _DRINK_SHOP_CONFIG_H_
#define _DRINK_SHOP_CONFIG_H_
#include <stdint.h>
#include <string>
using namespace std;

class configuration
{

    /**
     * The Singleton's constructor should always be private to prevent direct
     * construction calls with the `new` operator.
     */

private:
    configuration(const uint16_t port, const string log_sink, const string log_level,const bool deamon): 
      port_(port),log_sink_(log_sink),log_level_(log_level),deamon_(deamon){}
    configuration(){}
    uint16_t port_;
    string bind_ip_;
    string log_sink_;
    string log_level_;
    bool deamon_;
    static configuration* instance;

public:
    /**
     * Singletons should not be cloneable.
     */
    configuration(configuration &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const configuration &) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static configuration *get_instance(const uint16_t port, const string log_sink, const string log_level, const bool deamon);
    static configuration *get_instance();

    /**
     * Finally, any singleton should define some business logic, which can be
     * executed on its instance.
     */

    const uint16_t port() const;
    void set_port(const uint16_t port);
    void set_bind_ip(const string& bind_ip);
    const string bind_ip() const;
    const string log_level() const;
    void set_log_level(const string& log_level);
    const string log_sink() const;
    void set_log_sink(const string& log_sink);
    const bool deamon() const;
    void set_deamon(const bool deamon);
    const char *version();
    int parse_from_commandline(int argc, char **argv);
};



#endif