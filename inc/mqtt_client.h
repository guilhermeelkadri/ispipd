#pragma once

#include <cstring>
#include <mosquittopp.h>
#include <thread>

#include "thread_safe_queue.h"

namespace mqtt
{
    namespace params
    {
        // Mqtt broker params
        extern const char *host;
        extern const int port;
        extern const char *username;
        extern const char *password;
    }

class mqtt_client : public mosqpp::mosquittopp 
{
public:
    mqtt_client(const char *host, const int port, const char *username, 
        const char *password, ThreadSafeQueue &dataQueue);
    ~mqtt_client();
    void run();
        

private:
    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(int mid, int qos_count, const int *granted_qos) override;
    void on_publish(int mid) override;
    bool connect();
    void mqtt_publisher();

    std::string m_username;
    std::string m_password;
    std::string m_host;
    int m_port;
    ThreadSafeQueue *m_queue;
    std::unique_ptr<std::thread> m_tpublisher;
};



}