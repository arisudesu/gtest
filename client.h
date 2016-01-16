#ifndef CLIENT_H
#define CLIENT_H

#define GLFW_INCLUDE_NONE

#include <string>
#include <GLFW/glfw3.h>

class IClientEventHandler;
class Client
{
    GLFWwindow* m_window;
    IClientEventHandler *m_handler;

public:
    Client(IClientEventHandler *handler);
    ~Client();

    bool Initialize(int width, int height, const std::string title);
    void Terminate();

    void ProcessEvents();
    void SwapBuffers();
};

class IClientEventHandler
{
public:
    virtual void onWindowClose() = 0;
    virtual void onKeyPress(int key, int scancode, int mods) = 0;
};

#endif // CLIENT_H
