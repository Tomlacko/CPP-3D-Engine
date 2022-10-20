#include "entity.hpp"
#include <utilities.hpp>
#include <cmath>


void Entity::init() {
    Object::init();

}


glm::vec3 Entity::getLookDir() const {
    return rotToDir(rotation);
}


void Entity::addImpulse(const glm::vec3& dir, float amount) {
    glm::vec3 norm = glm::normalize(dir);
    if(!glm::any(glm::isnan(norm))) {
        impulses += norm*amount;
    }
}
void Entity::addImpulse(const glm::vec3& impulse) {
    impulses += impulse;
}
void Entity::addImpulse(const glm::vec2& angle, float amount) {
    addImpulse(rotToDir(angle), amount);
}

void Entity::addForce(const glm::vec3& dir, float amount) {
    glm::vec3 norm = glm::normalize(dir);
    if(!glm::any(glm::isnan(norm))) {
        forces += norm*amount;
    }
}
void Entity::addForce(const glm::vec3& force) {
    forces += force;
}
void Entity::addForce(const glm::vec2& angle, float amount) {
    addForce(rotToDir(angle), amount);
}


void Entity::preTick(float deltaTime) {
    motion += impulses/*/mass*/ + (forces/mass)*deltaTime;
    posChange += motion*deltaTime;
}

void Entity::midTick(float deltaTime) {

}

void Entity::postTick(float deltaTime) {
    changePosBy(posChange);

    //fake floor
    /*if(position.y<=0) {
        position.y = 0; //pos was already changed so it knows it was modified
        motion.y = 0;
        onGround = true;
    }
    else {
        onGround = false;
    }*/

    forces = {0,(gravity ? -9.8*mass : 0),0};
    impulses = {0,0,0};
    posChange = {0,0,0};

    motion /= std::pow(2, deltaTime*(airResistance+(onGround?friction:0)));
    //addForce(-motion*glm::abs(motion)*(airResistance));

    onGround = false;
}

void Entity::tick(float deltaTime) {
    preTick(deltaTime);
    midTick(deltaTime);
    postTick(deltaTime);
}

bool Entity::checkCollision(Object* obj) {
    Hitbox a = getHitbox();
    Hitbox b = obj->getHitbox();

    if(!a.intersecting(b)) return false;

    glm::vec3 pushVec = a.getPushOutVector(b);

    if(pushVec.x!=0) motion.x = 0;
    if(pushVec.y!=0) motion.y = 0;
    if(pushVec.z!=0) motion.z = 0;

    if(pushVec.y>0) onGround = true;

    changePosBy(pushVec);

    return true;
}