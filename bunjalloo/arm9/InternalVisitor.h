#ifndef InternalVisitor_h_seen
#define InternalVisitor_h_seen
#include "DefaultVisitor.h"
#include "UnicodeString.h"

class InternalVisitor: public DefaultVisitor
{
  public:
    InternalVisitor(const std::string & name);

    inline bool found() const
    {
      return m_found;
    }

    inline int index() const
    {
      return m_index;
    }

    bool visit(HtmlAnchorElement & element);

  private:
    UnicodeString m_name;
    int m_index;
    bool m_found;
};
#endif
