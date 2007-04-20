#ifndef HtmlImageElement_h_seen
#define HtmlImageElement_h_seen
#include "HtmlElement.h"

class HtmlImageElement : public HtmlElement
{
  public:
    HtmlImageElement(const std::string & tagName) 
      : HtmlElement(tagName),m_hasAltText(false)
    {}

    HtmlElement * clone() const;
    virtual void setAttribute(const std::string & name, 
        const UnicodeString & value);
    bool hasAltText() const;
  protected:
    UnicodeString m_src;
    UnicodeString m_alt;
    UnicodeString m_name;
    bool m_hasAltText;
    const UnicodeString * attributePtr(const std::string & name) const;
    void copyAttributes(HtmlElement * copyTo) const;
};
#endif
