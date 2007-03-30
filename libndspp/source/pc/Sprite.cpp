#include "libnds.h"
#include "Sprite.h"
#include "HardwareHandler.cpp"
#include "SpriteHandler.h"
#include <stdio.h>

using namespace nds;
typedef unsigned short uint16;
static unsigned short * OAM(new unsigned short[0x2000]);
static unsigned short * OAM_SUB(new unsigned short[0x2000]);

static SpriteEntry s_sprites[2][128];
static pSpriteRotation s_affine = (pSpriteRotation)(s_sprites);
bool Sprite::s_used[2][128];

Sprite::Sprite(int screen):
  m_screen(screen),
  m_index(-1),
  m_used(s_used[screen==0?screen:1]),
  m_OAM(screen==0?OAM:OAM_SUB)
{
  initialise(); 
  SpriteHandler::add(this);
}

Sprite::Sprite(int screen, int w, int h, int tile, int colors):
  m_screen(screen),
  m_index(-1),
  m_used(s_used[screen==0?screen:1]),
  m_OAM(screen==0?OAM:OAM_SUB)
{
  initialise(); 
  this->tile(tile);
  this->setSize(w,h);
  this->color(colors);
}

Sprite::~Sprite()
{
  // make sure sprite is not drawn
  doubleSize(true);
  rotateScale(false);
  update();
  m_used[m_index] = false;
}

int Sprite::y() const
{ 
  if (!valid()) return 0;
  // unpack y
  return OBJ_Y(s_sprites[m_screen][m_index].attribute[0]);
}

void Sprite::y(unsigned char y)
{ 
  if (!valid()) return;
  // pack y
  SpriteEntry & sp = s_sprites[m_screen][m_index];
  sp.attribute[0] &= 0xFF00;
  sp.attribute[0] |= y;
}


bool Sprite::rotateScale() const
{ 
  if (!valid()) return false;
  return s_sprites[m_screen][m_index].attribute[0]&ATTR0_ROTSCALE;
}

void Sprite::setAttrBit(unsigned int attrn, unsigned int bits, bool set, unsigned int clear)
{
  SpriteEntry & sp = s_sprites[m_screen][m_index];
  unsigned short & attr = ( attrn==0 ? sp.attribute[0]: ( attrn==1 ? sp.attribute[1] : sp.attribute[2] ) );
  if (clear == 0) {
    clear = ~bits;
  } else {
    clear = ~clear;
  }
  attr &= clear;
  attr |= set?bits:0;
}

void Sprite::rotateScale(bool s)
{ 
  if (!valid()) return;
  setAttrBit(0,ATTR0_ROTSCALE,s);
}

bool Sprite::doubleSize() const
{ 
  if (!valid()) return false;
  return s_sprites[m_screen][m_index].attribute[0]&ATTR0_DISABLED;
}

void Sprite::doubleSize(bool s)
{ 
  if (!valid()) return;
  setAttrBit(0,ATTR0_DISABLED,s);
}

unsigned int Sprite::mode() const
{ 
  if (!valid()) return 0;
  return ((s_sprites[m_screen][m_index].attribute[0]&(ATTR0_BMP))>>10);
}

void Sprite::mode(unsigned int mode)
{
  if (!valid()) return;
  if (mode>3)
    return;
  setAttrBit(0,(mode)<<10,mode,ATTR0_BMP);
}

bool Sprite::mosaic() const
{ 
  if (!valid()) return false;
  return s_sprites[m_screen][m_index].attribute[0]&ATTR0_MOSAIC;
}

void Sprite::mosaic(bool m)
{ 
  if (!valid()) return;
  setAttrBit(0,ATTR0_MOSAIC,m);
}
unsigned int Sprite::color() const
{ 
  if (!valid()) return 0;
  return (s_sprites[m_screen][m_index].attribute[0]&ATTR0_COLOR_256)?256:16;
}

void Sprite::color(unsigned int col)
{ 
  if (!valid()) return;
  switch (col)
  {
    case 16:
      setAttrBit(0,ATTR0_COLOR_16,true,ATTR0_COLOR_256);
      break;
    case 256:
      setAttrBit(0,ATTR0_COLOR_256,true);
      break;
    default:
      break;
  }
}

// attribute[1]
unsigned int Sprite::x() const
{
  if (!valid()) return 0;
  return OBJ_X(s_sprites[m_screen][m_index].attribute[1]);
}

