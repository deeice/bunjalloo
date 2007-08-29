#ifndef TextField_h_seen
#define TextField_h_seen
#include "TextContainer.h"
#include "UnicodeString.h"

class TextArea;
class TextField: public TextContainer
{
  public:
    TextField(const UnicodeString & text);

    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

};
#endif
