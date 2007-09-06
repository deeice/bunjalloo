#include "FormCheckBox.h"
#include "HtmlElement.h"
#include "HtmlInputElement.h"

FormCheckBox::FormCheckBox(HtmlElement * element):m_element(element)
{
  // listen to clicks from myself
  setListener(this);
  setSelected(( (HtmlInputElement*) element)->checked());
}

void FormCheckBox::pressed(ButtonI * self)
{
  self->setSelected( not self->selected());
  ( (HtmlInputElement*)m_element)->setChecked(self->selected());
}
