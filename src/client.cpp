#define GLEQ_IMPLEMENTATION
#include "include/client.hpp"

using namespace clibraries;

#include "include/gleq.h"
#include <stdexcept>

const Client::GLFWContainer Client::m_glfw;

Client::GLFWContainer::GLFWContainer()
{
    glfwInit();
}

Client::GLFWContainer::~GLFWContainer()
{
    glfwTerminate();
}

Client::Client(const int width, const int height, const std::string& title, IClientEventHandler& handler):
    m_window(0),
    m_handler(handler)
{
    Initialize(width, height, title);
}
#include <iostream>
Client::Client(Client&& o):
    m_window(o.m_window),
    m_handler(o.m_handler)
{
    o.m_window = nullptr;
    std::cout << "Client " << static_cast<void*>(&o) << " being moved to " << static_cast<void*>(this) << std::endl;
}

Client::~Client()
{
    Terminate();
    std::cout << "Client " << static_cast<void*>(this) << " being destroyed" << std::endl;
}

bool Client::Initialize(const int width, const int height, const std::string& title)
{
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, false);

    m_window = glfwCreateWindow(width, height, title.c_str(), 0, 0);
    if (!m_window)
        return false;

    gleqTrackWindow(m_window);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    return true;
}

void Client::Terminate()
{
    if (m_window != 0)
    {
        glfwMakeContextCurrent(0);
        glfwDestroyWindow(m_window);
        m_window = 0;
    }
}

void Client::ProcessEvents()
{
    glfwPollEvents();

    GLEQevent event;
    while (gleqNextEvent(&event))
    {
        switch (event.type)
        {
        case GLEQ_WINDOW_CLOSED:
            m_handler.onWindowClose();
            break;

        case GLEQ_KEY_PRESSED:
        case GLEQ_KEY_REPEATED:
            m_handler.onKeyPress(static_cast<KeyCode>(event.key.key), event.key.scancode, event.key.mods);
            break;

        case GLEQ_CURSOR_MOVED:
            m_handler.onCursorMove(event.pos.x, event.pos.y);
        default:
            break;
        }
        gleqFreeEvent(&event);
    }
}

void Client::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool Client::HasKeyPressed(KeyCode key)
{
    return glfwGetKey(m_window, key);
}
