#ifndef Link_h_seen
#define Link_h_seen
#include <string>
#include <list>
#include "Rectangle.h"

class Link
{
  public:
    Link(const std::string & a);
    ~Link();

    void appendClickZone(int x, int y, int w, int h);

    bool hitTest(int x, int y) const;
    std::string href() const;

    void highlight() const;

  private:
    std::string m_anchor;
    typedef std::list<nds::Rectangle*> RectangleList;
    RectangleList m_clickZones;
};

#endif
