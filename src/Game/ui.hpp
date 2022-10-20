#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <texture.hpp>
#include <program.hpp>


class UI;

enum class UIElementType {
    ImageElement, TextElement
};



struct UIElementUBO {
    glm::vec2 position;
    glm::vec2 scale;
    glm::ivec4 texCoords;
    glm::vec2 anchoring;
    glm::vec2 _padding {0,0};
};


//////////////////////////////////////////////////



class UIElement {

protected:
    glm::vec2 position;
    glm::vec2 scale {1, 1};
    glm::vec2 anchoring;

    bool visible = true;



    UIElement(UI* parentUI, glm::vec2 anchoring, glm::vec2 pos, glm::vec2 scale)
    : position(pos), scale(scale), anchoring(anchoring), ui(parentUI) {
    }

    virtual void updateUBO() const = 0;

public:

    mutable bool outdatedUBO = true;
    UI* ui;


    virtual ~UIElement() = default;
/*
    UIElement(const UIElement&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    UIElement(UIElement&&) = default;
    UIElement& operator=(UIElement&&) = default;*/


    [[nodiscard]] virtual UIElementType getType() const = 0;

    [[nodiscard]] bool isVisible() const {return visible;}
    virtual void hide();
    virtual void show();
};





class UIImageElement: public UIElement {

    glm::ivec4 texCoords;

    mutable UIElementUBO ubo;


    void updateUBO() const override;

public:

    UIImageElement(UI* parentUI, glm::vec2 anchoring, glm::ivec4 texCoords, glm::vec2 pos, glm::vec2 scale = {1, 1});

    UIImageElement(const UIImageElement&) = delete;
    UIImageElement& operator=(const UIImageElement&) = delete;
    UIImageElement(UIImageElement&&) = default;
    UIImageElement& operator=(UIImageElement&&) = default;


    [[nodiscard]] UIElementType getType() const override {return UIElementType::ImageElement;}

    void hide() override;
    void show() override;

    void getData(std::vector<UIElementUBO>& output) const;

};



class UITextElement: public UIElement {

    GLuint bufferID = 0;
    mutable std::vector<UIElementUBO> letterUBOs;

    std::string text;

    void updateUBO() const override;

public:

    UITextElement(UI* parentUI, const std::string& text, glm::vec2 anchoring, glm::vec2 pos, glm::vec2 scale = {1, 1});
    ~UITextElement() override;

    UITextElement(const UITextElement&) = delete;
    UITextElement& operator=(const UITextElement&) = delete;
    UITextElement(UITextElement&&) = default;
    UITextElement& operator=(UITextElement&&) = default;

    [[nodiscard]] UIElementType getType() const override {return UIElementType::TextElement;}

    void setText(const std::string& newText);


    bool draw() const;
};



////////////////////////////////////////////////////////////////////


class UI {
    int screenWidth;
    int screenHeight;

    Texture fontTexture {"textures/font.png", false};
    Texture guiTexture {"textures/gui.png", false};

    GLProgram progUI {"shaders/ui.vert", "shaders/ui.frag"};

    std::unordered_map<std::string, UIImageElement> imageElements;
    std::unordered_map<std::string, UITextElement> textElements;


    GLuint imageElementsBufferID = 0;

    mutable std::vector<UIElementUBO> imageElementUBOs;



    void updateUBOs() const;

public:
    mutable bool outdatedUBOs = true;


    UI(int screenWidth, int screenHeight);
    ~UI();

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;


    void addImageElement(const std::string& name, glm::vec2 anchoring, glm::ivec4 texCoords, glm::vec2 pos, glm::vec2 scale = {1, 1});
    void addTextElement(const std::string& name, const std::string& text, glm::vec2 anchoring, glm::vec2 pos, glm::vec2 scale = {1, 1});

    UIImageElement* getImageElement(const std::string& name);
    UITextElement* getTextElement(const std::string& name);

    UIElement* click(float x, float y);

    void render(float time) const;


    void resize(int screenWidth, int screenHeight);
    int getScreenWidth() const {return screenWidth;}
    int getScreenHeight() const {return screenHeight;}
};


