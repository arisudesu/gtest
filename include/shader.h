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
    void attach(GLenum shader_type, const std::string source);
    void attachFile(GLenum shader_type, const std::string filename);
    void link();
    void use();
    GLint getAttributeLocation(const std::string name);
    GLint getUniformLocation(const std::string name);
};

#endif // SHADER_H
