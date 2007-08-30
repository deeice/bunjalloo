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
  m_open(false),
  m_scrollPane(new ScrollPane),
  m_button(new Button(string2unicode("")))
{
  // implement as a scroll bar + buttons?
  m_scrollPane->setBackgroundColor(WidgetColors::COMBOBOX_DROP_DOWN);
  m_button->setDecoration(false);
  m_button->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
}

ComboBox::~ComboBox()
{
  delete m_scrollPane;
  delete m_button;
}

void ComboBox::addItem(const UnicodeString & item)
{
  if (m_button->text().empty())
  {
    m_button->setText(item);
  }
  Button * b = new Button(item);
  b->setListener(this);
  // need to add callbacks to button so that the combobox is updated.
  b->setSize(m_bounds.w, m_bounds.h);
  b->setDecoration(false);
  b->setBackgroundColor(WidgetColors::COMBOBOX_DROP_DOWN);
  m_items++;
  m_scrollPane->add(b);
  m_scrollPane->setTopLevel(false);
  int idealHeight = (m_bounds.h+2)*m_items;
  if (idealHeight > (192/2))
  {
    idealHeight = 192/2;
  }

  m_scrollPane->setSize(m_bounds.w, idealHeight);
  m_scrollPane->setScrollIncrement(m_bounds.h);
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
      m_button->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
    else
      m_button->setBackgroundColor(WidgetColors::COMBOBOX_SELECTED);

    return true;
  }
  if (m_open and m_scrollPane->touch(x, y))
  {
    //m_button->setPressed(true);
    return true;
  }
  m_open = false;
  m_button->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
  return true;
}

void ComboBox::setLocation(unsigned int x, unsigned int y)
{
  Component::setLocation(x, y);
  m_button->setLocation(x, y);
}

void ComboBox::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w, h);
  m_button->setSize(w-4, h);
  
  m_scrollPane->setSize(m_bounds.w, m_scrollPane->preferredSize().h);
  m_preferredHeight = m_button->preferredSize().h;
}

void ComboBox::paint(const nds::Rectangle & clip)
{
  // TODO: add the drop down triangle.
  m_button->paint(clip);
  bool down(true);
  if (m_open)
  {
    m_scrollPane->setLocation(m_bounds.x, m_bounds.bottom());
    if (m_scrollPane->bounds().bottom() > nds::Canvas::instance().height())
    {
      m_scrollPane->setLocation(m_bounds.x, m_bounds.top() - m_scrollPane->height());
      down = false;
    }
    ScrollPane::setPopup(m_scrollPane);
  }
  else
  {
    ScrollPane::removePopup(m_scrollPane);
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
  m_button->setText(button->text());
  button->setSelected(false);
  m_open = false;
  m_button->setBackgroundColor(WidgetColors::COMBOBOX_FOREGROUND);
}
