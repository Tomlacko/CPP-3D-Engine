#include "ui.hpp"



UIImageElement::UIImageElement(UI* parentUI, glm::vec2 anchoring, glm::ivec4 texCoords, glm::vec2 pos, glm::vec2 scale)
: UIElement(parentUI, anchoring, pos, scale), texCoords(texCoords) {

}


UITextElement::UITextElement(UI* parentUI, const std::string& text, glm::vec2 anchoring, glm::vec2 pos, glm::vec2 scale)
: UIElement(parentUI, anchoring, pos, scale), text(text) {
    glCreateBuffers(1, &bufferID);
}

UITextElement::~UITextElement() {
    glDeleteBuffers(1, &bufferID);
}



void UIElement::hide() {
    visible = false;
}
void UIElement::show() {
    visible = true;
}

void UIImageElement::hide() {
    UIElement::hide();
    ui->outdatedUBOs = true;
}

void UIImageElement::show() {
    UIElement::show();
    ui->outdatedUBOs = true;
}


void UITextElement::setText(const std::string& newText) {
    text = newText;
    outdatedUBO = true;
}



void UIImageElement::updateUBO() const {
    ubo.position = position;
    ubo.scale = scale;
    ubo.texCoords = texCoords;
    ubo.anchoring = anchoring;
}

void UITextElement::updateUBO() const {
    static constexpr const int LETTER_SIZE = 8;
    static constexpr const int LETTERS_PER_LINE = 16;

    letterUBOs.clear();


    float aspectRatio = (float)ui->getScreenHeight()/ui->getScreenWidth();
    int shorterSide = std::min(ui->getScreenWidth(), ui->getScreenHeight());

    glm::vec2 step {LETTER_SIZE, LETTER_SIZE};
    step = (step/(float)shorterSide)*scale*2.0f;
    if(aspectRatio<1) step.x *= aspectRatio;
    else step.y /= aspectRatio;
    step.y *= 1.5; //line spacing

    glm::vec2 pos {0, 0};
    int gridX, gridY;

    //TODO anchoring whole text segment

    for(unsigned char letter : text) {
        if(letter=='\n') {
            pos.x = 0;
            pos.y-=1;
            continue;
        }

        if(letter!=' ') {
            gridX = (letter % LETTERS_PER_LINE) * LETTER_SIZE;
            gridY = (letter / LETTERS_PER_LINE) * LETTER_SIZE;

            letterUBOs.push_back({position + step*pos, scale, {gridX, gridY, gridX+LETTER_SIZE, gridY+LETTER_SIZE}, anchoring});
        }

        pos.x+=1;
    }

    //load it into buffer
    glNamedBufferData(bufferID, letterUBOs.size()*sizeof(UIElementUBO), letterUBOs.data(), GL_DYNAMIC_DRAW);

    outdatedUBO = false;
}


void UIImageElement::getData(std::vector<UIElementUBO>& output) const {
    if(!visible) return;

    if(outdatedUBO) updateUBO();

    output.push_back(ubo);
}


bool UITextElement::draw() const {
    if(!visible) return false;

    if(outdatedUBO) updateUBO();


    //bind letter buffer into shader (0 = binding point)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferID);

    //draw image elements (as instances)
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, letterUBOs.size());

    return true;
}



UI::UI(int screenWidth, int screenHeight): screenWidth(screenWidth), screenHeight(screenHeight) {
    glCreateBuffers(1, &imageElementsBufferID);
    //glNamedBufferStorage(UIBufferID, elementUBOs.size()*sizeof(UIElementUBO), nullptr/*lights.data()*/, GL_DYNAMIC_STORAGE_BIT);
}

UI::~UI() {
    glDeleteBuffers(1, &imageElementsBufferID);
}



void UI::addImageElement(const std::string& name, glm::vec2 anchoring, glm::ivec4 texCoords, glm::vec2 pos, glm::vec2 scale /* = {1, 1}*/) {
    outdatedUBOs = true;
    imageElements.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(this, anchoring, texCoords, pos, scale));
}

void UI::addTextElement(const std::string& name, const std::string& text, glm::vec2 anchoring, glm::vec2 pos, glm::vec2 scale /* = {1, 1}*/) {
    textElements.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(this, text, anchoring, pos, scale));
}

UIImageElement* UI::getImageElement(const std::string& name) {
    auto it = imageElements.find(name);
    if(it == imageElements.end()) return nullptr;
    return &it->second;
}
UITextElement* UI::getTextElement(const std::string& name) {
    auto it = textElements.find(name);
    if(it == textElements.end()) return nullptr;
    return &it->second;
}


void UI::updateUBOs() const {
    //ask each element for new data
    imageElementUBOs.clear();
    for(const auto& [name, elem] : imageElements) {
        elem.getData(imageElementUBOs);
    }

    //load it into buffer
    glNamedBufferData(imageElementsBufferID, imageElementUBOs.size() * sizeof(UIElementUBO), imageElementUBOs.data(), GL_DYNAMIC_DRAW);
    //glNamedBufferSubData(UIBufferID, 0, elementUBOs.size()*sizeof(UIElementUBO), elementUBOs.data());

    outdatedUBOs = false;
}


void UI::render(float time) const {
    if(imageElements.empty() && textElements.empty()) return;

    progUI.use();

    glUniform2i(0, screenWidth, screenHeight);
    glUniform1f(1, time);
    //glUniform1ui(2, 0u); //settingFlags, 0=img, 1=text


    //---image elements---
    if(!imageElements.empty()) {
        guiTexture.use(0);

        if(outdatedUBOs) updateUBOs();

        //bind buffer into shader (0 = binding point)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, imageElementsBufferID);

        //draw image elements (as instances)
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, imageElementUBOs.size());
    }


    //---text elements---
    if(!textElements.empty()) {
        fontTexture.use(0);

        for(const auto& [name, elem] : textElements) {
            elem.draw();
        }
    }
}

UIElement* UI::click(float x, float y) {
    //TODO
    return nullptr;
}

void UI::resize(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    for(const auto& [name, elem] : textElements) {
        elem.outdatedUBO = true;
    }
}