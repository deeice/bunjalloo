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
  if (data[0] < 0xc0 or data[0] > 0xfd)
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
