#ifndef TextListener_h_seen
#define TextListener_h_seen

class TextEntryI;
/** Interface for a class that is interested in editting a TextEntryI.*/
class TextListener
{
  public:
    /** Destructor for this interface.*/
    virtual ~TextListener() { }

    /** This method is typically called by the TextEntryI when it is touched by
     * the user.
     * @param entry the TextEntryI that is to be editted.
     */
    virtual void editText(TextEntryI * entry) = 0;
};
#endif
