#pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>

/*
#include <thread>
#include <chrono>

#define DEBUG_SLEEP(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
*/

#define DEBUG_LOG(a) std::cout << (a)
#define DEBUG_LOG_SEP(a) std::cout << (a) << " | "
#define DEBUG_LOG_LN(a) std::cout << (a) << std::endl
#define DEBUG_LOG_NEWLN std::cout << std::endl


const std::string load_file(const std::string& file_name);

float clamp(float low, float val, float high);

glm::vec3 rotToDir(const glm::vec2& angle);
glm::vec2 dirToRot(const glm::vec3& dir);