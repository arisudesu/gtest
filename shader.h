#ifndef SHADER_H
#define SHADER_H

#include <glbinding/gl/gl.h>

using namespace gl;

class Shader
{
    GLuint handle;

public:
    Shader();
    ~Shader();
    void attach(GLenum shader_type, std::string source);
    void attachFile(GLenum shader_type, std::string filename);
    void link();
    void use();
};

#endif // SHADER_H
