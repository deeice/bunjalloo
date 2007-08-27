#ifndef Component_h_seen
#define Component_h_seen

#include <vector>
#include "Rectangle.h"

class Component
{
  public:
    Component();
    virtual ~Component();
    virtual void paint(const nds::Rectangle & clip);
    virtual void add(Component * child);

    /** Set location in coordinates of this Component's parent.
     */
    virtual void setLocation(unsigned int x, unsigned int y);

    virtual void setSize(unsigned int w, unsigned int h);
    nds::Rectangle bounds() const;
    nds::Rectangle preferredSize() const;
    inline int width() const;
    inline int height() const;
    inline int x() const;
    inline int y() const;

    virtual bool touch(int x, int y);

  protected:
    nds::Rectangle m_bounds;
    
    /** The width that this component would like to be. */
    int m_preferredWidth;
    /** The height that this component would like to be. */
    int m_preferredHeight;

    std::vector<Component *> m_children;
};

int Component::width() const
{
  return m_bounds.w;
}
int Component::height() const
{
  return m_bounds.h;
}

int Component::x() const
{
  return m_bounds.x;
}
int Component::y() const
{
  return m_bounds.y;
}

#endif
