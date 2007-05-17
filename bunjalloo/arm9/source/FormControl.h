#ifndef FormControl_h_seen
#define FormControl_h_seen

#include "UnicodeString.h"
class Rectangle;
class TextArea;
class ControllerI;
class HtmlElement;
class URI;

class FormControl
{
  public:
    static const int MAX_SIZE;
    static const int MIN_SIZE;
    FormControl(HtmlElement * element);
    virtual ~FormControl();
    virtual void draw(TextArea * gfx);

    enum InputType
    {
      KEYBOARD,
      ONE_CLICK,
      MENU
    };

    /** Does this control require keyboard input? .
     * @return true if it does. false if only clicks are needed.
     */
    virtual InputType inputType() const;

    /** Input a unicode string.
     * @param str the input value.
     */
    virtual void input(const UnicodeString & str);

    /** input a touch value after clicking.
     * @param str the input touch.
     */
    virtual void input(int x, int y, ControllerI & controller, URI & uri);

    void setHeight(int h);
    void setPosition(int x, int y);
    int width() const;
    bool hitTest(int x, int y) const;

  protected:
    Rectangle * m_size;
    HtmlElement * m_element;
    std::string m_processedData;
};

#endif
