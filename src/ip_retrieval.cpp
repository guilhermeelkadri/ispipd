
#include "ip_retrieval.h"

namespace sock
{
    namespace params
    {
        // IP retrievel params
        const char *hostname = "api64.ipify.org";       // API hostname
        const char *port = "80";                        // HTTP port
        const char *request = "GET /?format=json HTTP/1.1\r\nHost: api64.ipify.org\r\n\r\n"; // HTTP request
    }

IpRetrievalHandler::IpRetrievalHandler
    (const char* hostname, const char* port, const char* request, ThreadSafeQueue &queue)
        : m_hostname(hostname), 
        m_port(port), m_request(request), m_queue(&queue)
{
     
}    

IpRetrievalHandler::~IpRetrievalHandler()
{
    if(m_tretrieval
    && m_tretrieval->joinable())
    {
        m_tretrieval.reset();
        
        std::cout << "Stop IP retrieval thread" << std::endl;
    }
}

void
IpRetrievalHandler::run()
{
    m_tretrieval = std::unique_ptr<std::thread> 
        (new std::thread(&sock::IpRetrievalHandler::ip_retrieval, this));

    // m_tretrieval->join();      
}

std::string
IpRetrievalHandler::sendCustomRequest(const char* hostname, const char* port, const char* request)
{
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return "";
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
    {
        perror("socket");
        freeaddrinfo(res);
        return "";
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("connect");
        close_socket(sockfd);
        freeaddrinfo(res);
        return "";
    }

    ssize_t bytes_sent = send(sockfd, request, strlen(request), 0);
    if (bytes_sent == -1)
    {
        perror("send");
        close_socket(sockfd);
        freeaddrinfo(res);
        return "";
    }

    char response[1024];
    ssize_t bytes_received = recv(sockfd, response, sizeof(response), 0);
    if (bytes_received == -1)
    {
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

void
IpRetrievalHandler::ip_retrieval()
{    
    while (true)
    {
        std::string response = this->sendCustomRequest(m_hostname, m_port, m_request);

        if (!response.empty())
        {
            std::cout << "Response:\n"
                      << response << std::endl;
        }
        else
        {
            std::cerr << "Error sending custom request." << std::endl;
        }

        m_queue->push(response);

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

} /* namespace socket */