#include "player.hpp"
#include <utilities.hpp>
#include <cmath>
#include <glm/gtx/vector_angle.hpp>

#include <debug.hpp>




void Player::init() {
    Entity::init();

    mass = 80;

    //flyMode(true);
}


void Player::changeRotBy(float x, float y) {
    rotation.x = mod(rotation.x + x, PI2);
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
        if(moveH) movementControls += glm::vec3( //flying towards cursor
            std::sin(rotation.x+moveAngle)*std::cos(rotation.y),
            std::sin(rotation.y)*std::cos(moveAngle),
            std::cos(rotation.x+moveAngle)*std::cos(rotation.y)
        );
        if(moveV) movementControls.y += dir.y; //flying up and down
    }
    else {
        if(moveH) movementControls += glm::vec3( //horizontal walking
            std::sin(rotation.x+moveAngle),
            0,
            std::cos(rotation.x+moveAngle)
        );
        if(onGround && dir.y==1) addImpulse({0,1,0}, jumpStrength); //jumping
    }
    if(glm::length(movementControls)>1) movementControls = glm::normalize(movementControls);

    //TODO fix A D flying movement
}

void Player::flyMode(bool activate) {
    onGround = false;
    if(activate) {
        gravity = false;
        flying = true;

        motion = {0,0,0};
        forces = {0,0,0};
        impulses = {0,0,0};
    }
    else { //walking
        gravity = true;
        flying = false;
    }
}

void Player::preTick(float deltaTime) {
    Entity::preTick(deltaTime);
    posChange += movementControls*(running?runSpeed:speed)*deltaTime;
}

void Player::midTick(float deltaTime) {
    Entity::midTick(deltaTime);
}

void Player::postTick(float deltaTime) {
    movementControls /= std::pow(2, deltaTime*(movementSlowdown/(onGround?1:4)));

    if(glm::length(movementControls) < 0.05) movementControls = {0,0,0};

    Entity::postTick(deltaTime);
}

void Player::tick(float deltaTime) {
    preTick(deltaTime);
    midTick(deltaTime);
    postTick(deltaTime);
    DEBUG_LOG("Position: ");
    DEBUG_LOG_SEP(position.x);
    DEBUG_LOG_SEP(position.y);
    DEBUG_LOG_SEP(position.z);
    DEBUG_LOG("Motion: ");
    DEBUG_LOG_SEP(motion.x);
    DEBUG_LOG_SEP(motion.y);
    DEBUG_LOG_LN(motion.z);
}