#ifndef ButtonListener_h_seen
#define ButtonListener_h_seen

class ButtonI;
class ButtonListener
{
  public:
    virtual ~ButtonListener() {};
    virtual void pressed(ButtonI * button) = 0;
};
#endif
