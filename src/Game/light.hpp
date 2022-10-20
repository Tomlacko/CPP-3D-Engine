#pragma once

#include <glm/glm.hpp>

/*
enum class LightType {
    directional, point
};
*/

struct LightUBO {
    glm::vec4 position; //.w == 1 is point light, 0 is directional  //works as radius
    glm::vec4 ambient_color; //.w unused
    glm::vec4 diffuse_color; //.w maxIntensity
    glm::vec4 specular_color; //.w unused


    LightUBO() = default;

    LightUBO(float radius, float maxIntensity, float x, float y, float z, float r, float g, float b)
    : position(x, y, z, radius/*static_cast<float>(type)*/), diffuse_color(r, g, b, 1/maxIntensity), specular_color(r, g, b, 1) {}

    LightUBO(glm::vec4 pos, glm::vec4 amb, glm::vec4 diff, glm::vec4 spec)
    : position(pos), ambient_color(amb), diffuse_color(diff), specular_color(spec) {}
};


/*
class Light {

public:
    LightUBO ubo;
};

*/