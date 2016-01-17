#include "shader.h"
#include <fstream>

Shader::Shader()
{
    handle = glCreateProgram();
}

Shader::~Shader()
{
    glDeleteProgram(handle);
}

void Shader::attach(GLenum shader_type, std::string source)
{
    GLuint shader = glCreateShader(shader_type);
    const GLchar* csource = source.c_str();
    glShaderSource(shader, 1, &csource, 0);
    glCompileShader(shader);
    glAttachShader(handle, shader);
}

void Shader::attachFile(GLenum shader_type, std::string filename)
{
    std::ifstream file(filename);
    std::string source = "", line = "";

    if (!file)
        throw std::runtime_error("Shader not loaded: " + filename);

    while (std::getline(file, line))
        source += line + "\n";

    attach(shader_type, source);
}

void Shader::link()
{
    glLinkProgram(handle);
}

void Shader::use()
{
    glUseProgram(handle);
}
