#include "Rectangle.h"

bool Rectangle::hit(int testx, int testy) const
{
  return ( (testx >= x and testx < (x + w))
      and  (testy >= y and testy < (y + h)));
}
