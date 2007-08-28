/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include <assert.h>
#include "libnds.h"
#include "ndspp.h"
#include "File.h"
#include "Font.h"
//#include "FormControl.h"
#include "Canvas.h"
#include "Link.h"
#include "Palette.h"
#include "TextArea.h"
#include "UTF8.h"

using namespace nds;
using namespace std;
const static nds::Color EDGE(20,20,20);
const static nds::Color SHADOW(28,28,28);

static const unsigned int intDelimiters[] = {0x0020, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d};
static const UnicodeString s_delimiters(intDelimiters,6);
static const int INDENT(16);

TextArea::TextArea(Font * font) : 
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
  m_appendPosition(0),
  m_editable(false)
{
  setFont(font);
  m_document.clear();
  m_preferredHeight = m_font->height();
  m_preferredWidth = Canvas::instance().width();
}

static void deleteLink(Link * link)
{
  delete link;
}

void TextArea::removeClickables()
{
  for_each(m_links.begin(), m_links.end(), deleteLink);
  m_links.clear();
}

void TextArea::setFont(Font * font)
{
  delete m_font;
  m_font = font;
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

UnicodeString & TextArea::text()
{
  return m_document;
}

bool TextArea::isEditable() const
{
  return m_editable;
}
void TextArea::setEditable(bool edit)
{
  m_editable = edit;
}

bool TextArea::touch(int x, int y)
{
  if (not m_editable)
  {
    return Component::touch(x, y);
  }
  else
  {
    // touch...
    if (m_bounds.hit(x, y))
    {
      return true;
    }
    return false;
  }
}

void TextArea::clearText()
{
  m_document.clear();
  m_appendPosition = 0;
  m_preferredHeight = m_font->height();
}

void TextArea::appendText(const UnicodeString & unicodeString)
{
  // append text, adding in new lines as needed to wrap.
  int currPosition = 0;
  // find the next space character
  UnicodeString::const_iterator it(unicodeString.begin());
  for (; it != unicodeString.end();)
  {
    const UnicodeString word(nextWord(unicodeString, currPosition));
    int size = textSize(word);
    
    // if the word ends with a new line, then increment the height.
    // otherwise, if we go off the end of the line, increment the height.
    if (word[word.length()-1] == '\n')
    {
      m_appendPosition = 0;
      m_preferredHeight += m_font->height();
    }
    else if ((m_appendPosition + size) > width())
    {
      // this word overflows the line
      m_document += '\n';
      m_appendPosition = 0;
      m_preferredHeight += m_font->height();
    }
    m_document += word;
    m_appendPosition += size;
    advanceWord(unicodeString, word.length(), currPosition, it);
  }
}

void TextArea::setSize(unsigned int w, unsigned int h)
{
  if (m_bounds.w != (int)w) {
    Component::setSize(w, h);
    UnicodeString tmp(m_document);
    clearText();
    //m_preferredHeight = 0;
    if (not tmp.empty())
      appendText(tmp);
    if (m_preferredHeight == 0)
      m_preferredHeight = m_font->height();
  } else {
    Component::setSize(w, h);
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
  m_cursorx = m_initialCursorx; 
  m_cursory += m_font->height();
}

void TextArea::checkLetter(Font::Glyph & g)
{
  if ( (m_cursorx + g.width) > m_bounds.right())
  {
    incrLine();
  }
}

const UnicodeString TextArea::nextWord(const UnicodeString & unicodeString, int currPosition) const
{
  // TODO: words that are longer than Canvas::width() break everything.
  UnicodeString word;
  if (m_parseNewline)
  {
    // if we are parsing new lines, look for the next delimiter
    unsigned int position = unicodeString.find_first_of(s_delimiters,currPosition);
    position = position==string::npos?unicodeString.length():position;
    word = unicodeString.substr(currPosition,position-currPosition+1);
  }
  else
  {
    // not parsing new lines, so look for the next proper word, then the next non word
    unsigned int position = unicodeString.find_first_not_of(s_delimiters,currPosition);
    position = unicodeString.find_first_of(s_delimiters,position);
    position = position==string::npos?unicodeString.length():position;
    // now make a word delimted with spaces.
    word = unicodeString.substr(currPosition,position-currPosition);
    word += ' ';
  }
  int size(textSize(unicodeString));
  if (size > width())
  {
    // This is a very long word, split it up
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
      if ( (size + g.width) >= width()) {
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
    unsigned int position = unicodeString.find_first_not_of(s_delimiters,currPosition+wordLength);
    position = position==string::npos?unicodeString.length():position;
    it += position - currPosition;
    currPosition = position;
  }
}

void TextArea::printu(const UnicodeString & unicodeString)
{
  UnicodeString::const_iterator it(unicodeString.begin());
  for (; it != unicodeString.end() and m_cursory < m_bounds.bottom(); ++it)
  {
    unsigned int value(*it);
    if ( doSingleChar(value) )
    {
      break;
    }
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
  if (m_cursorx > m_bounds.right())
  {
    incrLine();
  }
  return (m_cursory > m_bounds.bottom());
}

void TextArea::setDefaultColor()
{
  m_bgCol = m_basePalette[0];
  m_fgCol = 0;
  setTextColor(m_fgCol);
}

void TextArea::setBackgroundColor(unsigned short color)
{
  if (color != m_bgCol)
  {
    m_bgCol = color;
    setTextColor(m_fgCol);
  }
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
  Canvas::instance().fillRectangle(x(),y(),width(), height(), m_bgCol);
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
    /* If the font is not opened, then set the bg red */
    Canvas::instance().fillRectangle(0,0,256,192,Color(31,0,0));
    setBackgroundColor(Color(31,0,0));
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

/** Paint the text area. */
void TextArea::paint(const nds::Rectangle & clip)
{
  // TextArea wants to paint from 0,0 to 256,END_Y
  // ScrollView says "start at position X" instead... (the clip square)
  // TextArea then thinks "position X, that is really line N"
  // and draws there until it runs out of space.
  //
  // Theres "width" which is the possible width (wraps at)
  // then theres clip-width which is where it should draw to.
  setCursor(m_bounds.x, m_bounds.y);
  Canvas::instance().fillRectangle(clip.x, clip.y, clip.w, clip.h, m_bgCol);
  if (m_editable) {
    Canvas::instance().horizontalLine(clip.x, clip.top(), clip.w, EDGE);
    Canvas::instance().verticalLine(clip.left(), clip.top(), clip.h, EDGE);
    Canvas::instance().horizontalLine(clip.x, clip.bottom()-1, clip.w, SHADOW);
    Canvas::instance().verticalLine(clip.right(), clip.top(), clip.h, SHADOW);
  }
  printu(m_document);
}

std::string TextArea::asString() const
{
  return unicode2string(m_document);
}
