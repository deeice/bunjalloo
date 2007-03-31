#ifndef SDLhandler_h_seen
#define SDLhandler_h_seen

#include <SDL/SDL.h>
#include <string>
#include "libnds.h"
class SDLhandler
{
  public:
    static const int WIDTH;
    static const int HEIGHT;
    static SDLhandler & instance();

    /** Check if there is an sdlEvent. 
     * @return true if quit, false otherwise.
     */
    bool sdlEvent() const;

    /** Draw a pixel.
     * @param x pixel position.
     * @param y pixel position.
     * @param layer 0 for screen 0 background, 1 for screen 1 background, 2 for
     * screen 0 sprite, 3 for screen 1 sprite
     * @param palette palette index to use for the pixel.
     */
    void drawPixel(int x, int y, unsigned int layer, unsigned int palette);

#if 0
    /** Load a GBA style palette.
     */
    void setPaletteData(const unsigned short * palette, int size);

    void loadPalette(const std::string & fileName);
#endif

    void waitVsync();

    void enableVblank(VoidFunctionPointer fn);

    int frame() const;

    void playSound(const void * data, int length);
    int initSound(int rate, int format);

    unsigned short * backgroundPaletteMem();
    unsigned short * subBackgroundPaletteMem();
    unsigned short * spritePaletteMem();
    unsigned short * subSpritePaletteMem();
    Uint32 decodeColor(unsigned short rgb16);

    unsigned short * vramMain(int offset);
    unsigned short * vramSub(int offset);
    unsigned short * spriteGfx();
    unsigned short * subSpriteGfx();

    void setFade(int level);
    void setWhite(int level);
    void lcdSwap();
    void mainOnTop();
    void mainOnBottom();

  private:
    static SDL_Rect GAP;

    static Uint32 vblank(Uint32 interval, void* param);
    static void mixaudio(void *unused, Uint8 *stream, int len);
    static int totalHeight();

    SDL_Surface *m_screen;
    SDL_TimerID m_vblank;
    int m_scale;
    int m_frames;
    bool m_isFullScreen;
    bool m_hasSound;
    unsigned short m_backgroundPalette[256];
    unsigned short m_subBackgroundPalette[256];
    unsigned short m_spritePalette[256];
    unsigned short m_subSpritePalette[256];
    Uint32 m_backgroundPaletteSDL[256];
    Uint32 m_subBackgroundPaletteSDL[256];
    Uint32 m_spritePaletteSDL[256];
    Uint32 m_subSpritePaletteSDL[256];
    unsigned short * m_vramMain;
    unsigned short * m_vramSub;
    unsigned short * m_spriteGfx;
    unsigned short * m_subSpriteGfx;

    bool m_mainOnTop;
    VoidFunctionPointer m_fn;

    int m_fadeLevel;
    int m_whiteLevel;

    SDLhandler();
    ~SDLhandler();

    int init();
    void drawGap();
    void clear();
};
#endif
