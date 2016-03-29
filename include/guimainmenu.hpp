class GuiMenuCallback;

class GuiMenuCallback
{
public:
    virtual void onItemSelect() = 0;
    virtual ~GuiMenuCallback() {};
};

class GuiMainMenu
{
    static const int m_numItems = 5;
    int m_selectedItem;
    GuiMenuCallback& m_callback;

public:
    GuiMainMenu(GuiMenuCallback& callback):
        m_selectedItem(0),
        m_callback(callback)
    {}
    inline void navigateNext() { m_selectedItem = (m_selectedItem + 1) % m_numItems; };
    inline void navigatePrevious() { m_selectedItem = (m_selectedItem - 1 + m_numItems) % m_numItems; };
    inline void select() { m_callback.onItemSelect(); };
    inline int getSelectedItem() { return m_selectedItem; };
    void Render();
};


