#ifndef HtmlPreElement_h_seen
#define HtmlPreElement_h_seen

#include "HtmlElement.h"

class HtmlPreElement: public HtmlElement
{

  public:
    HtmlPreElement(const std::string & tagName) 
      : HtmlElement(tagName) 
    {
      m_block = true;
    }

    virtual void appendText(unsigned int value);
  
};
#endif
