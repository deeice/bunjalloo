#ifndef ButtonI_h_seen
#define ButtonI_h_seen

class ButtonListener;
/** An abstract button class that handles selection and setting callback
 * listeners.*/
class ButtonI
{
  public:
    
    /** Set the initial state of the class.*/
    ButtonI(): m_selected(false),m_listener(0){}

    /** End of lifetime.*/
    virtual ~ButtonI() {}

    /** Set the selection state.
     * @param selected the boolean flag for selection.
     */
    inline void setSelected(bool selected=true)
    {
      m_selected = selected;
    }

    /** Get the selection state.
     * @return true if this object is selected, false otherwise.
     */
    inline bool selected() const
    {
      return m_selected;
    }

    /** Set the callback ButtonListener for this ButtonI.
     * @param listener the listener to use.
     */
    inline void setListener(ButtonListener * listener)
    {
      m_listener = listener;
    }
    /** Get the current callback ButtonListener for this ButtonI.
     * @return the currently set listener.
     */
    inline ButtonListener * listener() const
    {
      return m_listener;
    }

    /** Remove the current callback ButtonListener.
     * @param listener remove this listener. (not fully implemented - just removes the only listener)
     */
    inline void removeListener(ButtonListener * listener)
    {
      m_listener = 0;
    }

  private:
    bool m_selected;
    ButtonListener * m_listener;

};
#endif
