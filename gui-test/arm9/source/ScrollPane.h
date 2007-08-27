#ifndef ScrollPane_h_seen
#define ScrollPane_h_seen

#include "Component.h"

class ScrollPane: public Component
{

  public:
    ScrollPane();
    void setSize(unsigned int w, unsigned int h);
    void setScrollIncrement(int scrollIncrement);

    void up();
    void down();
    void upBlock();
    void downBlock();
    void scrollToPercent(int value);

    void paint(const nds::Rectangle & clip);

  private:
    int m_scrollIncrement;

    void showScrollBar(const nds::Rectangle & clip);
};

#endif
