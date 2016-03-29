#include "include/textrenderer.hpp"
#include <stdexcept>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Font::Font(const std::string& filename, unsigned int glyph_size):
    m_glyph_size(glyph_size)
{
    if (FT_Init_FreeType(&m_ft))
    {
        throw std::runtime_error("FreeType library not initialized");
    }

    if (FT_New_Face(m_ft, filename.c_str(), 0, &m_face))
    {
        throw std::runtime_error("Font " + filename + "cannot be loaded");
    }
    FT_Set_Pixel_Sizes(m_face, 0, glyph_size);
}

Font::~Font()
{
}

unsigned char* Font::GetGlyphBitmap(unsigned long codepoint)
{
    FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
    return m_face->glyph->bitmap.buffer;
}

unsigned int Font::GetGlyphWidth(unsigned long codepoint)
{
    FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
    return m_face->glyph->bitmap.width;
}

unsigned int Font::GetGlyphTop(unsigned long codepoint)
{
    FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
    return m_face->glyph->bitmap_top;
}

unsigned int Font::GetGlyphHeight(unsigned long codepoint)
{
    FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
    return m_face->glyph->bitmap.rows;
}

unsigned int Font::GetGlyphAdvance(unsigned long codepoint)
{
    FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
    return (float)m_face->glyph->advance.x / 64.0;
}

TextRenderer::TextRenderer(Font& font):
    m_font(font),
    m_quad(0),
    m_texcoord(0),
    m_tex(0),
    m_shader()
{
    glGenTextures(1, &m_tex);
    glGenBuffers(1, &m_quad);

    const float texcoord[][2] =
    {
        {0, 0}, {1, 0}, {0, 1},
        {1, 0}, {1, 1}, {0, 1}
    };
    glGenBuffers(1, &m_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);

    m_shader.attachFile(GL_VERTEX_SHADER, "data/shader/text_v.glsl");
    m_shader.attachFile(GL_FRAGMENT_SHADER, "data/shader/text_f.glsl");
    m_shader.link();
}

TextRenderer::~TextRenderer()
{
    glDeleteTextures(1, &m_tex);
    glDeleteBuffers(1, &m_quad);
    glDeleteBuffers(1, &m_texcoord);
}

void TextRenderer::RenderText(const std::string& text, int x, int y)
{
    float pos = 0.0;

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 0x2600);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 0x2600);

    m_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(m_shader.getUniformLocation("texture"), 0);

    glm::mat4 proj = glm::ortho(0.0 - (double)x, 800.0 - (double)x, 600.0 - (double)y, 0.0 - (double)y);
    glUniformMatrix4fv(m_shader.getUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(proj));

    const float baseline = m_font.GetFontBaseline();

    for (auto iter = text.begin(); iter != text.end(); ++iter)
    {
        const float h = m_font.GetGlyphHeight(*iter),
                    w = m_font.GetGlyphWidth(*iter),
                    t = m_font.GetGlyphTop(*iter);

        const float quad[][3] =
        {
            { pos  ,  baseline - t, 0 },
            { pos+w,  baseline - t, 0 },
            { pos  ,  baseline - t + h, 0 },
            { pos+w,  baseline - t, 0 },
            { pos+w,  baseline - t + h, 0 },
            { pos  ,  baseline - t + h, 0 }
        };
        glBindBuffer(GL_ARRAY_BUFFER, m_quad);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

        glVertexAttribPointer(m_shader.getAttributeLocation("position"), 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(m_shader.getAttributeLocation("position"));

        glBindBuffer(GL_ARRAY_BUFFER, m_texcoord);

        glVertexAttribPointer(m_shader.getAttributeLocation("texcoord"), 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), 0);
        glEnableVertexAttribArray(m_shader.getAttributeLocation("texcoord"));

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            0x1903,
            w,
            h,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_font.GetGlyphBitmap(*iter)
        );

        glDrawArrays(GL_TRIANGLES, 0, 6);
        pos += m_font.GetGlyphAdvance(*iter);
    }
}
