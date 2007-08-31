#include "EditableTextArea.h"
#include "WidgetColors.h"
#include "TextListener.h"
#include "Canvas.h"
using nds::Canvas;

EditableTextArea::EditableTextArea(Font * font):TextArea(font)
{
}

void EditableTextArea::paint(const nds::Rectangle & clip)
{
  TextArea::paint(clip);
  Canvas::instance().horizontalLine(clip.x, clip.top(), clip.w, WidgetColors::BUTTON_SHADOW);
  Canvas::instance().verticalLine(clip.left(), clip.top(), clip.h, WidgetColors::BUTTON_SHADOW);
  Canvas::instance().horizontalLine(clip.x, clip.bottom()-1, clip.w, WidgetColors::SCROLLBAR_ARROW);
  Canvas::instance().verticalLine(clip.right(), clip.top(), clip.h, WidgetColors::SCROLLBAR_ARROW);
}

const UnicodeString & EditableTextArea::text() const
{
  return document();
}

void EditableTextArea::setText(const UnicodeString & text)
{
  clearText();
  appendText(text);
}

bool EditableTextArea::touch(int x, int y)
{
  if (m_bounds.hit(x, y))
  {
    if (listener())
    {
      listener()->editText(this);
    }
    return true;
  }
  return false;
}
