class GuiMainMenu;

class GuiMenuCallback
{
public:
    virtual void onItemSelect(GuiMainMenu&) = 0;
    virtual ~GuiMenuCallback() {};
};

class GuiMainMenu
{
    const int m_numItems;
    unsigned int m_selectedItem;
    GuiMenuCallback& m_callback;
    bool m_b;

public:
    GuiMainMenu(GuiMenuCallback& callback, bool b):
        m_selectedItem(0),
        m_callback(callback),
        m_b(b),
        m_numItems(b ? 5 : 2)
    {}
    inline void navigateNext() { m_selectedItem = (m_selectedItem + 1) % m_numItems; };
    inline void navigatePrevious() { m_selectedItem = (m_selectedItem - 1 + m_numItems) % m_numItems; };
    inline void select() { m_callback.onItemSelect(*this); };
    inline unsigned int getSelectedItem() { return m_selectedItem; };
    void Render();
};


