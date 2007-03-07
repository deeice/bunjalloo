#include <nds.h>
#include <stdio.h>
#include "ndspp.h"
#include "Text16.h"
// external data
extern const u16 _binary_chaosfont_raw_start[];

// static data
const static int DEFAULT_OFFSET(605);
const static int TEXT_PALETTE_ENTRY(1);
static const int SPACE_CHAR_INDEX(32);
static const int FILLED_CHAR_INDEX((int)('z'+2));
static const int TRANSP_CHAR_INDEX((int)('z'+1));
static const char EMPTY_MESSAGE[] = "                               ";

const int Text16::FIRST_CHAR_INDEX((int) '\'');
const int Text16::MESSAGE_X(0);
const int Text16::MESSAGE_Y(22);

// namespaces
using namespace nds;

// Text16 implementation
const int Text16::MAX_LETTERS(85);

Text16::Text16():
  m_screen(0),
  m_bg(new nds::Background(m_screen,1,0,30)),
  m_mapOffset(DEFAULT_OFFSET)
{
  // each 8 by 8 tile takes up 16 bytes
  int realOffset  = m_mapOffset*16;
  // the font consists of 84 letters
  // each one is 16*8, 4bpp = 16*4 pixels = 64 u8 transfers, 32 u16 transfers per char
  // total = 32*84
  u16 * tileData = m_bg->tileData();
  dmaCopy(_binary_chaosfont_raw_start,&tileData[realOffset],64*MAX_LETTERS);
  m_bg->enable();
  
}

Text16 & Text16::instance()
{
  static Text16 s_instance;
  return s_instance;
}
Text16::~Text16() {}

// set the colour for the text in a given palette
void Text16::setColour(int pal, int col) {
  Palette p(m_screen,pal);
  p[TEXT_PALETTE_ENTRY] = col;
}

unsigned short Text16::colour(int pal) const {
  Palette p(m_screen, pal);
  return p[TEXT_PALETTE_ENTRY];
}


void Text16::print(const char* text, int x, int y , int pal)
{

  unsigned int indx=0;
  unsigned int newx = x;
  unsigned short * mapData = m_bg->mapData();
	
  while( (text[indx]!= 0) ) 
  {
    int letter = text[indx] - FIRST_CHAR_INDEX;
    // draw a letter...
    if (text[indx] != SPACE_CHAR_INDEX) {
      mapData[newx+y*32] = letter*2+m_mapOffset;
      mapData[newx+(y+1)*32] = letter*2 + 1 + m_mapOffset;
    }
    else {
      letter = TRANSP_CHAR_INDEX - FIRST_CHAR_INDEX;
      mapData[newx+y*32] = letter*2+1+m_mapOffset;//9+bgmapoffset16+97;
      mapData[newx+(y+1)*32] = letter*2+1+m_mapOffset;//9+bgmapoffset16+97;
    }
    // set the palette at these locations too
    mapData[newx+y*32] &= ~0xF000; 
    mapData[newx+y*32] |= (pal<<12)&0xF000;
    mapData[newx+(y+1)*32] &= ~0xF000; 
    mapData[newx+(y+1)*32] |= (pal<<12)&0xF000;
    
    newx++;
    indx++;
  }
}


// place a single character from the alphabet at position x, y
// this is used to draw, eg. the ^/v arrow thing
void Text16::putChar(int ch, int x, int y, int pal, u32 extraFlags) {
  // put the character in spot x, y with palette pal
  unsigned short * mapData = m_bg->mapData();
  if (ch != 0) {
    mapData[x+y*32] = ch*2 + m_mapOffset;
  } else {
    mapData[x+y*32] = 0;
  } 
  mapData[x+y*32] &= ~0xF000; 
  mapData[x+y*32] |= (pal<<12)&0xF000;
  mapData[x+y*32] |= extraFlags; 
}

void Text16::int2a(int n, char s[]) {
  siprintf(s,"%d",n);
}

void Text16::clear()  {
  unsigned short * mapData = m_bg->mapData();
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      mapData[x+y*32] = 0; 
    }
  }
}

void Text16::clearMessage() {
  print(EMPTY_MESSAGE, MESSAGE_X,MESSAGE_Y, 12);
}
void Text16::displayMessage(const char * message) {
  print(EMPTY_MESSAGE, MESSAGE_X,MESSAGE_Y, 12);
  setColour(12, Color(0,30,30));
  print(message, MESSAGE_X,MESSAGE_Y, 12);
}
