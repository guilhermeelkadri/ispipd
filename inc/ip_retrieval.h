#pragma once

#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <memory>

#include "thread_safe_queue.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket(s) closesocket(s)
#else
#include <unistd.h>
#define close_socket(s) close(s)
#endif

namespace sock
{
    namespace params
    {
        // IP retrievel params
        extern const char *hostname;       // API hostname
        extern const char *port;           // HTTP port
        extern const char *request;        // HTTP request
    }
class IpRetrievalHandler 
{
public:
    IpRetrievalHandler(const char* hostname, const char* port, const char* request, ThreadSafeQueue &queue);
    ~IpRetrievalHandler();

    std::string sendCustomRequest(const char* hostname, const char* port, const char* request);
    void run();
    
private:
    void ip_retrieval();

    const char* m_hostname;
    const char* m_port;
    const char* m_request;
    ThreadSafeQueue *m_queue;
    std::unique_ptr<std::thread> m_tretrieval; 
};

} /* namespace socket */