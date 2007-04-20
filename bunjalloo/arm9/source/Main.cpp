#include "ndspp.h"
#include "libnds.h"
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
      c->doUri(localFile);
    }
    else {
      c->doUri(fileName);
    }
  }
  else
  {
    c->showLicence();
  }
  c->mainLoop();
}
