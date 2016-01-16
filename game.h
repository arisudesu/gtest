#ifndef GAME_H
#define GAME_H

#include "client.h"

class Game : public IClientEventHandler
{
    Client *m_client;
    bool m_bDone;

public:
    Game()
        : m_bDone(false)
    {};
    int  Run();

    void onWindowClose();
    void onKeyPress(int key, int scancode, int mods);

    void MenuSelection();
};

#endif // GAME_H
