#pragma once


//#include <glm/glm.hpp>
#include "entity.hpp"



class Player: public Entity {

protected:

    float eyeHeight = 1.7;
    float speed = 25;
    float jumpStrength = 5;

    float maxAngleV = PI/2;
    float minAngleV = -PI/2;

    bool flying = false;
    bool frozen = false;


    void init();

public:
    static constexpr glm::vec3 BODY_SIZE {0.5, 1.8, 0.5};

    Player(const glm::vec3& pos, const glm::vec2& rot): Entity(pos, rot, BODY_SIZE) {
        init();
    }

    void flyMode(bool activate);
    [[nodiscard]] bool isFlying() const {return flying;}

    void freeze() {frozen = true;}
    void unfreeze() {frozen = false;}
    [[nodiscard]] bool isFrozen() const {return frozen;}

    [[nodiscard]] glm::vec3 getEyePos() const {
        return {position.x, position.y+eyeHeight, position.z};
    }

    [[nodiscard]] float getAirDrag() const override {return flying ? 0.1 : 0.005;}
    [[nodiscard]] float getFriction() const override {return flying ? 0.1 : 0.001;}


    void changeRotBy(float x, float y) override;
    void setRot(float x, float y) override;
    void setRot(const glm::vec2& rot) override;

    void move(const glm::vec3& dir);

    void tick(float deltaTime) override;
};