#ifndef ScrollPane_h_seen
#define ScrollPane_h_seen

#include "Component.h"
#include "Scrollable.h"

class ScrollBar;

class ScrollPane: public Component, Scrollable
{

  public:
    ScrollPane();
    void setSize(unsigned int w, unsigned int h);
    void setLocation(unsigned int x, unsigned int y);
    void setScrollIncrement(int scrollIncrement);
    int scrollIncrement() const;

    void up();
    void down();
    void upBlock();
    void downBlock();
    void scrollToPercent(int value);

    /** Set this to be the top level scroll pane. */
    void setTopLevel(bool topLevel=true);
    bool topLevel() const;

    void paint(const nds::Rectangle & clip);

    bool touch(int x, int y);

  private:
    int m_scrollIncrement;
    bool m_topLevel;
    bool m_canScrollUp;
    bool m_canScrollDown;
    ScrollBar * m_scrollBar;

    void showScrollBar(const nds::Rectangle & clip);
};

#endif
