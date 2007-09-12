/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "UnicodeString.h"
#include "UTF8.h"
#ifdef ARM9
#define sprintf_platform siprintf
#else
#define sprintf_platform sprintf
#endif

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
        sprintf_platform(buffer, "%02X", encoded[i]);
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

void stripWhitespace(UnicodeString & modify)
{
  if (modify.empty())
    return;
  static const UnicodeString delimter(string2unicode(" \r\n	"));
  int firstNonBlank = modify.find_first_not_of(delimter);
  int lastNonBlank = modify.find_last_not_of(delimter);
  modify = modify.substr(firstNonBlank, (lastNonBlank-firstNonBlank+1));
}

