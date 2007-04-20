#ifndef InputText_h_seen
#define InputText_h_seen

#include "FormControl.h"
class HtmlElement;
class TextArea;

class InputText: public FormControl
{

  public:
    InputText(HtmlElement * element, const TextArea * textArea);
    virtual void draw(TextArea * gfx);
    virtual InputType inputType() const;
    virtual void input(const UnicodeString & str);

  private:
    // the user input
};
#endif
