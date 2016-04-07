#include <functional>

class GuiMenuBase;
class GuiMainMenu;

class GuiMenuCallback
{
public:
    virtual ~GuiMenuCallback() {};
};

class GuiMenuBase
{
    const unsigned int m_numItems;
    unsigned int m_selectedItem;
    std::function<void(GuiMenuBase&)> m_callback;

protected:
    GuiMenuBase(int, std::function<void(GuiMenuBase&)>);
    virtual ~GuiMenuBase() { };
public:
    void navigateNext();
    void navigatePrevious();
    void select();
    unsigned int getSelectedItem() const;
    virtual void Render() = 0;
};


class GuiMainMenu : public GuiMenuBase
{
public:
    GuiMainMenu(std::function<void(GuiMenuBase&)>);
    ~GuiMainMenu() { }
    void Render();
};

class GuiExitMenu : public GuiMenuBase
{
public:
    GuiExitMenu(std::function<void(GuiMenuBase&)>);
    ~GuiExitMenu() { }
    void Render();
};

class GuiPauseMenu : public GuiMenuBase
{
public:
    GuiPauseMenu(std::function<void(GuiMenuBase&)>);
    ~GuiPauseMenu() { }
    void Render();
};
