#ifndef RichTextArea_h_seen
#define RichTextArea_h_seen

#include "TextArea.h"

/** Class that adds clickable links, etc, to a standard TextArea. */
class RichTextArea: public TextArea
{
  public:
    RichTextArea(Font * font);
    ~RichTextArea();

    /** Reimplemented from TextArea.*/
    void appendText(const UnicodeString & unicodeString);

    /** Add a link to the text.
     * @param href the document address to link to,
     */
    void addLink(const std::string & href);

    /** End the link. */
    void endLink();

    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

  protected:
    /** Overloaded from TextArea. This checks the current char vs the links to
     * see if the current character is a link or not.*/
    virtual void printu(const UnicodeString & unicodeString);

  private:
    enum ControlState {
      STATE_TEXT,
      STATE_LINK,
    };
    typedef std::list<Link*> LinkList;
    LinkList m_links;
    /** Keep track of the current document size. */
    unsigned int m_currentPosition;
    ControlState m_state;
    // for painting:
    unsigned int m_nextEvent;
    unsigned int m_nextEventType;
    LinkList::const_iterator m_currentLink;
    unsigned int m_paintPosition;
    unsigned int m_documentSize;

    /** Delete the links */
    void removeClickables();
    int documentSize(int endLine=-1) const;
    void handleNextEvent();
    void checkSkippedLines(int skipLines);
    int pointToCharIndex(int x, int y) const;

};
#endif
