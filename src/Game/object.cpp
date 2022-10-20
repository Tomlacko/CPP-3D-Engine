#include "object.hpp"
#include <texture.hpp>
#include <cmath>
#include <limits>

#include <glm/gtx/rotate_vector.hpp>



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

Hitbox Object::getHitbox() const {
    glm::vec3 a;
    glm::vec3 b;
    if(geometry) {
        float ax = std::numeric_limits<float>::infinity();
        float ay = ax; float az = ax;
        float bx = -ax; float by = bx; float bz = bx;

        for(const auto& m : *geometry) {
            glm::vec3 v = m->minCorner;
            if(v.x < ax) ax = v.x;
            if(v.y < ay) ay = v.y;
            if(v.z < az) az = v.z;

            if(v.x > bx) bx = v.x;
            if(v.y > by) by = v.y;
            if(v.z > bz) bz = v.z;

            v = m->maxCorner;
            if(v.x < ax) ax = v.x;
            if(v.y < ay) ay = v.y;
            if(v.z < az) az = v.z;

            if(v.x > bx) bx = v.x;
            if(v.y > by) by = v.y;
            if(v.z > bz) bz = v.z;
        }

        a = {ax, ay, az};
        b = {bx, by, bz};

        //a = geometry->front()->minCorner;
        //b = geometry->front()->maxCorner;
    }
    else {
        glm::vec3 size2 = size/2.0f;
        a = -size2;
        b = size2;
        if(centeredOnBottom) {
            a.y = 0;
            b.y = size.y;
        }
    }

    a *= scale;
    b *= scale;

    if(geometry) {
        glm::vec3 verts[8] {
                a,
                {a.x, a.y, b.z},
                {a.x, b.y, a.z},
                {a.x, b.y, b.z},
                {b.x, a.y, a.z},
                {b.x, a.y, b.z},
                {b.x, b.y, a.z},
                b
        };

        float ax = std::numeric_limits<float>::infinity();
        float ay = ax; float az = ax;
        float bx = -ax; float by = bx; float bz = bx;

        for(glm::vec3& v : verts) {
            v = glm::rotateY(glm::rotateX(v, rotation.y), rotation.x);

            if(v.x < ax) ax = v.x;
            if(v.y < ay) ay = v.y;
            if(v.z < az) az = v.z;

            if(v.x > bx) bx = v.x;
            if(v.y > by) by = v.y;
            if(v.z > bz) bz = v.z;
        }

        a = {ax, ay, az};
        b = {bx, by, bz};
    }
    return Hitbox(a+position, b+position, false);
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
    if(!visible) return;

    bool wasMatrixOutdated = modelMatrixOutdated; //because updating it changes the status
    if(wasMatrixOutdated) updateModelMatrix();
    if(wasMatrixOutdated || uboOutdated) glNamedBufferSubData(bufferID, 0, sizeof(ObjectUBO), &ubo);
    uboOutdated = false;

    //bind UBO buffer (2 = binding within shader)
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, bufferID);

    if(textures) textures->use();
    else glUniform1ui(0, 0u); //bind mask 0

    if(geometry) {
        for(const auto& mesh : *geometry) {
            mesh->draw();
        }
    }
}