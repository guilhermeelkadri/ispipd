#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ip_retrieval.h"
#include "mqtt_client.h"
#include "thread_safe_queue.h"

using namespace ::testing;

// Mocking dependencies
class MockThreadSafeQueue : public ThreadSafeQueue
{
public:
    MOCK_METHOD(void, push, (std::string data), ());
    MOCK_METHOD(std::string, pop, (), ());
};

TEST(ThreadSafeQueueTest, PushAndPop) 
{
    MockThreadSafeQueue mockQueue;

    EXPECT_CALL(mockQueue, push("TestString")).Times(1);
    mockQueue.push("TestString");

    EXPECT_CALL(mockQueue, pop())
        .WillOnce(Return("TestString"));

    std::string result = mockQueue.pop();

    EXPECT_EQ(result, "TestString");
}

class MockMqttClient : public mqtt::mqtt_client 
{
public:
    MockMqttClient(const char *host, int port, const char *username, const char *password, ThreadSafeQueue &queue)
        : mqtt::mqtt_client(host, port, username, password, queue) {}

    MOCK_METHOD(void, on_connect, (int rc), (override));
    MOCK_METHOD(void, on_message, (const struct mosquitto_message *message), (override));
    MOCK_METHOD(void, on_subscribe, (int mid, int qos_count, const int *granted_qos), (override));
    MOCK_METHOD(void, on_publish, (int mid), (override));
    MOCK_METHOD(bool, connect, (), ());
    MOCK_METHOD(void, mqtt_publisher, (), ());
};

TEST(MqttClientTest, OnConnectSuccess) 
{
    MockThreadSafeQueue mockQueue;
    MockMqttClient client
        (mqtt::params::host, mqtt::params::port, mqtt::params::username, mqtt::params::password, mockQueue);

    std::cout << "Chegou aqui" << std::endl;

    EXPECT_CALL(client, connect())
        .WillOnce(Return(true));
    

    // EXPECT_CALL(client, on_connect(0))
    //     .Times(1);

    // client.connect();

}


int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}