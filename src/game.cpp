#include <glm/gtc/type_ptr.hpp>

#include "include/game.hpp"
#include "include/client.hpp"
#include "include/shader.hpp"
#include "include/textrenderer.hpp"

#include <sstream>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace gl;

int Game::Run()
{
    if (!m_client.Initialize(800, 600, "Test"))
        return -1;

    glbinding::Binding::initialize();

    glViewport(0, 0, 800, 600);
    glClearColor(1.0, 1.0, 1.0, 1.0);
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

    while (!m_bDone)
    {
        m_client.ProcessEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_menu.Render();

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

        m_client.SwapBuffers();
    }
    return 0;
}

void Game::onWindowClose()
{
    m_bDone = true;
}

void Game::onKeyPress(Client::KeyCode key, int /*scancode*/, int /*mods*/)
{
    switch (key)
    {
    case Client::KEY_UP:
        m_menu.navigatePrevious();
        break;

    case Client::KEY_DOWN:
        m_menu.navigateNext();
        break;

    case Client::KEY_RETURN:
        m_menu.select();
        break;
    }
}

void Game::onCursorMove(float, float)
{
}

void Game::onItemSelect()
{
    switch (m_menu.getSelectedItem())
    {
    case 0:
        break;

    case 4:
        m_bDone = true;
        break;
    }
}
