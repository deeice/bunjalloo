#ifndef Wifi9_h_seen
#define Wifi9_h_seen

#ifdef ARM7
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

      /** Connect to the AP using the built in WFC settings. */
      void connect();

      //! Status from dswifi, saves including dswifi header.
      enum WifiStatus
      {
        DISCONNECTED,   //!< Not trying to connect
        SEARCHING,      //!< Looking for AP
        AUTHENTICATING, //!< Connecting.
        ASSOCIATING,    //!< Connecting.
        ACQUIRINGDHCP,  //!< Connected to AP, getting IP
        ASSOCIATED,     //!< Connected
        CANNOTCONNECT,  //!< Error, unable to connect
      };

      /** Get the wifi connection status.
       * @returns the current Wifi status
       */
      WifiStatus status() const;

      /*! @brief fetch the status of the connection.
       * @returns true if connected, false otherwise.
       */
      bool connected() const;
      
      /*! @brief disconnect from the AP.
       * This disconnects and also tells the arm7 code to do the same.
       */
      void disconnect();

    private:
      bool m_connected;
      //! construct the class, calls initialise
      Wifi9();
      //! initialise the DS wifi hardware but do not connect.
      void initialise();

  };
};
#endif
