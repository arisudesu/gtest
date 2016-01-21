#include "game.h"
#include "client.h"
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <iostream>
#include "shader.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace gl;

int Game::Run()
{
    if (!m_client.Initialize(800, 600, "Test"))
        return -1;

    glbinding::Binding::initialize();

    /*Shader s;
    s.attachFile(GL_VERTEX_SHADER, "data/shader/vertex.glsl");
    s.attachFile(GL_FRAGMENT_SHADER, "data/shader/fragment.glsl");
    s.link();
    s.use();

    glm::mat4 view(1.0);
    glUniformMatrix4fv(s.getUniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(view));




    const float mesh[][3] = {
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f }
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh), mesh, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof mesh[0], 0);
    glEnableVertexAttribArray(0);

    glViewport(0, 0, 800, 600);*/
    glClearColor(0.4, 0.6, 0.9, 1.0);
    glClearDepth(1.0);

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
        //glDrawArrays(GL_TRIANGLES, 0, sizeof mesh / sizeof mesh[0]);

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
