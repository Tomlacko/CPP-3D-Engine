#pragma once


//#include <glm/glm.hpp>
#include "entity.hpp"



class Player: public Entity {

protected:

    float maxAngleV = PI/2;
    float minAngleV = -PI/2;
    float eyeHeight = 1.8;
    float speed = 3;
    float runSpeed = 5;
    float jumpStrength = 5;
    float movementSlowdown = 10;
    glm::vec3 movementControls;

    bool flying = false;
    bool running = false;


    void init();

public:

    Player(const glm::vec3& pos, const glm::vec2& rot): Entity(pos, rot, {1,1,1}, {0.8, 1.9, 0.8}) {
        init();
    }

    void flyMode(bool activate);
    [[nodiscard]] bool isFlying() const {return flying;}

    void setRunning(bool activate) {running = activate;}
    [[nodiscard]] bool isRunning() const {return running;}


    [[nodiscard]] glm::vec3 getEyePos() const {
        return {position.x, position.y+eyeHeight, position.z};
    }


    void changeRotBy(float x, float y) override;
    void setRot(float x, float y) override;
    void setRot(const glm::vec2& rot) override;

    void move(const glm::vec3& dir);

    void preTick(float deltaTime) override;
    void midTick(float deltaTime) override;
    void postTick(float deltaTime) override;
    void tick(float deltaTime) override;
};