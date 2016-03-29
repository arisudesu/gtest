#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "include/shader.hpp"

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glbinding/gl/gl.h>

using namespace gl;

class Font
{
    FT_Library m_ft;
    FT_Face m_face;
public:
    Font(const std::string& filename, int glyph_size);
    ~Font();
    unsigned char* GetGlyphBitmap(unsigned long codepoint);
    unsigned int GetGlyphWidth(unsigned long codepoint);
    unsigned int GetGlyphHeight(unsigned long codepoint);
};

class TextRenderer
{
    Font& m_font;
    GLuint m_quad, m_texcoord, m_tex;
    Shader m_shader;

public:
    TextRenderer(Font& font);
    ~TextRenderer();
    void RenderText(const std::string& text, int x, int y);
};

#endif // TEXTRENDERER_H
