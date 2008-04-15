/*
  Copyright (C) 2008 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Button.h"
#include "Canvas.h"
#include "ComboBox.h"
#include "Component.h"
#include "Config.h"
#include "Document.h"
#include "EditPopup.h"
#include "HtmlElement.h"
#include "Language.h"
#include "Link.h"
#include "LinkListener.h"
#include "NodeDumper.h"
#include "Rectangle.h"
#include "ScrollPane.h"
#include "Stylus.h"
#include "View.h"
#include "WidgetColors.h"

EditPopup::EditPopup(View * parent):
  m_parent(parent)
{
  setVisible(false);
  addItem(T("edsel"));
  addItem(T("delsel"));
  m_preferredHeight = scrollPane()->height();
  m_preferredWidth = scrollPane()->width();
  setSize(m_preferredWidth, m_preferredHeight);
  Stylus::instance()->registerListener(this);
}

EditPopup::~EditPopup()
{
  Stylus::instance()->unregisterListener(this);
}

void EditPopup::setElement(HtmlElement * element)
{
  m_element = element;
}

void EditPopup::pressed(ButtonI * button)
{
  // A button on the scroll bar is pressed. See which it was...
  if (button == ((Button*)scrollPane()->childAt(0)))
  {
    // edit the link
  }
  else
  {
    // Remove the element from the bookmark file.
    // This is *insane*.
    HtmlElement * p(m_element->parent());
    p->remove(m_element);
    // now dump the Bookmark file to disk
    {
      NodeDumper dumper(Config::BOOKMARK_FILE);
      HtmlElement * root((HtmlElement*)m_parent->document().rootNode());
      root->accept(dumper);
    }
    m_parent->bookmarkUrl();
  }
  // make sure the selection is reset.
  ((Button*)scrollPane()->childAt(0))->setSelected(false);
  ((Button*)scrollPane()->childAt(1))->setSelected(false);
  setVisible(false);
}

void EditPopup::paint(const nds::Rectangle & clip)
{
  if ( not m_visible)
  {
    return;
  }
  scrollPane()->setLocation(m_bounds.x, m_bounds.top());
  if (scrollPane()->bounds().bottom() > nds::Canvas::instance().height())
  {
    scrollPane()->setLocation(m_bounds.x, m_bounds.top() - scrollPane()->height());
  }

  if (scrollPane()->bounds().right() > nds::Canvas::instance().width())
  {
    scrollPane()->setLocation(m_bounds.x - scrollPane()->width(), scrollPane()->bounds().top());
  }

  nds::Rectangle scrollPaneClip(scrollPane()->bounds());
  nds::Canvas::instance().fillRectangle(scrollPaneClip.x,
      scrollPaneClip.y, scrollPaneClip.w, scrollPaneClip.h, scrollPane()->backgroundColor());
  scrollPane()->paint(scrollPane()->bounds());
}

bool EditPopup::stylusUp(const Stylus * stylus)
{
  if (not visible())
  {
    return false;
  }
  FOR_EACH_CHILD(stylusUp);
  return false;
}

bool EditPopup::stylusDownFirst(const Stylus * stylus)
{
  if (not visible())
  {
    return false;
  }
  FOR_EACH_CHILD(stylusDownFirst);
  // nothing hit? hide me then
  setVisible(false);
  return false;
}


