#include "entity.hpp"
#include <cmath>



void Entity::init() {
    Object::init();

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

void Entity::addForce(const glm::vec3& dir, float amount) {
    glm::vec3 norm = glm::normalize(dir);
    if(!glm::any(glm::isnan(norm))) {
        forces += norm*amount;
    }
}
void Entity::addForce(const glm::vec3& force) {
    forces += force;
}

void Entity::tick(float deltaTime) {
    if(gravity) addForce({0, -9.87, 0});

    float resistance = std::pow(std::min(getResistance(), 1.0f), deltaTime);


    if(gravity) {
        motion.x *= resistance;
        motion.y *= std::pow(std::min(getResistance(), 1.0f), deltaTime/10);
        motion.z *= resistance;
    }
    else motion *= resistance;

    //motion -= motion*getResistance()*deltaTime;
    motion += forces*deltaTime + impulses;
    changePosBy(motion*deltaTime);

    if(position.y<=0) {
        position.y = 0; //pos was already changed so it knows it was modified
        motion.y = 0;
        onGround = true;
    }
    else {
        onGround = false;
    }



    impulses = {0,0,0};
    forces = {0,0,0};
}