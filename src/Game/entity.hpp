#pragma once


//#include <glm/glm.hpp>
#include "object.hpp"
#include <utilities.hpp>




class Entity: public Object {
protected:
    glm::vec3 motion;
    glm::vec3 forces;
    glm::vec3 impulses;

    bool onGround = false;
    bool gravity = true;

    void init();

public:
    Entity(const glm::vec3& pos, const glm::vec2& rot, const glm::vec3& sz): Object(pos, rot, sz) {
        init();
    }
    Entity(const glm::vec3& pos, const glm::vec2& rot): Object(pos, rot) {
        init();
    }

    glm::vec3 getLookDir() const {
        return rotToDir(rotation);
    }

    virtual float getAirDrag() const {return 0.9;}
    virtual float getFriction() const {return 0.4;}
    float getResistance() const {return onGround ? getFriction() : getAirDrag();}

    void setMotion(float x, float y, float z) {
        motion = {x, y, z};
    }
    void setMotion(const glm::vec3& mot) {
        motion = mot;
    }

    void addImpulse(const glm::vec3& dir, float amount);
    void addImpulse(const glm::vec3& impulse);
    void addImpulse(const glm::vec2& angle, float amount) {
        addImpulse(rotToDir(angle), amount);
    }

    void addForce(const glm::vec3& dir, float amount);
    void addForce(const glm::vec3& force);
    void addForce(const glm::vec2& angle, float amount) {
        addForce(rotToDir(angle), amount);
    }


    virtual void tick(float deltaTime);
};