#include "libnds.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "SDLhandler.h"
#include "Background.h"
#include "Sprite.h"
#include "HardwareHandler.cpp"
#include "BackgroundHandler.h"
#include "SpriteHandler.h"
#include "Keys.h"
#include "Video.h"

using namespace std;
const int SDLhandler::WIDTH(32*8);
const int SDLhandler::HEIGHT(2*24*8);
SDL_Rect SDLhandler::GAP = { 0, 192, SDLhandler::WIDTH, 16};

SDLhandler::SDLhandler():
    m_screen(0),
    m_vblank(0),
    m_scale(1),
    m_frames(0),
    m_isFullScreen(false),
    m_hasSound(true),
    m_vramMain(0),
    m_vramSub(0),
    m_mainOnTop(true),
    m_fn(0),
    m_fadeLevel(0),
    m_fadeLevelMain(0),
    m_fadeLevelSub(0),
    m_whiteLevel(0),
    m_whiteLevelMain(0),
    m_whiteLevelSub(0)
{
  init();
  m_vramMain = new unsigned short[0x16000];
  m_vramSub = new unsigned short[0x16000];
  m_spriteGfx = new unsigned short[0x2000];
  m_subSpriteGfx = new unsigned short[0x2000];
  for (int i = 0; i < 256; ++i)
  {
    m_backgroundPalette[i] = 0;
    m_subBackgroundPalette[i] = 0;
    m_spritePalette[i] = 0;
    m_subSpritePalette[i] = 0;
  }
  ::memset(m_vramMain, 0, 0x16000*2);
  ::memset(m_vramSub, 0, 0x16000*2);
  ::memset(m_spriteGfx, 0, sizeof(m_spriteGfx));
  ::memset(m_subSpriteGfx, 0, sizeof(m_subSpriteGfx));
  drawGap();
}

SDLhandler::~SDLhandler()
{
  if (m_vblank) {
    SDL_RemoveTimer(m_vblank);
  }
}

Uint32 SDLhandler::vblank(Uint32 interval, void* param) {
  SDLhandler * self((SDLhandler*)param);
  self->m_frames++;
  return interval;
}

int SDLhandler::frame() const
{
  return m_frames;
}

void SDLhandler::enableVblank(VoidFunctionPointer fn)
{
  // runs at 30 frames... hmm..
  if (!m_vblank) {
    m_vblank =  SDL_AddTimer((10/10)*10, vblank, this);
  }
  m_fn = fn;
  waitVsync();
}

int SDLhandler::totalHeight()
{
   return HEIGHT+GAP.h;
}

int SDLhandler::init()
{
  char msg[256];
  int initFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
  if (m_hasSound) {
    initFlags |= SDL_INIT_AUDIO;
  }

  if (SDL_Init(initFlags) < 0)
  {
    sprintf (msg, "Couldn't initialize SDL: %s\n", SDL_GetError ());
    printf(msg);
    return 1;
  }

  atexit (SDL_Quit);
  int flags = SDL_SWSURFACE;
  if (m_isFullScreen) {
    flags |= SDL_FULLSCREEN;
  } else {
    flags |= SDL_RESIZABLE;
  }
  m_screen = SDL_SetVideoMode (m_scale*WIDTH,m_scale*totalHeight(), 16, flags);
  if (m_screen == NULL)
  {
    sprintf (msg, "Couldn't set 16 bit video mode: %s\n",
        SDL_GetError ());
    printf(msg);
    return 2;
  }
  SDL_WM_SetCaption ("SDL Application", NULL);

  /*
  if (m_hasSound) {
    if (initSound() == 0) {
      sprintf (msg, "Couldn't start sound: %s\n",
          SDL_GetError ());
      printf(msg);
    }
  }
  */


  return 0;
}

bool SDLhandler::sdlEvent() const
{
  SDL_Event event;
  
  /* Check for events */
  while (SDL_PollEvent (&event))
  {
    switch (event.type)
    {
      case SDL_QUIT:
        return 1;
        break;
      default:
        break;
    }
  }
  return 0;
}

int SDLhandler::initSound(int freq, int format)
{
  if (not m_hasSound) {
    return true;
  }
  SDL_AudioSpec fmt;

  /* Set 16-bit stereo audio at 16Khz */
  fmt.freq = freq;
  fmt.format = format;
  fmt.channels = 1;
  fmt.samples = 512;        /* A good value for games */
  fmt.callback = mixaudio;
  fmt.userdata = NULL;

  bool soundOK(false);
  /* Open the audio device and start playing sound! */
  if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
    soundOK = false;
    char msg[256];
    sprintf (msg, "Couldn't start sound: %s\n",
        SDL_GetError ());
    printf(msg);
  }
  soundOK = true;
  SDL_PauseAudio(0);
  return soundOK;
}
unsigned short * SDLhandler::backgroundPaletteMem()
{
  return m_backgroundPalette;
}
unsigned short * SDLhandler::subBackgroundPaletteMem()
{
  return m_subBackgroundPalette;
}
unsigned short * SDLhandler::spritePaletteMem()
{
  return m_spritePalette;
}
unsigned short * SDLhandler::subSpritePaletteMem()
{
  return m_subSpritePalette;
}

