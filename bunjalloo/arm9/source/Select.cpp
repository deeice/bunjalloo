#include "Select.h"
#include "Rectangle.h"
#include "HtmlElement.h"
#include "HtmlOptionElement.h"
#include "TextArea.h"


Select::Select(HtmlElement * select):
  FormControl(select)
{}

void Select::addOption(const HtmlElement * option, const TextArea* textArea)
{
  if (option->hasChildren())
  {
    if (option->firstChild()->isa("#TEXT"))
    {
      // option has name - the text - and a value (if set)
      m_options.push_back(option);
      int size = textArea->textSize(option->firstChild()->text());
      if ( size > m_size->w)
      {
        if (size > MAX_SIZE)
          size = MAX_SIZE;
        m_size->w = size;
      }
    }
  }
}

void Select::draw(TextArea * textArea)
{
  FormControl::draw(textArea);
  ElementVector::const_iterator it(m_options.begin());
  for (; it != m_options.end(); ++it)
  {
    // print the option
    const HtmlOptionElement * option( (HtmlOptionElement*)(*it));
    if ( option->selected() )
    {
      // test setting the value attribute!
      textArea->printu(option->firstChild()->text());
      break;
    }
  }
}
