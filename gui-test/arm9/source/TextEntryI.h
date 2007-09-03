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
#ifndef TextEntryI_h_seen
#define TextEntryI_h_seen

class TextListener;
/** Interface for a class that stores and manages text data.*/
class TextEntryI
{
  public:
    virtual ~TextEntryI() {}

    /** Get the text that this TextEntryI holds.
     * @param returnString this reference is filled with the text.
     */
    virtual void text(UnicodeString & returnString) const = 0;

    /** Set the text that this widget holds.
     * @param text the new text.
     */
    virtual void setText(const UnicodeString & text) = 0;

    /** Set the callback TextListener for this TextEntryI.
     * @param listener the listener to use.
     */
    inline void setListener(TextListener * listener)
    {
      m_listener = listener;
    }
    /** Get the current callback TextListener for this TextEntryI.
     * @return the currently set listener.
     */
    inline TextListener * listener() const
    {
      return m_listener;
    }

    /** Remove the current callback TextListener.
     * @param listener remove this listener. (not fully implemented - just removes the only listener)
     */
    inline void removeListener(TextListener * listener)
    {
      m_listener = 0;
    }

    /** See if this widget is allowed to hold multi-line text. Multi-line
     * text may contain new line characters.
     * @return true if can hold multiple lines, false otherwise.
     */
    virtual bool isMultiLine() const = 0;
  private:
    TextListener * m_listener;
};
#endif
