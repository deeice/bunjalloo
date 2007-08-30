#ifndef ButtonI_h_seen
#define ButtonI_h_seen

class ButtonListener;
class ButtonI
{
  public:
    
    ButtonI(): m_selected(false),m_listener(0){}

    virtual ~ButtonI() {}

    void setSelected(bool selected=true)
    {
      m_selected = selected;
    }

    bool selected() const
    {
      return m_selected;
    }

    inline void setListener(ButtonListener * listener)
    {
      m_listener = listener;
    }
    inline ButtonListener * listener() const
    {
      return m_listener;
    }
    inline void removeListener(ButtonListener * listener)
    {
      m_listener = 0;
    }

  private:
    bool m_selected;
    ButtonListener * m_listener;

};
#endif
