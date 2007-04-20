#include <vector>
#include "ElementFactory.h"
#include "HtmlAnchorElement.h"
#include "HtmlImageElement.h"
#include "HtmlMetaElement.h"
#include "HtmlBodyElement.h"
#include "HtmlBlockElement.h"
#include "HtmlPreElement.h"
#include "HtmlOptionElement.h"
#include "HtmlFormElement.h"
#include "HtmlInputElement.h"
#include "HtmlElement.h"


HtmlElement * ElementFactory::create(const std::string & elementType)
{
  if (elementType == "a")
  {
    return new HtmlAnchorElement(elementType);
  }
  else if (elementType == "meta")
  {
    return new HtmlMetaElement(elementType);
  }
  else if (elementType == "img")
  {
    return new HtmlImageElement(elementType);
  }
  else if (elementType == "body")
  {
    return new HtmlBodyElement(elementType);
  }
  else if (elementType == "p" or elementType == "div")
  {
    return new HtmlBlockElement(elementType);
  }
  else if (elementType == "pre")
  {
    return new HtmlPreElement(elementType);
  }
  else if (elementType == "option")
  {
    return new HtmlOptionElement(elementType);
  }
  else if (elementType == "form")
  {
    return new HtmlFormElement(elementType);
  }
  else if (elementType == "input")
  {
    return new HtmlInputElement(elementType);
  }
  /*
  else if (elementType == "ul" or elementType == "ol" or elementType == "li")
  {
    // lists have a pricipal box, then inline item boxes.
    return new HtmlListElement(elementType);
  }
  */

  return new HtmlElement(elementType);
}

HtmlElement * ElementFactory::create(const std::string & elementType,
    const AttributeVector & attrs)
{
  HtmlElement * element(create(elementType));
  AttributeVector::const_iterator it(attrs.begin());
  for (; it != attrs.end(); ++it) {
    Attribute * attr(*it);
    element->setAttribute(attr->name, attr->value);
  }
  return element;
}

void ElementFactory::remove(HtmlElement * element)
{
  delete element;
}
