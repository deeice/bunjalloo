#ifndef HtmlOptionElement_h_seen
#define HtmlOptionElement_h_seen
#include "HtmlElement.h"

class HtmlOptionElement : public HtmlElement
{
  public:
    HtmlOptionElement(const std::string & tagName) 
      : HtmlElement(tagName), m_selected(false) { }

    virtual void setAttribute(const std::string & name, const UnicodeString & value);

    bool selected() const;
  private:
    bool m_selected;
    UnicodeString m_value;
    UnicodeString m_label;

    const UnicodeString * attributePtr(const std::string & name) const;
};
#endif
