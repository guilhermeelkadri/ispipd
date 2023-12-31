#include <iostream>
#include <getopt.h>
#include <functional>

#include "mqtt_client.h"
#include "ip_retrieval.h"

#define PROGRAM_NAME "ispipd"
#define BINARY_NAME  "ispipd"
#define VERSION      "1.5.0"

#define PROGRAM_VERSION_DATETIME \
        PROGRAM_NAME \
        " version " VERSION ", " \
        "compiled " __DATE__ " at " __TIME__        

void usage()
{
    std::cout <<
        "Usage: ispipd [OPTION]\n\n"
        "Options:\n\n"
        "  -h, --help           Show help\n"
        "  -v, --version        Show version\n"
    << std::endl;
}

void parse_opts(int argc, char* argv[])
{
    const char * const short_opts = "vh";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"version",no_argument , nullptr, 'v'},
        {nullptr, no_argument, nullptr, 0}
    };

    while(true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        switch (opt)
        {
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
            break;
        
        case 'v':
            std::cout << PROGRAM_VERSION_DATETIME << std::endl;
            exit(EXIT_SUCCESS);
            break;
        
        case '?':
        default:
            return;
        }
    }
}

int main(int argc, char* argv[])
{
    parse_opts(argc, argv);

    std::cout << "Hello, from " << PROGRAM_VERSION_DATETIME << std::endl;

    ThreadSafeQueue dataQueue;

    auto socket = new sock::IpRetrievalHandler 
        (sock::params::hostname, sock::params::port, sock::params::request, dataQueue);
    
    auto client = new mqtt::mqtt_client 
        (mqtt::params::host, mqtt::params::port, mqtt::params::username, mqtt::params::password, dataQueue);

    socket->run();
    client->run();

    delete socket;
    delete client;

    return 0;
}
