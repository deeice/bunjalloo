#ifndef ComboBox_h_seen
#define ComboBox_h_seen
#include "Component.h"
#include "UnicodeString.h"
#include "ButtonListener.h"

class ScrollPane;
class Button;
class ComboBox: public Component, 
                private ButtonListener
{
  public:
    ComboBox();
    ~ComboBox();
    void addItem(const UnicodeString & item);
    bool touch(int x, int y);
    void paint(const nds::Rectangle & clip);
    virtual void setLocation(unsigned int x, unsigned int y);
    void setSize(unsigned int w, unsigned int h);

    void pressed(ButtonI * button);

  private:
    int m_items;
    bool m_open;
    ScrollPane * m_scrollPane;
    Button * m_button;
    
};
#endif
