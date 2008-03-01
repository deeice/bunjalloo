#ifndef BrowseToolbar_h_seen
#define BrowseToolbar_h_seen

#include "Toolbar.h"

/** The "main" Toolbar with back, forward, stop, reload, etc buttons. */
class BrowseToolbar: public Toolbar
{
  public:
    BrowseToolbar(View & view);
    virtual void updateIcons();
    virtual void tick();
    virtual void setVisible(bool visible);

  protected:
    void handlePress(int i);
    void layout();

  private:
    int m_angle;
    // hidden - ie. not minimized
    bool m_hidden;

    void setHidden(bool hidden);
    void setHiddenIconExpand();
    void setHiddenIconContract();
};
#endif
