#include "UnicodeString.h"
#include "UTF8.h"
std::string unicode2string(const UnicodeString & ustr)
{
  std::string str;
  UnicodeString::const_iterator it(ustr.begin());
  unsigned char encoded[6];
  for ( ; it != ustr.end() ; ++it)
  {
    int used = UTF8::encode(*it, encoded);
    if (used == 1) {
      str += *it;
    }
    else
    {
      char buffer[4];
      for (int i = 0; i < used; ++i)
      {
        // convert to %hex
        str += '%';
        sprintf(buffer, "%02X", encoded[i]);
        str += buffer;
      }
    }
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

void stripWhitespace(std::string & modify)
{
  if (modify.empty())
    return;
  static const std::string delimter(" \r\n	");
  int firstNonBlank = modify.find_first_not_of(delimter);
  int lastNonBlank = modify.find_last_not_of(delimter);
  modify = modify.substr(firstNonBlank, (lastNonBlank-firstNonBlank+1));
}

