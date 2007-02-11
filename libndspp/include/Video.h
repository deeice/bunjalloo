#ifndef Video_h_seen
#define Video_h_seen

#ifndef BIT
#define BIT(number) (1<<(number))
#endif
namespace nds
{
  /*! @brief Controls the video hardware */
  class Video
  {
    public:

      /*! @brief use this instead of a constructor. 
       * @param screen Screen 0 is the main (top) screen, screen 1 is the bottom (sub) screen.
       * @returns the instance of the video system for screen 0 or 1
       */
      static Video& instance(int screen=0);

      /*! @brief Set the video mode.
       * @param mode Mode value 0-6. Mode 6 is new on the DS.
       */
      void mode(unsigned int mode);

      /*! @brief get the video mode.
       * @returns Mode value 0-6
       */
      unsigned int mode() const;

      /*! @brief Enable or disable objects (sprites).
       * @param enable true to enable objects, false to disable them
       */
      void enableObjects(bool enable=true);

      /*! @brief Get the status of the objects (sprites)
       * @returns true if objects enabled, false otherwise
       */
      bool objectsEnabled() const;
      
      /*! @brief Set the dimensions of the object map. May be 1D or 2D.
       * @param dimensions 1 or 2 for 1D or 2D.
       */
      void objectMapDimensions(int dimensions);

      /*! @brief get the dimensions of the object map. May be 1D or 2D.
       * @returns dimensions 1 or 2 for 1D or 2D.
       */
      int objectMapDimensions() const;
      
      /*! @brief clears video ram. */
      void clear();

      //! Defines the backgrounds to blend between
      typedef enum {
        BLDMOD_BG0=BIT(0), //!< Layer 0
        BLDMOD_BG1=BIT(1), //!< Layer 1
        BLDMOD_BG2=BIT(2), //!< Layer 2
        BLDMOD_BG3=BIT(3), //!< Layer 3
        BLDMOD_OBJ=BIT(4), //!< Layer 4
        BLDMOD_BD =BIT(5)  //!< Backdrop layer
      } BLDMOD_BG_t;

      //! Defines the type of blending between layers
      typedef enum {
        BLDMOD_OBJECT=0, //!< Blend objects
        BLDMOD_ALPHA=1,  //!< Alpha blend layers
        BLDMOD_WHITE=2,  //!< White out
        BLDMOD_FADE=3    //!< Fade to black
      } BLDMOD_MODE_t;

      /*! @brief Perform hardware blending between layers.
       * @param first or top list of layers. e.g. BLDMOD_BG0|BLDMOD_BG1
       * @param second or bottom list of layers. e.g. BLDMOD_BD
       * @param mode the type of blending to perform
       */
      void blend(BLDMOD_MODE_t mode=BLDMOD_ALPHA,
          int first=BLDMOD_BG1 | BLDMOD_BG0 | BLDMOD_BG2 | BLDMOD_OBJ, 
          int second=BLDMOD_BD);
      
      /*!@brief set the fade level (BLDMOD_FADE)
       * @param level the level of fading - from 0-16. 0 being bright, 16 being fully black.
       */
      void setFade(int level);
      
      /*!@brief set the white level (BLDMOD_WHITE)
       * @param level the level of white - from 0-16. 0 being normal, 16 being fully white.
       */
      void setWhite(int level);

      /*!@brief fade from normal to black.
       * @param fadeout true to fade to black, false to fade back in
       * @param speed the speed of the fade out, higher value is slower
       */
      void fade(bool fadeout=true, unsigned int speed=1);
      /*!@brief fade from normal to white.
       * @param towhite true to brighten to white, false to fade back to normal
       * @param speed the speed of the whiteout, higher value is slower
       */
      void whiteout(bool towhite=true, unsigned int speed=1);

      /*! @brief Set 3D mode
       * @param td if true sets 3D mode, if false does not
       */
      void threeD(bool td);

      /*! @brief Get 3D mode
       * @returns the current state of 3D. True if 3D is set false otherwise.
       */
      bool threeD() const;

      /*! @brief screen capture
       */
      void capture() const;

      //! Switch the screens. By default the main screen is on top and the sub screen is on teh touch screen.
      static void swapScreens();

      //! Make this screen go to the top.
      void setToTop();
      //! Make this screen go to the bottom.
      void setToBottom();

    private:
      //! the background register to use
      volatile unsigned int & m_DISPCNT;
      //! The screen used.
      int m_screen;

      /*! @brief Constructs a Video display for the main or sub screen.
       * @param screen 0 for the main screen, 1 for the sub screen.
       */
      Video(int screen);

      //! Set the default VRAM banks. Uses bank A for the main screen, bank C for the sub screen.
      void setBanks();
    
  }; 
  
};
#endif
