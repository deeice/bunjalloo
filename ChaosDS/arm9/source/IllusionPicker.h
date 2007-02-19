#ifndef IllusionPicker_h_seen
#define IllusionPicker_h_seen

#include "SpellSelect.h"

class IllusionPicker: public SpellSelect
{
  public:
    virtual void show();
    virtual void handleKeys();
};
#endif
