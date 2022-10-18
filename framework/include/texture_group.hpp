#pragma once

#include <vector>
#include <string>

#include <texture.hpp>



static constexpr unsigned int TEXTURE_TYPES_COUNT = 4;
static constexpr char const* TEXTURE_TYPE_NAMES[TEXTURE_TYPES_COUNT] {"diffuse", "normal", "height", "specular"};



class TextureGroup {
    unsigned int total = 0;
    unsigned int current = 0;
    float accumTime = 0;

    std::vector<Texture> texFramesVecs[TEXTURE_TYPES_COUNT];

public:
    float animFrequency = 0; //seconds



    explicit TextureGroup(float animFreq = 0): animFrequency(animFreq) {
    }
    std::vector<Texture>& type(unsigned int typeID) {return texFramesVecs[typeID];}
    void init();


    void setCurrentFrame(unsigned int cur) {current = cur%total;}
    [[nodiscard]] unsigned int getCurrentFrame() const {return current;}

    void update(float deltaTime);
    void use(unsigned int textureInitialBindingPoint = 0, unsigned int maskUniformLocation = 0) const;
};