static const int NUM_SOUNDS(2);
struct sample {
    const unsigned char *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

void SDLhandler::mixaudio(void *unused, Uint8 *stream, int len)
{
    int i;
    Sint32 amount;

    for ( i=0; i<NUM_SOUNDS; ++i ) {
        amount = (sounds[i].dlen-sounds[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
        SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
        sounds[i].dpos += amount;
    }
}

void SDLhandler::playSound(const void * data, int length)
{
  int useId = -1;
  for ( int index = 0; index<NUM_SOUNDS; ++index ) {
    if ( sounds[index].dpos == sounds[index].dlen ) {
      useId = index;
    }
  }
  if ( useId == -1 )
    return;

  SDL_LockAudio();
  sounds[useId].data = (const unsigned char*)data;
  sounds[useId].dlen = length;
  sounds[useId].dpos = 0;
  SDL_UnlockAudio();
}


SDLhandler & SDLhandler::instance()
{
  static SDLhandler s_instance;
  return s_instance;
}

Uint32 SDLhandler::decodeColor(unsigned short rgb16)
{
  int red = (rgb16&0x1f)*8;
  int blue = ((rgb16>>5)&0x1f)*8;
  int green = ((rgb16>>10)&0x1f)*8;

  if (m_fadeLevel) {
    float fred = red;
    float fgreen = green;
    float fblue = blue;
    fred /= (float)m_fadeLevel;
    fgreen /= (float)m_fadeLevel;
    fblue /= (float)m_fadeLevel;
    red = (int)fred;
    green = (int)fgreen;
    blue = (int)fblue;
    if (m_fadeLevel == 16) {
      red = green = blue = 0;
    }
  }
  if (m_whiteLevel) {
    // if white = 16, then all are 256
    // if white = 8, then c = (256 - n)
    // if white = 0, then all are normal color
    red = (int)(m_whiteLevel * (256/16));
    green = (int)(m_whiteLevel * (256/16));
    blue = (int)(m_whiteLevel * (256/16));
  }
  return SDL_MapRGB(m_screen->format, red, blue, green);
}

/*
void SDLhandler::setPaletteData(const unsigned short * palette, int size)
{
  if (size >= 256)
    size = 256;
  for (int i = 0; i < size; ++i)
  {
    m_palette[i] = decodeColor(*palette++);
  }

  SDL_Rect rect;
  rect.w = WIDTH*m_scale;
  rect.h = totalHeight()*m_scale;
  rect.x = 0;
  rect.y = 0;
  if ( SDL_MUSTLOCK(m_screen) ) 
    SDL_LockSurface(m_screen);
  SDL_FillRect (m_screen, &rect,m_palette[0]);
  if ( SDL_MUSTLOCK(m_screen) ) 
    SDL_UnlockSurface(m_screen);
  drawGap();
}
*/

void SDLhandler::drawGap() {
  int fadeLevel = m_fadeLevel;
  m_fadeLevel = 0;
  SDL_FillRect (m_screen, &GAP, decodeColor( (20|(20<<5)|(20<<10)) ) );
  m_fadeLevel = fadeLevel;
}

void SDLhandler::clear()
{

  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = WIDTH;
  rect.h = HEIGHT/2;
  int colour(m_backgroundPaletteSDL[0]);
  if (not m_mainOnTop) {
    colour = m_subBackgroundPaletteSDL[0];
  }
  if (Video::instance(0).mode() != 5) {
    SDL_FillRect (m_screen, &rect, colour);
  }
  else
  {
    u16 * vram = vramMain(0);
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
      for (int y = 0; y < SCREEN_HEIGHT; ++y)
      {
        drawPixel(x, y, 0, vram[x+y*SCREEN_WIDTH]);
      }
    }
  }
  if (m_mainOnTop) {
    colour = m_subBackgroundPaletteSDL[0];
  } else {
    colour = m_backgroundPaletteSDL[0];
  }
  rect.x = 0;
  rect.y = GAP.y+GAP.h;
  rect.w = WIDTH;
  rect.h = HEIGHT/2;
  if (Video::instance(1).mode() != 5) {
    SDL_FillRect (m_screen, &rect, colour);
  }
  else
  {
    u16 * vram = vramSub(0);
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
      for (int x = 0; x < SCREEN_WIDTH; ++x)
      {
        drawPixel(x, y, 1, vram[x+y*SCREEN_WIDTH]);
      }
    }
  }

  drawGap();
}

void SDLhandler::drawPixel(int x, int y, unsigned int layer, unsigned int palette)
{
  if (layer == 0 or layer == 2) {
    if (not m_mainOnTop)
    {
      y += 192;
    }
  } 
  else if (layer == 1 or layer == 3)
  {
    if (m_mainOnTop)
    {
      y += 192;
    }
  }
  //int origY = y;
  if (y >= GAP.y)
  {
    y += GAP.h;
  }

  SDL_Rect rect;
  rect.w = 1*m_scale;
  rect.h = 1*m_scale;
  rect.x = x*m_scale;
  rect.y = y*m_scale;
  Uint32 colour = 0;
  
  bool usePalette(true);
  if (layer == 0 or layer == 1) {
    if (nds::Video::instance(layer).mode() == 5) {
      colour = decodeColor(palette);
      usePalette = false;
    }
  }
  if (usePalette) {
    switch (layer) {
      case 1:
        colour = m_subBackgroundPaletteSDL[palette];
        break;
      case 2:
        colour = m_spritePaletteSDL[palette];
        break;
      case 3:
        colour = m_subSpritePaletteSDL[palette];
        break;

      default:
        colour = m_backgroundPaletteSDL[palette];
        break;
    }
  }
#if 0
  if (layer == 2 )
    printf("Fill @%d,%d pal %d col %x lay %d\n", rect.x, origY,palette, colour, layer);
#endif
  SDL_FillRect (m_screen, &rect, colour);
}

#if 0
void SDLhandler::loadPalette(const std::string & fileName)
{
  ifstream palFile;
  palFile.open(fileName.c_str(), ios::in);
  // read the lot
  if (palFile.is_open())
  {
    palFile.seekg(0, ios::end);
    int size = palFile.tellg();
    char * data = new char[size+2];
    palFile.seekg(0, ios::beg);
    palFile.read(data, size);
    data[size] = 0;
    setPaletteData((unsigned short*)data, size/2);
    delete [] data;
  }
}
#endif
unsigned short * SDLhandler::vramMain(int offset)
{
  return &m_vramMain[offset];
}

bool SDLhandler::inGap(int y) const
{
  return (y >= GAP.y and y < (GAP.y+GAP.h));
}

unsigned short * SDLhandler::vramSub(int offset) {
  return &m_vramSub[offset];
}
void SDLhandler::waitVsync()
{
  int tmpGF = m_frames;

  // pallete conversions:
  for (int i = 0; i < 256; i++)
  {
    m_fadeLevel = m_fadeLevelMain;
    m_whiteLevel = m_whiteLevelMain;
    m_backgroundPaletteSDL[i] = decodeColor(m_backgroundPalette[i]);
    m_spritePaletteSDL[i] = decodeColor(m_spritePalette[i]);

    m_fadeLevel = m_fadeLevelSub;
    m_whiteLevel = m_whiteLevelSub;
    m_subBackgroundPaletteSDL[i] = decodeColor(m_subBackgroundPalette[i]);
    m_subSpritePaletteSDL[i] = decodeColor(m_subSpritePalette[i]);
#if 0
    if (m_backgroundPaletteSDL[i] or
        m_subBackgroundPaletteSDL[i] or
        m_spritePaletteSDL[i] or
        m_subSpritePaletteSDL[i])
    {
      printf("%3d: %08x %08x %08x %08x\n", i, m_backgroundPaletteSDL[i],
          m_subBackgroundPaletteSDL[i],
          m_spritePaletteSDL[i],
          m_subSpritePaletteSDL[i]);
    }
#endif
  }

  clear();
  BackgroundHandler::render();
  SpriteHandler::render();
  
  SDL_Flip(m_screen);
  SDL_Event event;
  while( SDL_PollEvent( &event ) ) {
    switch( event.type ) {
      case SDL_QUIT:
        exit(0);
        break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        // offset the y position
        if (event.button.y > 191) {
          if (inGap(event.button.y)) {
            break;
          }
          event.button.y -= GAP.h;
          event.button.y -= 192;
          Keys::instance().handleMouseEvent(event.button);
        }
        break;

      case SDL_KEYUP:
      case SDL_KEYDOWN:
        Keys::instance().handleKeyEvent(event.key);
        break;
    }
  }
  while (tmpGF == m_frames) {
    SDL_Delay(5);
  }
  if (m_fn)
    m_fn();
}

void SDLhandler::setFade(int screen, int level)
{
  if (level > 16)
    level = 16;
  if (level < 0)
    level = 0;
  if (screen)
    m_fadeLevelSub = level;
  else 
    m_fadeLevelMain = level;
}
void SDLhandler::setWhite(int screen, int level)
{
  if (level > 16)
    level = 16;
  if (level < 0)
    level = 0;
  if (screen)
    m_whiteLevelSub = level;
  else 
    m_whiteLevelMain = level;
}

void SDLhandler::lcdSwap()
{
  m_mainOnTop = not m_mainOnTop;
}
void SDLhandler::mainOnTop()
{
  m_mainOnTop = true;
}
void SDLhandler::mainOnBottom()
{
  m_mainOnTop = false;
}

unsigned short * SDLhandler::spriteGfx()
{
  return m_spriteGfx;
}

unsigned short * SDLhandler::subSpriteGfx()
{
  return m_subSpriteGfx;
}

