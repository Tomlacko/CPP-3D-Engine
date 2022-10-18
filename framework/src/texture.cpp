#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cmath>

GLuint load_texture_2d(const std::string &filename) {
  int width, height, channels;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 4);

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);

  glTextureStorage2D(texture,
                     std::log2(width),
                     GL_RGBA8,
                     width, height);

  glTextureSubImage2D(texture,
                      0,                         //
                      0, 0,                      //
                      width, height,             //
                      GL_RGBA, GL_UNSIGNED_BYTE, //
                      data);

  stbi_image_free(data);

  glGenerateTextureMipmap(texture);

  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}