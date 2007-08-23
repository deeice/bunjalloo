/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
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
    bool finished() const;

  protected:
    void handle(void * bufferIn, int amountRead);
    

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
    void finish();
};

#endif
