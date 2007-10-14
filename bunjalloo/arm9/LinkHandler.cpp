#include "Canvas.h"
#include "ComboBox.h"
#include "Component.h"
#include "Link.h"
#include "Button.h"
#include "LinkHandler.h"
#include "LinkListener.h"
#include "Rectangle.h"
#include "ScrollPane.h"
#include "WidgetColors.h"

LinkHandler::LinkHandler(LinkListener * parent):
  m_parent(parent)
{
  setVisible(false);
  addItem(string2unicode("url"));
  addItem(string2unicode("image"));
  m_preferredHeight = scrollPane()->height();
}

void LinkHandler::setLink(const Link * link)
{
  m_link = link;
}

void LinkHandler::pressed(ButtonI * button)
{
  // A button on the scroll bar is pressed. See which it was...
  if (((Button*)scrollPane()->childAt(0))->selected())
  {
    // goto link
    Link link(m_link->href());
    m_parent->linkClicked(&link);
  }
  else
  {
    // goto src
    Link link(Link::STATE_COLOR);
    link.setColor(WidgetColors::LINK_IMAGE);
    link.setSrc(m_link->src());
    m_parent->linkClicked(&link);
  }
  // make sure the selection is reset.
  ((Button*)scrollPane()->childAt(0))->setSelected(false);
  ((Button*)scrollPane()->childAt(1))->setSelected(false);
}

bool LinkHandler::touch(int x, int y)
{
  if (m_visible)
  {
    if (scrollPane()->bounds().hit(x, y))
    {
      scrollPane()->touch(x, y);
    }
    setVisible(false);
    return true;
  }
  return false;
}

void LinkHandler::paint(const nds::Rectangle & clip)
{
  if ( not m_visible)
  {
    return;
  }
  scrollPane()->setLocation(m_bounds.x, m_bounds.bottom());
  if (scrollPane()->bounds().bottom() > nds::Canvas::instance().height())
  {
    scrollPane()->setLocation(m_bounds.x, m_bounds.top() - scrollPane()->height());
  }

  nds::Rectangle scrollPaneClip(scrollPane()->bounds());
  nds::Canvas::instance().fillRectangle(scrollPaneClip.x,
      scrollPaneClip.y, scrollPaneClip.w, scrollPaneClip.h, scrollPane()->backgroundColor());
  scrollPane()->paint(scrollPane()->bounds());
}
