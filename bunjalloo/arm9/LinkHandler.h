#ifndef LinkHandler_h_seen
#define LinkHandler_h_seen

#include "ButtonListener.h"
#include "ComboBox.h"
class LinkListener;
class Link;

/** Handle Link presses that are images so the user can decide to follow the
 * anchor or view the image.
 */
class LinkHandler: public ComboBox
{

  public:
    /** Create the instance.
     * @param parent the LinkListener that received the linkClicked() call.
     */
    LinkHandler(LinkListener * parent);

    /** Set the Link to handle. It should be a img/anchor type Link.
     * @param link the Link to deal with.
     */
    void setLink(const Link * link);

    // reimplemented from ComboBox
    virtual void pressed(ButtonI * button);
    virtual bool touch(int x, int y);
    virtual void paint(const nds::Rectangle & clip);
  private:
    LinkListener * m_parent;
    const Link * m_link;
};
#endif
