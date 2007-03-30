#include <string.h>
#include "libnds.h"

void swiCopy(const void * source, void * dest, int flags)
{
  size_t amount(flags & 0xfffff);
  if (flags & COPY_MODE_WORD)
  {
    amount /= 2;
  }
  if (flags & COPY_MODE_FILL)
  {
    // ??
  }
  // void *memcpy(void *dest, const void *src, size_t n);
  ::memcpy(dest, source, amount*2);
}
