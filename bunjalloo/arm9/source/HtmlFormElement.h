#ifndef HtmlFormElement_h_seen
#define HtmlFormElement_h_seen

#include "HtmlElement.h"
class HtmlFormElement:public HtmlElement
{
  public:
    HtmlFormElement(const std::string & tagName) 
      : HtmlElement(tagName)
    {}

  protected:
    UnicodeString m_action;
    UnicodeString m_method;
    const UnicodeString * attributePtr(const std::string & name) const;
};
#endif
