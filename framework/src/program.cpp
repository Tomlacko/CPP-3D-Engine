#include "program.hpp"
#include <utilities.hpp>


GLProgram::GLProgram(const GLuint vertShaderID, const GLuint fragShaderID)
:vertexShaderID(vertShaderID), fragmentShaderID(fragShaderID),
 programID(glCreateProgram()) {

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
}


GLProgram::GLProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
:vertexShaderID(createShader(vertexShaderPath, GL_VERTEX_SHADER)),
 fragmentShaderID(createShader(fragmentShaderPath, GL_FRAGMENT_SHADER)),
 programID(glCreateProgram()) {

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
}


GLProgram::~GLProgram() {
    glDeleteProgram(programID);
}


GLuint GLProgram::createShader(const std::string& filepath, GLenum shaderType) {
    const std::string shaderCode = load_file(filepath);
    const char* shaderCString = shaderCode.data();

    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCString, nullptr);
    glCompileShader(shaderID);

    return shaderID;
}