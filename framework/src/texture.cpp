#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cmath>


Texture::Texture(const std::string& filename, bool interpl /* = true*/, bool loadInGL /* = true*/, bool delData /* = true*/)
:pixeldata(stbi_load(filename.c_str(), &width, &height, &channels, 4)), interpolated(interpl) {
    if(!pixeldata) {
        throw std::runtime_error("Image failed to load!");
    }
    if(loadInGL) load(delData);
}

Texture::Texture(int w, int h, int ch /*= 4*/, bool interpl /*= false*/)
:width(w), height(h), channels(ch), pixeldata(std::make_unique<unsigned char[]>(w*h*ch)), interpolated(interpl) {
    if(!pixeldata) {
        throw std::runtime_error("Image failed to allocate!");
    }
}

void Texture::load(bool delData /* = true*/) {
    if(!pixeldata) throw std::runtime_error("Attempted to load texture with no data!");

    if(loadedInGL) glDeleteTextures(1, &bufferID);

    glCreateTextures(GL_TEXTURE_2D, 1, &bufferID);
    glTextureStorage2D(bufferID,
                       interpolated ? std::log2(width) : 1, //mipmaps
                       GL_RGBA8,
                       width, height);

    glTextureSubImage2D(bufferID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixeldata.get());

    //stbi_image_free(data); //already managed by unique_ptr instead
    if(delData) pixeldata.reset();

    if(interpolated) glGenerateTextureMipmap(bufferID);

    glTextureParameteri(bufferID, GL_TEXTURE_MIN_FILTER, interpolated ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
    glTextureParameteri(bufferID, GL_TEXTURE_MAG_FILTER, interpolated ? GL_LINEAR : GL_NEAREST);

    //default value = GL_REPEAT
    glTextureParameteri(bufferID, GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTextureParameteri(bufferID, GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
    glTextureParameteri(bufferID, GL_TEXTURE_WRAP_R , GL_CLAMP_TO_EDGE);

    loadedInGL = true;
}

/*GLuint Texture::getID() const {
    if(!loadedInGL) throw std::runtime_error("Attempted to get GL ID of unloaded texture!");
    return bufferID;
}
const GLuint* Texture::getIDptr() const {
    if(!loadedInGL) throw std::runtime_error("Attempted to get GL ID ptr of unloaded texture!");
    return &bufferID;
}*/

void Texture::use(int bindingUnit /* = 0*/) const {
    if(loadedInGL) glBindTextureUnit(bindingUnit, bufferID);
    else throw std::runtime_error("Attempted to use unloaded texture!");
}

Texture::~Texture() {
    if(loadedInGL) glDeleteTextures(1, &bufferID);
}