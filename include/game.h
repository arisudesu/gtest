#ifndef GAME_H
#define GAME_H

#include "include/client.h"

class Game : public IClientEventHandler
{
    Client m_client;
    bool m_bDone;

public:
    Game() :
        m_client(this),
        m_bDone(false)
    {};
    int  Run();

    void onWindowClose();
    void onKeyPress(int, int, int);
    void onCursorMove(float, float);
};

#endif // GAME_H
