#pragma once

#include "thread_safe_queue.h"

void thread_ip_retrieval(ThreadSafeQueue<std::string> &dataQueue);