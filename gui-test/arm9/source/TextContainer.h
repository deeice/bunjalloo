#ifndef TextContainer_h_seen
#define TextContainer_h_seen

#include "Component.h"
#include "UnicodeString.h"

class TextArea;
/** A container that prevents text from overflowing its boundary. */
class TextContainer: public Component
{
  public:

    /** The width of the decoration used. */
    static const int BORDER_WIDTH;
    /** The height of the decoration used. */
    static const int BORDER_HEIGHT;

    /** Create an empty container.*/
    TextContainer();
    
    /** Create a container with the given text.
     * @param text the text to display
     */
    TextContainer(const UnicodeString & text);

    /** Set the text contents. 
     * @param text the new text contents.
     */
    void setText(const UnicodeString & text);

    /** Get the text contents. Note that not all of the text may have been
     * displayed.
     * @return the current text.
     */
    const UnicodeString & text() const;

    /** Set the background color.
     * @param color the color to use.
     */
    void setBackgroundColor(unsigned short color);

    // Reimplemented from Component.
    virtual void paint(const nds::Rectangle & clip);
    virtual void setSize(unsigned int w, unsigned int h);

  protected:
    // protected, since inheriting classes can get at m_children anyway.
    inline TextArea* textArea()
    {
      return (TextArea*)m_children.front();
    }

  private:
    UnicodeString m_text;
    void layout();

};
#endif
