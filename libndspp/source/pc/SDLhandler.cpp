/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "libnds.h"
#include <GL/gl.h>
#include <GL/glu.h>
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
SDL_Rect SDLhandler::GAP = { 0, 192, SDLhandler::WIDTH, 0};

SDLhandler::SDLhandler():
    m_screen(0),
    m_alpha(255),
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
    m_whiteLevelSub(0),
    m_threeD(false)
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

void SDLhandler::initGL( )
{
  int h = totalHeight();
  int y = 0;
  if (m_threeD)
  {
    if (!m_mainOnTop)
    {
      y = 192 + GAP.h;
    }
    h = 192;
  }
  glViewport( 0, y, WIDTH, h );
  /*
  GLfloat ratio = ( GLfloat )256. / ( GLfloat )192.*2;
  gluPerspective( 70.0f, ratio, 0.1f, 100.0f );
  */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  glOrtho(0, WIDTH, y+h, y, -1.0, 1.0);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
}
int SDLhandler::init()
{
  char *gap(getenv("NDS_GAP"));
  if (gap)
  {
    int h = strtol(gap, 0, 0);
    if (h > 0)
    {
      GAP.h = h;
    }
  }

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
  int flags = SDL_SWSURFACE | SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
  if (m_isFullScreen) {
    flags |= SDL_FULLSCREEN;
  } else {
    flags |= SDL_RESIZABLE;
  }
  m_screen = SDL_SetVideoMode (m_scale*WIDTH,m_scale*totalHeight(), 32, flags);
  if (m_screen == NULL)
  {
    sprintf (msg, "Couldn't set 16 bit video mode: %s\n",
        SDL_GetError ());
    printf(msg);
    return 2;
  }

  for (int i = 0; i < 4; ++i)
  {
    m_layer[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, m_scale*WIDTH, m_scale*totalHeight(),
        16, 0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F);
    //SDL_SetAlpha(m_layer[i], SDL_SRCALPHA|SDL_RLEACCEL, 0);
    if (m_layer[i] == NULL)
    {
      sprintf (msg, "Couldn't create 16 bit video surface: %s\n",
          SDL_GetError ());
      printf(msg);
      return 2;
    }
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

  initGL();
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
  setDirty();
  return m_backgroundPalette;
}
unsigned short * SDLhandler::subBackgroundPaletteMem()
{
  setDirty();
  return m_subBackgroundPalette;
}
unsigned short * SDLhandler::spritePaletteMem()
{
  setDirty();
  return m_spritePalette;
}
unsigned short * SDLhandler::subSpritePaletteMem()
{
  setDirty();
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
  return SDL_MapRGBA(m_layer[0]->format, red, blue, green, m_alpha);
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
  /*
  glColor3b(200, 200, 200);
  glBegin(GL_QUADS);
    glVertex3f(-1.0f,0.2f,0.0f);
    glVertex3f(1.0f,0.2f,0.0f);
    glVertex3f(1.0f,-0.2f,0.0f);
    glVertex3f(-1.0f,-0.2f,0.0f);
  glEnd();
  */
  /*
  int fadeLevel = m_fadeLevel;
  m_fadeLevel = 0;
  SDL_FillRect (m_screen, &GAP, decodeColor( (20|(20<<5)|(20<<10)) ) );
  m_fadeLevel = fadeLevel;
  */
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
    SDL_FillRect (m_layer[0], &rect, colour);
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
  SDL_BlitSurface(m_layer[0], 0, m_layer[2], 0);
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
    SDL_FillRect (m_layer[1], &rect, colour);
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
  SDL_BlitSurface(m_layer[1], 0, m_layer[3], 0);

  drawGap();
}

void SDLhandler::setAlpha(int alpha)
{
  m_alpha = alpha;
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
        colour = decodeColor(m_spritePalette[palette]);
        break;
      case 3:
        colour = decodeColor(m_subSpritePalette[palette]);
        break;

      default:
        colour = m_backgroundPaletteSDL[palette];
        break;
    }
  }
  SDL_FillRect (m_layer[layer], &rect, colour);
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
  setDirty();
  return &m_vramMain[offset];
}

bool SDLhandler::inGap(int y) const
{
  return (y >= GAP.y and y < (GAP.y+GAP.h));
}

unsigned short * SDLhandler::vramSub(int offset) {
  setDirty();
  return &m_vramSub[offset];
}

int powerOfTwo( int value )
{
  int result = 1 ;
  while ( result < value )
    result *= 2 ;
  return result;
}


