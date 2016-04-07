#ifndef CLIENT_H
#define CLIENT_H

#define GLFW_INCLUDE_NONE
namespace clibraries
{
    #include <GLFW/glfw3.h>
}
#include <string>

class IClientEventHandler;
class Client
{
    clibraries::GLFWwindow* m_window;
    IClientEventHandler& m_handler;

    struct GLFWContainer
    {
        GLFWContainer();
        ~GLFWContainer();
    };
    static const GLFWContainer m_glfw;

private:
    Client(const Client&);
    const Client& operator=(const Client&);

public:
    Client(const int width, const int height, const std::string& title, IClientEventHandler& handler);
    Client(Client&&);
    ~Client();

    bool Initialize(const int width, const int height, const std::string& title);
    void Terminate();

    void ProcessEvents();
    void SwapBuffers();

    enum KeyCode
    {
        KEY_W = 87,
        KEY_S = 83,
        KEY_A = 65,
        KEY_D = 68,
        KEY_UP = 265,
        KEY_DOWN = 264,
        KEY_SPACE = 32,
        KEY_RETURN = 257,
        KEY_ESCAPE = GLFW_KEY_ESCAPE,
    };

    bool HasKeyPressed(KeyCode key);

};

class IClientEventHandler
{
public:
    virtual void onWindowClose() = 0;
    virtual void onKeyPress(Client::KeyCode key, int scancode, int mods) = 0;
    virtual void onCursorMove(float x, float y) = 0;
    virtual ~IClientEventHandler() {};
};

#endif // CLIENT_H
