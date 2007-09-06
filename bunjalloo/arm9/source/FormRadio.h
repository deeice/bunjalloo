#ifndef FormRadio_h_seen
#define FormRadio_h_seen
#include <map>
#include "ButtonGroup.h"

class ButtonI;
class HtmlElement;
class FormRadio: public ButtonGroup
{
  public:
    void add(ButtonI * button, HtmlElement * element);

    void pressed(ButtonI * button);

  private:
    typedef std::map<ButtonI*, HtmlElement *> ButtonMap;
    ButtonMap m_buttonElements;
};

#endif
