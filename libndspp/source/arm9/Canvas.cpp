#include <string.h>
#include "nds.h"
#include "Canvas.h"
#include "Background.h"
#include "Video.h"

using namespace nds;

static const int BUFFER_SIZE = 256*256;

Canvas & Canvas::instance()
{
  static Canvas s_canvas;
  return s_canvas;
}

Canvas::~Canvas(){}

Canvas::Canvas():
  m_bgMain(new Background(0,3,0,31)),
  m_bgSub(new Background(1,3,0,31)),
  m_frontMain((unsigned short*)BG_BMP_RAM(0)),
  m_backMain( m_frontMain + BUFFER_SIZE ),
  m_frontSub((unsigned short*)BG_BMP_RAM_SUB(0)),
  m_backSub((unsigned short*)BG_BMP_RAM(16))
{
  // set up background.
  Video & main(Video::instance());
  Video & sub(Video::instance(1));
  sub.setMode(5);
  main.setMode(5);

  // redo the banks - we want enough VRAM for the main and sub screen
  // Can't use hardware double buffering on sub screen, have to fudge it.
  vramSetMainBanks( VRAM_A_MAIN_BG_0x06000000,
                    VRAM_B_MAIN_BG_0x06020000,
                    VRAM_C_SUB_BG,
                    VRAM_D_MAIN_BG_0x06040000);
  /*
  vramSetBankI(VRAM_I_SUB_SPRITE);
  vramSetBankF(VRAM_F_MAIN_SPRITE);
  vramSetBankG(VRAM_G_MAIN_SPRITE);
  */

  m_bgMain->enable();
  m_bgSub->enable();

  m_bgMain->update();
  m_bgSub->update();

  main.setToTop();
}

int Canvas::height() const
{
  // from nds/arm9/video.h
  return SCREEN_HEIGHT*2;
}
int Canvas::width() const
{
  // from nds/arm9/video.h
  return SCREEN_WIDTH;
}

void Canvas::drawPixel(int x, int y, int colour)
{
  if (x >= SCREEN_WIDTH or y >= (2*SCREEN_HEIGHT)) {
    return;
  }
  //uint16 * gfx( (y < 192) ? m_backMain:m_backSub );
  uint16 * gfx( (y < 192) ? m_frontMain:m_frontSub );
  if (y >= 192)
    y -= 192;
  gfx[x+y*256] = colour|BIT(15);
}

void Canvas::fillRectangle(int x, int y, int w, int h, int colour)
{
  for (int j = 0; j < h; j++) {
    horizontalLine(x, y+j, w, colour);
  }
}

void Canvas::verticalLine(int x, int y, int length, int colour)
{
  for (int i = 0 ; i < length; ++i)
  {
    drawPixel(x, y+i, colour);
  }
}

void Canvas::horizontalLine(int x, int y, int length, int colour)
{
  for (int i = 0 ; i < length; ++i)
  {
    drawPixel(x+i, y, colour);
  }
}

void Canvas::drawRectangle(int x, int y, int w, int h, int colour)
{
  horizontalLine(x,y,w,colour);
  horizontalLine(x,y+h,w,colour);
  verticalLine(x,y,h,colour);
  verticalLine(x+w,y,h,colour);
}

void Canvas::endPaint()
{
  // swap main screen
  //unsigned short* temp = m_frontMain;
  //m_frontMain = m_backMain;
  //m_backMain = temp;
  // flip 
  // base is 16KB and screen size is 256x256x2 (128KB)
  //BG3_CR ^= BG_BMP_BASE( 128 / 16 );

  // copy sub screen - buffer size * 2 since is 16 bit
  //memcpy(m_frontMain, m_backMain, SCREEN_WIDTH*SCREEN_HEIGHT*2);
  //memcpy(m_frontSub, m_backSub, SCREEN_WIDTH*SCREEN_HEIGHT*2);

}
