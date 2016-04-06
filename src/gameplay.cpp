#include "include/gameplay.hpp"

#include <glbinding/gl/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GamePlay::GamePlay():
    s(),
    vao(0),
    r(0.0)
{
    s.attachFile(GL_VERTEX_SHADER, "data/shader/vertex.glsl");
    s.attachFile(GL_FRAGMENT_SHADER, "data/shader/fragment.glsl");
    s.link();

    const float size = 3.0;
    const float skyboxVertData[][3] =
    {
        {-size, size, size}, { size, size, size}, { size,-size, size}, {-size,-size, size}, // front
        { size, size,-size}, {-size, size,-size}, {-size,-size,-size}, { size,-size,-size}, // back
        {-size, size,-size}, { size, size,-size}, { size, size, size}, {-size, size, size}, // top
        { size,-size,-size}, {-size,-size,-size}, {-size,-size, size}, { size,-size, size}, // bottom
        {-size, size,-size}, {-size, size, size}, {-size,-size, size}, {-size,-size,-size}, // left
        { size, size, size}, { size, size,-size}, { size,-size,-size}, { size,-size, size}  // right
    };

    const int skyboxIndData[] =
    {
        0, 3, 1,  1, 3, 2, // front
        4, 7, 5,  5, 7, 6, // back
        8,11, 9,  9,11,10, // top
        12,15,13, 13,15,14, // bottom
        16,19,17, 17,19,18, // left
        20,23,21, 21,23,22  // right
    };

    GLuint vbo, ibo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertData), skyboxVertData, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(s.getAttributeLocation("position"), 3, GL_FLOAT, GL_TRUE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(s.getAttributeLocation("position"));

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndData), skyboxIndData, GL_STATIC_DRAW);

    glBindVertexArray(0);

    float r = 0.0;


}
GamePlay::~GamePlay()
{
}

void GamePlay::Update()
{
    r += 0.1f;
}

void GamePlay::Render()
{


    glm::mat4 perspective = glm::perspective(glm::radians(70.0), 800.0/600.0, 0.3, 50.0);
    glm::mat4 proj =
            perspective * glm::lookAt(glm::vec3(10.0,0.0,0.0), glm::vec3(cos(r), sin(r), 0), glm::vec3(0.0, 0.0, 1.0));

    s.use();
    glUniformMatrix4fv(s.getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
