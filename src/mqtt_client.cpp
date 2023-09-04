#include <cstring>
#include <mosquittopp.h>

#include "mqtt_client.h"

namespace mqtt
{
    namespace topic
    {
        // Publish a message to a topic
        int mid = 1;
        const char *topic = "isp/ip";
        const int qos = 2;
        bool retain = false;
    }
}

class mqtt_client : public mosqpp::mosquittopp 
{
public:
    mqtt_client(const char *host, int port, 
        const char *username, const char *password)
        : 
        m_host(host), m_port(port), 
        m_username(username), m_password(password)
        {
            username_pw_set(m_username.c_str(), m_password.c_str());
        }

    void on_connect(int rc) override 
    {
        if (rc == 0) 
        {
            std::cout << "Connected to the broker." << std::endl;
        } else {
            std::cerr << "Connection failed: " << mosqpp::strerror(rc) << std::endl;
        }
    }

    void on_message(const struct mosquitto_message *message) override 
    {
        std::cout << "Received message on topic: " << message->topic << std::endl;
        std::cout << "Message: " << static_cast<char *>(message->payload) << std::endl;
    }

    void on_subscribe(int mid, int qos_count, const int *granted_qos) override 
    {
        std::cout << "Subscribed to topic." << std::endl;
    }

    void on_publish(int mid) override
    {
        std::cout << "Message published (Mid: " << mid << ")" << std::endl;
    }

    bool connect() 
    {
        return (connect_async(m_host.c_str(), m_port) == MOSQ_ERR_SUCCESS); 
    }

private:
    std::string m_username;
    std::string m_password;
    std::string m_host;
    int m_port;
};


void thread_mqtt_client(ThreadSafeQueue<std::string> &dataQueue) 
{
    const char *host = "localhost";
    const int port = 1883;
    const char *username = "username";
    const char *password = "password";
     
    mosqpp::lib_init();  // Initialize the Mosquitto library

    mqtt_client client(host, port, username, password);

    if(client.connect())
    {
        client.loop_start();
    }
    else
    {
        std::cerr << "NOT connected" << std::endl;
    }

    while(true)
    {
        std::string ip = dataQueue.pop();

        int publishResult = client.publish(&mqtt::topic::mid, mqtt::topic::topic, 
            ip.size(), ip.c_str(), mqtt::topic::qos, mqtt::topic::retain);

        if (publishResult == MOSQ_ERR_SUCCESS)
        {
            std::cout << "Message published successfully." << std::endl;
        }
        else
        {
            std::cerr << "Publish failed: " << mosqpp::strerror(publishResult) << std::endl;
        }
    }


    mosqpp::lib_cleanup();  // Clean up the Mosquitto library

}

