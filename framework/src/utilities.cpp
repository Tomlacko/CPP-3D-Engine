#include "utilities.hpp"
#include <fstream>
#include <cmath>

const std::string load_file(const std::string &file_name) {
    std::ifstream infile{file_name};

    if(!infile) {
        throw std::string("File ") + file_name + " does not exists";
    }

    return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
}

float clamp(float low, float val, float high) {
    return std::max(std::min(val, high), low);
}

glm::vec3 rotToDir(const glm::vec2& rot) {
    return {std::sin(rot.x)*std::cos(rot.y), std::sin(rot.y), std::cos(rot.x)*std::cos(rot.y)};
}

glm::vec2 dirToRot(const glm::vec3& dir) {
    //TODO
    return {0, 0};
}