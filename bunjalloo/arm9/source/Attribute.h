#ifndef Attribute_h_seen
#define Attribute_h_seen
#include <vector>
#include "UnicodeString.h"
//! Attribute for an element.
struct Attribute
{
  std::string name;   //!< name of the attribute.
  UnicodeString value;  //!< value, stripped of leading/trailing quotes.
};

typedef std::vector<Attribute*> AttributeVector;
#endif
