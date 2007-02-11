#include <ctype.h>
#include "Misc.h"

// default seed
static unsigned int s_random(14071977);
void Misc::seedRand(unsigned int seed) {
  s_random = seed;
}

// churn the random number
void Misc::churnRand(void) {
  s_random *= 663608941;
}

unsigned int Misc::getRand(unsigned int maximum) {
  unsigned long long result = maximum;
  result *= s_random;
  churnRand();
  return (unsigned int)(result >> 32);
}

char * Misc::trim(char * str) {
  char *s = str;
  char *d = str;
  
  // eat trailing white space
  while (*s)
    s++;
  while (str < s && isspace(*(s-1)))
    *s-- = '\0';
  
  *s = '\0';
  
  s = str;
  // has leading space?
  if (isspace(*s))
  {
    // eat white space 
    while (isspace(*s))
      s++;
    
    // copy
    while (*s)
      *d++ = *s++;
    *d = '\0';
  } 
  return str;
}
