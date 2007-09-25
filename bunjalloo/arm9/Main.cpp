/*
  Copyright (C) 2007 Richard Quirk

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
#include "URI.h"
#include "Controller.h"
#include "File.h"

using namespace nds;
int main(int argc, char * argv[])
{
  irqInit();
  irqSet(IRQ_VBLANK,0);
  Controller * c = new Controller;
  if (argc > 1) {
    std::string fileName(argv[1]);
    File check;
    check.open(fileName.c_str());
    if (check.is_open()) {
      check.close();
      std::string localFile = "file:///";
      localFile += fileName;
      c->doUri(URI(localFile));
    }
    else {
      c->doUri(URI(fileName));
    }
  }
  else
  {
    c->showLicence();
  }
  c->mainLoop();
}
