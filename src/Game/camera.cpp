#include "camera.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float width, float height): aspect(width/height) {
    //camera uniform buffer, prepared only, data is pumped in only on request
    glCreateBuffers(1, &cameraBufferID);
    glNamedBufferStorage(cameraBufferID, sizeof(CameraUBO), nullptr/*&ubo*/, GL_DYNAMIC_STORAGE_BIT);
}

Camera::~Camera() {
    glDeleteBuffers(1, &cameraBufferID);
}

glm::mat4 Camera::projectionMatrix() const {
    float rangeInv = 1/(farClip-nearClip);
    float f = 1/std::tan(glm::radians(FOV)/2);

    return {
            f/aspect, 0,               0,              0,
            0,        f,               0,              0,
            0,        0, (nearClip+farClip)*rangeInv,  1,
            0,        0, -nearClip*farClip*rangeInv*2, 0
    };

    //return glm::perspectiveLH(glm::radians(FOV), aspect, nearClip, farClip);

    /*float d = farClip/(farClip-nearClip);
    float f = 1/std::tan(glm::radians(FOV)/2);

    return {
            f/aspect, 0,      0,      0,
            0,       -f,      0,      0,
            0,        0,      d,      1,
            0,        0, -nearClip*d, 0
    };*/
}

glm::mat4 Camera::viewMatrix() const {
    float cosx = std::cos(rotation.x);
    float cosy = std::cos(rotation.y);
    float sinx = std::sin(rotation.x);
    float siny = std::sin(rotation.y);

    return glm::inverse(glm::mat4(
            cosx,       0,         -sinx,       0,
           -sinx*siny,  cosy,      -cosx*siny,  0,
            sinx*cosy,  siny,       cosx*cosy,  0,
            position.x, position.y, position.z, 1
    ));

    //return glm::lookAtLH(position, {0,0,0}, {0,1,0});
}

void Camera::use(int bindingIndex /* = 0*/) const {
    ubo.projection = projectionMatrix();
    ubo.view = viewMatrix();
    ubo.position = {position, 1.0f};

    //load new cameraUBO struct values into camera buffer
    glNamedBufferSubData(cameraBufferID, 0, sizeof(CameraUBO), &ubo);

    //bind this buffer to current shader's uniform binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, cameraBufferID);
}

