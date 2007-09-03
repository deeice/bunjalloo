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
