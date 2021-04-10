#include "include/guimenu.hpp"
#include "include/textrenderer.hpp"
#include <iostream>

GuiMenuBase::GuiMenuBase(int numItems, std::function<void(GuiMenuBase &)> callback) :
        m_numItems(numItems),
        m_selectedItem(0),
        m_callback(callback) {}

void GuiMenuBase::navigateNext() {
    m_selectedItem = (m_selectedItem + 1) % m_numItems;
}

void GuiMenuBase::navigatePrevious() {
    m_selectedItem = (m_selectedItem - 1 + m_numItems) % m_numItems;
}

void GuiMenuBase::select() {
    m_callback(*this);
}

unsigned int GuiMenuBase::getSelectedItem() const {
    return m_selectedItem;
}

GuiMainMenu::GuiMainMenu(std::function<void(GuiMenuBase &)> callback) :
        GuiMenuBase(5, callback) {}

void GuiMainMenu::Render() {
    static Font sans("data/fonts/menu.ttf", 24);
    static TextRenderer sansRender(sans);

    static const std::array<std::string, 5> items
            {
                    "Start", "Options", "About", "Debug", "Exit"
            };

    unsigned int count = 0;
    for (auto i: items) {
        if (getSelectedItem() == count)
            sansRender.RenderText("[ " + i + " ]", 39, 400 + count * 30);
        else
            sansRender.RenderText(i, 50, 400 + count * 30);
        ++count;
    }
}


GuiExitMenu::GuiExitMenu(std::function<void(GuiMenuBase &)> callback) :
        GuiMenuBase(2, callback) {}

void GuiExitMenu::Render() {
    static Font sans("data/fonts/menu.ttf", 24);
    static TextRenderer sansRender(sans);

    static const std::array<std::string, 2> items
            {
                    "Yes", "No"
            };

    unsigned int count = 0;
    for (auto i: items) {
        if (getSelectedItem() == count)
            sansRender.RenderText("[ " + i + " ]", 39, 400 + count * 30);
        else
            sansRender.RenderText(i, 50, 400 + count * 30);
        ++count;
    }
}


GuiPauseMenu::GuiPauseMenu(std::function<void(GuiMenuBase &)> callback) :
        GuiMenuBase(2, callback) {}

void GuiPauseMenu::Render() {
    static Font sans("data/fonts/menu.ttf", 24);
    static TextRenderer sansRender(sans);

    static const std::array<std::string, 2> items
            {
                    "Continue", "Exit to main menu"
            };

    unsigned int count = 0;
    for (auto i: items) {
        if (getSelectedItem() == count)
            sansRender.RenderText("[ " + i + " ]", 39, 400 + count * 30);
        else
            sansRender.RenderText(i, 50, 400 + count * 30);
        ++count;
    }
}
