#ifndef ViewRender_h_seen
#define ViewRender_h_seen

class View;
class TextArea;
class HtmlElement;
#include "UnicodeString.h"

class ViewRender
{
  public:
    ViewRender(View * self);
    void render();

  private:
    View * m_self;
    const HtmlElement * m_lastElement;

    void walkTree(const HtmlElement * element);
    bool applyFormat(const HtmlElement * element);
    void preFormat(const HtmlElement * element);
    void postFormat(const HtmlElement * element);
    void setBgColor(const HtmlElement * body);
    void renderSelect(const HtmlElement * body);
    void doImage(const UnicodeString & unicode);
    void renderInput(const HtmlElement * body);

};
#endif
