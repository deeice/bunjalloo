#ifndef FormTextArea_h_seen
#define FormTextArea_h_seen

#include "UnicodeString.h"
#include "ScrollPane.h"
#include "TextEntryI.h"
#include "TextListener.h"

class HtmlElement;
class EditableTextArea;
class FormTextArea: public ScrollPane, public TextEntryI, public TextListener
{
  public:
    FormTextArea(HtmlElement * element);
    ~FormTextArea() {}

    void layoutViewer();

    void editText(TextEntryI *);

    /** Implements TextEntryI by proxy to the textArea.  */
    virtual bool isMultiLine() const;
    virtual void text(UnicodeString & returnString) const;
    virtual void setText(const UnicodeString & text);

  private:
    inline EditableTextArea * textArea();
    inline EditableTextArea * textArea() const;
    HtmlElement * m_element;

};
#endif
