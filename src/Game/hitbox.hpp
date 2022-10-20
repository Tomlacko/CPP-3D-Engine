#pragma once

#include <glm/glm.hpp>




//a.xyz <= b.xyz

class Hitbox {


public:
    glm::vec3 a; //min corner
    glm::vec3 b; //max corner

    bool moveable;
    bool active = true;


    Hitbox(const glm::vec3& a, const glm::vec3& b, bool moveable, bool active = true): a(a), b(b), moveable(moveable), active(active) {
    }

    [[nodiscard]] glm::vec3 getCenter() const {
        return (a+b)/2.0f;
    }

    [[nodiscard]] bool pointInHitboxInclusive(const glm::vec3& p) const {
        return (a.x <= p.x && p.x <= b.x) && (a.y <= p.y && p.y <= b.y) && (a.z <= p.z && p.z <= b.z);
    }
    [[nodiscard]] bool pointInHitbox(const glm::vec3& p) const {
        return (a.x < p.x && p.x < b.x) && (a.y < p.y && p.y < b.y) && (a.z < p.z && p.z < b.z);
    }


    [[nodiscard]] bool intersectingInclusive(const Hitbox& h) const {
        return (a.x <= h.b.x && b.x >= h.a.x) && (a.y <= h.b.y && b.y >= h.a.y) && (a.z <= h.b.z && b.z >= h.a.z);
    }
    [[nodiscard]] bool intersecting(const Hitbox& h) const {
        return (a.x < h.b.x && b.x > h.a.x) && (a.y < h.b.y && b.y > h.a.y) && (a.z < h.b.z && b.z > h.a.z);
    }

    //glm::vec3 pushPointOut(const glm::vec3& p) const;

    glm::vec3 getPushOutVector(Hitbox& h);
    glm::vec3 separate(Hitbox& h);

    void move(const glm::vec3& amount);

};