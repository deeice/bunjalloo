#include "ndspp.h"
#include "libnds.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "Font.h"
#include "vera.h"

TextArea * TextAreaFactory::create()
{
  TextArea * t = new TextArea(new Font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start));
  t->setPalette((const char*)_binary_vera_pal_bin_start, 32);
  return t;
}
