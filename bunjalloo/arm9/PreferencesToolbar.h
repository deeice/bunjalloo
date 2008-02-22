#ifndef PreferenceToolbar_h_seen
#define PreferenceToolbar_h_seen

#include "Toolbar.h"

class PreferencesToolbar: public Toolbar
{
  public:
    PreferencesToolbar(View & view);
    virtual void updateIcons();
    virtual void tick();

  protected:
    void handlePress(int i);

};
#endif
