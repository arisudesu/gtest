#ifndef GAME_H
#define GAME_H

#include "include/client.hpp"
#include "include/guimenu.hpp"
#include "include/gameplay.hpp"

class Game : public IClientEventHandler, public GuiMenuCallback
{
    Client m_client;
    GuiMainMenu m_menu;
    GuiExitMenu m_exitmenu;
    GuiPauseMenu m_pausemenu;
    bool m_bDone;
    GuiMenuBase* m_activeMenu;
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

    void OnItemSelect(GuiMenuBase&);
    void OnItemSelect(GuiMainMenu&);

    void onItemSelect(GuiMainMenu&);
    void onItemSelect(GuiExitMenu&);
    void onItemSelect(GuiMenuBase&);
};

#endif // GAME_H
