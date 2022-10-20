#include "hitbox.hpp"
#include <cmath>

/*
glm::vec3 Hitbox::pushPointOut(const glm::vec3& p) const {

}*/


glm::vec3 Hitbox::getPushOutVector(Hitbox& h) {
    //no intersection
    if((h.a.x >= b.x || a.x > h.b.x) || (h.a.y > b.y || a.y > h.b.y) || (h.a.y > b.y || a.y > h.b.y)) return {0,0,0};

    glm::vec3 c = getCenter();
    glm::vec3 hc = h.getCenter();

    float iX = hc.x>c.x ? b.x-h.a.x : h.b.x-a.x;
    float iY = hc.y>c.y ? b.y-h.a.y : h.b.y-a.y;
    float iZ = hc.z>c.z ? b.z-h.a.z : h.b.z-a.z;
    /*float iX = std::min(b.x, h.b.x) - std::max(a.x, h.a.x);
    float iY = std::min(b.y, h.b.y) - std::max(a.y, h.a.y);
    float iZ = std::min(b.z, h.b.z) - std::max(a.z, h.a.z);*/

    glm::vec3 dir = glm::sign(c - hc); //subtracted in reverse order to get opposite direction

    //separate in direction of smallest intersection (this object's point of view)
    glm::vec3 sep;
    if(iY <= iX && iY <= iZ) sep = {0, iY*dir.y, 0};
    else if(iX <= iY && iX <= iZ) sep = {iX*dir.x, 0, 0};
    else sep = {0, 0, iZ*dir.z};

    return sep;
}

glm::vec3 Hitbox::separate(Hitbox& h) {
    //immoveable objects
    if(!moveable && !h.moveable) return {0,0,0};

    glm::vec3 pushout = getPushOutVector(h);

    if(moveable && h.moveable) pushout /= 2.0f;


    if(moveable) move(pushout);
    if(h.moveable) h.move(-pushout);

    return moveable ? pushout : glm::vec3(0,0,0);
}

void Hitbox::move(const glm::vec3& amount) {
    a+=amount;
    b+=amount;
}