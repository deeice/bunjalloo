#include <cstdlib>
#include <nds.h>
#include "ndspp.h"
#include "Line.h"
#include "Arena.h"
#include "Graphics.h"
#include "SpellData.h"

using namespace nds;
bool Line::doLine(Line_t type)
{
  Line l(type);
  return l.execute();
}

Line::Line(Line_t ls):
  m_type(ls),
  m_theArena(Arena::instance()),
  m_pattern(0),
  m_tableOffset(0),
  m_segmentCount(0),
  m_length(0),
  m_gfx(0),
  m_end(0),
  m_colour(0)
{
  switch (m_type) 
  {
    case ARROW: 
      m_colour = Color(31,31,31); m_tableOffset = 0x2A; m_pattern = 1; break;
    case SHORT_ARROW: 
      m_colour = Color(31,31, 0); m_tableOffset = 0x0E; m_pattern = 2; break;
    case SPELL: 
      m_colour = Color( 0,31,31); m_tableOffset = 0x3C; m_pattern = 3; break;
    case DRAGON: 
      m_colour = Color(31, 0, 0); m_tableOffset = 0x64; m_pattern = 3; break;
    case BOLT: 
      m_colour = Color( 0,31,10); m_tableOffset = 0x08; m_pattern = 3; break;
    case LIGHTNING: 
      m_colour = Color(31,25,10); m_tableOffset = 0x64; m_pattern = 3; break;
    default: 
      break;
  }
  m_mapData = m_theArena.m_bg->mapData();
  m_tileData = m_theArena.m_bg->tileData();
}

Line::~Line()
{
  if (m_type != SIGHT)
  {
    Graphics::loadAllPalettes();
  }
}


void Line::xyUpper(int index, int & x, int & y) const
{
  Arena::getXY(index, x, y);
  x *= 16;
  y *= 16;
}

bool Line::isPixelSet(unsigned int x, unsigned int y) const
{
  // check a pixel is set
  // fix for disappearing fire/ghost/wraith
  int chaos_tile = x/16 + (y/16)*16;
  int creature = m_theArena.m_arena[0][chaos_tile];
  bool dead(m_theArena.m_arena[2][chaos_tile] == 4);
  if (dead 
      or chaos_tile == m_theArena.m_targetIndex 
      or chaos_tile == m_theArena.m_startIndex 
      or creature < SPELL_KING_COBRA 
      or creature == SPELL_WRAITH 
      or creature == SPELL_GHOST 
      or creature == SPELL_MAGIC_FIRE) 
  {
    return false;
  }
  
  int tile = 1 + (x/8) + ((y/8)*30);
  // then from here it's easy (ish) just work out the remainder from /8
  int xrem = x&0x7;
  int yrem = y&0x7;
  
  int address = tile*16 /*16 16-bit vals per tile*/ + (xrem/4) + (yrem*2);
  
  u16 pixel = m_tileData[address];
  if ((x & 3) == 0) {
    return  ((pixel & 0x000F) != 0);
  } else if ((x & 3) == 1) {
    return  ((pixel & 0x00F0) != 0);
  } else if ((x & 3) == 2) {    
    return  ((pixel & 0x0F00) != 0);
  } else { //if ((x & 3) == 3)
    return  ((pixel & 0xF000) != 0);
  }
}

