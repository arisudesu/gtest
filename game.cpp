#include "game.h"
#include "client.h"
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <iostream>
#include "shader.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>

using namespace gl;

/*void debugfunc(GLenum source​, GLenum type​, GLuint id​,
               GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​)
{
    std::cout << message​ << std::endl;
}*/

int Game::Run()
{
    if (!m_client.Initialize(800, 600, "Test"))
        return -1;

    glbinding::Binding::initialize();

    std::cout << glGetString(GL_RENDERER) << glGetString(GL_VERSION) << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //glDebugMessageCallback(debugfunc, 0);
    glViewport(0, 0, 800, 600);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(0.0);

    Shader s;
    s.attachFile(GL_VERTEX_SHADER, "data/shader/vertex.glsl");
    s.attachFile(GL_FRAGMENT_SHADER, "data/shader/fragment.glsl");
    s.link();
    s.use();

    GLint position = s.getAttributeLocation("position");
    GLint color = s.getAttributeLocation("color");


    static const float triangleMesh[] = {
            /* 1 вершина, позиция: */ -1.0f, -1.0f, -2.0f, /* цвет: */ 1.0f, 0.0f, 0.0f,
            /* 2 вершина, позиция: */  0.0f,  1.0f, -2.0f, /* цвет: */ 0.0f, 1.0f, 0.0f,
            /* 3 вершина, позиция: */  1.0f, -1.0f, -2.0f, /* цвет: */ 0.0f, 0.0f, 1.0f
    };

    static const float colors[] = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 0.0
    };

    GLuint meshVAO, meshVBO, colorVBO;
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);

    glGenBuffers(1, &meshVBO);
    glGenBuffers(1, &colorVBO);

    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), triangleMesh, GL_STATIC_DRAW);

    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(position);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(color);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0), 800.0/600.0, 0.5, 5.0);
    glUniformMatrix4fv(s.getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));


    s.use();
    glBindVertexArray(meshVAO);

    while (!m_bDone)
    {
        m_client.ProcessEvents();

        if (m_client.HasKeyPressed(Client::KEY_W))
        {}
        if (m_client.HasKeyPressed(Client::KEY_S))
        {}
        if (m_client.HasKeyPressed(Client::KEY_A))
        {}
        if (m_client.HasKeyPressed(Client::KEY_D))
        {}


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 500);

        m_client.SwapBuffers();
    }
    return 0;
}

void Game::onWindowClose()
{
    m_bDone = true;
}

void Game::onKeyPress(int /*key*/, int /*scancode*/, int /*mods*/)
{
}

void Game::onCursorMove(float, float)
{
}
