#include "object.hpp"
#include <texture.hpp>
#include <cmath>


//Mesh Object::cube = Mesh::cube();



void Object::init() {
    glCreateBuffers(1, &bufferID);
    //maybe not needed
    glNamedBufferStorage(bufferID, sizeof(ObjectUBO), nullptr/*&ubo*/, GL_DYNAMIC_STORAGE_BIT);
}

Object::~Object() {
    glDeleteBuffers(1, &bufferID);
}


void Object::changeRotBy(float x, float y) {
    rotation.x = std::fmod(rotation.x + x, PI2);
    rotation.y = std::fmod(rotation.y + y, PI2);
    modelMatrixOutdated = true;
}

void Object::setRot(float x, float y) {
    rotation.x = std::fmod(x, PI2);
    rotation.y = std::fmod(y, PI2);
    modelMatrixOutdated = true;
}

void Object::setRot(const glm::vec2& rot) {
    rotation.x = std::fmod(rot.x, PI2);
    rotation.y = std::fmod(rot.y, PI2);
    modelMatrixOutdated = true;
}

std::pair<glm::vec3, glm::vec3> Object::getAABB() const {
    float sx = (size.x/2) * scale.x;
    float sy = (size.y/2) * scale.y;
    float sz = (size.z/2) * scale.z;
    return {
            {position.x-sx, position.y-(centeredOnBottom?0:sy), position.z-sz},
            {position.x+sx, position.y+(centeredOnBottom?sy*2:sy), position.z+sz}
    };
}

void Object::updateModelMatrix() const {
    //translation * rotation.x (AXIS Y) * rotation.y (AXIS X) * scale
    //applied from right to left

    float sx = scale.x;
    float sy = scale.y;
    float sz = scale.z;

    float sinx = std::sin(rotation.x);
    float cosx = std::cos(rotation.x);
    float siny = std::sin(rotation.y);
    float cosy = std::cos(rotation.y);

    ubo.model_matrix = {
            sx*cosx, 0, -sx*sinx, 0,
            -sy*sinx*siny, sy*cosy, -sy*cosx*siny, 0,
            sz*sinx*cosy, sz*siny, sz*cosx*cosy, 0,
            position.x, position.y, position.z, 1
    };

    modelMatrixOutdated = false;


    //STEPS TAKEN:

    //piecewise
    /*ubo.model_matrix = (
        glm::mat4(//translate
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            px, py, pz, 1
        ) * glm::mat4(//rotate.x (Y axis)
            std::cos(rx), 0, -std::sin(rx), 0,
            0, 1, 0, 0,
            std::sin(rx), 0, std::cos(rx), 0,
            0, 0, 0, 1
        ) * glm::mat4(//rotate.y (X axis)
            1, 0, 0, 0,
            0, std::cos(ry), -std::sin(ry), 0,
            0, std::sin(ry), std::cos(ry), 0,
            0, 0, 0, 1
        ) * glm::mat4(//scale
            sx, 0, 0, 0,
            0, sy, 0, 0,
            0, 0, sz, 0,
            0, 0, 0, 1
        )
    );*/

    //multiplied in reverse order (by mistake lol)
    /*ubo.model_matrix = {
            sx*cosx, -sy*sinx*siny, -sz*sinx*cosy, 0,
            0, sy*cosy, -sz*siny, 0,
            sx*sinx, sy*cosx*siny, sz*cosx*cosy, 0,
            sx*(px*cosx + pz*sinx), sy*(py*cosy + siny*(pz*cosx-px*sinx)), sz*(-py*siny + cosy*(pz*cosx-px*sinx)), 1
    };*/
}

void Object::render(float time) const {
    bool wasMatrixOutdated = modelMatrixOutdated; //because updating it changes the status
    if(wasMatrixOutdated) updateModelMatrix();
    if(wasMatrixOutdated || uboOutdated) glNamedBufferSubData(bufferID, 0, sizeof(ObjectUBO), &ubo);
    uboOutdated = false;

    //bind UBO buffer (2 = binding within shader)
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, bufferID);

    if(texture) texture->use(0);

    glUniform1i(1, rightHanded?-1:1);

    //temporary
    cube.draw();
    for(const auto& mesh : geometry) {
        mesh->draw();
    }
}