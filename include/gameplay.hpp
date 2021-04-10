#include "include/shader.hpp"

class GamePlay {
    GLuint vao;
    Shader s;
    float r;

public:
    GamePlay();

    ~GamePlay();

    void Update();

    void Render();
};
