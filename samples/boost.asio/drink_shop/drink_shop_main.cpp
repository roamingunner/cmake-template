#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include "aixlog.hpp"
#include "drink_shop_config.hpp"
#include "drink_shop_maker.hpp"
#include "drink_shop_packer.hpp"
#include "drink_shop_receptionist.hpp"



using namespace std;


/* -------------------- daemonize --------------------*/
void daemonize() {
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

int main(int argc, char **argv)
{
    /* code */
    configuration* conf = configuration::get_instance(10000,"console","notice",false);

    conf->parse_from_commandline(argc,argv);

    // setup aixlog
    auto logl = AixLog::Severity::notice;
    if (!conf->log_level().compare("trace")) {
        logl = AixLog::Severity::trace;
    } else if (!conf->log_level().compare("debug")) {
        logl = AixLog::Severity::debug;
    } else if (!conf->log_level().compare("info")) {
        logl = AixLog::Severity::info;
    } else if (!conf->log_level().compare("notice")) {
        logl = AixLog::Severity::notice;
    } else if (!conf->log_level().compare("warning")) {
        logl = AixLog::Severity::warning;
    }
    if (!conf->log_sink().compare("console")) {
        auto sink = std::make_shared<AixLog::SinkCout>(logl);
        AixLog::Log::init({sink});
    } else if (!conf->log_sink().compare("syslog")) {
        //
    } else {
        auto sink = std::make_shared<AixLog::SinkFile>(logl, conf->log_sink());
        AixLog::Log::init({sink});
    }

    if (conf->deamon()) {
        daemonize();
    }
    
    try
    {
        io_context ioc;
        shared_ptr<ip::tcp::endpoint> ep;
        if (conf->bind_ip().length()){
            ep = make_shared<ip::tcp::endpoint>(ip::address::from_string(conf->bind_ip()),conf->port());
        }else{
            ep = make_shared<ip::tcp::endpoint>(ip::tcp::v4(),conf->port());
        }

        shared_ptr<drink_shop_receptionist> receptionist = make_shared<drink_shop_receptionist>(ioc,*ep);
        shared_ptr<drink_shop_maker> marker = make_shared<drink_shop_maker>();
        shared_ptr<drink_shop_packer> packer = make_shared<drink_shop_packer>();
        shared_ptr<null_handler> end = make_shared<null_handler>();
        marker->set_next(packer.get())->set_next(receptionist.get())->set_next(end.get());
        receptionist->set_entry_handle(marker.get());
        LOG(NOTICE) << "start drink shop open\n";
        receptionist->do_accept();
        ioc.run();
    }
    catch(const std::exception& e)
    {
        LOG(FATAL) << e.what() << '\n';
    }

    return 0;
}
