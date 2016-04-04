#include <glm/gtc/type_ptr.hpp>

#include "include/game.hpp"
#include "include/client.hpp"
#include "include/shader.hpp"
#include "include/textrenderer.hpp"

#include <iostream>
#include <sstream>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace gl;

Game::Game():
    m_client(800, 600, "Test", *this),
    m_menu(*this, true),
    m_exitmenu(*this, false),
    m_bDone(false),
    m_activeMenu(&m_menu)
{
    glbinding::Binding::initialize();
}

int Game::Run()
{
    glViewport(0, 0, 800, 600);
    glClearColor(1.0, 1.0, 0.8, 1.0);
    glClearDepth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    /*glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);*/

    // TODO: use std::chrono
    double time = clibraries::glfwGetTime();
    unsigned int framecount = 0;
    float fps = 0;

    static Font sans("data/fonts/menu.ttf", 14);
    static TextRenderer sansRender(sans);


    Shader s;
    s.attachFile(GL_VERTEX_SHADER, "data/shader/vertex.glsl");
    s.attachFile(GL_FRAGMENT_SHADER, "data/shader/fragment.glsl");
    s.link();

    const float size = 3.0;
    const float skyboxVertData[][3] =
    {
        {-size, size, size}, { size, size, size}, { size,-size, size}, {-size,-size, size}, // front
        { size, size,-size}, {-size, size,-size}, {-size,-size,-size}, { size,-size,-size}, // back
        {-size, size,-size}, { size, size,-size}, { size, size, size}, {-size, size, size}, // top
        { size,-size,-size}, {-size,-size,-size}, {-size,-size, size}, { size,-size, size}, // bottom
        {-size, size,-size}, {-size, size, size}, {-size,-size, size}, {-size,-size,-size}, // left
        { size, size, size}, { size, size,-size}, { size,-size,-size}, { size,-size, size}  // right
    };

    const int skyboxIndData[] =
    {
        0, 3, 1,  1, 3, 2, // front
        4, 7, 5,  5, 7, 6, // back
        8,11, 9,  9,11,10, // top
        12,15,13, 13,15,14, // bottom
        16,19,17, 17,19,18, // left
        20,23,21, 21,23,22  // right
    };

    GLuint vao, vbo, ibo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertData), skyboxVertData, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(s.getAttributeLocation("position"), 3, GL_FLOAT, GL_TRUE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(s.getAttributeLocation("position"));

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndData), skyboxIndData, GL_STATIC_DRAW);

    glBindVertexArray(0);

    float r = 0.0;

    glm::mat4 perspective = glm::perspective(glm::radians(70.0), 800.0/600.0, 0.3, 50.0);
    glm::mat4 proj =
            perspective * glm::lookAt(glm::vec3(10.0,0.0,0.0), glm::vec3(cos(r), sin(r), 0), glm::vec3(0.0, 0.0, 1.0));

    glEnable(GL_MULTISAMPLE);
    GLuint colorRbo, depthRbo;

    glGenRenderbuffers(1, &colorRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, 800, 600);
    //std::cout << glGetError() << std::endl;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLuint mainFbo;
    glGenFramebuffers(1, &mainFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFbo);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

    GLint samples;
    glGetIntegerv(GL_MAX_SAMPLES, &samples);
    //std::cout << samples << " " << glGetError() << std::endl;

    //std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!m_bDone)
    {
        m_client.ProcessEvents();

        r += 0.1;
        proj = perspective * glm::lookAt(glm::vec3(10.0,0.0,0.0), glm::vec3(cos(r), sin(r), 0), glm::vec3(0.0, 0.0, 1.0));

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mainFbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.use();
        glUniformMatrix4fv(s.getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        m_activeMenu->Render();

        if (clibraries::glfwGetTime() - time >= 1)
        {
            fps = framecount / (float)(clibraries::glfwGetTime() - time);
            time = clibraries::glfwGetTime();
            framecount = 0;
        }
        framecount ++;

        std::ostringstream strfps;
        strfps << "fps:" << fps;
        sansRender.RenderText(strfps.str(), 0, 0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFbo);

        glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
        glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        //std::cout << glGetError() << std::endl;
        m_client.SwapBuffers();
    }
    return 0;
}

void Game::onWindowClose()
{
    //m_bDone = true;
}

void Game::onKeyPress(Client::KeyCode key, int /*scancode*/, int /*mods*/)
{
    switch (key)
    {
    case Client::KEY_UP:
        m_activeMenu->navigatePrevious();
        break;

    case Client::KEY_DOWN:
        m_activeMenu->navigateNext();
        break;

    case Client::KEY_RETURN:
        m_activeMenu->select();
        break;

    default:
        ;
    }
}

void Game::onCursorMove(float, float)
{
}

void Game::onItemSelect(GuiMainMenu& o)
{
    if (&o == &m_menu)
    {
        switch (o.getSelectedItem())
        {
        case 4:
            m_activeMenu = &m_exitmenu;
            break;
        }
    }

    if (&o == &m_exitmenu)
    {
        switch (o.getSelectedItem())
        {
        case 0:
            m_bDone = true;

        case 1:
            m_activeMenu = &m_menu;
            break;
        }
    }
}
