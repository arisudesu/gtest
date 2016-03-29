#include "include/game.hpp"
#include "include/client.hpp"
#include "include/shader.hpp"

#include <iostream>
#include <iomanip>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);


    while (!m_bDone)
    {
        m_client.ProcessEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_menu.Render();

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
    case Client::KeyCode::KEY_UP:
        std::cout << "up" << std::endl;
        m_menu.navigatePrevious();
        break;

    case Client::KeyCode::KEY_DOWN:
        std::cout << "down" << std::endl;
        m_menu.navigateNext();
        break;

    case Client::KeyCode::KEY_SPACE:
        std::cout << "space" << std::endl;
        m_menu.select();
        break;
    }
}

void Game::onCursorMove(float, float)
{
}

void Game::onItemSelect()
{
    std::cout << "selected menu item: " << m_menu.getSelectedItem() << std::endl;

    switch (m_menu.getSelectedItem())
    {
    case 0:
        break;

    case 4:
        m_bDone = true;
        break;
    }
}
