/*
  Copyright (C) 2007,2008 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ndspp.h"
#include "libnds.h"
#include "ButtonGroup.h"
#include "Button.h"
#include "Canvas.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "EditableTextArea.h"
#include "Font.h"
#include "Keyboard.h"
#include "RadioButton.h"
#include "RichTextArea.h"
#include "ScrollPane.h"
#include "Stylus.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextField.h"
#include "vera.h"

extern const char _binary_test_map_bin_start[];

using namespace nds;
int main(int argc, char * argv[])
{
  irqInit();
  irqSet(IRQ_VBLANK,0);
  static Font font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start);
  TextAreaFactory::setFont(&font);
  TextAreaFactory::usePaletteData((const char*)_binary_vera_pal_bin_start, 32);
  Keyboard * keyBoard = new Keyboard();
  ScrollPane scrollPane;
  keyBoard->setTopLevel(&scrollPane);
#if 1
  TextField * tf = new TextField(string2unicode("Enter the value here. This line is too big to fit in"));
  TextField * passwd = new TextField(UnicodeString());
  RichTextArea * rich = (RichTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_RICH);
  Button * goBtn = new Button(string2unicode("Go"));
  rich->appendText(string2unicode("This is some long text "));
  passwd->setSize(60,18);
  
  scrollPane.setTopLevel();
  scrollPane.add(rich);
  scrollPane.add(passwd);
  scrollPane.add(goBtn);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());
  scrollPane.add(tf);
  scrollPane.setScrollIncrement(13);

#else
  RichTextArea * rich = (RichTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_RICH);
  TextArea * t = TextAreaFactory::create();
  EditableTextArea * t2 = (EditableTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_EDIT);
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
  shw = "Go";
  str = string2unicode(shw);
  Button * b4 = new Button(str);
  //b4->setSize(6*13, 10);
  shw = "Last one!";
  str = string2unicode(shw);
  Button * b5 = new Button(str);
  combo->addItem(str);
  combo2->addItem(str);
  combo2->addItem(str);

  t2->setListener(keyBoard);
  
  shw = "A text field that has a very, very long and pointless string";
  str = string2unicode(shw);
  TextField * tf1 = new TextField(str);
  tf1->setSize(160, 18);
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
  keyBoard->setTopLevel(&scrollPane);
  scrollPane.setTopLevel();
  scrollPane.add(rich);
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

  std::string richText1("This is normal text. ");
  std::string richText2("This is a link.");
  std::string richText3("Longer text, but still normal. ");
  std::string richText4("Normal text. ");
  std::string richText5("Link text. ");
  rich->appendText(string2unicode(richText1));
  rich->insertNewline();
  rich->appendText(string2unicode(richText1));
  rich->appendText(string2unicode(richText1));
  rich->appendText(string2unicode(richText3));
  rich->appendText(string2unicode(richText4));
  rich->addLink("www.link1.com");
  rich->appendText(string2unicode(richText2));
  rich->endLink();
  rich->appendText(string2unicode(richText3));
  rich->appendText(string2unicode(richText1));
  rich->addLink("www.link2.com");
  rich->appendText(string2unicode(richText5));
  rich->endLink();


  std::string s(_binary_test_map_bin_start, 1000);
  t->appendText(string2unicode(s));
  std::string s2(&_binary_test_map_bin_start[1200], 1300);
  t2->appendText(string2unicode(s2));
  subPane->setSize(t2->width(), 100);
  t2->setParentScroller(subPane);
#endif
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
    {
      //printf("%s\n",t2->asString().c_str());
      break;
    }

    if (keys & KEY_Y) {
      scrollPane.setVisible(not scrollPane.visible());
      keyBoard->setVisible(not keyBoard->visible());
      needsPainting = true;
    }

    if (keys & KEY_TOUCH)
    {
      touchPosition tp = touchReadXY();
      Stylus stylus;
      stylus.update(Stylus::DOWN, true, tp.px, tp.py+SCREEN_HEIGHT);
      needsPainting = keyBoard->dirty();
      if (not needsPainting)
        needsPainting = scrollPane.dirty();
    }

    needsPainting |= keyBoard->tick();
    if (needsPainting)
    {
      scrollPane.paint(scrollPane.preferredSize());
      keyBoard->paint(scrollPane.preferredSize());
      Canvas::instance().endPaint();
    }
    needsPainting = false;
  }
  delete keyBoard;
}
