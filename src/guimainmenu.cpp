#include "include/guimainmenu.hpp"
#include "include/textrenderer.hpp"

void GuiMainMenu::Render()
{
    static Font sans("data/fonts/menu.ttf", 32);
    static TextRenderer sansRender(sans);

    if (getSelectedItem() == 0)
    {
        sansRender.RenderText("> Start", 0, 0);
    }
    else
    {
        sansRender.RenderText("Start", 0, 0);
    }

    if (getSelectedItem() == 1)
    {
        sansRender.RenderText("> Options", 0, 32);
    }
    else
    {
        sansRender.RenderText("Options", 0, 32);
    }

    if (getSelectedItem() == 2)
    {
        sansRender.RenderText("> About", 0, 64);
    }
    else
    {
        sansRender.RenderText("About", 0, 64);
    }
    if (getSelectedItem() == 3)
    {
        sansRender.RenderText("> Debug", 0, 96);
    }
    else
    {
        sansRender.RenderText("Debug", 0, 96);
    }
    if (getSelectedItem() == 4)
    {
        sansRender.RenderText("> Exit", 0, 128);
    }
    else
    {
        sansRender.RenderText("Exit", 0, 128);
    }
}
