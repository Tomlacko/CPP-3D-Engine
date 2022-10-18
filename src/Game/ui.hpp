#pragma once

#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>
#include <texture.hpp>


class UIElement {

protected:
    Texture* texture = nullptr;
    glm::vec2 position;
    glm::vec2 scale {1, 1};

    mutable bool uboOutdated = true;


public:

    UIElement() {

    }


    virtual void draw() const = 0;
};



class ImageElement: public UIElement {

public:

    void draw() const override;
};



class TextElement: public UIElement {

public:

    void draw() const override;
};




class UI {
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
};