#include "program.hpp"

GLuint compile_shader(const std::string &path, GLenum shader_type) {
  const std::string shader_source = load_file(path);
  const char *shader_str = shader_source.data();

  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_str, nullptr);
  glCompileShader(shader);

  return shader;
}

GLuint create_program(const std::string &vertex_path, const std::string &fragment_path) {
  GLuint vertex_shader = compile_shader(vertex_path, GL_VERTEX_SHADER);
  GLuint fragment_shader = compile_shader(fragment_path, GL_FRAGMENT_SHADER);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);

  return program;
}