#include "FormTextArea.h"
#include "HtmlElement.h"
#include "HtmlTextAreaElement.h"
#include "TextAreaFactory.h"
#include "EditableTextArea.h"
#include "Canvas.h"

FormTextArea::FormTextArea(HtmlElement * element) :
  m_element(element)
{
  EditableTextArea * text( (EditableTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_EDIT));
  setStretchChildren();
  add(text);
  int fontSize(text->font().height());
  setSize(nds::Canvas::instance().width(), fontSize);
  if (m_element->hasChildren() and m_element->firstChild()->isa("#TEXT"))
  {
    text->setText(m_element->firstChild()->text());
  }
  HtmlTextAreaElement * textElement((HtmlTextAreaElement*)m_element);
  text->setSize(fontSize * textElement->cols(), fontSize * textElement->rows());
  m_preferredHeight = fontSize * textElement->rows();
  text->setListener(this);
  text->setParentScroller(this);
  layoutViewer();
}

void FormTextArea::layoutViewer()
{
  int scrollTo(textArea()->caretLine() * textArea()->font().height() * 256 / textArea()->preferredSize().h);
  scrollToPercent(scrollTo);
}

void FormTextArea::editText(TextEntryI * entry)
{
  if (listener())
    listener()->editText(this);
}

inline EditableTextArea * FormTextArea::textArea()
{
  return (EditableTextArea*)m_children.back();
}

inline EditableTextArea * FormTextArea::textArea() const
{
  return (EditableTextArea*)m_children.back();
}

bool FormTextArea::isMultiLine() const
{
  return textArea()->isMultiLine();
}

void FormTextArea::text(UnicodeString & returnString) const
{
  textArea()->text(returnString);
}

void FormTextArea::setText(const UnicodeString & text)
{
  textArea()->setText(text);
  if (m_element->hasChildren() and m_element->firstChild()->isa("#TEXT"))
  {
    m_element->firstChild()->text() = text;
  }
  layoutViewer();
}