void Sprite::x(unsigned int x)
{ 
  if (!valid()) return;
  SpriteEntry & sp = s_sprites[m_screen][m_index];
  sp.attribute[1] &= 0xFE00;
  sp.attribute[1] |= x&0x1FF;
}

// from gba_sprites.h
// attribute 0
#define OBJ_SHAPE(m)		((m)<<14)
// attribute 1
#define OBJ_SIZE(m)		((m)<<14)
// attribute 2
#define OBJ_CHAR(m)		((m)&0x03ff)
enum SPRITE_SIZECODE {        // v   hex   bits atr0 atr1
		Sprite_8x8,   // 0    0    0000  SQ  8 
		Sprite_16x16, // 1    1    0001  SQ  16
		Sprite_32x32, // 2    2    0010  SQ  32
		Sprite_64x64, // 3    3    0011  SQ  64
		Sprite_16x8,  // 4    4    0100  WI  8
		Sprite_32x8,  // 5    5    0101  WI  16
		Sprite_32x16, // 6    6    0110  WI  32
		Sprite_64x32, // 7    7    0111  WI  64
		Sprite_8x16,  // 8    8    1000  TA  8
		Sprite_8x32,  // 9    9    1001  TA  16
		Sprite_16x32, // 10   A    1010  TA  32
		Sprite_32x64  // 11   B    1011  TA  64
};


// shape is SQUARE TALL WIDE 
Sprite::SPRITE_SHAPE Sprite::shape() const
{ 
  if (!valid()) return NOT_VALID;
  return static_cast<Sprite::SPRITE_SHAPE>((s_sprites[m_screen][m_index].attribute[0]&OBJ_SHAPE(3))>>14);
}

void Sprite::shape(Sprite::SPRITE_SHAPE shape)
{ 
  if (!valid()) return;
  unsigned int s = static_cast<unsigned int>(shape);
  if (s > 3)
    return;
  setAttrBit(0,OBJ_SHAPE(s),true,OBJ_SHAPE(3));
}

unsigned int Sprite::rotate() const
{ 
  if (!valid()) return 0;
  return (s_sprites[m_screen][m_index].attribute[1]>>9)&15;
}

void Sprite::rotate(unsigned int rotate)
{ 
  if (!valid()) return;
  if (rotate > 15)
    return;
  setAttrBit(1,ATTR1_ROTDATA(rotate),true,ATTR1_ROTDATA(15));
}

bool Sprite::hflip() const
{ 
  if (!valid()) return false;
  return (bool)(s_sprites[m_screen][m_index].attribute[1]&ATTR1_FLIP_X);
}

void Sprite::hflip(bool b)
{ 
  if (!valid()) return;
  setAttrBit(1,ATTR1_FLIP_X,b);
}

bool Sprite::vflip() const
{ 
  if (!valid()) return false;
  return (s_sprites[m_screen][m_index].attribute[1]&ATTR1_FLIP_Y);
}

void Sprite::vflip(bool b)
{
  if (!valid()) return;
  setAttrBit(1,ATTR1_FLIP_Y,b);
}

static bool size_to_sprite_sizecode(unsigned int w, unsigned int h, SPRITE_SIZECODE & size)
{
  // given w and h, return size
  bool ok = false;
  if (w == 8) {
    switch (h)
    {
      case 8:
        size = Sprite_8x8;
        ok = true;
        break;
      case 16:
        size = Sprite_8x16;
        ok = true;
        break;
      case 32:
        size = Sprite_8x32;
        ok = true;
        break;
    }
  } else if (w == 16) {
    switch (h)
    {
      case 8:
        size = Sprite_16x8;
        ok = true;
        break;
      case 16:
        size = Sprite_16x16;
        ok = true;
        break;
      case 32:
        size = Sprite_16x32;
        ok = true;
        break;
    }
  } else if (w == 32) {
    switch (h)
    {
      case 8:
        size = Sprite_32x8;
        ok = true;
        break;
      case 16:
        size = Sprite_32x16;
        ok = true;
        break;
      case 32:
        size = Sprite_32x32;
        ok = true;
        break;
      case 64:
        size = Sprite_32x64;
        ok = true;
        break;
    }
  } else if (w == 64) {
    switch (h)
    {
      case 32:
        size = Sprite_64x32;
        ok = true;
        break;
      case 64:
        size = Sprite_64x64;
        ok = true;
        break;
    }
  }
  return ok;
}

#define SPRITE_SIZECODE_TO_SIZE(n,m) case Sprite_##n##x##m:\
{\
  w = n; h = m; \
  break; \
}

