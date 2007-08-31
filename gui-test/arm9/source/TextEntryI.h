#ifndef TextEntryI_h_seen
#define TextEntryI_h_seen

class TextListener;
class TextEntryI
{
  public:
    virtual ~TextEntryI() {}

    virtual const UnicodeString & text() const = 0;
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
  private:
    TextListener * m_listener;
};
#endif
