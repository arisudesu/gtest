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
using namespace std::placeholders;

Client get_client_with_context(IClientEventHandler& e)
{
    Client c(400, 300, "rvalue test", e);
    std::cout << "Address of c inside get_client_with_context: " << &c << std::endl;
    glbinding::Binding::initialize();
    return c;
}

Game::Game():
    m_client(get_client_with_context(*this)),
    m_menu(std::bind(
        static_cast<void (Game::*)(GuiMenuBase&)>(&Game::OnItemSelect), this, _1)
    ),
    m_exitmenu(std::bind(
        static_cast<void (Game::*)(GuiMenuBase&)>(&Game::OnItemSelect), this, _1)
    ),
    m_pausemenu(std::bind(
        static_cast<void (Game::*)(GuiMenuBase&)>(&Game::OnItemSelect), this, _1
    )),
    m_bDone(false),
    m_activeMenu(&m_menu),
    m_gp(nullptr),
    m_gpPaused(false)
{
    std::cout << "Address of m_client: " << &m_client << std::endl;
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

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mainFbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_gp)
        {
            if (!m_gpPaused)
            {
                m_gp->Update();
            }
            m_gp->Render();
        }

        if (m_activeMenu)
        {
            m_activeMenu->Render();
        }

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
    if (m_gp)
    {
        switch (key)
        {
        case Client::KEY_ESCAPE:
            if (m_gpPaused)
            {
                m_gpPaused = false;
                m_activeMenu = nullptr;
            }
            else
            {
                m_gpPaused = true;
                m_activeMenu = &m_pausemenu;
            }
            break;

        default:
            ;
        }
    }

    if (m_activeMenu)
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
}

void Game::onCursorMove(float, float)
{
}

void Game::OnItemSelect(GuiMenuBase &/*o*/)
{
    std::cout << "bound function" << std::endl;
}

void Game::OnItemSelect(GuiMainMenu &/*o*/)
{
    std::cout << "bound function 2" << std::endl;
}

void Game::onItemSelect(GuiExitMenu& /*o*/)
{
}

void Game::onItemSelect(GuiMainMenu& o)
{
    std::cout << "mainmenu" << std::endl;
    switch (o.getSelectedItem())
    {
    case 0:
        m_activeMenu = nullptr;
        m_gpPaused = false;
        m_gp = new GamePlay;
        break;

    case 4:
        m_activeMenu = &m_exitmenu;
        break;
    }
}

void Game::onItemSelect(GuiMenuBase& o)
{
    std::cout << "generic menu" << std::endl;

    if (&o == &m_exitmenu)
    {
        switch (o.getSelectedItem())
        {
        case 0:
            m_bDone = true;
            break;

        case 1:
            m_activeMenu = &m_menu;
            break;
        }
    }

    if (&o == &m_pausemenu)
    {
        switch (o.getSelectedItem())
        {
        case 0:
            m_gpPaused = false;
            m_activeMenu = nullptr;
            break;

        case 1:
            m_activeMenu = &m_menu;
            delete m_gp;
            break;
        }
    }
}