// check line of sight between m_startIndex and m_targetIndex
bool Line::execute() 
{
  // init segment counter
  m_segmentCount = 0;
  m_length = 0;
  // check from start_index to index if LOS
  
  // line of sight OK
  bool losBlocked(false);
  
  int startx, starty, targetx, targety;
  
  xyUpper(m_theArena.m_startIndex, startx, starty);
  startx-=8; starty-=8;
  
  xyUpper(m_theArena.m_targetIndex, targetx, targety);
  targetx-=8; targety-=8;
  
  u16 deltax = abs(startx - targetx);
  u16 deltay = abs(starty - targety);
  
  
  int numpixels;
  int xinc1, xinc2;
  int yinc1, yinc2;
  int d, dinc1, dinc2;

  if(deltax >= deltay) {
    //If x is independent variable
    numpixels = deltax + 1;
    d = (2 * deltay) - deltax;
    dinc1 = deltay * 2;
    dinc2 = (deltay - deltax) * 2;
    xinc1 = 1;
    xinc2 = 1;
    yinc1 = 0;
    yinc2 = 1;
  }
  else
  {
    //If y is independant variable
    numpixels = deltay + 1;
    d = (2 * deltax) - deltay;
    dinc1 = deltax * 2;
    dinc2 = (deltax - deltay) * 2;
    xinc1 = 0;
    xinc2 = 1;
    yinc1 = 1;
    yinc2 = 1;
  }
  
  //Move the right direction
  if(startx > targetx)
  {
      xinc1 = -xinc1;
      xinc2 = -xinc2;
  }
  if(starty > targety)
  {
      yinc1 = -yinc1;
      yinc2 = -yinc2;
  }
  
  int currentx(startx);
  int currenty(starty);
  
  m_end = targetx+targety*16;
  for(int i = 0; i < numpixels; i++)    {
    if (m_type == SIGHT) {
      if (isPixelSet(currentx, currenty)) 
      {
        losBlocked = true;
      } 
    } else {
      // not the LOS check... do the line drawing
      drawLineSegment(currentx, currenty);  
    }
    
    if(d < 0) {
      d = d + dinc1;
      currentx = currentx + xinc1;
      currenty = currenty + yinc1;
    }
    else {
      d = d + dinc2;
      currentx = currentx + xinc2;
      currenty = currenty + yinc2;
    }
  }
  return losBlocked;
}

// the line table stores the current positions...
static u8 s_lineTable[0x68];

// based on code at b7d3 and b84d
void Line::drawLineSegment(int x, int y) 
{
  s_lineTable[0] = (u8)x;
  s_lineTable[1] = (u8)y;

  innerDrawSegment(x,y);
  // if reach the end of line, jump...
  if ( (x+(y*16)) != m_end) {
    if ( ((m_segmentCount & 1) == 0)) { 
      swiWaitForVBlank();
    }
    for (int i = 0x64; i >= 0; i--) {
      s_lineTable[i+2] = s_lineTable[i];
    }
    
    if (m_length < m_tableOffset) {
      m_length += 2;
    }
    u8 newx, newy;
    newx = s_lineTable[m_length];
    newy = s_lineTable[m_length+1];
    
    m_pattern |= 0x80;
    innerDrawSegment(newx,newy);
    
    m_pattern = m_pattern & 0x7f;
  }  else {
    // end of line reached
    int loops = (m_length/2) + 2;
    int index(m_length);
    for (int i = 0; i < loops; i++) {
      int newx(s_lineTable[index]);
      int newy(s_lineTable[index+1]);
      m_pattern |= 0x80;
      if ( ((i & 1) == 0) or m_pattern == 3) {
        swiWaitForVBlank();
      }
      innerDrawSegment(newx,newy);
      
      index -= 2;
    }
    
  }
  
  
}

void Line::innerDrawSegment(int x, int y) 
{
  int A = m_pattern & 0x7f;
  if ( A == 3 ) {
    // b85d...
    setPixel(x,y);
    setPixel(x,y+1);
    setPixel(x,y-1);
    setPixel(x+1,y);
    setPixel(x-1,y);
    
    if ( m_type == DRAGON or m_type == LIGHTNING) {
      setPixel(x-2,y+2);
      setPixel(x,y+3);
      setPixel(x+2,y+2);
      setPixel(x+3,y);
      setPixel(x+2,y-2);
      setPixel(x,y-3);
      setPixel(x-2,y-2);
      setPixel(x-3,y);
      
    }
    
  } else {
    // draw pixel at x,y and return
    setPixel(x,y);
  }
  m_segmentCount++;
}

