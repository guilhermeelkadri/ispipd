#pragma once

#include <iostream>
#include <condition_variable>
#include <queue>
#include <string>

class ThreadSafeQueue
{
public:
    void push(std::string data)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_queue.push(data);
        
        m_cv.notify_one();
    }

    std::string pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait(lock, [&]{ return !m_queue.empty(); });

        std::string data = m_queue.front();

        m_queue.pop();

        return data;
    }

private:
    std::queue<std::string> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;

};
