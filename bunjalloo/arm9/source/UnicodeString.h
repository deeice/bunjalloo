#ifndef UnicodeString_h_seen
#define UnicodeString_h_seen
#include <string>

/** This typedef is used to represent unicode strings. Unicode character
 * values are up to 32 bit so an unsigned int is used.
 */
typedef std::basic_string<unsigned int> UnicodeString;

std::string unicode2string(const UnicodeString & ustr);
UnicodeString string2unicode(const std::string & str);
bool isWhitespace(unsigned int value);

#endif
