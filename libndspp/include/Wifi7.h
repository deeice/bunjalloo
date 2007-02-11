#ifndef Wifi7_h_seen
#define Wifi7_h_seen

#ifndef ARM7
#error Wifi7 is only available on the ARM7
#endif

namespace nds {
  /*! @brief Wifi7 is a helper class for the arm7 dswifi code.
   *
   *  This class initialises the DS wifi hardware and has a main loop that
   *  handles connections and disconnections.
   */
  class Wifi7
  {
    public:
      //! creates an instance of the class and initialises the wifi hardware.
      Wifi7();
      
      /*! @brief an infinite loop that handles connect, disconnect and reconnect.
       * Calls the software interrrupt vblank wait in between to use less power.
       */
      void mainLoop();
    private:
      //! is the hardware enabled or not
      bool m_enabled;
      //! initialise the hardware
      void initialise();
  };
};

#endif
