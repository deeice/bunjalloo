
#include <nds.h>
#include <stdio.h>
#include "ndspp.h"
#include "Arena.h"
#include "ChaosData.h"
#include "Graphics.h"


// external data
extern const unsigned short _binary_bg_raw_start[];

// static defines
static const int ARENA_SCREEN(0);
static const int ARENA_BORDER_1(2);
static const int ARENA_BORDER_2(1);
static const int ARENA_CORNER_TILE(601);
static const int ARENA_VERT_EDGE_TILE(602);
static const int ARENA_HORZ_EDGE_TILE(603);
static const int BG_SOLID_TILE(604);

static const int X_LIMIT(31);
static const int Y_LIMIT(23);
const int Arena::WIZARD_INDEX(0x2A);

// namespace usage
using namespace nds;

Arena::Arena():m_bg(new Background(ARENA_SCREEN,0,0,28)),m_playerCount(0)
{
  this->reset();
  m_bg->enable();
}
Arena::~Arena()
{
  delete m_bg;
}

Arena & Arena::instance()
{
  static Arena s_instance;
  return s_instance;
}

void Arena::reset()
{
  unsigned short * mapData = m_bg->mapData();
  for (int i=1, y=2; y < 22; y++) {
    for (int x=2; x < 32; x++) {
      mapData[x+y*32] = i++;
    }
  }
}

void Arena::clear()
{
  for (int x = 0; x < 15; x++){
    for (int y = 0; y < 10; y++){
      this->clearSquare(x,y);
    }
  }
}

void Arena::clearSquare(int x, int y) {
  // clears a square in the arena
  unsigned int startOAM = 16+x*2*16+2*y*30*16;
  // FIXME: use DMA
  u16 * tileData = m_bg->tileData();
  
  for (int i = 0; i < 32; i++)
  {
    tileData[startOAM+i] = 0;
  }
  startOAM = 16+x*2*16+(2*y+1)*30*16; // x*2*16+(2*y+1)*30*16;
  for (int i = 0; i < 32; i++)
  {
    tileData[startOAM+i] = 0;
  }
  /*
  REG_DMA3SAD = (u32)&zero; // 16 bit reads into VRAM
  REG_DMA3DAD = (u32)&bg_level.tileData[startOAM]; // 16 bit reads into VRAM
  REG_DMA3CNT = 32 |DMA_SOURCE_FIXED|DMA_16NOW; // 16 bit transfers from gfx array via DMA  
  
  REG_DMA3SAD = (u32)&zero; // 16 bit reads into VRAM
  REG_DMA3DAD = (u32)&bg_level.tileData[startOAM]; // 16 bit reads into VRAM
  REG_DMA3CNT = 32 |DMA_SOURCE_FIXED|DMA_16NOW; // 16 bit transfers from gfx array via DMA  
  */
}

void Arena::drawGfx8(
    const unsigned short * gfx, 
    const unsigned short * map, 
    int x, int y, int frame) {
  // draws graphics on the 8 by 8 tile boundaries
  // ie, to draw on 16*16 tiles, multiply x, y by 2
  int index = frame*4; // 4 tiles for each 16*16 gfx 
  unsigned short * tileData = m_bg->tileData();
  // each 8*8 tile takes up 32 bytes.
  unsigned int startOAM = 16+x*16+y*30*16;
  // get start address in screen
  for (int loop = 0; loop < 2; loop++) {
    // id = <the tile id in the map file at this position>
    int id = map[index++];
    swiCopy(&(gfx[id*16]), &tileData[loop*16+startOAM],16);
  }
  startOAM = 16+x*16+(y+1)*30*16 - 32;
  for (int loop = 2; loop < 4; loop++) {
    // id = <the tile id in the map file at this position>
    int id = map[index++];
    swiCopy(&(gfx[id*16]), &tileData[loop*16+startOAM],16);
  }
}