void Line::drawPixel4bpp(unsigned short x, unsigned short y) 
{
  // draw a pixel to the non rot bg, assume the tiles are set up already
  // use "1" as the colour
  
  // convert the x position to the x,y position to the 8*8 tile
  // each tile is 8 pixels wide, but these are stored in quadruples 
  // (pairs of 8 bit values, each 8 bit value holds 2 pixels)
  // therefore each 8*8 block is just 16 "reads" big.
  // 16 * 16 = 256 bits. We have 4 bits per pixel, so 256 bits = 64 pixels
  
  // the pixel could be either one of the 4 4bit blocks 
  //       1111 0000 0000 0000    F000 -> 0FFF
  //       0000 1111 0000 0000    0F00 -> F0FF
  //       0000 0000 1111 0000    00F0 -> FF0F
  //       0000 0000 0000 1111    000F -> FFF0
  // to determine the 4 bit block, need to think about the pixel.
  // pixel 0 4 is in lower 4 bits   00  100     &3 == 0
  // pixel 1 5 is in next 4 bits    01  101     &3 == 1
  // pixel 2 6 is in next 4 bits    10  110     &3 == 2
  // pixel 3 7 is in next 4 bits    11  111     &3 == 3
  
  /* tile     add           x at top left corner      y
      1         20          0                         0
      2         40          8                         0
      3         60...       16                        0 
      31        3E0         0                         8
      32        400         8                         8....
      
      so to get tile from x,y... need to 1 + (y/8 * 30 + x/8)
  */
  
  int tile = 1 + (x/8) + ((y/8)*30);
  // then from here it's easy (ish) just work out the remainder from /8
  //u8 xrem = x - ((x>>3)<<3); //&7!
  //u8 yrem = y - ((y>>3)<<3); //&7!

  int xrem = x&7;
  int yrem = y&7;
  
  int address = tile*16 /*16 16-bit vals per tile*/ + (xrem/4) + (yrem*2);
  
  u16 pixel = m_tileData[address];
  if ((x & 3) == 0) {
    // lower bits are removed...
    pixel |= 0x000F;
  } else if ((x & 3) == 1) {
    pixel |= 0x00F0;
  } else if ((x & 3) == 2) {    
    pixel |= 0x0F00;
  } else if ((x & 3) == 3) {    
    pixel |= 0xF000;
  }
  
  m_tileData[address] = pixel;
}

// sets or unsets pixel depending on line segment and other things
// b8bd...
void Line::setPixel(unsigned short x, unsigned short y) 
{
  
  if ( (m_pattern & 0x80) == 0x80) {
    // remove pixel here
    removePixel(x,y);
    return;
  }
  if ( m_type == DRAGON or m_type == LIGHTNING) {
    if ( (m_segmentCount & 0x7) != 0 ) 
      return;
  }
  
  drawPixel4bpp(x, y);
  u16 tile = ((x/8)+1) + ((y/8)+1)*32;
  // get current palette... copy here..
  int pal = (m_mapData[33+tile] & 0xF000)>>12;
  Palette p(0,pal);
  p[15] = m_colour;
}



void Line::removePixel(unsigned short x, unsigned short y) 
{
  // FIXME: factorise this code
  // remove the pixel at x, y
  int tile = 1 + (x/8) + ((y/8)*30);
  // then from here it's easy (ish) just work out the remainder from /8
  int xrem = x&0x7;
  int yrem = y&0x7;
  
  int address = tile*16 /*16 16-bit vals per tile*/ + (xrem / 4) + (yrem * 2);
  
  u16 pixel = m_tileData[address];
  
  if ((x & 3) == 0) {
    pixel &= 0xFFF0;
  } else if ((x & 3) == 1) {
    pixel &= 0xFF0F;
  } else if ((x & 3) == 2) {    
    pixel &= 0xF0FF;
  } else if ((x & 3) == 3) {    
    pixel &= 0x0FFF;
  }
  m_tileData[address] = pixel;
}

