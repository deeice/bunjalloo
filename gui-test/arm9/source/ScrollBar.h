#ifndef ScrollBar_h_seen
#define ScrollBar_h_seen

#include "Component.h"

class Scrollable;

class ScrollBar: public Component
{

  public:
    ScrollBar();

    void setTotal(int total);
    void setValue(int value);

    int total() const;
    int value() const;

    void setScrollable(Scrollable * scrollable);

    bool touch(int x, int y);

    void paint(const nds::Rectangle & clip);
    void setLocation(int x, int y);

  private:
    int m_total;
    int m_value;
    int m_handleSize;
    int m_handlePosition;
    Scrollable * m_scrollable;
    int m_handleHeld;

    void calculateHandle();

};
#endif
