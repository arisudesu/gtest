#include "include/texture.h"

Texture::Texture(const AbstractTextureLoader& loader)
{
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    const GLvoid *data = (const GLvoid *)&loader.GetData()[0];

    glTexImage2D(GL_TEXTURE_2D,
        0,
        loader.GetFormat(),
        loader.GetWidth(),
        loader.GetHeight(),
        0,
        loader.GetFormat(),
        GL_UNSIGNED_BYTE,
        data
    );
}
