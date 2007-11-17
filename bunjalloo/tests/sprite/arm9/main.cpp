#include "nds.h"
#include <vector>
#include "libnds.h"
#include "Canvas.h"
#include "Video.h"
#include "File.h"
#include "Image.h"
#include "Sprite.h"
#include "Palette.h"
static const int screen = 1;

void initConsole()
{
  // set the mode for 2 text layers and two extended background layers
  videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE|DISPLAY_SPR_1D);

  // set the sub background up for text display (we could just print to one
  // of the main display text backgrounds just as easily
  videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE|DISPLAY_SPR_ACTIVE|DISPLAY_SPR_1D); //sub bg 0 will be used to print text

  // set the first bank as background memory and the third as sub background memory
  // B and D are not used (if you want a bitmap greater than 256x256 you will need more
  // memory so another vram bank must be used and mapped consecutivly
  vramSetMainBanks(	VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_SPRITE,
      VRAM_C_SUB_BG , VRAM_D_LCD);

  // set up text background for text
  SUB_BG0_CR = BG_MAP_BASE(31);

  BG_PALETTE_SUB[255] = RGB15(31,31,31);//by default font will be rendered with color 255

  //consoleInit() is a lot more flexible but this gets you up and running quick
  consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);
}

static void loadImage(Image & image, unsigned short * oamData)
{
  //sprite.setAffine(256, 0, 0, 256);
  //sprite.rotateScale(0);
  nds::ObjectPalette p(screen);
  p.load(image.palette(), image.paletteSize()*2);

  const unsigned short * data = image.data();
  // need to load in 16x16 tile order

  // 0  1  2  3  4  5  6  7
  // 8  9  10 11
  //
  // load order 0,1,4,5  2,3,6,7
  // tileIndex = tileX + tileY*tilesX
  //
  // for (metaTile = 0; metaTile < metaTileCound; metaTile++)
  //   for (y = 0; y < 2; y++)  
  //     for (x = 0; x < 2; x++)  
  //       tileIndex = metaTile*2 +  y*tilesX + x;
  //
  //
  //int tilesX = image.width() / 8;
  //int tilesY = image.height() / 8;
  int bytesPerRow = image.width();
  int tiles16x = image.width() / 16;
  int tiles16y = image.height() / 16;
  int oamIndex = 0;
  for (int metay = 0; metay < tiles16y ; metay++)
  {
    for (int metax = 0; metax < tiles16x ; metax++)
    {
      // loop over the 8 by 8 tiles
      // startx = x + metax * 2;
      // starty = y + metay * 2;
      // tileIndex = starty * tilesX + startx;
      for (int y = 0; y < 2; y++)
      {
        for (int x = 0; x < 2; x++)
        {
          // copy tile at tileX, tileY to next oam slot
          int tileY = y + metay*2;
          int tileX = x + metax*2;
          // index into oam:
          //int tileIndex = y*tilesX + meta*2 + x;
          // int tileIndex = tileY*tilesX + tileX;
          unsigned short * oamSlot = &oamData[oamIndex*8*8/2];
          oamIndex++;
          // const unsigned char * src = data + (tileIndex*8*bytesPerRow);
          const unsigned short * src = data + (tileY*8*bytesPerRow) + (tileX*8);
          for (int row = 0; row < 8; row++)
          {
            for (int col = 0; col < 4; col++, src+=2)
            {
              unsigned short pixelPair = src[0]|(src[1]<<8);
              *oamSlot++ = pixelPair;
            }
            src -= 8;
            src += bytesPerRow;
          }

        }
      }
    }
  }
}

int main(void) {

  irqInit();
  irqSet(IRQ_VBLANK, 0);
  //initConsole();
  Image image("open.png", true);
  nds::Palette p(0);
  p[0] = nds::Color(31, 31, 31);
  nds::Canvas & canvas = nds::Canvas::instance();
  nds::Sprite bigSprite(screen, 64, 32, 0, 256);
  bigSprite.x(32);
  bigSprite.y(128);
  std::vector<nds::Sprite*> sprites;
  for (int i = 0; i < 12; i++)
  {
    // would be 4 tiles per sprite (16x16 = 4*8x8) 
    // but the tile count is for 4bpp tiles, 256 color 8bpp require twice as much vram.
    nds::Sprite * sprite(new nds::Sprite(screen, 16, 16, i*8, 256));
    sprite->x(i*16);
    sprite->y(i*16);
    sprites.push_back(sprite);

  }
  if (image.isValid())
  {
    const unsigned short * data = image.data();
    if (data)
    {
      unsigned short * oamData = sprites.front()->oamData();
      loadImage(image, oamData);

      canvas.fillRectangle(0, 0, canvas.width(), canvas.height(), nds::Color(21,21,21));
      canvas.endPaint();

      int size = sprites.size();
      for ( int i = 0; i < size ; ++i)
      {
        nds::Sprite * sprite(sprites[i]);
        sprite->enable();
        sprite->update();
      }
      bigSprite.enable();
      bigSprite.update();
    }
  }
  nds::Sprite * white(sprites[0]);
  white->translucent(true);
  white->update();
  nds::Video & main(nds::Video::instance(screen));
  main.blend(nds::Video::BLDMOD_OBJECT,
      0,
      nds::Video::BLDMOD_BG3|nds::Video::BLDMOD_BG2 | nds::Video::BLDMOD_BG1 | nds::Video::BLDMOD_BG0 | nds::Video::BLDMOD_BD
      );

  int affine = 0;
  int fadeLvl = 0;
  while(1)
  {
    affine+=32;
    affine &= 0x1ff;
    // Compute sin and cos
    nds::Sprite * spinner(sprites[7]);
    spinner->rotateScale(true);
    spinner->rotate(1);
    u16 cosAng = COS[affine] / 16;
    u16 sinAng = SIN[affine] / 16;
    spinner->setAffine(cosAng, sinAng, -sinAng, cosAng);
    fadeLvl++;
    fadeLvl&=0xf;
    main.setBlendAB(fadeLvl^0xf,fadeLvl);
    for (int i = 0; i < 12; ++i)
    {
      nds::Sprite * s(sprites[i]);
      int y = s->y();
      if (y == SCREEN_HEIGHT-16)
      {
        y = 0;
      }
      else
      {
        y++;
      }
      s->y(y);
      s->update();
    }
    swiWaitForVBlank();
  }

  return 0;
}
