#include "ButtonI.h"
#include "HtmlElement.h"
#include "HtmlInputElement.h"
#include "FormRadio.h"

void FormRadio::add(ButtonI * button, HtmlElement * element)
{
  ButtonGroup::add(button);
  m_buttonElements[button] = element;
  if (((HtmlInputElement*)element)->checked() or m_buttonElements.size() == 1)
  {
    pressed(button);
  }
}

void FormRadio::pressed(ButtonI * button)
{
  ButtonGroup::pressed(button);
  // find the selected one.
  ButtonMap::const_iterator it(m_buttonElements.begin());
  for (; it != m_buttonElements.end(); ++it)
  {
    HtmlInputElement * input((HtmlInputElement*)it->second);
    input->setChecked(it->first->selected());
  }
}
