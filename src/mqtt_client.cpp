
#include "mqtt_client.h"

namespace mqtt
{
namespace topic
{
    // Publish a message to a topic
    int mid = 1;
    const char *topic = "isp/ip/";
    const int qos = 2;
    bool retain = false;
}

namespace params
{
    // Mqtt broker params
    const char *host = "localhost";
    const int port = 1883;
    const char *username = "user";
    const char *password = "password";
}

mqtt_client::mqtt_client(const char *host, const int port, const char *username, 
    const char *password, ThreadSafeQueue &queue)
        : 
        m_host(host), m_port(port), 
        m_username(username), m_password(password), m_queue(&queue)
{
    username_pw_set(m_username.c_str(), m_password.c_str());  
}

mqtt_client::~mqtt_client()
{
    if(m_tpublisher
    && m_tpublisher->joinable())
    {
        // m_tpublisher.reset();
        
        std::cout << "Stop MQTT publisher thread" << std::endl;
    }
}

void
mqtt_client::run()
{
    m_tpublisher = std::unique_ptr<std::thread> 
        (new std::thread(&mqtt::mqtt_client::mqtt_publisher, this)); 

    m_tpublisher->join();   
}

void mqtt_client::on_connect(int rc)
{
    if (rc == 0)
    {
        std::cout << "Connected to the broker." << std::endl;
    }
    else
    {
        std::cerr << "Connection failed: " << mosqpp::strerror(rc) << std::endl;
    }
}

void mqtt_client::on_message(const struct mosquitto_message *message)
{
    std::cout << "Received message on topic: " << message->topic << std::endl;
    std::cout << "Message: " << static_cast<char *>(message->payload) << std::endl;
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    std::cout << "Subscribed to topic." << std::endl;
}

void mqtt_client::on_publish(int mid)
{
    std::cout << "Message published (Mid: " << mid << ")" << std::endl;
}

bool mqtt_client::connect()
{
    return (connect_async(m_host.c_str(), m_port) == MOSQ_ERR_SUCCESS);
}

void mqtt_client::mqtt_publisher()
{
    mosqpp::lib_init(); // Initialize the Mosquitto library

    // mqtt_client client(m_host, m_port, m_username, m_password);

    if (this->connect())
    {
        std::cout << "Connected" << std::endl;
        this->loop_start();
    }
    else
    {
        std::cerr << "NOT connected" << std::endl;
    }

    while (true)
    {
        std::string ip = m_queue->pop();

        int publishResult = this->publish(&mqtt::topic::mid, mqtt::topic::topic,
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

    mosqpp::lib_cleanup(); // Clean up the Mosquitto library
}

} /* namespace mqtt */