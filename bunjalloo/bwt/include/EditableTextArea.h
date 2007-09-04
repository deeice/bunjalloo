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
#ifndef EditableTextArea_h_seen
#define EditableTextArea_h_seen

#include "TextArea.h"
#include "TextEntryI.h"

class ScrollPane;

/** An EditableTextArea includes a cursor, handles user input and allows random
 * text insertion at any position.
 */
class EditableTextArea: public TextArea, public TextEntryI
{
  public:
    /** Create a new instance. 
     * @param font the font to be used.
     */
    EditableTextArea(Font * font);

    /** Reimplemented (shadowed) from TextArea::appendText()
     * @param unicodeString the string to append to the text
     */
    void appendText(const UnicodeString & unicodeString);
    /** Delete a character at the current caret position.*/
    void deleteChar();
    /** Clear all text, reset caret. */
    void clearText();

    /** Get the current caret line number.
     * @return the line number where the caret is located.
     */ 
    int caretLine() const;
    /** Set the parent ScrollPane that holds this EditableTextArea.
     * @param scrollPane the parent
     */
    void setParentScroller(ScrollPane * scrollPane);

    /** Check if the caret is currently visible.
     * @return true if visible, false otherwise.
     */
    bool caretVisible() const;

    // interface implementation
    virtual void text(UnicodeString & returnString) const;
    virtual void setText(const UnicodeString & text);
    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);
    virtual bool isMultiLine() const;

  private:
    int m_caretLine;
    int m_caretChar;
    int m_caretPixelX;
    bool m_appendedNewLine;
    ScrollPane * m_scrollPane;

    void setCaret(int x, int y);
    void recalculateCaret();
    void resizeParent();

};

#endif
