#ifndef Wifi9_h_seen
#define Wifi9_h_seen

#ifndef ARM9
#error Wifi9 is only available on the ARM9
#endif

namespace nds
{
  /*! @brief Wifi9 is a helper class for the arm9 dswifi code.
   *
   *  This class initialises the DS wifi hardware and handles connections and disconnections.
   */
  class Wifi9
  {
    public:

      /*! @brief use this instead of a constructor to create an instance of the class.
       * @returns an initialised, but not connected, wifi class for the arm9 processor.
       */
      static Wifi9 & instance();

      /*! @brief connect to the AP using the built in WFC settings
       * @returns true if connceted succesfully, false otherwise.
       */
      bool connect();

      /*! @brief fetch the status of the connection.
       * @returns true if connected, false otherwise.
       */
      bool connected() const;
      
      /*! @brief disconnect from the AP.
       * This disconnects and also tells the arm7 code to do the same.
       */
      void disconnect();

    private:
      //! current connection status flag
      bool m_connected;
      //! construct the class, calls initialise
      Wifi9();
      //! initialise the DS wifi hardware but do not connect.
      void initialise();

  };
};
#endif
