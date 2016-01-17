#include "game.h"
#include "client.h"
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <iostream>

using namespace gl;

int Game::Run()
{
    if (!m_client.Initialize(800, 600, "Test"))
        return -1;

    glbinding::Binding::initialize();

    while (!m_bDone)
    {
        m_client.ProcessEvents();

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClearDepth(0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_client.SwapBuffers();
    }
    return 0;
}

void Game::onWindowClose()
{
    m_bDone = true;
}

void Game::onKeyPress(int key, int scancode, int mods)
{
    std::cout << key << ", " << scancode << ", " << mods << std::endl;
}
