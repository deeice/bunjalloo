#ifndef HttpClient_h_seen
#define HttpClient_h_seen

#include "Client.h"

class Controller;
class URI;

class HttpClient: public nds::Client
{
  public:

    enum ConnectionState {
      WIFI_OFF,         //!< Initial state.
      CONNECT_WIFI,     //!< Wifi is being started.
      CONNECT_SOCKET,   //!< Can use sockets, connect to server
      GET_URL,          //!< Have connected to server, get url.
      READING_FIRST,    //!< Sent Rqst, see if there is a response.
      READING_ALL,      //!< Response OK, read all data.
      FINISHED,         //!< Finished reading data.
      FAILED            //!< Failed to do anything.
    };


    HttpClient(const char * ip, int port, const URI & uri);
    void setController(Controller * c);

    /** Has the connection and web page fetch succeeded.
     * @return true if the attempt at getting the web page was ok, false otherwise.
     */
    bool hasPage() const;

    /** Handle the next stage of the connection process.  */
    void handleNextState();

    /** Has the fetching finished?.
     * @return true if it has, false otherwise.
     */ 
    bool finished();

  protected:
    void handle(void * bufferIn, int amountRead);
    
    //bool connectCallback();

    // void writeCallback();
    // void readCallback();

    void finish();

    void debug(const char * s);


  private:
    int  m_total;
    bool m_finished;
    int  m_connectAttempts;
    int  m_reconnects;
    const URI & m_uri;
    ConnectionState m_state;
    Controller * m_self;

    void wifiConnection();
    void get(const URI & uri);
    void readFirst();
    void readAll();
};

#endif
