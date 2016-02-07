#define GLEQ_IMPLEMENTATION
#include "include/client.h"
#include "include/gleq.h"
#include <stdexcept>

Client::Client(IClientEventHandler *handler):
    m_window(0),
    m_handler(handler)
{
    if (!glfwInit())
        throw std::runtime_error("GLFW initialization failed!");
}

Client::~Client()
{
    Terminate();
    glfwTerminate();
}

bool Client::Initialize(int width, int height, const std::string title)
{
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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
            m_handler->onWindowClose();
            break;

        case GLEQ_KEY_PRESSED:
            m_handler->onKeyPress(event.key.key, event.key.scancode, event.key.mods);
            break;

        case GLEQ_CURSOR_MOVED:
            m_handler->onCursorMove(event.pos.x, event.pos.y);
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