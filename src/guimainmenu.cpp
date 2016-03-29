#include "include/guimainmenu.hpp"
#include "include/textrenderer.hpp"

void GuiMainMenu::Render()
{
    static Font sans("data/fonts/menu.ttf", 24);
    static TextRenderer sansRender(sans);

    if (getSelectedItem() == 0)
    {
        sansRender.RenderText("[ Start ]", 50, 400);
    }
    else
    {
        sansRender.RenderText("Start", 50, 400);
    }

    if (getSelectedItem() == 1)
    {
        sansRender.RenderText("[ Options ]", 50, 432);
    }
    else
    {
        sansRender.RenderText("Options", 50, 432);
    }

    if (getSelectedItem() == 2)
    {
        sansRender.RenderText("[ About ]", 50, 464);
    }
    else
    {
        sansRender.RenderText("About", 50, 464);
    }
    if (getSelectedItem() == 3)
    {
        sansRender.RenderText("[ Debug ]", 50, 496);
    }
    else
    {
        sansRender.RenderText("Debug", 50, 496);
    }
    if (getSelectedItem() == 4)
    {
        sansRender.RenderText("[ Exit ]", 50, 528);
    }
    else
    {
        sansRender.RenderText("Exit", 50, 528);
    }
}
