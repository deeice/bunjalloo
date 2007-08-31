#ifndef TextListener_h_seen
#define TextListener_h_seen

class TextEntryI;
class TextListener
{
  public:
    virtual ~TextListener() { }

    virtual void editText(TextEntryI * entry) = 0;
};
#endif
