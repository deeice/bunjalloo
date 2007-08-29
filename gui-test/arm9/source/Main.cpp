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
#include "ndspp.h"
#include "libnds.h"
#include "TextArea.h"
#include "Canvas.h"
#include "Font.h"
#include "ScrollPane.h"
#include "TextAreaFactory.h"
#include "TextField.h"
#include "Button.h"
#include "ComboBox.h"

extern const char _binary_test_map_bin_start[];

using namespace nds;
int main(int argc, char * argv[])
{
  irqInit();
  irqSet(IRQ_VBLANK,0);

  TextArea * t = TextAreaFactory::create();
  TextArea * t2 = TextAreaFactory::create();
  // t2->setBackgroundColor(Color(31,31,0));
  t2->setEditable();
  ScrollPane * subPane = new ScrollPane;
  subPane->add(t2);
  subPane->setTopLevel(false);
  subPane->setSize(Canvas::instance().width(),Canvas::instance().height());
  subPane->setScrollIncrement(t2->font().height());
  ComboBox * combo = new ComboBox();
  combo->setSize(60, 18);
  ComboBox * combo2 = new ComboBox();
  combo2->setSize(90, 18);
  std::string shw("Hello World");
  UnicodeString str = string2unicode(shw);
  Button * b1 = new Button(str);

  shw = "Combo box!";
  str = string2unicode(shw);
  combo->addItem(str);
  combo2->addItem(str);
  combo2->addItem(str);
  shw = "Another One - with very wide text";
  str = string2unicode(shw);
  Button * b2 = new Button(str);
  combo->addItem(str);
  combo2->addItem(str);
  combo2->addItem(str);
  shw = "Three- wide, but fixed width";
  str = string2unicode(shw);
  Button * b3 = new Button(str);
  b3->setSize(60, 18);
  combo2->addItem(str);
  combo2->addItem(str);
  shw = "Another Button";
  str = string2unicode(shw);
  Button * b4 = new Button(str);
  shw = "Last one!";
  str = string2unicode(shw);
  Button * b5 = new Button(str);
  combo->addItem(str);
  combo2->addItem(str);
  combo2->addItem(str);
  
  shw = "A text field that has a very, very long and pointless string";
  str = string2unicode(shw);
  TextField * tf1 = new TextField(str);
  tf1->setSize(120, 18);
  combo2->addItem(str);

  ScrollPane scrollPane;
  scrollPane.setTopLevel();
  scrollPane.add(t);
  scrollPane.add(b1);
  scrollPane.add(b2);
  scrollPane.add(b3);
  scrollPane.add(tf1);
  scrollPane.add(b4);
  scrollPane.add(combo2);
  scrollPane.add(subPane);
  scrollPane.add(combo);
  scrollPane.add(b5);
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
    u16 keys = keysDownRepeat();
    //u16 keys = keysHeld();
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
      needsPainting = scrollPane.touch(tp.px, tp.py+SCREEN_HEIGHT) ;
    }

    if (needsPainting) {
      scrollPane.paint(scrollPane.preferredSize());
      Canvas::instance().endPaint();
    }
    needsPainting = false;
  }
}
