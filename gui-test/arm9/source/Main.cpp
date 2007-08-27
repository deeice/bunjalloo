#include "ndspp.h"
#include "libnds.h"
#include "TextArea.h"
#include "Canvas.h"
#include "Font.h"
#include "ScrollPane.h"
#include "vera.h"

extern const char _binary_test_map_bin_start[];

using namespace nds;
int main(int argc, char * argv[])
{
  irqInit();
  irqSet(IRQ_VBLANK,0);

  TextArea * t = new TextArea(new Font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start));
  t->setPalette((const char*)_binary_vera_pal_bin_start, 32);

  TextArea * t2 = new TextArea(new Font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start));
  t2->setPalette((const char*)_binary_vera_pal_bin_start, 32);
  t2->setBackgroundColor(Color(31,31,0));
  ScrollPane scrollPane;
  scrollPane.add(t);
  scrollPane.add(t2);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());
  scrollPane.setScrollIncrement(t->font().height());

  //std::string s(_binary_test_map_bin_start, strlen(_binary_test_map_bin_start));
  std::string s(_binary_test_map_bin_start, 1000);
  t->appendText(string2unicode(s));
  std::string s2(&_binary_test_map_bin_start[650], 1300);
  t2->appendText(string2unicode(s2));

  //Rectangle screen = {0, 0, Canvas::instance().width(), Canvas::instance().height()};
  bool needsPainting = true;
  while(true)
  {
    swiWaitForVBlank();
    scanKeys();
    u16 keys = keysDownRepeat();
    if (keys & KEY_UP) 
    {
      scrollPane.up();
      needsPainting = true;
    }
    if (keys & KEY_DOWN) 
    {
      scrollPane.down();
      needsPainting = true;
    }

    if (needsPainting) {
      scrollPane.paint(scrollPane.preferredSize());
      Canvas::instance().endPaint();
    }
    needsPainting = false;
  }
}
