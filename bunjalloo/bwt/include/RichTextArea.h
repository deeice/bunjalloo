/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef RichTextArea_h_seen
#define RichTextArea_h_seen

#include "TextArea.h"

class LinkListener;
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

    void setColor(unsigned short color);

    void endColor();

    /** Add a listener for when a Link is clicked.
     */
    void addLinkListener(LinkListener * linkListener);

    /** Insert a new line - usefull for when we don't parse new lines
     */
    void insertNewline();

    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

  protected:
    /** Overloaded from TextArea. This checks the current char vs the links to
     * see if the current character is a link or not.*/
    virtual void printu(const UnicodeString & unicodeString);

  private:
    enum ControlState {
      STATE_PLAIN,
      STATE_LINK,
      STATE_COLOR
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
    LinkListener * m_linkListener;

    /** Delete the links */
    void removeClickables();
    int documentSize(int endLine=-1) const;
    void handleNextEvent();
    void checkSkippedLines(int skipLines);
    int pointToCharIndex(int x, int y) const;

};
#endif
