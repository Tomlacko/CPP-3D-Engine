#include "player.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include <utilities.hpp>



void Player::init() {
    Entity::init();

    //flyMode(true);
}


void Player::changeRotBy(float x, float y) {
    rotation.x = std::fmod(rotation.x + x, PI2);
    rotation.y = clamp(minAngleV, rotation.y + y, maxAngleV);
}

void Player::setRot(float x, float y) {
    rotation.x = std::fmod(x, PI2);
    rotation.y = clamp(minAngleV, y, maxAngleV);
}

void Player::setRot(const glm::vec2& rot) {
    rotation.x = std::fmod(rot.x, PI2);
    rotation.y = clamp(minAngleV, rot.y, maxAngleV);
}

//vec3 dir: unnormalized keyboard direction vector
void Player::move(const glm::vec3& dir) {
    static const glm::vec2 BASE {0, 1};

    if(frozen) return;

    bool moveH = true;
    bool moveV = true;

    float moveAngle;
    glm::vec2 normDir = glm::normalize(glm::vec2(dir.x, dir.z));
    if(glm::any(glm::isnan(normDir)))
        moveH = false;
    else
        moveAngle = glm::orientedAngle(normDir, BASE);

    if(dir.y==0)
        moveV = false;

    if(flying) {
        if(moveH) addForce({
            std::sin(rotation.x+moveAngle)*std::cos(rotation.y),
            std::sin(rotation.y)*std::cos(moveAngle),
            std::cos(rotation.x+moveAngle)*std::cos(rotation.y)
        }, speed);
        if(moveV) addForce({0, dir.y, 0}, speed); //flying up and down
    }
    else {
        if(moveH) addForce({
            std::sin(rotation.x+moveAngle),
            0,
            std::cos(rotation.x+moveAngle)
        }, speed);
        if(onGround && dir.y==1) addImpulse({0,1,0}, jumpStrength); //jumping
    }
}

void Player::flyMode(bool activate) {
    if(activate) {
        gravity = false;
        flying = true;
        onGround = false;
    }
    else { //walking
        gravity = true;
        flying = false;
    }
}

void Player::tick(float deltaTime) {
    Entity::tick(deltaTime);

}