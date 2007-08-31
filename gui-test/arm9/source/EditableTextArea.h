#ifndef EditableTextArea_h_seen
#define EditableTextArea_h_seen

#include "TextArea.h"
#include "TextEntryI.h"

class EditableTextArea: public TextArea, public TextEntryI
{
  public:
    EditableTextArea(Font * font);

    virtual const UnicodeString & text() const;

    virtual void setText(const UnicodeString & text);

    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);


};

#endif
