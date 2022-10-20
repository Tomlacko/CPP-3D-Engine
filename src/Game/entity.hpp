#pragma once


//#include <glm/glm.hpp>
#include "object.hpp"




class Entity: public Object {
protected:
    glm::vec3 motion;
    glm::vec3 forces;
    glm::vec3 impulses;

    glm::vec3 posChange;


    float airResistance = 0.05;
    float friction = 10;

    float mass = 20;

    bool onGround = false;
    bool gravity = true;
    bool frozen = false;



    void init();

public:
    Entity(const glm::vec3& pos, const glm::vec2& rot, const glm::vec3& sc = {1,1,1}, Meshes* geom = nullptr, TextureGroup* texGrp = nullptr): Object(pos, rot, sc, geom, texGrp) {
        init();
    }
    Entity(const glm::vec3& pos, const glm::vec2& rot, const glm::vec3& sc, const glm::vec3& sz, TextureGroup* texGrp = nullptr): Object(pos, rot, sc, sz, texGrp) {
        init();
    }

    [[nodiscard]] glm::vec3 getLookDir() const;

    void freeze() {frozen = true;}
    void unfreeze() {frozen = false;}
    [[nodiscard]] bool isFrozen() const {return frozen;}


    void setMotion(float x, float y, float z) {
        motion = {x, y, z};
    }
    void setMotion(const glm::vec3& mot) {
        motion = mot;
    }

    void addImpulse(const glm::vec3& dir, float amount);
    void addImpulse(const glm::vec3& impulse);
    void addImpulse(const glm::vec2& angle, float amount);

    void addForce(const glm::vec3& dir, float amount);
    void addForce(const glm::vec3& force);
    void addForce(const glm::vec2& angle, float amount);

    virtual void preTick(float deltaTime);
    virtual void midTick(float deltaTime);
    virtual void postTick(float deltaTime);
    virtual void tick(float deltaTime);

    virtual bool checkCollision(Object* obj);
};