#include "libnds.h"
#include "Video.h"
#include "Sprite.h"
#include "SDLhandler.h"

using namespace nds;
static unsigned int SUB_DISPLAY_CR(0);
static unsigned int DISPLAY_CR(0);
static unsigned short SUB_BLEND_CR;
static unsigned short BLEND_CR;
static unsigned short BLEND_Y;
#if 0
static unsigned short SUB_BLEND_Y(0);
#endif

Video::Video(int screen):
  m_DISPCNT(screen?SUB_DISPLAY_CR:DISPLAY_CR),
  m_screen(screen)
{
  powerON(POWER_ALL_2D);
  m_DISPCNT = 0;
  clear();
  enableObjects();
  objectMapDimensions(1);
  setBanks();
}

void Video::setBanks()
{
  //FIXME - too hard coded?
  if (m_screen)
  {
    /*
    vramSetBankC(VRAM_C_SUB_BG);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    */
  } else {
    /*
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankE(VRAM_E_MAIN_SPRITE);
    */
  }
}

Video& Video::instance(int screen)
{
  static Video s_intance0(0);
  static Video s_intance1(1);
  return screen?s_intance1:s_intance0;
}

#define BLDMOD_MODE(n)    (n<<6)
void Video::blend(BLDMOD_MODE_t mode, int first, int second)
{
  int blnd((first)|((second)<<8)|(BLDMOD_MODE(mode)));
  if (m_screen)
    SUB_BLEND_CR = blnd;
  else
    BLEND_CR = blnd;
}

void Video::clear()
{
  // clear vram 
  // delegate to gl?
  /*
  unsigned int * vram = (unsigned int*)VRAM_CR;
  for (int i = 0; i < 0x3100/8; ++i) {
    *vram++ = 0;
    *vram++ = 0;
    *vram++ = 0;
    *vram++ = 0;
  }

  for (int i = 0; i < 256; ++i) {
    BG_PALETTE[i] = 0;
    BG_PALETTE_SUB[i] = 0;
    SPRITE_PALETTE[i] = 0;
    SPRITE_PALETTE_SUB[i] = 0;
  }
  */
  /* Clear all sprites */
  Sprite::disableAll(m_screen);
}

void Video::enableObjects(bool enable)
{
  /*
  m_DISPCNT &= ~DISPLAY_SPR_ACTIVE;
  m_DISPCNT |= (enable?DISPLAY_SPR_ACTIVE:0);
  */
}
void Video::fade(bool fadeout, unsigned int speed)
{
  for (int loop = 0; loop < 17; loop++) {
    for (unsigned int i = 0; i < speed; ++i) {
      swiWaitForVBlank();
    }
    if (fadeout)
      setFade(loop);
    else
      setFade(16-loop);
  }
}

bool Video::objectsEnabled() const
{
  //return (m_DISPCNT&DISPLAY_SPR_ACTIVE);
  return false;
}

void Video::setFade(int level)
{
  /*blend( BLDMOD_FADE,
         BLDMOD_BG0 | BLDMOD_BG1 | BLDMOD_BG2| BLDMOD_OBJ ,
         BLDMOD_BD );*/
  SDLhandler::instance().setFade(level);
  //BLEND_Y = level;
}

void Video::mode(unsigned int mode)
{
  if (mode > 6)
    mode = 6;
  // MODE_0_2D is 0x10000, but no need to unset then set 0x10000
  // i.e. always have to have SOME mode, even if it is just mode 0
  m_DISPCNT &= ~(1|2|3|4|5|6);
  m_DISPCNT |= MODE_0_2D|mode;
}

unsigned int Video::mode() const
{
  // could have &7, but this is clearer and does the same job
  return m_DISPCNT & (1|2|3|4|5|6);
}

