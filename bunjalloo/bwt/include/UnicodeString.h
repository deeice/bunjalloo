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
#ifndef UnicodeString_h_seen
#define UnicodeString_h_seen
#include <string>
#include <vector>

/** This typedef is used to represent Unicode strings. Unicode character
 * values are up to 32 bit so an unsigned int is used.
 */
typedef unsigned short unicodeint;
typedef std::basic_string<unicodeint> UnicodeString;

/** Convert a Unicode string to a regular char string, by "escaping" Unicode characters.
 * @param ustr the Unicode string to convert.
 * @param byteencode encode to UTF8 bytes. If false, then non-ASCII characters
 *                          are encoded in HTML escape format (%8F, etc)
 * @return the char string with any Unicode characters escaped to hex values.
 */
std::string unicode2string(const UnicodeString & ustr, bool byteencode=false);

/** Convert a char string to a Unicode one.
 * @param str the char string to convert.
 * @return the equivalent Unicode string.
 */
UnicodeString string2unicode(const std::string & str);

/** Check if the value is white space.
 * @param value the character value to check.
 * @return true if @a value is whitespace, false otherwise.
 */
bool isWhitespace(unsigned int value);

/** Strip the whitespace from the front and back of the given string.
 * @param modify the string that is stripped in place.
 */
void stripWhitespace(std::string & modify);
void stripWhitespace(UnicodeString & modify);

template<typename StringType>
void tokenize(const StringType& str,
              std::vector<StringType>& tokens,
              const StringType& delimiters)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

#endif
