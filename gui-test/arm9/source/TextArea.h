#ifndef TextArea_h_seen
#define TextArea_h_seen

#include "Font.h"
#include "UnicodeString.h"
#include "Component.h"
#include <list>

class Link;
class FormControl;
/** A widget for displaying text.*/
class TextArea : public Component
{
  public:
    /** Constructor.*/
    TextArea(Font * font);
    /** Destructor.*/
    ~TextArea();

    /** Set the palette.
     * @param fileName the name of the font palette.
     */
    void setPalette(const std::string & fileName);

    virtual void paint(const nds::Rectangle & clip);

    /** Set the palette.
     * @param data the palette data
     * @param size the size of the palette data
     */
    void setPalette(const char * data, unsigned int size);

    /** Set the cursor position. This is where the text will be "drawn" the
     * next time a print routine is called.
     * @param x the cursor x position in pixels.
     * @param y the cursor y position in pixels.
     */
    void setCursor(int x, int y);

    /** Print encoded text using the current encoding.
     * @param data the encoded text data.
     * @param amount the number of bytes of encoded data.
     */
    void print(const char * data, int amount);

    /** Append text to the text area.
     * @param unicodeString the text to append.
     */
    void appendText(const UnicodeString & unicodeString);

    /** Print a unicode string starting at the current cursor position. The
     * values are not decoded from the string.
     * @param unicodeString a string containing unicode values.
     */
    //void printu(const UnicodeString & unicodeString);

    /** Get the current font.
     * @return the current font.
     */
    inline const Font & font() const;

    /** Set the font to use. TextArea takes control of the memory and deletes
     * on end/font reset.
     * @param font the font to use from now on.
     */
    void setFont(Font * font);

    /** Set the current encoding. @see print().
     * @param encoding the encoding to use for all encoded text.
     */
    void setEncoding(const std::string & encoding="utf-8");

    /** Set the text colour. Since the text is antialiased, this creates a coloured
     * scale palette based on the original grey-scale palette.
     * @param color the new text colour in 15 bit RGB format.
     */
    void setTextColor(unsigned short color);

    /** Set the background colour. Since the text is antialiased, this creates a coloured
     * scale palette based on the original grey-scale palette.
     * @param color the new text colour in 15 bit RGB format.
     */
    void setBackgroundColor(unsigned short color);

    void setDefaultColor();
    void clear();

#if 0
    /** Set the start line to draw the text from. All text before the line is
     * reached is skipped. This can be used to "scroll" text up and down.
     * @param line the line of text at which to start the printing.
     */
    void setStartLine(int line, bool removeClickables=true);

    /** Get the current start line value.
     * @return the line to start the text printing at.
     */
    int startLine() const;

    inline bool parseNewline() const;
    inline void setParseNewline(bool parse=true);
    void insertNewline(int count=1);
    inline void setLink(bool isLink=true);
    inline void setForm(bool isForm=true);

    void addLink(const std::string & href);
    /*
    void addFormControl(FormControl * formCtrl);
    */
    Link * clickLink(int x, int y) const;
    /*
    FormControl * clickForm(int x, int y) const;
    */

    void increaseIndent();
    void decreaseIndent();
#endif

    int textSize(const UnicodeString & unicodeString) const;
  private:
    Font * m_font;
    unsigned short * m_palette;
    unsigned short * m_basePalette;
    int m_paletteLength;
    std::string m_encoding;
    int m_startLine;
    bool m_foundPosition;
    bool m_parseNewline;
    enum ControlState {
      TEXT,
      LINK,
      FORM
    };
    ControlState m_currentControl;
    UnicodeString m_document;
    UnicodeString m_textLine;


    int m_cursorx;
    int m_cursory;
    int m_initialCursorx;
    int m_initialCursory;
    typedef std::list<Link*> LinkList;
    /*
    typedef std::list<FormControl*> FormList;
    */
    LinkList m_links;
    //FormList m_formControls;
    unsigned short m_bgCol;
    unsigned short m_fgCol;
    int m_indentLevel;

    //! Position that the current line cursor is at.
    int m_appendPosition;

    void printAt(Font::Glyph & g, int xPosition, int yPosition);
    void incrLine();
    void checkLetter(Font::Glyph & g);
    bool doSingleChar(unsigned int value);
#if 0
    void printuImpl(const UnicodeString & unicodeString);
    void printuWord(const UnicodeString & word);
#endif
    
    void removeClickables();

    const UnicodeString nextWord(const UnicodeString & unicodeString, 
        int currPosition) const;
    void advanceWord(const UnicodeString & unicodeString, int wordLength,
        int & currPosition, UnicodeString::const_iterator & it) const;
    void printu(const UnicodeString & unicodeString);
};

// inline implementations

const Font & TextArea::font() const {
  return *m_font;
}
#if 0
bool TextArea::parseNewline() const
{
  return m_parseNewline;
}
void TextArea::setParseNewline(bool parse)
{
  m_parseNewline = parse;
}
void TextArea::setLink(bool isLink)
{
  m_currentControl = isLink?LINK:TEXT;
}
void TextArea::setForm(bool isForm)
{
  m_currentControl = isForm?FORM:TEXT;
}
#endif
#endif
