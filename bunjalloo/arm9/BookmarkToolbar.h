#ifndef BookmarkToolbar_h_seen
#define BookmarkToolbar_h_seen

#include "Toolbar.h"

class BookmarkToolbar: public Toolbar
{
  public:
    BookmarkToolbar(View & view);
    virtual void updateIcons();
    virtual void tick();

  protected:
    void handlePress(int i);
    void layout();

};
#endif
