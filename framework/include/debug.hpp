#pragma once

#include <iostream>

/*
#include <thread>
#include <chrono>

#define DEBUG_SLEEP(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
*/

#define DEBUG_LOG(a) std::cout << (a)
#define DEBUG_LOG_SEP(a) std::cout << (a) << " | "
#define DEBUG_LOG_LN(a) std::cout << (a) << std::endl
#define DEBUG_LOG_NEWLN std::cout << std::endl
