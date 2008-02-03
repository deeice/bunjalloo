
/** Stylus idea inspired by Jeff Lait's Powder.
 * User places the pointer on the screen in an area. He then let's go.
 * If the touch and let go are in the same place (more or less) then it is a valid click.
 * If the user holds down for a while, then that is not a click - it's a hold.
 */
class Stylus
{

  public:
    //! number of frames before a click turns into a hold.
    const static int HOLD_LIMIT;

    /** Create the stylus object. */
    Stylus();
    /** End of its life.*/
    ~Stylus();

    /** Reset the state to start a new event. */
    void reset();

    enum ClickType {
      WAITING,        //!< Waiting for a new event.
      CANCEL,         //!< Cancelled the click, let go.
      WAFFLE,         //!< Still don't know what to do, will cancel.
      CLICK,          //!< Clicked normally (quick press and release).
      HELD            //!< Holding down on one spot.
    };

    /** Get the user click type.*/
    ClickType clickType() const;

    /** Get the start point of the event.
     * @param x the x position.
     * @param y the y position.
     */
    void startPoint(int & x, int & y) const;

    /** Get the end point of the event. If the event has not ended, then repeat
     * calls may return different results. This allows unsure user stylus
     * scraping.
     * @param x the x position.
     * @param y the y position.
     */
    void endPoint(int & x, int & y) const;

    /** Update the stylus state.
     * @param touch is the stylus down?
     * @param x the x pos
     * @param y the y pos
     */
    void update(bool touch, int x, int y);

  private:
    int m_startX;
    int m_startY;
    int m_lastX;
    int m_lastY;
    int m_holding;
    ClickType m_state;

};
