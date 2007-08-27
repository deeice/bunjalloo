#ifndef Scrollable_h_seen
#define Scrollable_h_seen

class Scrollable
{
  public:
    Scrollable() {}
    virtual ~Scrollable() {}

    virtual void up() = 0;
    virtual void down() = 0;
    virtual void upBlock() = 0;
    virtual void downBlock() = 0;
    virtual void scrollToPercent(int value) = 0;
    virtual void setScrollIncrement(int incr) = 0;
    virtual int scrollIncrement() const = 0;
};

#endif
