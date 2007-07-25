#include <assert.h>
#include "libnds.h"
#include "ndspp.h"
#include "Canvas.h"
#include "Config.h"
#include "File.h"
#include "Font.h"
#include "FormControl.h"
#include "Link.h"
#include "Palette.h"
#include "TextArea.h"
#include "UTF8.h"

using namespace nds;
using namespace std;
static const unsigned int intDelimiters[] = {0x0020, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d};
static const UnicodeString delimiters(intDelimiters,6);
static const int INDENT(16);

TextArea::TextArea(const Config & config) : 
  m_config(config),
  m_font(0),
  m_palette(0),
  m_basePalette(0),
  m_paletteLength(0),
  m_encoding("utf-8"),
  m_startLine(0),
  m_foundPosition(false),
  m_parseNewline(true),
  m_currentControl(TEXT),
  m_bgCol(0),
  m_fgCol(0),
  m_indentLevel(0)
{
  string fontname = m_config.font();
  init(fontname);
}

void TextArea::setStartLine(int line, bool rmclicks)
{
  m_startLine = line;
  m_foundPosition = false;
  if (rmclicks)
  {
    removeClickables();
  }
}

static void deleteLink(Link * link)
{
  delete link;
}
static void deleteFormCntrl(FormControl * fc)
{
  delete fc;
}

void TextArea::removeClickables()
{
  for_each(m_links.begin(), m_links.end(), deleteLink);
  m_links.clear();
  for_each(m_formControls.begin(), m_formControls.end(), deleteFormCntrl);
  m_formControls.clear();
}

int TextArea::startLine() const
{
  return m_startLine;
}

void TextArea::init(const std::string & fontBase)
{
  m_font=new Font(fontBase);
  setPalette(fontBase+".pal");
  //m_font = new Font(_binary_vera_img_start,_binary_vera_map_start);
  //setPalette((char*)_binary_vera_pal_start, 32);
}

void TextArea::printAt(Font::Glyph & g, int xPosition, int yPosition)
{
  const unsigned char * data = g.data;
  int dataInc = (m_font->totalWidth() - g.width)/2;
  for (int y = 0; y < g.height; ++y)
  {
    for (int x = 0; x < g.width/2; ++x)
    {
      unsigned char pixelPair = *data++;
      int pix1 = ((pixelPair)&0xf);
      if (pix1)
        Canvas::instance().drawPixel(xPosition+(x*2), yPosition+y, m_palette[pix1]);
      int pix2 = ((pixelPair>>4)&0xf);
      if (pix2)
        Canvas::instance().drawPixel(xPosition+(x*2)+1, yPosition+y, m_palette[pix2]);
    }
    data += dataInc;
  }
}

void TextArea::setCursor(int x, int y)
{
  m_cursorx = x;
  m_cursory = y;
  m_initialCursorx = x;
  m_initialCursory = y;
}

void TextArea::incrLine()
{
  m_cursorx = m_indentLevel; 
  m_cursory += m_font->height();
}

void TextArea::checkLetter(Font::Glyph & g)
{
  if (m_cursorx + g.width > Canvas::instance().width())
  {
    incrLine();
  }
}

void TextArea::insertNewline(int count)
{
  bool parseNewline = m_parseNewline;
  setParseNewline(true);
  UnicodeString newlines(count, '\n');
  printu(newlines);
  setParseNewline(parseNewline);
}

const UnicodeString TextArea::nextWord(const UnicodeString & unicodeString, int currPosition) const
{
  // TODO: words that are longer than Canvas::width() break everything.
  UnicodeString word;
  if (m_parseNewline)
  {
    // if we are parsing new lines, look for the next delimiter
    unsigned int position = unicodeString.find_first_of(delimiters,currPosition);
    position = position==string::npos?unicodeString.length():position;
    word = unicodeString.substr(currPosition,position-currPosition+1);
  }
  else
  {
    // not parsing new lines, so look for the next proper word, then the next non word
    unsigned int position = unicodeString.find_first_not_of(delimiters,currPosition);
    position = unicodeString.find_first_of(delimiters,position);
    position = position==string::npos?unicodeString.length():position;
    // now make a word delimted with spaces.
    word = unicodeString.substr(currPosition,position-currPosition);
    word += ' ';
  }
  int size(textSize(unicodeString));
  if (size > Canvas::instance().width())
  {
    // uh oh - this will break things.
    UnicodeString::const_iterator it(word.begin());
    UnicodeString shorterWord;
    int size(0);
    for (; it != word.end(); ++it)
    {
      unsigned int value(*it);
      if (value == UTF8::MALFORMED)
        value = '?';
      Font::Glyph g;
      m_font->glyph(value, g);
      if ( (size + g.width) >= Canvas::instance().width()) {
        return shorterWord;
      }
      shorterWord += *it;
      size += g.width;
    }
  }
  return word;
}