void Arena::countdownAnim()
{
  // counts down the arena[1] values and updates the arena 2 val if needed
  int i;   
  // FIXME - hardcoded...
  for (i = 0; i < 0x9f; i++) {
    if (m_arena[0][i] >= 2) {
      if ( (--m_arena[1][i]) == 0) {
        m_arena[2][i]++;
        if (m_arena[2][i] == 4) {
          m_arena[2][i] = 0;
        }
        if (m_arena[2][i] == 5) {
          m_arena[2][i]--;
        } 

        // each spell has a "speed" count
        m_arena[1][i] = anim_speed_data[m_arena[0][i]-2];
      }
    }
  }

}



void Arena::decorativeBorder(int pal, unsigned short col1,  unsigned short col2) {
  
  // set the colours
  Palette palette(ARENA_SCREEN,pal);
  palette[ARENA_BORDER_1] = col1;
  palette[ARENA_BORDER_2] = col2;
  u16 * tileData = m_bg->tileData();
  u16 * mapData = m_bg->mapData();
  // And these magic numbers??
  // board size = 10*15, == 20*30 tiles
  // each tile is 32 bytes (8*8 pixels, 4 bits per pixel)
  // 20*30*32 = 19200, but have to write in u16, so divide by 2 
  dmaCopy(_binary_bg_raw_start, &tileData[16 + (19200/2)], 128);

  // top left corner
  mapData[0] = ARENA_CORNER_TILE|(pal<<12);
  // top right corner
  mapData[X_LIMIT] = ARENA_CORNER_TILE|TILE_FLIP_HORZ|(pal<<12);
  // bottom left corner
  mapData[Y_LIMIT*32] = ARENA_CORNER_TILE|TILE_FLIP_VERT|(pal<<12);
  // bottom right corner
  mapData[X_LIMIT+Y_LIMIT*32] = ARENA_CORNER_TILE|TILE_FLIP_HORZ|TILE_FLIP_VERT|(pal<<12);
  // now fill the sides
  // top edge
  for (int x=1; x < X_LIMIT; x++) {
    mapData[x] = ARENA_HORZ_EDGE_TILE|(pal<<12);
  }
  
  // bottom edge
  for (int x=1; x < X_LIMIT; x++) {
    mapData[x+Y_LIMIT*32] = ARENA_HORZ_EDGE_TILE|TILE_FLIP_VERT|(pal<<12);
    
  }
  
  // left edge
  for (int y=1; y < Y_LIMIT; y++) {
    mapData[y*32] = ARENA_VERT_EDGE_TILE|(pal<<12);
  }
  
  // right edge
  for (int y=1; y < Y_LIMIT; y++) {
    mapData[X_LIMIT+y*32] = ARENA_VERT_EDGE_TILE|TILE_FLIP_HORZ|(pal<<12);
  }
  m_bg->xScroll(0);
  m_bg->yScroll(0);
  m_bg->update();
}
int Arena::players() const
{
  return m_playerCount;
}
void Arena::players(int p)
{
  m_playerCount = p;
}
int Arena::currentPlayer() const
{
  return m_currentPlayer;
}
void Arena::currentPlayer(int p)
{
  m_currentPlayer = p;
}



void Arena::setPalette8(int x, int y, int palette)
{
  // 8 bit resolution, 4 square size setting...
  // mask out the old palette and set the current one
  // get the tile ids for the 4 gba tiles at the ARENA x,y
  u16 tile[4];
  // 66 comes from the fact that there are 2 rows before hand (32 *2) and 2 blocks
  // at the start of each row
  // x = 0 = 132
  tile[0] = 66+x+y*32;    tile[1] = 66+1+x+y*32;
  tile[2] = tile[0]+32;   tile[3] = tile[1]+32;
  
  u16 * mapData = m_bg->mapData();
  mapData[tile[0]] &= ~0xF000; mapData[tile[0]] |= (palette<<12)&0xF000;
  mapData[tile[1]] &= ~0xF000; mapData[tile[1]] |= (palette<<12)&0xF000;
  mapData[tile[2]] &= ~0xF000; mapData[tile[2]] |= (palette<<12)&0xF000;
  mapData[tile[3]] &= ~0xF000; mapData[tile[3]] |= (palette<<12)&0xF000;
}

void Arena::setWizardSquare(int square, int id)
{
  m_arena[0][square] = id+WIZARD_INDEX;
  m_arena[3][square] = id;
}

