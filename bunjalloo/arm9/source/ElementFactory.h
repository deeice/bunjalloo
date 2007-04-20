#ifndef ElementFactory_h_seen
#define ElementFactory_h_seen

#include <string>
#include "Attribute.h"
class HtmlElement;

class ElementFactory
{
  public:
    static HtmlElement * create(const std::string & elementType);
    static HtmlElement * create(const std::string & elementType,
        const AttributeVector & attrs);

    static void remove(HtmlElement * element);

  private:
    ElementFactory();
    ~ElementFactory();
};
#endif
