#ifndef Link_h_seen
#define Link_h_seen
#include <string>
#include <list>

class HtmlElement;
class Rectangle;
class Link
{
  public:
    Link(const HtmlElement * a);
    ~Link();

    void appendClickZone(int x, int y, int w, int h);

    bool hitTest(int x, int y) const;
    std::string href() const;

    void highlight() const;

  private:
    const HtmlElement * m_anchor;
    typedef std::list<Rectangle*> RectangleList;
    RectangleList m_clickZones;
};

#endif
