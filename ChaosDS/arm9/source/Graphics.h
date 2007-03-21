#ifndef Graphics_h_seen
#define Graphics_h_seen

namespace nds{
class Color;
}
class Graphics
{
  public:
    //! The traditional Chaos colour scheme, based on the speccy's bright palette.
    static const unsigned short int  s_chaosColours[];
    // static helper functions
    //! initialise the screen setup
    static void initialiseScreen();
    //! clear all palettes
    static void clearPalettes();

    /** Draw a frame of the splat animation.
     * @param x position.
     * @param y position.
     * @param frame frame to draw.
     */
    static void draw_splat_frame(int x, int y, int frame);

    /*! @brief load a pre-defined palette to the given palette index.
     * @param from the pre defined palette to load from
     * @param to the background palette to load to
     */
    static void loadPalette(int from, int to);
    
    /*! @brief load a pre-defined palettes to their equivalent palette indices.
     */
    static void loadAllPalettes();
    
    //! fetch the gfx instance
    static Graphics & instance();

    /** Load a sprite from the given graphics and map data. This is similar to
     * background map idea and "emulates" the bg hardware for loading data to
     * sprites.
     * @param gfx the graphics data.
     * @param map the map data.
     */
    static void loadSprite(const unsigned short * gfx, 
                           const unsigned short * map);
    
    // member functions
    /*! @brief set the parameters used to animate text
     * @param c the colour step value
     * @param g the gradient step value
     */
    void setAnimationParams(signed char c, signed char g);
    
    /*! @brief set the parameters used to animate text
     * @param c the colour step value
     */
    void setAnimationColour(signed char c);
    /*! @brief Animate the text of a selection.
     * @param pal the palette to animate
     * @param col the base colour to use
     */
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
