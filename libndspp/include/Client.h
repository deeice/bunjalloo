#ifndef Client_h_seen
#define Client_h_seen

struct sockaddr_in;
namespace nds {
  class Client
  {
    public:

      /*!@brief Create a TCP client that connects to an IP address and port
       * @param ip IP address to connect to.
       * @param port port to connect to.
       */
      Client(const char * ip, int port);

      /*! @brief virtual destructor. Closes the socket (if open) */
      virtual ~Client();

      /*! @brief Connect to the given IP addres and port.*/
      void connect();

      /*! @brief Read bytes from the server address and port.
       * @return the number of bytes read. -1 for an error.
       */
      int read();

      /*! @brief Disconnect from the server.*/
      void disconnect();

      /*! @brief Write data to the target address and port. 
       * @param data pointer to the data to send
       * @param size size in bytes of each element of data
       * @returns number of bytes written
       * */
      unsigned int write(const void * data, unsigned int size);

      /*! @brief Fetch the current status of the connection.
       * @returns true if connected false otherwise.*/
      inline bool isConnected() const { return m_connected; }

      /** Set the connection details.
       * @param ip IP address to connect to.
       * @param port port to connect to.
       */
      inline void setConnection(const char * ip, int port)
      {
        m_ip = ip;
        m_port = port;
      }

    protected:
      /*! @brief Pure virtual function to handle the bytes read from the server
       * @param buffer Data recieved from the other end.
       * @param amountRead Number of bytes read.
       */
      virtual void handle(void * buffer, int amountRead) = 0;
      /*! @brief Pure virtual function to handle the end of the reading. */
      // virtual void finish() = 0;

      /*! @brief debug string 
       * @param c Characters to print (or whatever). */
      virtual void debug(const char * c)=0;

      /*! Called before recv to check if we have all the expected data. The idea
       * is that the protocol used should specify the amount of data at the
       * beginning of the recvs, then we should check this against the current
       * running total.
       * @return true if we have finished, false if we should try to read more data.
       *
       */
      /*
       virtual bool finished()=0;
      */

      /** called during connection every second while waiting.
       * @return true if should keep trying, false to give up.
       */
      //virtual bool connectCallback()=0;

      /*
      //! called during write every second while waiting.
      virtual void writeCallback()=0;
      //! called during read every second while waiting.
      virtual void readCallback()=0;
      */


    private:
      //! The IP address
      const char * m_ip;
      //! The port on the server to connect to
      int m_port;
      //! The socket
      int m_tcp_socket;
      //! The status of the connection
      bool m_connected;
      //! The time to wait between selects (in seconds)
      int m_timeout;

      bool connect(sockaddr_in & socketAddress);

      Client(const Client &);
      const Client operator=(const Client &);
  };
}
#endif
