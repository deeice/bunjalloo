#ifndef HtmlInputElement_h_seen
#define HtmlInputElement_h_seen

#include "HtmlElement.h"
class HtmlInputElement:public HtmlElement
{
  public:
    HtmlInputElement(const std::string & tagName) 
      : HtmlElement(tagName)
    {}

  protected:
    UnicodeString m_type;
    UnicodeString m_value;
    UnicodeString m_name;
    UnicodeString m_size;
    const UnicodeString * attributePtr(const std::string & name) const;
};
#endif
