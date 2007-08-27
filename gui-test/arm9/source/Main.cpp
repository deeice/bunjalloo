#include "ndspp.h"
#include "libnds.h"
#include "TextArea.h"
#include "Canvas.h"
#include "Font.h"
#include "ScrollPane.h"
#include "TextAreaFactory.h"
#include "Button.h"

extern const char _binary_test_map_bin_start[];

using namespace nds;
int main(int argc, char * argv[])
{
  irqInit();
  irqSet(IRQ_VBLANK,0);

  TextArea * t = TextAreaFactory::create();
  TextArea * t2 = TextAreaFactory::create();
  t2->setBackgroundColor(Color(31,31,0));
  ScrollPane * subPane = new ScrollPane;
  subPane->add(t2);
  subPane->setTopLevel(false);
  subPane->setSize(Canvas::instance().width(),Canvas::instance().height());
  subPane->setScrollIncrement(t2->font().height());

  std::string shw("Hello World");
  UnicodeString str = string2unicode(shw);
  Button * b = new Button(str);

  ScrollPane scrollPane;
  scrollPane.setTopLevel();
  scrollPane.add(t);
  scrollPane.add(b);
  scrollPane.add(subPane);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());
  scrollPane.setScrollIncrement(t->font().height());

  //std::string s(_binary_test_map_bin_start, strlen(_binary_test_map_bin_start));
  std::string s(_binary_test_map_bin_start, 1000);
  t->appendText(string2unicode(s));
  std::string s2(&_binary_test_map_bin_start[1200], 1300);
  t2->appendText(string2unicode(s2));
  subPane->setSize(t2->width(), 100);
  scrollPane.setLocation(0,0);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());

  //Rectangle screen = {0, 0, Canvas::instance().width(), Canvas::instance().height()};
  bool needsPainting = true;
  while(true)
  {
    swiWaitForVBlank();
    scanKeys();
    //u16 keys = keysDownRepeat();
    u16 keys = keysHeld();
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
    if (keys & KEY_TOUCH)
    {
      touchPosition tp = touchReadXY();
      //printf("tp x %d tp y %d\n", tp.px, tp.py);
      needsPainting = scrollPane.touch(tp.px, tp.py+SCREEN_HEIGHT) ;
    }

    if (needsPainting) {
      scrollPane.paint(scrollPane.preferredSize());
      Canvas::instance().endPaint();
    }
    needsPainting = false;
  }
}
