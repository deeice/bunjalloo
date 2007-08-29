#ifndef ButtonListener_h_seen
#define ButtonListener_h_seen

class Button;
class ButtonListener
{
  public:
    virtual ~ButtonListener() {};
    virtual void pressed(Button * button) = 0;
};
#endif
