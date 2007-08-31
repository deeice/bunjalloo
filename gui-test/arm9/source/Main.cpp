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
#include "EditableTextArea.h"
#include "TextField.h"
#include "Button.h"
#include "ComboBox.h"
#include "RadioButton.h"
#include "ButtonGroup.h"
#include "CheckBox.h"
#include "Keyboard.h"

extern const char _binary_test_map_bin_start[];

using namespace nds;
int main(int argc, char * argv[])
{
  irqInit();
  irqSet(IRQ_VBLANK,0);

  TextArea * t = TextAreaFactory::create();
  EditableTextArea * t2 = (EditableTextArea*)TextAreaFactory::create(true);
  // t2->setBackgroundColor(Color(31,31,0));

  ScrollPane * subPane = new ScrollPane;
  subPane->add(t2);
  subPane->setTopLevel(false);
  subPane->setSize(Canvas::instance().width(),Canvas::instance().height());
  subPane->setScrollIncrement(t2->font().height());
  subPane->setStretchChildren(true);
  ComboBox * emptyCombo = new ComboBox();
  ComboBox * oneValCombo = new ComboBox();
  ComboBox * combo = new ComboBox();
  combo->setSize(60, 18);
  ComboBox * combo2 = new ComboBox();
  combo2->setSize(90, 18);
  std::string shw("Hello World");
  UnicodeString str = string2unicode(shw);
  Button * b1 = new Button(str);

  shw = "Combo box!";
  str = string2unicode(shw);
  oneValCombo->addItem(str);
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

  Keyboard * keyBoard = new Keyboard();
  t2->setListener(keyBoard);
  
  shw = "A text field that has a very, very long and pointless string";
  str = string2unicode(shw);
  TextField * tf1 = new TextField(str);
  tf1->setSize(120, 18);
  tf1->setListener(keyBoard);
  combo2->addItem(str);

  TextArea * rbLabel = TextAreaFactory::create();
  shw = "Radio button label.";
  str = string2unicode(shw);
  rbLabel->appendText(str);

  TextArea * cbLabel = TextAreaFactory::create();
  shw = "CheckBox label.";
  str = string2unicode(shw);
  cbLabel->appendText(str);

  RadioButton * rb = new RadioButton();
  RadioButton * rb2 = new RadioButton();
  RadioButton * rb3 = new RadioButton();
  CheckBox * cb = new CheckBox();
  ButtonGroup bg;
  bg.add(rb);
  bg.add(rb2);
  bg.add(rb3);

  ScrollPane scrollPane;
  scrollPane.setTopLevel();
  scrollPane.add(t);
  scrollPane.add(b1);
  scrollPane.add(b2);
  scrollPane.add(b3);
  scrollPane.add(tf1);
  scrollPane.add(b4);
  scrollPane.add(emptyCombo);
  scrollPane.add(oneValCombo);
  scrollPane.add(combo2);
  scrollPane.add(rb);
  scrollPane.add(rb2);
  scrollPane.add(rb3);
  scrollPane.add(rbLabel);
  scrollPane.add(cb);
  scrollPane.add(cbLabel);
  scrollPane.add(subPane);
  scrollPane.add(combo);
  scrollPane.add(b5);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());
  scrollPane.setScrollIncrement(t->font().height());

  //std::string s(_binary_test_map_bin_start, strlen(_binary_test_map_bin_start));
  std::string s(_binary_test_map_bin_start, 1000);
  t->appendText(string2unicode(s));
  //std::string s2(&_binary_test_map_bin_start[1200], 1300);
  std::string s2("\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff");
  t2->appendText(string2unicode(s2));
  subPane->setSize(t2->width(), 100);
  scrollPane.setLocation(0,0);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());

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
      scrollPane.up();
      needsPainting = true;
    }
    if (keys & KEY_DOWN) 
    {
      scrollPane.down();
      scrollPane.down();
      needsPainting = true;
    }
    if (keys & KEY_X)
      break;

    if (keys & KEY_Y) {
      scrollPane.setVisible(not scrollPane.visible());
      keyBoard->setVisible(not keyBoard->visible());
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
