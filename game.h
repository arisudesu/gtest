#ifndef GAME_H
#define GAME_H

#include "client.h"

class Game : public IClientEventHandler
{
    Client m_client;
    bool m_bDone;
    float x, y, z;

public:
    Game() :
        m_client(this),
        m_bDone(false),
        x(0.0),
        y(0.0),
        z(0.0)
    {};
    int  Run();

    void onWindowClose();
    void onKeyPress(int, int, int);
};

#endif // GAME_H
