/*
  Copyright (C) 2007 Richard Quirk

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
#ifndef HttpClient_h_seen
#define HttpClient_h_seen

#include "Client.h"
#include "URI.h"

class Controller;
class SslClient;

class HttpClient: public nds::Client
{
  public:

    enum ConnectionState {
      WIFI_OFF,         //!< Initial state.
      CONNECT_WIFI,     //!< Wifi is being started.
      CONNECT_SOCKET,   //!< Can use sockets, connect to server
      SSL_HANDSHAKE,    //!< Have connected to server, using HTTPS, do SSL handshake.
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

    ConnectionState state() const;

    /** Has the fetching finished?.
     * @return true if it has, false otherwise.
     */
    bool finished() const;

    /** Handle raw data - not encrypted
     * @param bufferIn the data
     * @param amountRead the amount read in bytes
     */
    void handleRaw(void * bufferIn, int amountRead);

    void print(const char * s);

  protected:
    void handle(void * bufferIn, int amountRead);


    void debug(const char * s);


  private:
    int  m_total;
    bool m_finished;
    int  m_connectAttempts;
    int  m_reconnects;
    URI m_uri;
    ConnectionState m_state;
    Controller * m_controller;
    int  m_maxConnectAttempts;
    bool m_hasSsl;
    SslClient * m_sslClient;

    void wifiConnection();
    void get(const URI & uri);

    void proxyConnect();
    void readFirst();
    void readAll();
    void finish();
    inline bool isSsl() const;
};

#endif
