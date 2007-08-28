#ifndef TextField_h_seen
#define TextField_h_seen
#include "Component.h"
#include "UnicodeString.h"

class TextArea;
class TextField: public Component
{
  public:
    TextField(const UnicodeString & text);
    ~TextField();
    void setText(const UnicodeString & text);

    virtual void setSize(unsigned int w, unsigned int h);
    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

  private:
    UnicodeString m_text;
    TextArea * m_textArea;
    void layout();
};
#endif
