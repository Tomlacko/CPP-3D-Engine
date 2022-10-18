#pragma once

//#include <glad/glad.h>
//#include <glm/glm.hpp>

#include "object.hpp"




/*
 * rotation {0,0} is looking towards +Z (south)
*/


struct CameraUBO {
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 position;
};


class Camera: public Object {
    float nearClip = 0.01;
    float farClip = 1000;

    float FOV = 70;
    float aspect = 1;

    mutable CameraUBO ubo;
    GLuint cameraBufferID = 0;

public:
    Camera(float width, float height);
    ~Camera() override;

    void setFOV(float fov) {FOV = fov;}
    float getFOV() {return FOV;}

    void setAspect(float width, float height) {aspect = width/height;}
    float getAspect() {return aspect;}

    [[nodiscard]] glm::mat4 projectionMatrix() const;
    [[nodiscard]] glm::mat4 viewMatrix() const;

    void use(int bindingIndex = 0) const;
};