static void sprite_sizecode_to_size(SPRITE_SIZECODE size, unsigned int & w, unsigned int & h)
{
  switch (size)
  {
    SPRITE_SIZECODE_TO_SIZE(8,8);
    SPRITE_SIZECODE_TO_SIZE(16,16);
    SPRITE_SIZECODE_TO_SIZE(32,32);
    SPRITE_SIZECODE_TO_SIZE(64,64);
    SPRITE_SIZECODE_TO_SIZE(16,8);
    SPRITE_SIZECODE_TO_SIZE(32,8);
    SPRITE_SIZECODE_TO_SIZE(32,16);
    SPRITE_SIZECODE_TO_SIZE(64,32);
    SPRITE_SIZECODE_TO_SIZE(8,16);
    SPRITE_SIZECODE_TO_SIZE(8,32);
    SPRITE_SIZECODE_TO_SIZE(16,32);
    SPRITE_SIZECODE_TO_SIZE(32,64);
  }
}

void Sprite::getSize(unsigned int &w, unsigned int &h) const
{
  if (!valid()) return;
  unsigned int shapeFlag = (s_sprites[m_screen][m_index].attribute[0]>>14);
  unsigned int sizeFlag  = (s_sprites[m_screen][m_index].attribute[1]>>14);
  SPRITE_SIZECODE size = static_cast<SPRITE_SIZECODE>( sizeFlag | (shapeFlag<<2) );
  sprite_sizecode_to_size(size, w, h);
}

void Sprite::setSize(unsigned int w, unsigned int h)
{
  if (!valid()) return;
  SPRITE_SIZECODE size;
  bool success = size_to_sprite_sizecode(w, h, size);
  if (success) {
    unsigned int shapeFlag = (size& 0xC)>>2;
    unsigned int sizeFlag = (size & 0x3);
    setAttrBit(0,OBJ_SHAPE(shapeFlag),true,OBJ_SHAPE(3));
    setAttrBit(1,OBJ_SIZE(sizeFlag),true,OBJ_SIZE(3));
  }
}



// SPRITE_SIZECODE size OBJ_SIZE Sprite_32x8 etc
unsigned int Sprite::width() const
{ 
  if (!valid()) return 0;
  unsigned int w, h;
  getSize(w, h);
  return w;
}

void Sprite::width(unsigned int width)
{ 
  if (!valid()) return;
  unsigned int w, h;
  getSize(w, h);
  setSize(width, h);
}

unsigned int Sprite::height() const
{
  if (!valid()) return 0;
  unsigned int w, h;
  getSize(w, h);
  return h;
}

void Sprite::height(unsigned int height)
{ 
  if (!valid()) return;
  unsigned int w, h;
  getSize(w, h);
  setSize(w, height);
}


// attribute[2]
unsigned int Sprite::tile() const
{
  if (!valid()) return 0;
  return OBJ_CHAR(s_sprites[m_screen][m_index].attribute[2]);
}

void Sprite::tile(unsigned int tile)
{ 
  if (!valid()) return;
  setAttrBit(2,OBJ_CHAR(tile),true,OBJ_CHAR(0xFFFF));
}

unsigned int Sprite::priority() const
{
  if (!valid()) return 0;
  return (s_sprites[m_screen][m_index].attribute[2]&ATTR2_PRIORITY(3))>>10;
}

void Sprite::priority(unsigned int priority)
{ 
  if (!valid()) return;
  setAttrBit(2,ATTR2_PRIORITY(priority&3),true,ATTR2_PRIORITY(3));
}

unsigned int Sprite::palette() const
{ 
  if (!valid()) return 0;
  return (s_sprites[m_screen][m_index].attribute[2] & ATTR2_PALETTE(15))>>12;
}

void Sprite::palette(unsigned int palette)
{ 
  if (!valid()) return;
  setAttrBit(2,ATTR2_PALETTE(palette&15),true,ATTR2_PALETTE(15));
}

bool Sprite::translucent() const
{ 
  if (!valid()) return false;
  return s_sprites[m_screen][m_index].attribute[0]&ATTR0_TYPE_BLENDED;
}

void Sprite::translucent(bool b)
{ 
  if (!valid()) return;
  setAttrBit(0,ATTR0_TYPE_BLENDED,b);
}

bool Sprite::window() const
{ 
  if (!valid()) return false;
  return s_sprites[m_screen][m_index].attribute[0]&ATTR0_TYPE_WINDOWED;
}

void Sprite::window(bool b)
{ 
  if (!valid()) return;
  setAttrBit(0,ATTR0_TYPE_WINDOWED,b);
}


