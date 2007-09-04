/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef UnicodeString_h_seen
#define UnicodeString_h_seen
#include <string>

/** This typedef is used to represent Unicode strings. Unicode character
 * values are up to 32 bit so an unsigned int is used.
 */
typedef std::basic_string<unsigned int> UnicodeString;

/** Convert a Unicode string to a regular char string, by "escaping" Unicode characters.
 * @param ustr the Unicode string to convert.
 * @return the char string with any Unicode characters escaped to hex values.
 */
std::string unicode2string(const UnicodeString & ustr);

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

#endif
