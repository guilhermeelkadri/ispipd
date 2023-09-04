#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>

#include "ip_retrieval.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket(s) closesocket(s)
#else
#include <unistd.h>
#define close_socket(s) close(s)
#endif

class CustomProtocolHandler {
public:
    CustomProtocolHandler(const char* hostname, const char* port, const char* request)
        : hostname(hostname), port(port), request(request) {}

    std::string sendCustomRequest() {
        struct addrinfo hints, *res;
        int status;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM; // TCP socket

        if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
            std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
            return "";
        }

        int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            freeaddrinfo(res);
            return "";
        }

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
            perror("connect");
            close_socket(sockfd);
            freeaddrinfo(res);
            return "";
        }

        ssize_t bytes_sent = send(sockfd, request, strlen(request), 0);
        if (bytes_sent == -1) {
            perror("send");
            close_socket(sockfd);
            freeaddrinfo(res);
            return "";
        }

        char response[1024];
        ssize_t bytes_received = recv(sockfd, response, sizeof(response), 0);
        if (bytes_received == -1) {
            perror("recv");
            close_socket(sockfd);
            freeaddrinfo(res);
            return "";
        }

        close_socket(sockfd);
        freeaddrinfo(res);

        response[bytes_received] = '\0';
        return response;
    }

private:
    const char* hostname;
    const char* port;
    const char* request;
};

void thread_ip_retrieval(ThreadSafeQueue<std::string> &dataQueue)
{
    const char* hostname = "api64.ipify.org"; // API hostname
    const char* port = "80"; // HTTP port
    const char* request = "GET /?format=json HTTP/1.1\r\nHost: api64.ipify.org\r\n\r\n";

    while (true)
    {

        CustomProtocolHandler handler(hostname, port, request);
        std::string response = handler.sendCustomRequest();

        if (!response.empty())
        {
            std::cout << "Response:\n"
                      << response << std::endl;
        }
        else
        {
            std::cerr << "Error sending custom request." << std::endl;
        }

        dataQueue.push(response);

        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}