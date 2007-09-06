#ifndef FormCheckBox_h_seen
#define FormCheckBox_h_seen
#include "CheckBox.h"
#include "ButtonListener.h"

class HtmlElement;
class ButtonI;
class FormCheckBox: public CheckBox, public ButtonListener
{
  public:
    FormCheckBox(HtmlElement * element);

    virtual void pressed(ButtonI * self);

  private:
    HtmlElement * m_element;
};
#endif
