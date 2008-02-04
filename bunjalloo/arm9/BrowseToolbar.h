#ifndef BrowseToolbar_h_seen
#define BrowseToolbar_h_seen

#include "Toolbar.h"

class BrowseToolbar: public Toolbar
{
  public:
    BrowseToolbar(Document & doc, Controller & cont, View & view);
    virtual void updateIcons();
    virtual void tick();

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
