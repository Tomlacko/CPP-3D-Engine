#pragma once

#include <glad/glad.h>
#include <string>
#include <memory>


class Texture {
    int width;
    int height;
    int channels;
    std::unique_ptr<unsigned char[]> pixeldata;
    bool interpolated;

    bool loadedInGL;
    GLuint bufferID;

public:

    Texture(int width, int height, int channels = 4, bool interpl = false);
    explicit Texture(const std::string& filename, bool interpl = true, bool loadInGL = true, bool delData = true);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    ~Texture();


    void load(bool delData = true);

    bool isLoaded() const {return loadedInGL;}
    bool isInterpolated() const {return interpolated;}

    int getWidth() const {return width;}
    int getheight() const {return height;}
    int getChannels() const {return channels;}

    /*GLuint getID() const;
    const GLuint* getIDptr() const;*/

    void use(int bindingUnit = 0) const;

};

GLuint load_texture_2d(const std::string& filename);