void TextArea::advanceWord(const UnicodeString & unicodeString, int wordLength,
    int & currPosition, UnicodeString::const_iterator & it) const
{
  if (m_parseNewline) {
    it += wordLength;
    currPosition += wordLength;
  }
  else {
    unsigned int position = unicodeString.find_first_not_of(delimiters,currPosition+wordLength);
    position = position==string::npos?unicodeString.length():position;
    it += position - currPosition;
    currPosition = position;
  }
}
void TextArea::increaseIndent()
{
  m_indentLevel += INDENT;
  m_cursorx = m_indentLevel;
}
void TextArea::decreaseIndent()
{
  m_indentLevel -= INDENT;
  m_cursorx = m_indentLevel;
}

void TextArea::printu(const UnicodeString & unicodeString)
{
  // skip until we reach startLine
  int currPosition(0);
  if (not m_foundPosition) {
    UnicodeString::const_iterator it(unicodeString.begin());
    int finalLine(m_font->height()*m_startLine);
    if (finalLine < 0)
    {
      m_cursory = -finalLine;
      m_initialCursory = m_cursory;
      finalLine = 0;
    }
    for (; it != unicodeString.end() and m_cursory < finalLine;)
    {
      // find the next space character
      const UnicodeString word(nextWord(unicodeString, currPosition));
      int size = textSize(word);
      if (m_cursorx + size > Canvas::instance().width())
      {
        incrLine();
      }
      if ( m_cursory >= finalLine)
        break;

      UnicodeString::const_iterator wordIt(word.begin());
      for (; wordIt != word.end() and m_cursory < finalLine; ++wordIt)
      {
        unsigned int value(*wordIt);
        if (value == UTF8::MALFORMED) {
          value = '?';
        }
        //assert(value != '\n');
        Font::Glyph g;
        m_font->glyph(value, g);
        if (m_parseNewline and value == '\n') {
          incrLine();
        } else if (value != '\n') {
          checkLetter(g);
        }
        if ( m_cursory >= finalLine)
          break;
        m_cursorx += g.width;
      }
      advanceWord(unicodeString, word.length(), currPosition, it);
    }
    if (m_cursory < finalLine) {
      return;
    }
    m_foundPosition = true;
    m_cursorx = m_initialCursorx+m_indentLevel;
    m_cursory = m_initialCursory;
  }
  if ( m_cursory < Canvas::instance().height())
  {
    UnicodeString printString = unicodeString.substr(currPosition , unicodeString.length()-currPosition);
    printuImpl(printString);
  }
}

int TextArea::textSize(const UnicodeString & unicodeString) const
{
  UnicodeString::const_iterator it(unicodeString.begin());
  int size(0);
  for (; it != unicodeString.end(); ++it)
  {
    unsigned int value(*it);
    if (value == UTF8::MALFORMED)
      value = '?';
    Font::Glyph g;
    m_font->glyph(value, g);
    size += g.width;
  }
  return size;
}

void TextArea::printuWord(const UnicodeString & word)
{
  UnicodeString::const_iterator it(word.begin());
  for (; it != word.end(); ++it)
  {
    unsigned int value(*it);
    if ( doSingleChar(value) )
    {
      break;
    }
  }
}

void TextArea::printuImpl(const UnicodeString & unicodeString)
{

  UnicodeString::const_iterator it(unicodeString.begin());
  int currPosition(0);
  for (; it != unicodeString.end(); )
  {
    // find the next space character
    const UnicodeString word(nextWord(unicodeString, currPosition));
    int size = textSize(word);
    if (m_cursorx + size > Canvas::instance().width())
    {
      incrLine();
    }
    if (m_cursory > Canvas::instance().height()) {
      break;
    }
    switch (m_currentControl)
    {
      case LINK:
        {
          assert(not m_links.empty());
          Link * link = m_links.front();
          link->appendClickZone(m_cursorx, m_cursory, 
              size, m_font->height());
        }
        break;
      case TEXT:
        break;
      case FORM:
        break;
    }
    printuWord(word);
    advanceWord(unicodeString, word.length(), currPosition, it);
  }
}

Link * TextArea::clickLink(int x, int y) const
{
  LinkList::const_iterator it = m_links.begin();
  for (; it != m_links.end(); ++it)
  {
    Link * link = *it;
    if ( link->hitTest(x, y))
    {
      return link;
    }
  }
  return 0;
}

FormControl * TextArea::clickForm(int x, int y) const
{
  FormList::const_iterator it = m_formControls.begin();
  for (; it != m_formControls.end(); ++it)
  {
    FormControl * form = *it;
    if ( form->hitTest(x, y))
    {
      return form;
    }
  }
  return 0;
}

