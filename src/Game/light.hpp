#pragma once

#include <glm/glm.hpp>


struct LightUBO {
    glm::vec4 position; //.w == 1 is point light, 0 is directional
    glm::vec4 ambient_color; //.w unused
    glm::vec4 diffuse_color; //.w unused
    glm::vec4 specular_color; //.w unused
};


/*
class Light {

public:
    LightUBO ubo;
};

*/