#include "Casting.h"

int Casting::s_worldChaos(0);
int Casting::getWorldChaos() {
  return s_worldChaos;
}
void Casting::show()
{}
void Casting::animate()
{}
CurrentScreen_t Casting::getId() const
{
  return SCR_CASTING;
}
void Casting::handleKeys()
{}
