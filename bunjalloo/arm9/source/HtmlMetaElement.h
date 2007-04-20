#ifndef HtmlMetaElement_h_seen
#define HtmlMetaElement_h_seen
#include "HtmlElement.h"

class HtmlMetaElement : public HtmlElement
{
  public:
    HtmlMetaElement(const std::string & tagName);
    HtmlElement * clone() const;
  protected:
    UnicodeString m_httpEquiv;
    UnicodeString m_content;
    const UnicodeString * attributePtr(const std::string & name) const;
    void copyAttributes(HtmlElement * copyTo) const;
};
#endif
