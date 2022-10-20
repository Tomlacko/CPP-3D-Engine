#include <cmath>
#include <utilities.hpp>
#include "texture_group.hpp"
#include "texture.hpp"


void TextureGroup::init() {
    total = 0;
    for(const auto& texFramesVec : texFramesVecs) {
        unsigned int s = texFramesVec.size();
        if(s>total) total = s;
    }
}

void TextureGroup::update(float deltaTime) {
    if(total!=0 && animFrequency!=0 && deltaTime!=0) {
        float t = deltaTime+accumTime;
        int c = std::floor(t/animFrequency); //how many frames to advance
        current = mod(current+c, total); //set new current frame, clamped to range
        accumTime = t - animFrequency*c; //x mod y == x-y*floor(x/y)
    }
}

void TextureGroup::use(unsigned int textureInitialBindingPoint /* = 0*/, unsigned int maskUniformLocation /* = 0*/) const {
    unsigned int MASK = 0;
    if(total>0) { //if no textures initialized, none to bind
        for(unsigned int typeID=0; typeID<TEXTURE_TYPES_COUNT; typeID++) {
            if(texFramesVecs[typeID].size()>current) {
                texFramesVecs[typeID][current].use(textureInitialBindingPoint+typeID);
                MASK |= (1u<<typeID);
            }
        }
    }

    glUniform1ui(maskUniformLocation, MASK);
}