#include "include/guimainmenu.hpp"
#include "include/textrenderer.hpp"

void GuiMainMenu::Render()
{
    static Font sans("data/fonts/menu.ttf", 24);
    static TextRenderer sansRender(sans);

    static const std::array<std::string, 5> items
    {
        "Start", "Options", "About", "Debug", "Exit"
    };

    unsigned int count = 0;
    for (auto i: items)
    {
        if (getSelectedItem() == count)
        {
            sansRender.RenderText("[ " + i + " ]", 39, 400 + count * 30);
        }
        else
        {
            sansRender.RenderText(i, 50, 400 + count * 30);
        }
        ++ count;
    }
}
