#ifndef TextField_h_seen
#define TextField_h_seen
#include "TextContainer.h"
#include "TextEntryI.h"
#include "UnicodeString.h"

class TextArea;
/** An entry that displays a single line of text.*/
class TextField: public TextContainer, public TextEntryI
{
  public:

    /** Construct a new TextField.
     * @param text the initial text to show.
     */
    TextField(const UnicodeString & text);

    // from TextEntryI
    virtual void text(UnicodeString & returnString) const
    {
      returnString = TextContainer::text();
    }
    virtual void setText(const UnicodeString & text)
    {
      TextContainer::setText(text);
    }
    virtual bool isMultiLine() const 
    {
      return false;
    }

    // From Component.
    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

};
#endif
