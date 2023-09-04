#pragma once

#include "thread_safe_queue.h"

void thread_mqtt_client(ThreadSafeQueue<std::string> &dataQueue);