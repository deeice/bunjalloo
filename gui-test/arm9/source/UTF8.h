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
#ifndef UTF8_h_seen
#define UTF8_h_seen

class UTF8
{
  public:

    /** MALFORMED characters that do not conform to the UTF-8 standard.
     * The recommended solution is to print a <?> type marker.
     */
    static unsigned int MALFORMED;

    /** Decode a UTF-8 character array to a 32 bit value.
     * @param p pointer to the data.
     * @param returnValue the corresponding 31-bit Universal Character Set value.
     * @return the number of bytes in p that have been read.
     * */
    static unsigned int decode(const void * p, unsigned int & returnValue);

    /** Encode a 32 bit value to an array of 4 chars.
     * @param ucode 32 bit character code
     * @param encoded an array of 6 characters that shall contain the encoded data.
     * @return the number of bytes that are used in encoded.
     * */
    static int encode(signed int ucode, unsigned char encoded[6]);

    /* Interesting fact: ISO-8859-1 is cast from char to int. The characters
     * map directly to the ones in the font data. Hurray.
     */

};

#endif
