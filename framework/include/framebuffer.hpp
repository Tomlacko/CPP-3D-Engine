#pragma once

#include <glad/glad.h>



class Framebuffer {

    GLuint bufferID = 0;
    GLuint colorBufferID = 0;
    GLuint depthBufferID = 0;

    int width;
    int height;

    static GLuint current; //initialized in .cpp

public:

    Framebuffer(int w, int h);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    void resize(int w, int h);

    void use() const;

    [[nodiscard]] int getWidth() const {return width;}
    [[nodiscard]] int getHeight() const {return height;}

    [[nodiscard]] bool isActive() const {return current == bufferID;}
    [[nodiscard]] GLuint getID() const {return bufferID;}
    [[nodiscard]] GLuint getColorBufferID() const {return colorBufferID;}
    [[nodiscard]] GLuint getDepthBufferID() const {return depthBufferID;}

    static GLuint getCurrentlyBoundID() {return current;}

    static void useDefault(const Framebuffer* drawFrom, int w = -1, int h = -1);
    static void drawScreenCoveringTriangle() {glDrawArrays(GL_TRIANGLES, 0, 3);}
};

