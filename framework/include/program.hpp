#pragma once

#include <glad/glad.h>

#include "utilities.hpp"

GLuint compile_shader(const std::string &path, GLenum shader_type);
GLuint create_program(const std::string &vertex_path, const std::string &fragment_path);
