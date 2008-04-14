#ifndef NodeDumper_h_seen
#define NodeDumper_h_seen

namespace nds
{
  class File;
}
#include "UnicodeString.h"
#include "DefaultVisitor.h"

class NodeDumper: public DefaultVisitor
{
  public:
    NodeDumper(const std::string & outfile);
    ~NodeDumper();

    virtual bool visit(HtmlAnchorElement & element);
    virtual void begin(HtmlAnchorElement & element);
    virtual void end(HtmlAnchorElement & element);

    virtual bool visit(HtmlElement & element);
    virtual void begin(HtmlElement & element);
    virtual void end(HtmlElement & element);

  private:
    nds::File & m_file;

    // add an attribute like this: name="value", value extracted from element
    void addAttribute(HtmlElement & element, const std::string & name);
};
#endif
