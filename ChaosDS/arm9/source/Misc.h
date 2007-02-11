#ifndef Misc_h_seen
#define Misc_h_seen
// Hurray! Misc functions

namespace Misc
{
  unsigned int getRand(unsigned int maximum);
  void seedRand(unsigned int);
  void churnRand();
  char * trim(char * str);
  void orderTable(int count, unsigned char * table);
}
#endif
