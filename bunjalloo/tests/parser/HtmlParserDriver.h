#ifndef HtmlParserDriver_h_seen
#define HtmlParserDriver_h_seen

#include "UnicodeString.h"
#include "HtmlParser.h"
#include <vector>

class HtmlParserDriver : public HtmlParser
{

  public :

    UnicodeString m_data;
    std::vector<std::string> m_tags;
    std::vector< AttributeVector > m_attributes;

  protected:
    virtual void handleStartEndTag(const std::string & tag, const AttributeVector & attrs);
    virtual void handleStartTag(const std::string & tag, const AttributeVector & attrs);
    virtual void handleEndTag(const std::string & tag);
    virtual void handleData(unsigned int ucodeChar);

};

#endif
