#pragma once

#include <glad/glad.h>
#include <string>


class GLProgram {

public:
    const GLuint vertexShaderID;
    const GLuint fragmentShaderID;
    const GLuint programID;

    GLProgram(const GLuint vertShaderID, const GLuint fragShaderID);
    GLProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~GLProgram();

    GLProgram(const GLProgram&) = delete;
    GLProgram& operator=(const GLProgram&) = delete;

    void use() const {glUseProgram(programID);}


    static GLuint createShader(const std::string& filepath, GLenum shaderType);
};