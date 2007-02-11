#ifndef Graphics_h_seen
#define Graphics_h_seen

namespace nds{
class Color;
}
class Graphics
{
  public:
    // static helper functions
    static void initialiseScreen();
    static void clearPalettes();
    static void loadPalette(int from, int to);
    
    // fetch the gfx instance
    static Graphics & instance();
    
    // member functions
    void setAnimationParams(signed char c, signed char g);
    void setAnimationColour(signed char c);
    void animateSelection(int pal, nds::Color & col);
    
  private:
    signed char m_colour;
    signed char m_gradient;
    // Impossible to make a Graphics instance
    Graphics();
    // copy constructor
    Graphics(const Graphics&);
    Graphics & operator=(const Graphics&);
};

#endif
