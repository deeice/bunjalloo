/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Font.h"
#include "Palette.h"
#include "File.h"
#include "UTF8.h"
using namespace std;
using namespace nds;

class Range
{
  public:
    Range(unsigned int lower, unsigned int upper)
      : m_lower(lower), m_upper(upper)
    {
      //cout << "Range " << m_lower << " - " << m_upper<< endl;
    }
    bool contains(unsigned int value) const
    {
      return (value >= m_lower and value < m_upper);
    }
    unsigned int range() const
    {
      return m_upper - m_lower;
    }
    unsigned int lower() const
    {
      return m_lower;
    }
    static void remove(Range * r)
    {
      delete r;
    }
  private:
    unsigned int m_lower;
    unsigned int m_upper;
};

Font::Font(const unsigned char * imageData, 
    const unsigned char * mapData):
  m_width(8),
  m_height(8),
  m_glyphData(0),
  m_glyphSize(0),
  m_glyphPosition(0)
{
  init(imageData, mapData);
}

#define READ_U16(value, from) { value = (from[index]<<8) |(from[index+1]); index += 2; }
#define READ_U32(value, from) { value = (from[index]<<24) |(from[index+1]<<16)|(from[index+2]<<8)|(from[index+3]); index+=4;}
void Font::init(const unsigned char * imageData, const unsigned char * mapData)
{
  m_glyphData = imageData;
  unsigned int glyphCount = 0;
  int rangeCount = 0;
  int index = 0;
  READ_U16(m_height, mapData);
  READ_U16(m_width, mapData);
  READ_U16(glyphCount, mapData);
  READ_U16(rangeCount, mapData);
  for (int i = 0; i < rangeCount; ++i)
  {
    int lower, upper;
    READ_U32(lower, mapData);
    READ_U32(upper, mapData);
    m_range.push_back(new Range(lower, upper));
  }

  // now create the glyph size map
  //unsigned int glyphs = m_maxGlyph - m_minGlyph;
  m_glyphSize = new unsigned char[glyphCount];
  m_glyphPosition = new unsigned short[glyphCount];

  unsigned char * dstSize = m_glyphSize;
  const unsigned char * src = &mapData[index];
  unsigned short * dstPos = m_glyphPosition;
  for (unsigned int i = 0; i < glyphCount; ++i)
  {
    *dstSize++ = *src++;
    *dstPos++ = ((*src)<<8)|(*(src+1));
    src+=2;
  }
}

Font::Font(const std::string & fileName):
  m_width(8),
  m_height(8),
  m_glyphData(0),
  m_glyphSize(0),
  m_glyphPosition(0)
{
  File fontFile;
  fontFile.open((fileName+".img").c_str());
  // read the lot
  unsigned char * glyphData(0);
  unsigned char * data(0);
  if (fontFile.is_open())
  {
    int size = fontFile.size();
    glyphData = new unsigned char[size+1];
    fontFile.read((char*)glyphData);
    glyphData[size] = 0;
    fontFile.close();
  }

  // read the map data too
  File mapFile;
  mapFile.open((fileName+".map").c_str());
  // read the lot
  if (mapFile.is_open())
  {
    int size = mapFile.size();
    data = new unsigned char[size+2];
    mapFile.read((char*)data);
    data[size] = 0;
    mapFile.close();
  }
  if (glyphData and data) {
    init(glyphData, data);
  }
  if (data)
    delete[] data;
}

Font::~Font()
{
  for_each(m_range.begin(), m_range.end(), Range::remove);
  //delete [] m_glyphData;
  delete [] m_glyphSize;
  delete [] m_glyphPosition;
}

int Font::minGlyph() const
{
  if (not m_range.empty()) {
    const Range *range(m_range[0]);
    return range->lower();
  } else {
    return -1;
  }
}
int Font::valueToIndex(unsigned int glyphId) const
{
  vector<Range*>::const_iterator it(m_range.begin());
  int total = 0;
  for (; it != m_range.end(); ++it)
  {
    const Range * range(*it);
    if (range->contains(glyphId))
    {
      return glyphId + total - range->lower();
    }
    total += range->range();
  }
  return -1;
}

void Font::glyph(unsigned int value, Glyph & glyphData) const
{
  int glyph = valueToIndex(value);
  if (glyph == -1)
  {
    int min = minGlyph();
    if (min == -1) {
      glyphData.data = 0;
      return;
    }
    value = min;
    glyph = valueToIndex(value);
  }
  unsigned int position = m_glyphPosition[glyph];
  int offset = position / 2;

  glyphData.width = m_glyphSize[glyph];
  glyphData.height = m_height;
  glyphData.data = (const unsigned char*)(&m_glyphData[offset]);
}

void Font::textSize(const char * text, int amount, int & width, int & height, const std::string & encoding) const
{
  // length of text in bytes.
  bool utf8(encoding == "utf-8");
  width = 0;
  height = m_height;
  int maxWidth = 0;
  int total = 0;
  while (total < amount)
  {
    unsigned int value;
    unsigned int read(1);
    if (utf8) {
      read = UTF8::decode(text, value);
    } else {
      value = (int)(text[0]&0xff);
    }
    if (value == UTF8::MALFORMED) {
      value = '?';
    }
    if (value == '\n')
    {
      height += m_height;
      width = 0;
    } else {
      Glyph g;
      this->glyph(value, g);
      width += g.width;
    }
    if (width >= maxWidth) {
      maxWidth = width;
    }
    text += read;
    total += read;
  }
  width = maxWidth;
}