void TextArea::addLink(const HtmlElement * anchor)
{
  m_links.push_front(new Link(anchor));
  setLink(true);
}
void TextArea::addFormControl(FormControl * formCtrl)
{
  m_formControls.push_front(formCtrl);
  setForm(true);
  formCtrl->setHeight(m_font->height());
  // the form controls its own width.

  if (not m_foundPosition) {
    int finalLine(m_font->height()*m_startLine);
    if (finalLine < 0)
    {
      m_cursory = -finalLine;
      m_initialCursory = m_cursory;
      finalLine = 0;
    }
    if (m_cursorx + formCtrl->width() > Canvas::instance().width())
    {
      incrLine();
    }
    if (m_cursory < finalLine) {
      return;
    }
    m_foundPosition = true;
    m_cursorx = m_initialCursorx+m_indentLevel;
    m_cursory = m_initialCursory;
  }
  if (m_foundPosition and m_cursory < Canvas::instance().height() ) {
    // get the real position?
    assert(not m_formControls.empty());
    if (m_cursorx + formCtrl->width() > Canvas::instance().width())
    {
      incrLine();
    }
    int cursorx = m_cursorx;
    int cursory = m_cursory;
    formCtrl->setPosition(m_cursorx, m_cursory);
    formCtrl->draw(this);
    m_cursorx = cursorx + formCtrl->width();
    m_cursory = cursory;
  }
}

bool TextArea::doSingleChar(unsigned int value)
{
  if (m_palette) {
    if (value == UTF8::MALFORMED) {
      value = '?';
    }
    Font::Glyph g;
    m_font->glyph(value, g);
    if (m_parseNewline and value == '\n') {
      incrLine();
    } else if (value != '\n') {
      checkLetter(g);
      if (g.data) {
        printAt(g, m_cursorx, m_cursory);
      }
      m_cursorx += g.width;
    }
  } else {
    // could not load the font?
    Font::Glyph g;
    m_font->glyph(' ', g);
    Canvas::instance().fillRectangle(m_cursorx, m_cursory, 
        g.width, g.height, Color(31,0,0));
    m_cursorx += g.width;
  }
  if (m_cursorx > Canvas::instance().width())
  {
    incrLine();
  }
  return (m_cursory > Canvas::instance().height());
}

void TextArea::print(const char * text, int amount)
{
  int total = 0;
  while (total < amount)
  {
    unsigned int value;
    unsigned int read(1);
    if (m_encoding == "utf-8") {
      read = UTF8::decode(text, value);
    } else {
      value = (int)(text[0]&0xff);
    }
    if ( doSingleChar(value) )
    {
      break;
    }
    text += read;
    total += read;
  }
}

void TextArea::setDefaultColor()
{
  m_bgCol = m_basePalette[0];
  m_fgCol = 0;
  setTextColor(m_fgCol);
}

void TextArea::setBackgroundColor(unsigned short color)
{
  m_bgCol = color;
  setTextColor(m_fgCol);
}

void TextArea::setTextColor(unsigned short color)
{
  m_fgCol = color;
  Color newColor(color);
  Color bgCol(m_bgCol);
  // assuming m_basePalette is black and white...
  int mred = bgCol.red() - newColor.red();
  int mgreen = bgCol.green() - newColor.green();
  int mblue = bgCol.blue() - newColor.blue();

  for (int i = 0; i < m_paletteLength; ++i)
  {
    // convert from grey scale to colour scale.
    Color c(m_basePalette[i]);

    // y |       ____   y = mx+c
    //  c|___----
    //   |___________ 
    //                x
    //  c = initial colour      (intercept)
    //  m = 31 - initial  / 32  (gradient)
    //  x = grey value

    int yred   = (mred * c.red())/32 + newColor.red();
    int ygreen = (mgreen * c.green())/32 + newColor.green();
    int yblue  = (mblue * c.blue())/32 + newColor.blue();
    c.red(yred);
    c.green(ygreen);
    c.blue(yblue);

    m_palette[i] = c;

  }
}

void TextArea::clear()
{
  Canvas & canvas( Canvas::instance());
  canvas.fillRectangle(0,0,canvas.width(), canvas.height(), m_bgCol);
}

void TextArea::setPalette(const std::string & fileName)
{
  File palFile;
  palFile.open(fileName.c_str());
  // read the lot
  if (palFile.is_open())
  {
    int size = palFile.size();
    char * data = new char[size+2];
    palFile.read(data);
    data[size] = 0;
    m_palette = (unsigned short*) data;
    char * baseData = new char[size+2];
    copy(data, data+size, baseData);
    m_basePalette = (unsigned short*)baseData;
    m_paletteLength = size/2;
    setBackgroundColor(m_palette[0]);
  } else {
    Canvas::instance().fillRectangle(130,0,10,128,Color(31,0,0));
  }
}

void TextArea::setPalette(const char * data, unsigned int size)
{
  m_palette = new unsigned short[size/2];
  copy(data, data+size, (char*)m_palette);
  char * baseData = new char[size+2];
  copy(data, data+size, baseData);
  m_basePalette = (unsigned short*)baseData;
  m_paletteLength = size/2;
  setBackgroundColor(m_palette[0]);
}

TextArea::~TextArea()
{
  delete m_font;
  delete [] m_palette;
}

void TextArea::setEncoding(const std::string & encoding)
{
  m_encoding = encoding;
}
