#ifndef IllusionPicker_h_seen
#define IllusionPicker_h_seen

#include "SpellSelect.h"

class IllusionPicker: public SpellSelect
{
  public:
    IllusionPicker(); 
    virtual void show();
    virtual void handleKeys();
  private:
    static void yesCb(void * arg);
    static void noCb(void * arg);
};
#endif
