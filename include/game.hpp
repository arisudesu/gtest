#ifndef GAME_H
#define GAME_H

#include "include/client.hpp"
#include "include/guimainmenu.hpp"

class Game : public IClientEventHandler, public GuiMenuCallback
{
    Client m_client;
    GuiMainMenu m_menu;
    bool m_bDone;

public:
    Game() :
        m_client(*this),
        m_menu(*this),
        m_bDone(false)
    {};
    int  Run();

    void onWindowClose();
    void onKeyPress(Client::KeyCode, int, int);
    void onCursorMove(float, float);
    void onItemSelect();
};

#endif // GAME_H
