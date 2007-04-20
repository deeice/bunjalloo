#ifndef Button_h_seen
#define Button_h_seen

#include "FormControl.h"

class HtmlElement;
/** A crappy button thing.
 */
class Button: public FormControl
{
  public:
    Button(HtmlElement * element, const TextArea * textArea);
    virtual void draw(TextArea * gfx);

  private:
};
#endif