void Video::objectMapDimensions(int dimensions)
{
  /*
  unsigned short save = m_DISPCNT;
  m_DISPCNT &= ~DISPLAY_SPR_1D_LAYOUT;
  if (dimensions==1) {
    m_DISPCNT |= DISPLAY_SPR_1D_LAYOUT;
  }
  else if (dimensions != 2) {
    m_DISPCNT = save;
  }
  */
}
int Video::objectMapDimensions() const
{
  //return (m_DISPCNT & DISPLAY_SPR_1D_LAYOUT)?1:2;
  return 1;
}

void Video::setWhite(int level)
{
  if (level > 16)
    level = 16;
  if (level < 0)
    level = 0;
  blend( BLDMOD_WHITE,
        BLDMOD_BG2 | BLDMOD_BG1 | BLDMOD_BG0 | BLDMOD_BD | BLDMOD_OBJ,
         0 );
  BLEND_Y = level;
}

void Video::whiteout(bool towhite, unsigned int speed)
{
  for (int loop = 0; loop < 17; loop++) {
    for (unsigned int i = 0; i < speed; ++i) {
      swiWaitForVBlank();
    }
    if (towhite)
      setWhite(loop);
    else
      setWhite(16-loop);
  }
}

void Video::threeD(bool td)
{
  if (td) {
    // textures? should enable a vram bank for textures too
    powerON(POWER_ALL);
    //m_DISPCNT |= ENABLE_3D;
  } else {
    /*
    powerOFF(POWER_3D_CORE|POWER_MATRIX);
    m_DISPCNT &= ~ENABLE_3D;
    */
  }
}

bool Video::threeD() const
{
  //return (m_DISPCNT & ENABLE_3D);
  return true;
}

void Video::capture() const
{
#if 0
  // can only capture on video 0 (main screen)
  if (m_screen)
    return;
  
  // allocate write bank to LCD.
  vramSetBankB(VRAM_B_LCD);
  const int mode   = 0;  // what are the sources?
  const int source = threeD()?1:0;
  const int dst    = 1;           // A = 0, B = 1, C = 2, D = 3
  DISP_CAPTURE = 
    DCAP_ENABLE      |  // Enable/Busy flag
    DCAP_MODE(mode)  |  // Capture mode    (0=Source A, 1=Source B, 2/3=Sources A+B blended)
    DCAP_DST(0)      |  // VRAM Write Offset (0=00000h, 0=08000h, 0=10000h, 0=18000h)
    DCAP_SRC(source) |  // Source A          (0=Graphics Screen BG+3D+OBJ, 1=3D Screen
    DCAP_SIZE(3)     |  // Capture Size      (0=128x128, 1=256x64, 2=256x128, 3=256x192 dots)
    DCAP_OFFSET(0)   |  // VRAM Read Offset  (0=00000h, 0=08000h, 0=10000h, 0=18000h)
    DCAP_BANK(dst)   |  // VRAM Write Block  (0..3 = VRAM A..D) (VRAM must be allocated to LCDC)
    DCAP_A(16)       |  // Bland ignored if capture mode is 0 or 1
    DCAP_B(0);
  int loops = 4;
  while (DISP_CAPTURE & DCAP_ENABLE) {
    swiWaitForVBlank();
    // shouldn't block if the emulator is broken...
    if ((--loops)==0)
      break;
  }
  // and finally set to bg
  //vramSetBankC(VRAM_C_SUB_BG);
  // where does DISP_MMEM_FIFO come into this?
#endif
}

void Video::swapScreens()
{
  SDLhandler::instance().lcdSwap();
}

void Video::setToTop()
{
  if (m_screen == 0) {
    // we are the main screen
    // lcdMainOnTop();
    SDLhandler::instance().mainOnTop();
  } else {
    // we are the sub screen
    SDLhandler::instance().mainOnBottom();
  }
}

void Video::setToBottom()
{
  if (m_screen) {
    // we are the sub screen - so make the main one go to the top.
    SDLhandler::instance().mainOnTop();
  } else {
    // we are the main screen.
    SDLhandler::instance().mainOnBottom();
  }
}
