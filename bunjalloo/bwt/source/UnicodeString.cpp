/*
  Copyright (C) 2007,2008 Richard Quirk

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
using std::string;
using std::vector;

std::string unicode2string(const UnicodeString & ustr, bool byteencode)
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
        if (byteencode)
        {
          str += (char)encoded[i];
        }
        else
        {
          // convert to %hex
          str += '%';
          sprintf_platform(buffer, "%02X", encoded[i]);
          str += buffer;
        }
      }
    }
  }
  return str;
}

UnicodeString string2unicode(const std::string & str)
{
  UnicodeString ustr;
  const char * p = str.c_str();
  size_t len = str.length();
  for (size_t i = 0 ; i < len; )
  {
    unsigned int val;
    unsigned int read = UTF8::decode(p, val);
    ustr += val;
    p += read;
    i += read;
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
  size_t firstNonBlank = modify.find_first_not_of(delimter);
  size_t lastNonBlank = modify.find_last_not_of(delimter);
  if (firstNonBlank != std::string::npos and lastNonBlank != std::string::npos)
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

void tokenize(const std::string& str,
              std::vector<std::string>& tokens,
              const std::string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

