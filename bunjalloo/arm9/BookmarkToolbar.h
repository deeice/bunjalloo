#ifndef BookmarkToolbar_h_seen
#define BookmarkToolbar_h_seen

#include "Toolbar.h"

/** A Toolbar that is shown when the user wants to bookmark a web site or list
 * the current bookmarks that she has saved.
 */
class BookmarkToolbar: public Toolbar
{
  public:
    /** Create a BookmarkToolbar.
     * @param view the View item that helps handle user input.
     */
    BookmarkToolbar(View & view);

    /** Update the icon glyphs based on the current state of the world. */
    virtual void updateIcons();

    virtual void tick();

  protected:
    void handlePress(int i);
    void layout();

};
#endif
