#ifndef Select_h_seen
#define Select_h_seen

#include <vector>
#include "FormControl.h"

class HtmlElement;

/** A form select control. Shows a drop down list of items.
 */
class Select: public FormControl
{
  public:
    Select(HtmlElement * select);
    /** Add an option to the selection.
     * @param option the option to add.
     * @param textArea the text area - needed to get the font metrics for the option text.
     */
    void addOption(const HtmlElement * option, const TextArea * textArea);

    virtual void draw(TextArea * gfx);

  private:

    typedef std::vector<const HtmlElement*> ElementVector;
    ElementVector m_options;

};

#endif
