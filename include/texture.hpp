#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <glbinding/gl/gl.h>

class Texture;

class AbstractTextureLoader {
public:
    virtual Texture::TextureFormat GetFormat();

    virtual int GetWidth() = 0;

    virtual int GetHeight() = 0;

    virtual const vector<unsigned char> GetData() = 0;
};

class Texture {
    GLuint m_handle;

public:
    Texture(const AbstractTextureLoader &loader);

    void Bind();

    enum TextureFormat {
        RGBA = GL_RGBA,
    };
};

#endif // TEXTURE_H
