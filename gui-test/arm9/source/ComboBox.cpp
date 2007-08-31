#include "Palette.h"
#include "Canvas.h"
#include "ComboBox.h"
#include "ScrollPane.h"
#include "ScrollBar.h"
#include "Button.h"
#include "WidgetColors.h"
/*
static const nds::Color DROP_DOWN_COLOR(28,28,28);
static const nds::Color COMBOBOX_COLOR(26,26,26);
static const nds::Color COMBOBOX_COLOR_SELECTED(23,23,23);
*/

ComboBox::ComboBox():
  m_items(0),
  m_open(false)
{
  // implemented as a scroll bar + buttons.
  add(new ScrollPane);
  add(new Button);
  scrollPane()->setBackgroundColor(WidgetColors::COMBOBOX_DROP_DOWN);
  button()->setDecoration(false);
  button()->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
}

void ComboBox::addItem(const UnicodeString & item)
{
  if (button()->text().empty())
  {
    button()->setText(item);
    if (m_bounds.w == 0)
    {
      m_bounds.w = button()->width();
      m_bounds.h = button()->height();
    }
  }
  Button * b = new Button(item);
  b->setListener(this);
  // need to add callbacks to button so that the combobox is updated.
  b->setSize(m_bounds.w, m_bounds.h);
  b->setDecoration(false);
  b->setBackgroundColor(WidgetColors::COMBOBOX_DROP_DOWN);
  m_items++;
  scrollPane()->add(b);
  scrollPane()->setTopLevel(false);
  int idealHeight = (m_bounds.h+2)*m_items;
  if (idealHeight > (192/2))
  {
    idealHeight = 192/2;
  }

  scrollPane()->setSize(m_bounds.w, idealHeight);
  scrollPane()->setScrollIncrement(m_bounds.h);
}

bool ComboBox::touch(int x, int y)
{
  // if touch is in the button > toggle "menu"
  // if menu shown and touch is on menu > select item
  // if touch not on anything, make sure menu is hidden
  if (m_bounds.hit(x, y))
  {
    m_open = not m_open;
    if (not m_open)
      button()->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
    else
      button()->setBackgroundColor(WidgetColors::COMBOBOX_SELECTED);

    return true;
  }
  if (m_open and scrollPane()->touch(x, y))
  {
    //button()->setPressed(true);
    return true;
  }
  m_open = false;
  button()->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
  return true;
}

void ComboBox::setLocation(unsigned int x, unsigned int y)
{
  Component::setLocation(x, y);
  button()->setLocation(x, y);
}

void ComboBox::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w, h);
  button()->setSize(w-4, h);
  
  scrollPane()->setSize(m_bounds.w, scrollPane()->preferredSize().h);
  m_preferredHeight = button()->preferredSize().h;
}

void ComboBox::paint(const nds::Rectangle & clip)
{
  if (not m_items)
    return;

  // TODO: add the drop down triangle.
  button()->paint(clip);
  bool down(true);
  if (m_open)
  {
    scrollPane()->setLocation(m_bounds.x, m_bounds.bottom());
    if (scrollPane()->bounds().bottom() > nds::Canvas::instance().height())
    {
      scrollPane()->setLocation(m_bounds.x, m_bounds.top() - scrollPane()->height());
      down = false;
    }
    ScrollPane::setPopup(scrollPane());
  }
  else
  {
    ScrollPane::removePopup(scrollPane());
  }
  // draw the little drop down box thing.
  int headX = m_bounds.right() - 3;
  int headY = m_bounds.top() + m_bounds.h/2-1;
  if (down)
    ScrollBar::drawDownArrow(headX, headY);
  else
    ScrollBar::drawUpArrow(headX, headY);
  nds::Canvas::instance().verticalLine(headX-3, m_bounds.top(), m_bounds.h, WidgetColors::BUTTON_SHADOW);
}

void ComboBox::pressed(ButtonI * pressed)
{
  Button * button = (Button*)pressed;
  this->button()->setText(button->text());
  button->setSelected(false);
  m_open = false;
  this->button()->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
}
