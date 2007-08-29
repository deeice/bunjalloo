#ifndef TextContainer_h_seen
#define TextContainer_h_seen

#include "Component.h"
#include "UnicodeString.h"

class TextArea;
class TextContainer: public Component
{
  public:
    TextContainer();
    TextContainer(const UnicodeString & text);
    virtual ~TextContainer();

    virtual void setSize(unsigned int w, unsigned int h);

    void setText(const UnicodeString & text);
    const UnicodeString & text() const;

    void setBackgroundColor(unsigned short color);

    void paint(const nds::Rectangle & clip);

  private:
    UnicodeString m_text;
    TextArea * m_textArea;
    void layout();
};
#endif
