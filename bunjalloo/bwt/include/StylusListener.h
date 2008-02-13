#ifndef StylusListener_h_seen
#define StylusListener_h_seen

class Stylus;
/** Class. */
class StylusListener
{
  public:
    virtual ~StylusListener() {}

    /** Stylus up event.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusUp(const Stylus * stylus) = 0;

    /** Stylus is pressed for the first time since resetting.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusDownFirst(const Stylus * stylus) = 0;

    /** Stylus is pressed down and has triggered repeat.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusDownRepeat(const Stylus * stylus) = 0;

    /** Stylus is pressed down, it is just down, neither the first time nor repeating.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusDown(const Stylus * stylus) = 0;

};

#endif
