/*
  Copyright (C) 2008 Richard Quirk

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
#include <vector>
#include <string>
#include "ndspp.h"
#include "libnds.h"
#include "PatchDLDI.h"
#include "Button.h"
#include "Canvas.h"
#include "Font.h"
#include "File.h"
#include "RichTextArea.h"
#include "ScrollPane.h"
#include "Stylus.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextField.h"
#include "Language.h"
#include "vera.h"

#include "LinkListener.h"
#include "Link.h"

extern const char _binary_test_map_bin_start[];

class Patcher: public LinkListener
{
  public:
    virtual void linkClicked(Link * link);
};


using namespace nds;
int main(int argc, char * argv[])
{
  using std::string;
  using std::vector;
  irqInit();
  irqSet(IRQ_VBLANK,0);
  // wait for arm7 to read settings.
  for (int i = 0; i < 120; ++i)
    swiWaitForVBlank();

  static Font font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start);
  TextAreaFactory::setFont(&font);
  TextAreaFactory::usePaletteData((const char*)_binary_vera_pal_bin_start, 32);
  ScrollPane scrollPane;
  RichTextArea * rich = (RichTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_RICH);
  rich->appendText(T("Files to patch\n"));
  Patcher patcher;
  rich->addLinkListener(&patcher);

  vector<string> files;
  File::ls(".", files);

  for (vector<string>::const_iterator it(files.begin()); it != files.end(); ++it)
  {
    const string& file(*it);
    size_t pos = file.rfind(".nds");
    if (pos != string::npos)
    {
      rich->addLink( file, false);
      rich->appendText( string2unicode(file));
      rich->endLink();
      rich->appendText(string2unicode("\n"));

    }
  }

  scrollPane.setTopLevel();
  scrollPane.add(rich);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());
  scrollPane.setScrollIncrement(13);

  scrollPane.setLocation(0,0);
  scrollPane.setSize(Canvas::instance().width(),Canvas::instance().height());
  scrollPane.scrollToPercent(128);

  bool needsPainting = true;
  while(true)
  {
    swiWaitForVBlank();
    scanKeys();
    u16 keys = keysDownRepeat();
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
      break;
    }

    if (keys & KEY_Y) {
      scrollPane.setVisible(not scrollPane.visible());
      needsPainting = true;
    }

    if (keys & KEY_TOUCH)
    {
      touchPosition tp = touchReadXY();
      Stylus stylus;
      stylus.update(Stylus::DOWN, true, tp.px, tp.py+SCREEN_HEIGHT);
      rich->stylusDownFirst(&stylus);
      rich->stylusUp(&stylus);
      if (not needsPainting)
        needsPainting = scrollPane.dirty();
    }

    if (needsPainting)
    {
      scrollPane.paint(scrollPane.preferredSize());
      Canvas::instance().endPaint();
    }
    needsPainting = false;
  }
}

void Patcher::linkClicked(Link * link)
{
  const std::string & filename(link->href());
  printf("File: %s\n", filename.c_str());
  PatchDLDI patcher(filename.c_str());
  patcher.patch();
}
