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
#include "UTF8.h"
/*#include "SDLhandler.h"*/
unsigned int UTF8::MALFORMED((unsigned int)0xfffd);

bool continuation(const unsigned char * data, int count)
{
  for (int i = 0; i < count; ++i)
  {
    if ((data[i]&0xC0)!= 0x80)
      return false;
  }
  return true;
}

unsigned int UTF8::decode(const void * p, unsigned int & returnValue)
{
  const unsigned char * data = static_cast<const unsigned char*>(p);
  int returnLength = 1;
  if (data[0] < 0x80) {
    // regular ASCII char
    // 0x00000000 - 0x0000007F:
    returnValue = data[0];
    return 1;
  }
  if (data[0] < 0xC0 or data[0] > 0xfd)
  {
    // not a valid start byte.
    goto malformed;
  }

  if ( (data[0] & 0xE0) == 0xC0) {
    // 0x00000080 - 0x000007FF:
    if (continuation(&data[1],1)) {
      returnValue =  ((data[0] & 0x1f)<<6) | (data[1]&0x3f);
      returnLength = 2;
      goto checkLength;
    } else {
      goto malformed;
    }
  }
  if ( (data[0] & 0xF0) == 0xE0) {
    // 0x00000800 - 0x0000FFFF:
    if (continuation(&data[1],2)) {
      returnValue =  ((data[0] & 0xf)<<(12)) | ((data[1]&0x3f)<<6) | (data[2]&0x3f);
      returnLength = 3;
      goto checkLength;
    } else {
      goto malformed;
    }
  }

  if ( (data[0] & 0xF8) == 0xF0) {
    // 0x00010000 - 0x001FFFFF:
    if (continuation(&data[1],3)) {
      returnValue =  ((data[0] & 0x7)<<(18)) | ((data[1]&0x3f)<<12) | ((data[2]&0x3f)<<6) | (data[3]&0x3f);
      returnLength = 4;
      goto checkLength;
    } else {
      goto malformed;
    }
  }

  if ( (data[0] & 0xFC) == 0xF8) {
    //0x00200000 - 0x03FFFFFF:
    if (continuation(&data[1],4)) {
      returnValue =  ((data[0] & 0x3)<<(24)) | ((data[1]&0x3f)<<18) | ((data[2]&0x3f)<<12) | ((data[3]&0x3f)<<6) | (data[4]&0x3f);
      returnLength = 5;
      goto checkLength;
    } else {
      goto malformed;
    }
  }
  if ( (data[0] & 0xFE) == 0xFC) {
    // 0x04000000 - 0x7FFFFFFF:
    if (continuation(&data[1],5)) {
      returnValue =  ((data[0] & 0x1)<<(30)) | ((data[1]&0x3f)<<24) | ((data[2]&0x3f)<<18) | ((data[3]&0x3f)<<12) | ((data[4]&0x3f)<<6)
        | (data[5]&0x3f);
      returnLength = 6;
      goto checkLength;
    } else {
      goto malformed;
    }
  }

  // not implemented
  returnValue =  data[0];
  return 1;
malformed:
  returnValue = MALFORMED;
  return 1;
checkLength:
  if (
         ( (returnLength > 1) && (returnValue < 0x80))
      or ( (returnLength > 2) && (returnValue < 0x800))
      or ( (returnLength > 3) && (returnValue < 0x10000))
      or ( (returnLength > 4) && (returnValue < 0x200000))
      or ( (returnLength > 5) && (returnValue < 0x4000000))
     )
      {
    returnValue = MALFORMED;
  }
  if (returnValue != MALFORMED and 
      ( ( returnValue >= 0xd800 and returnValue <= 0xdfff)
        or returnValue == 0xfffe
        or returnValue == 0xffff
      )
     )
  {
    returnValue = MALFORMED;
  }
  return returnLength;
}

int UTF8::encode(signed int ucode, unsigned char encoded[6])
{
  if (ucode < 0x80) {
    // regular ASCII char
    // 0x00000000 - 0x0000007F:
    encoded[0] = ucode;
    return 1;
  }
  if (ucode >= 0x80 and ucode < 0x800)
  {
    // 2 bytes
    encoded[0] = 0xC0| ((ucode>>6) & 0x1f);
    encoded[1] = 0x80| (ucode & 0x3f);
    return 2;
  }
  if (ucode >= 0x800 and ucode < 0x10000)
  {
    // 3 bytes
    encoded[0] = 0xE0| ((ucode>>12) & 0xf);
    encoded[1] = 0x80| ((ucode>>6) & 0x3f);
    encoded[2] = 0x80| (ucode & 0x3f);
    return 3;
  }
  if (ucode >= 0x10000 and ucode < 0x00200000)
  {
    // 4 bytes
    encoded[0] = 0xF0| ((ucode>>18) & 0x7);
    encoded[1] = 0x80| ((ucode>>12) & 0x3f);
    encoded[2] = 0x80| ((ucode>>6) & 0x3f);
    encoded[3] = 0x80| (ucode & 0x3f);
    return 4;
  }
  if (ucode >= 0x00200000 and ucode <= 0x03FFFFFF)
  {
    // 5 bytes
    encoded[0] = 0xF8| ((ucode>>24) & 0x3);
    encoded[1] = 0x80| ((ucode>>18) & 0x3f);
    encoded[2] = 0x80| ((ucode>>12) & 0x3f);
    encoded[3] = 0x80| ((ucode>>6) & 0x3f);
    encoded[4] = 0x80| (ucode & 0x3f);
    return 5;
  }
  if (ucode >= 0x04000000 and ucode <= 0x7FFFFFFF)
  {
    // 6 bytes
    encoded[0] = 0xFC| ((ucode>>30) & 0x1);
    encoded[1] = 0x80| ((ucode>>24) & 0x3f);
    encoded[2] = 0x80| ((ucode>>18) & 0x3f);
    encoded[3] = 0x80| ((ucode>>12) & 0x3f);
    encoded[4] = 0x80| ((ucode>>6) & 0x3f);
    encoded[5] = 0x80| (ucode & 0x3f);
    return 6;
  }
  return 0;
}

