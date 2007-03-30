#include "libnds.h"
#include "Wifi9.h"

using namespace nds;

Wifi9::Wifi9():m_connected(false)
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
  return m_connected;
}

void Wifi9::initialise()
{ 
  // wifi init complete - wifi lib can now be used!
}

bool Wifi9::connect()
{ 
  // if connected, you can use the berkley sockets interface to connect to the internet
  if (m_connected)
    return true;
  m_connected = true;
  return true;
} 

void Wifi9::disconnect()
{
  m_connected = false;
}