int SDLhandler::uploadTextureFromSurface(
    SDL_Surface * sourceSurface,
    int colorKeyRed, int colorKeyGreen, int colorKeyBlue )
{

  /*
   * Use the surface width and height expanded to powers of 2 :
   * (one may call also gluScaleImage.
   */

  // Set up so that colorkey pixels become transparent :
  //Uint32 colorkey = SDL_MapRGBA( sourceSurface->format, colorKeyRed, colorKeyGreen, colorKeyBlue, 0 ) ;
  //SDL_SetColorKey( sourceSurface, SDL_SRCCOLORKEY, colorkey ) ;

  // Create an OpenGL texture for the image

  GLuint textureID ;
  glGenTextures( 1, &textureID ) ;
  glBindTexture( GL_TEXTURE_2D, textureID ) ;

  /* Map the alpha surface to the texture */
  glTexImage2D( GL_TEXTURE_2D,
      0,
      GL_RGBA,
      sourceSurface->w, sourceSurface->h,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      sourceSurface->pixels ) ;
  /* Prepare the filtering of the texture image */
#if 1
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) ;
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) ;
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) ;
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) ;
#endif

  return textureID ;

}
void SDLhandler::waitVsync()
{
  int tmpGF = m_frames;

  if (m_dirty)
  {
    // pallete conversions:
    for (int i = 0; i < 256; i++)
    {
      m_fadeLevel = m_fadeLevelMain;
      m_whiteLevel = m_whiteLevelMain;
      m_backgroundPaletteSDL[i] = decodeColor(m_backgroundPalette[i]);
      //m_spritePaletteSDL[i] = decodeColor(m_spritePalette[i]);

      m_fadeLevel = m_fadeLevelSub;
      m_whiteLevel = m_whiteLevelSub;
      m_subBackgroundPaletteSDL[i] = decodeColor(m_subBackgroundPalette[i]);
      //m_subSpritePaletteSDL[i] = decodeColor(m_subSpritePalette[i]);
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

    // blit doesn't work with open gl - have to upload as a texture
    //SDL_Flip(m_screen);
    SDL_Surface * alphaImage = SDL_CreateRGBSurface( SDL_SWSURFACE, m_layer[0]->w,
        512, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN // OpenGL RGBA masks
      0x000000FF,
      0x0000FF00,
      0x00FF0000,
      0xFF000000
#else
      0xFF000000,
      0x00FF0000,
      0x0000FF00,
      0x000000FF
#endif
      );
    for (int i = 0; i < 4; ++i)
    {
      SDL_BlitSurface(m_layer[i], 0, alphaImage, 0);
    }

    // get the user viewport and project
    GLint viewport[4];
    GLdouble projection[16];
    GLdouble modelview[16];
    if (m_threeD)
    {
      glGetIntegerv(GL_VIEWPORT,viewport);
      glGetDoublev(GL_PROJECTION_MATRIX,projection);
      glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    }

    initGL();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_TEXTURE ) ;
    glEnable( GL_TEXTURE_2D );
    // glLoadIdentity() ;
    GLuint texture = uploadTextureFromSurface(alphaImage, 0, 0, 0);
    SDL_FreeSurface( alphaImage ) ;
    glMatrixMode( GL_MODELVIEW ) ;
    // glLoadIdentity( );

    float y = 0.;
    float h = (float)m_layer[0]->h;
    if (m_threeD)
    {
      if (!m_mainOnTop)
        y = (float)(192+GAP.h);
      h = 192.;
    }
    glColor3f(1.0, 1.0, 1.0);

    //printf("Draw non-threed layer at 0, %f, size 256, %f\n", y, h);
    float one = ((float)h)/512.;
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f  (0.0f,y);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f  (256.0f, y);
      glTexCoord2f(1.0f, one);
      glVertex2f  (256.0f, y+h);
      glTexCoord2f(0.0f, one);
      glVertex2f  (0.0f, y+h);
    glEnd();
    glDeleteTextures(1, &texture);
    glDisable( GL_TEXTURE_2D );
    //printf("Write to %f %f\n", y, h);

    glFlush();
    if (m_threeD)
    {
      glMatrixMode( GL_PROJECTION );
      glLoadMatrixd(projection);
      glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }
    else
    {
      // 3D progs should call glFlush(0) but non 3D ones should not have to
      SDL_GL_SwapBuffers();
    }
  }
  SDL_Event event;
  while( SDL_PollEvent( &event ) ) {
    switch( event.type ) {
      case SDL_QUIT:
        exit(0);
        break;

      case SDL_MOUSEMOTION:
        {
          int x,y;
          Uint8 buttons = SDL_GetMouseState(&x, &y);
          if ( (buttons & SDL_BUTTON_LMASK) and y > 191) {
            if (inGap(y)) {
              break;
            }
            y -= GAP.h;
            y -= 192;
            Keys::instance().handleMouseMotion(x, y);
          }
        }
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
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
          exit(0);
        }
        Keys::instance().handleKeyEvent(event.key);
        break;
    }
  }
  while (tmpGF == m_frames) {
    SDL_Delay(5);
  }
  if (m_fn)
    m_fn();
  m_dirty = false;
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

void SDLhandler::clear3D()
{
  if (SDLhandler::instance().m_threeD) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  }
}

void SDLhandler::lcdSwap()
{
  m_mainOnTop = not m_mainOnTop;
  clear3D();
}
void SDLhandler::mainOnTop()
{
  m_mainOnTop = true;
  clear3D();
}
void SDLhandler::mainOnBottom()
{
  m_mainOnTop = false;
  clear3D();
}

unsigned short * SDLhandler::spriteGfx()
{
  setDirty();
  return m_spriteGfx;
}

unsigned short * SDLhandler::subSpriteGfx()
{
  setDirty();
  return m_subSpriteGfx;
}

void SDLhandler::setDirty()
{
  m_dirty = true;
}

void SDLhandler::setThreeD(bool td)
{
  m_threeD = td;
}
