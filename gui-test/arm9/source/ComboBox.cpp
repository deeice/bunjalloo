#include "ComboBox.h"
#include "ScrollPane.h"
#include "Button.h"

ComboBox::ComboBox():
  m_items(0),
  m_open(false),
  m_scrollPane(new ScrollPane),
  m_button(new Button(string2unicode("")))
{
  // implement as a scroll bar + buttons?
}

ComboBox::~ComboBox()
{}

void ComboBox::addItem(const UnicodeString & item)
{
  if (m_button->label().empty())
  {
    m_button->setLabel(item);
    printf("Set label: %s\n", unicode2string(item).c_str());
  }
  Button * b = new Button(item);
  // need to add callbacks to button so that the combobox is updated.
  b->setSize(m_bounds.w, m_bounds.h);
  m_items++;
  m_scrollPane->add(b);
  m_scrollPane->setTopLevel(false);
  m_scrollPane->setSize(m_bounds.w, m_bounds.h*m_items);
  m_scrollPane->setScrollIncrement(m_bounds.h);
}

bool ComboBox::touch(int x, int y)
{
  // if touch is in the button > toggle "menu"
  // if menu shown and touch is on menu > select item
  // if touch not on anything, make sure menu is hidden
  if (m_button->touch(x, y))
  {
    m_open = not m_open;
    m_button->setPressed(m_open);
    return true;
  }
  if (m_open and m_scrollPane->touch(x, y))
  {
    m_button->setPressed(true);
    return true;
  }

  m_open = false;
  return true;
}

void ComboBox::setLocation(unsigned int x, unsigned int y)
{
  Component::setLocation(x, y);
  m_button->setLocation(x, y);
  m_scrollPane->setLocation(x, y + m_bounds.h);
}

void ComboBox::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w, h);
  m_button->setSize(w, h);
  
  m_scrollPane->setSize(m_bounds.w, m_scrollPane->preferredSize().h);
  m_preferredHeight = m_button->preferredSize().h;
}

void ComboBox::paint(const nds::Rectangle & clip)
{
  // TODO: add the drop down triangle.
  m_button->paint(clip);
  if (m_open)
  {
    ScrollPane::s_popup = m_scrollPane;
    m_scrollPane->setLocation(m_bounds.x, m_bounds.bottom());
    // paint the scrollpane too.
    //nds::Rectangle thisClip = {clip.x, clip.y+m_bounds.h, m_bounds.w, m_bounds.h};
    //m_scrollPane->paint(clip);
    // this has to be done as a pop-up menu from within the toplevel ScrollPane - otherwise it gets drawn over.
    //m_scrollPane->paint(thisClip);
  }
  else
  {
    ScrollPane::s_popup = 0;
  }
}

