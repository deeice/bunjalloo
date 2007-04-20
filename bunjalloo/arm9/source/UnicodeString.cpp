#include "UnicodeString.h"
std::string unicode2string(const UnicodeString & ustr)
{
  std::string str;
  UnicodeString::const_iterator it(ustr.begin());
  for ( ; it != ustr.end() ; ++it)
  {
    str += *it;
  }
  return str;
}

UnicodeString string2unicode(const std::string & str)
{
  UnicodeString ustr;
  std::string::const_iterator it(str.begin());
  for ( ; it != str.end() ; ++it)
  {
    ustr += *it;
  }
  return ustr;
}

bool isWhitespace(unsigned int value)
{
  if (value > 0x0020)
    return false;
  return (
         value == 0x0020  // SPACE
      or value == 0x0009  // CHARACTER TABULATION
      or value == 0x000A  // LINE FEED
      or value == 0x000B  // LINE TABULATION
      or value == 0x000C  // FORM FEED
      );
}

