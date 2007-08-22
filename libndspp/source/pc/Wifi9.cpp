#include "libnds.h"
#include "Wifi9.h"

using namespace nds;

Wifi9::Wifi9()
{
  initialise();
}
Wifi9 & Wifi9::instance()
{
  static Wifi9 s_wifi9;
  return s_wifi9;
}

bool Wifi9::connected() const
{
  return status() == ASSOCIATED;
}

void Wifi9::initialise()
{ 
  // wifi init complete - wifi lib can now be used!
}

void Wifi9::connect()
{ 
  // nop
} 

Wifi9::WifiStatus Wifi9::status() const
{
  static WifiStatus s = DISCONNECTED;
  switch (s)
  {
    case ASSOCIATING:
      s = ASSOCIATED;
      break;
    default:
      s = ASSOCIATING;
      break;
  }
  return s;
}

void Wifi9::disconnect()
{
}