void Sprite::getAffine( 
    signed short & pa,
    signed short & pb,
    signed short & pc,
    signed short & pd) const
{
  if (!valid()) return;
  pa = s_affine[m_index].hdx;
  pb = s_affine[m_index].hdy;
  pc = s_affine[m_index].vdx;
  pd = s_affine[m_index].vdy;
}
void Sprite::setAffine( 
    signed short pa,
    signed short pb,
    signed short pc,
    signed short pd)
{
  if (!valid()) return;
   s_affine[m_index].hdx = pa;
   s_affine[m_index].hdy = pb;
   s_affine[m_index].vdx = pc;
   s_affine[m_index].vdy = pd;
}

void Sprite::update() const
{
  if (!valid()) return;
  SpriteEntry * oam = (SpriteEntry*)m_OAM;
  //DC_FlushRange(&s_sprites[m_screen][m_index],sizeof(SpriteEntry));
  dmaCopy(&s_sprites[m_screen][m_index], &oam[m_index], sizeof(SpriteEntry));
}

void Sprite::initialise(bool sync)
{
  if (!valid())
  {
    for (int i = 0; i < 128; i++)
    {
      if (!m_used[i]) {
        m_index = i;
        m_used[i] = true;
        s_sprites[m_screen][m_index].attribute[0] = 0;
        s_sprites[m_screen][m_index].attribute[1] = 0;
        s_sprites[m_screen][m_index].attribute[2] = 0;
        if (sync)
          update();
        break;
      }
    }
  }
}

void Sprite::copyOAM(int screen)
{
  if (screen == 0 || screen == 2) {
    //DC_FlushRange(s_sprites[0],128*sizeof(SpriteEntry));
    dmaCopy(s_sprites[0], OAM, 128 * sizeof(SpriteEntry));
    /*pSpriteEntry oam = (pSpriteEntry)OAM;
    for (int i = 0; i < 128; i++) {
      oam[i] = s_sprites[m_screen][i];
    }*/
  }
  if (screen == 1 || screen == 2) {
    //DC_FlushRange(s_sprites[1],128*sizeof(SpriteEntry));
    dmaCopy(s_sprites[1], OAM_SUB, 128 * sizeof(SpriteEntry));
    /*pSpriteEntry oam = (pSpriteEntry)OAM_SUB;
    for (int i = 0; i < 128; i++) {
      oam[i] = s_sprites[m_screen][i];
    }*/
  }
}

unsigned short * Sprite::oamData() const
{
  //unsigned int thisTile = this->tile();
  /*if (this->color() == 256) {
    thisTile /= 2;
  }*/
  /*uint16 * OAMData = SPRITE_GFX;
  if (m_screen) {
    OAMData = SPRITE_GFX_SUB;
  }
  return &OAMData[thisTile*16];
  */
  return 0;
}

// real worker funcions
void Sprite::loadTileData(const void * tileData, unsigned int length)
{
  uint16 * OAMData = this->oamData();
  dmaCopy(tileData, OAMData, length);
}
void Sprite::loadTileMapData(const void * t, const void * m, unsigned int length)
{
  // load data from tile data referencing the map - "emulates" the tile data in the bg
  const unsigned short* gfx = (const unsigned short*)t;
  const unsigned short* map = (const unsigned short*)m;
  unsigned int thisTile = this->tile();
  if (this->color() == 256) {
    thisTile /= 2;
  }
  thisTile *= 64;
  uint16 * OAMData = 0; //SPRITE_GFX;
  if (m_screen) {
    OAMData = 0;//SPRITE_GFX_SUB;
  }
  
  // get start address in screen
  for (unsigned int loop = 0; loop < length; loop++) {
    // id = <the tile id in the map file at this position>
    int id = map[loop];
    dmaCopy(&(gfx[id*16]), &OAMData[loop*16+thisTile],32);
  }
}

void Sprite::disableAll(int screen)
{
  int start(screen==1?1:0);
  SpriteEntry * sprite = s_sprites[start];
  int end(screen==2?256:128);
  for (int i = 0; i < end; ++i)
    sprite[i].attribute[0] = ATTR0_DISABLED;
  copyOAM(screen);
}

unsigned short Sprite::attribute(int i) const
{
  return s_sprites[m_screen][m_index].attribute[i&3];
}
void Sprite::render()
{
}
bool Sprite::operator<(const Sprite & other) const
{
  return other.m_index > m_index;
}
bool Sprite::operator>(const Sprite & other) const
{
  return not operator<(other);
}
