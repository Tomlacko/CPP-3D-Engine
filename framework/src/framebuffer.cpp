#include "framebuffer.hpp"



//default/clear colors (undrawn background)
const float default_color[4] = {0.698, 0.867, 1.0, 1.0};
const float default_depth[1] = {1.0};


//default frame buffer value
GLuint Framebuffer::current = 0;


Framebuffer::Framebuffer(int w, int h): width(w), height(h) {
    glCreateFramebuffers(1, &bufferID);

    //initialize color output texture
    glCreateTextures(GL_TEXTURE_2D, 1, &colorBufferID);
    glTextureStorage2D(colorBufferID, 1, GL_RGBA32F, width, height);

    //initialize depth output texture
    glCreateTextures(GL_TEXTURE_2D, 1, &depthBufferID);
    glTextureStorage2D(depthBufferID, 1, GL_DEPTH_COMPONENT32F, width, height);

    //associate color and depth attachments with color and depth textures
    glNamedFramebufferTexture(bufferID, GL_COLOR_ATTACHMENT0, colorBufferID, 0);
    glNamedFramebufferTexture(bufferID, GL_DEPTH_ATTACHMENT, depthBufferID, 0);
}


Framebuffer::~Framebuffer() {
    glDeleteTextures(1, &colorBufferID);
    glDeleteTextures(1, &depthBufferID);
    glDeleteFramebuffers(1, &bufferID);
}


void Framebuffer::resize(int w, int h) {
    width = w;
    height = h;

    //recreate and reattach all framebuffer's textures
    glDeleteTextures(1, &colorBufferID);
    glDeleteTextures(1, &depthBufferID);

    //re-initialize color output texture
    glCreateTextures(GL_TEXTURE_2D, 1, &colorBufferID);
    glTextureStorage2D(colorBufferID, 1, GL_RGBA32F, width, height);

    //re-initialize depth output texture
    glCreateTextures(GL_TEXTURE_2D, 1, &depthBufferID);
    glTextureStorage2D(depthBufferID, 1, GL_DEPTH_COMPONENT32F, width, height);

    //re-associate color and depth attachments with color and depth textures
    glNamedFramebufferTexture(bufferID, GL_COLOR_ATTACHMENT0, colorBufferID, 0);
    glNamedFramebufferTexture(bufferID, GL_DEPTH_ATTACHMENT, depthBufferID, 0);
}


void Framebuffer::use() const {
    //bind the Framebuffer to be drawn into
    glBindFramebuffer(GL_FRAMEBUFFER, bufferID);

    //clear color and depth attachments
    glClearNamedFramebufferfv(bufferID, GL_COLOR, 0, default_color);
    glClearNamedFramebufferfv(bufferID, GL_DEPTH, 0, default_depth);

    current = bufferID;
}

void Framebuffer::useDefault(const Framebuffer* drawFrom, int w /* = -1*/, int h /* = -1*/) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(drawFrom) {
        if(w==-1 || h==-1) glViewport(0, 0, drawFrom->getWidth(), drawFrom->getHeight());
        else glViewport(0, 0, w, h);

        //1st param is binding location in shader
        glBindTextureUnit(0, drawFrom->getColorBufferID());
        glBindTextureUnit(1, drawFrom->getDepthBufferID());
    }
    else {
        if(w==-1 || h==-1) throw "Cannot use default framebuffer - size not specified!";
        else glViewport(0, 0, w, h);
    }

    current = 0;
}