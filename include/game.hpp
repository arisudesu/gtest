#ifndef GAME_H
#define GAME_H

#include "include/client.hpp"
#include "include/guimainmenu.hpp"

class Game : public IClientEventHandler, public GuiMenuCallback
{
    Client m_client;
    GuiMainMenu m_menu;
    GuiMainMenu m_exitmenu;
    bool m_bDone;
    GuiMainMenu* m_activeMenu;
public:
    Game() :
        m_client(*this),
        m_menu(*this, true),
        m_exitmenu(*this, false),
        m_activeMenu(&m_menu),
        m_bDone(false)
    {};
    int  Run();

    void onWindowClose();
    void onKeyPress(Client::KeyCode, int, int);
    void onCursorMove(float, float);
    void onItemSelect(GuiMainMenu&);
};

#endif // GAME_H
