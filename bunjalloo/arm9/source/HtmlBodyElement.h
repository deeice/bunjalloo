#ifndef HtmlBodyElement_h_seen
#define HtmlBodyElement_h_seen
#include "HtmlElement.h"

class HtmlBodyElement : public HtmlElement
{
  public:

    HtmlBodyElement(const std::string & tagName) 
      : HtmlElement(tagName)
    {}

    HtmlElement * clone() const;
    unsigned int bgColor() const;
  protected:
    UnicodeString m_bgcolor;
    const UnicodeString * attributePtr(const std::string & name) const;
    void copyAttributes(HtmlElement * copyTo) const;
};
#endif
