#pragma once

#include <string>
#include <glm/glm.hpp>



const std::string load_file(const std::string& file_name);


float clamp(float low, float val, float high);

//true mathematical modulus, works for floats and negative numbers
double mod(double a, double b);
float mod(float a, float b);
int mod(int a, int b);
unsigned int mod(unsigned int a, unsigned int b);

double numsign(double n);
float numsign(float n);
int numsign(int n);

glm::vec3 rotToDir(const glm::vec2& rot);
glm::vec2 dirToRot(const glm::vec3& dir);


struct boolstr {
    std::string str;

    operator bool() const {
        return str=="true";
    }
};