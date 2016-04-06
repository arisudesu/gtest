#ifndef GAME_H
#define GAME_H

#include "include/client.hpp"
#include "include/guimainmenu.hpp"
#include "include/gameplay.hpp"

class Game : public IClientEventHandler, public GuiMenuCallback
{
    Client m_client;
    GuiMainMenu m_menu;
    GuiMainMenu m_exitmenu;
    GuiMainMenu m_pausemenu;
    bool m_bDone;
    GuiMainMenu* m_activeMenu;
    GamePlay *m_gp;
    bool m_gpPaused;

    Game(const Game&);
    Game operator=(const Game&);
public:
    Game();
    int  Run();

    void onWindowClose();
    void onKeyPress(Client::KeyCode, int, int);
    void onCursorMove(float, float);
    void onItemSelect(GuiMainMenu&);
};

#endif // GAME_H
