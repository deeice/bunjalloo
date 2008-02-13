/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RichTextArea_h_seen
#define RichTextArea_h_seen

#include <map>
#include "TextArea.h"
#include "Link.h"

class LinkListener;
/** Class that adds clickable Links, etc, to a standard TextArea. */
class RichTextArea: public TextArea
{
  public:
    RichTextArea(Font * font);
    ~RichTextArea();

    /** Overridden from TextArea. */
    virtual void appendText(const UnicodeString & unicodeString);

    /** Add a Link to the text.
     * @param href the document address to link to.
     * @param visited true if the link has been visited already.
     */
    void addLink(const std::string & href, bool visited=false);

    void add(Component * child);

    /** End the Link. */
    void endLink();

    void addImage(const std::string & src);

    /** End the image section.*/
    void endImage();

    /** Add a listener for when a Link is clicked.
     */
    void addLinkListener(LinkListener * linkListener);

    /** Insert a new line - usefull for when we don't parse new lines
     */
    void insertNewline();

    bool centred() const;
    void setCentred(bool centre=true);
    bool outlined() const;
    void setOutlined(bool outline=true);

    virtual int linesToSkip() const;

    virtual void paint(const nds::Rectangle & clip);
    virtual void setLocation(unsigned int x, unsigned int y);

    virtual bool stylusUp(const Stylus * stylus);
    virtual bool stylusDownFirst(const Stylus * stylus);
    virtual bool stylusDownRepeat(const Stylus * stylus);
    virtual bool stylusDown(const Stylus * stylus);

  protected:
    /** Overloaded from TextArea. This checks the current char vs the links to
     * see if the current character is a link or not.*/
    virtual void printu(const UnicodeString & unicodeString);

    virtual void incrLine();

  private:
    /** Keep track of the current document size. */
    unsigned int m_documentSize;
    unsigned int m_nextEvent;
    Link::EventType m_nextEventType;
    unsigned int m_paintPosition;
    Link::EventType m_state;
    typedef std::list<Link*> LinkList;
    LinkList m_links;
    // for painting:
    LinkList::const_iterator m_currentLink;
    LinkListener * m_linkListener;

    typedef std::map<int, int> LineHeightMap;
    LineHeightMap m_lineHeight;
    int m_lineNumber;

    unsigned int m_currentChildIndex;
    std::vector<unsigned int> m_childPositions;
    bool m_centred;
    bool m_outlined;

    Link * m_linkTouched;

    /** Delete the links */
    void removeClickables();
    unsigned int documentSize(int endLine=-1, unsigned int * childIndex=0) const;
    void handleNextEvent();
    void checkSkippedLines(int skipLines);
    int pointToCharIndex(int x, int y) const;

    void appendText_copyPaste(const UnicodeString & unicodeString);

    // get the document line at the clicked y position
    int lineAt(int y, int & leftover) const;
    int lineAt(int y) const;

    bool lineHasComponent(int line) const;
    // bool childTouch(Stylus & stylus);
    Link * linkAt(int index);
};
#endif
