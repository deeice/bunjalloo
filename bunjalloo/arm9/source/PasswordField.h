#ifndef PasswordField_h_seen
#define PasswordField_h_seen

#include "InputText.h"
class HtmlElement;
class TextArea;

class PasswordField: public InputText
{

  public:
    PasswordField(HtmlElement * element, const TextArea * textArea);
    virtual void draw(TextArea * gfx);

  private:
    // the user input
};
#endif
