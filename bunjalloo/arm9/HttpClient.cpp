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
#include <string>
#include "libnds.h"
#include "System.h"
#include "Config.h"
#include "Controller.h"
#include "CookieJar.h"
#include "Document.h"
#include "HttpClient.h"
#include "URI.h"
#include "File.h"
#include "Wifi9.h"

using namespace std;
using nds::Wifi9;

static const int MAX_CONNECT_ATTEMPTS(10);
extern const char * VERSION;

HttpClient::HttpClient(const char * ip, int port, const URI & uri) : 
  nds::Client(uri.server().c_str(),port), m_total(0), m_finished(false), m_connectAttempts(0),
  m_uri(uri),
  m_state(WIFI_OFF),
  m_maxConnectAttempts(MAX_CONNECT_ATTEMPTS)
{
  debug("In HttpClient, uri.server.c_str:");
  debug(uri.server().c_str());
  debug(uri.asString().c_str());
}

void HttpClient::setController(Controller * c)
{
  m_self = c;
  if (m_self->config().useProxy())
  {
    URI uri(m_self->config().proxy());
    setConnection(uri.server().c_str(), uri.port());
  }
  m_maxConnectAttempts = m_self->config().maxConnections();
  if (m_maxConnectAttempts == 0)
  {
    m_maxConnectAttempts = MAX_CONNECT_ATTEMPTS;
  }
}

// implement the pure virtual functions
void HttpClient::handle(void * bufferIn, int amountRead)
{
  char * buffer = (char*)bufferIn;
  buffer[amountRead] = 0;
  //printf("%s", buffer);
  m_self->m_document->appendData(buffer, amountRead);
  m_total += amountRead;
  //printf("0x0x End of buffer x0x0", buffer);
}

bool HttpClient::finished() const
{
  return m_finished;
}

void HttpClient::finish() { 
  //printf("%d\n",m_total);
  if (m_total == 0)
  {
    m_self->loadError();
  }
  else
  {
    m_self->m_document->setStatus(Document::LOADED);
  }
}

void HttpClient::debug(const char * s)
{
  if (0) {
    nds::File log;
    log.open("bunjalloo.log", "a");
    log.write(s);
    log.write("\n");
    // printf("debug:%s\n",s);
  } 
  //m_self->m_document->appendLocalData(s, strlen(s));
}

// GET stuff
void HttpClient::get(const URI & uri)
{
  if (isConnected())
  {
    string cookieString;
    m_self->m_document->cookieJar()->cookiesForRequest(uri, cookieString);

    string s;
    s += uri.method();
    s += " ";
    if (m_self->config().useProxy())
    {
      // for proxy connection, need to send the whole request:
      s += uri.asString();
    }
    else
    {
      s += uri.fileName();
    }
    s += " HTTP/1.1\r\n";
    s += "Host:" + uri.server()+"\r\n";
    s += "Connection: close\r\n";
    s += "Accept-charset: ISO-8859-1,UTF-8\r\n";
    //If the Accept-Encoding field-value is empty, then only the "identity" encoding is acceptable.
    // -- RFC2616-sec14
    s += "Accept-encoding: gzip,deflate\r\n";
    s += "Accept: text/html\r\n";
    s += "User-Agent: Bunjalloo (";
    s += nds::System::uname();
    s += ";r";
    s += VERSION;
    s += ")\r\n";
    s += cookieString;
    if (uri.requestHeader().empty())
    {
      s += "\r\n";
    }
    else
    {
      s += uri.requestHeader();
    }
    write(s.c_str(), s.length());
    m_finished = false;
  }
  // reset the document for downloading
  m_self->m_document->reset();
}

void HttpClient::wifiConnection()
{
  switch (Wifi9::instance().status())
  {
    case Wifi9::CANNOTCONNECT:
      debug("FAILED Wifi9::CANNOTCONNECT");
      m_state = FAILED;
      break;

    case Wifi9::DISCONNECTED:
    case Wifi9::SEARCHING:
    case Wifi9::AUTHENTICATING:
    case Wifi9::ASSOCIATING:
    case Wifi9::ACQUIRINGDHCP:
      break;

    case Wifi9::ASSOCIATED:
      m_state = CONNECT_SOCKET;
      break;

  }
}

void HttpClient::handleNextState()
{
  switch (m_state)
  {
    case WIFI_OFF:
      Wifi9::instance().connect();
      m_state = CONNECT_WIFI;
      break;

    case CONNECT_WIFI:
      wifiConnection();
      m_connectAttempts = 0;
      m_reconnects = 0;
      break;

    case CONNECT_SOCKET:
      // connect to the socket.
      setTimeout(5);
      this->connect();
      if (isConnected())
      {
        debug("Connected, get URL");
        m_state = GET_URL;
        m_connectAttempts = 0;
        swiWaitForVBlank();
        swiWaitForVBlank();
      }
      else
      {
        m_connectAttempts++;
        if (m_connectAttempts == m_maxConnectAttempts)
        {
          debug("FAILED m_connectAttempts == m_maxConnectAttempts");
          m_state = FAILED;
        }
      }
      break;

    case GET_URL:
      setTimeout(5);
      get(m_uri);
      swiWaitForVBlank();
      swiWaitForVBlank();
      m_state = READING_FIRST;
      break;

    case READING_FIRST:
      // read something, anything, to make sure all is well
      setTimeout(5);
      readFirst();
      break;

    case READING_ALL:
      // now we know the server is connected, read the remaining bytes.
      setTimeout(1);
      readAll();
      break;

    case FINISHED:
      m_finished = true;
      break;

    case FAILED:
      m_total = 0;
      m_finished = true;
      finish();
      break;
  }
}

bool HttpClient::hasPage() const
{
  return m_state == FINISHED;
}

void HttpClient::readFirst()
{
  int read = this->read();
  switch (read)
  {
    case CONNECTION_CLOSED:
      debug("readFirst returned 0 - Failure");
      m_state = FAILED;
      break;

    case READ_ERROR:
      {
        // not be ready yet. This is the select() returning early.
        m_connectAttempts++;
        if (m_connectAttempts >= m_maxConnectAttempts)
        {
          // once we reach the maximum number, retry the socket.
          this->disconnect();
          m_connectAttempts = 0;
          m_state = CONNECT_SOCKET;
          m_reconnects++;
          if (m_reconnects == 3)
          {
            debug("FAILED m_reconnects == 3");
            m_state = FAILED;
          }
        }
        swiWaitForVBlank();
        swiWaitForVBlank();
      }
      break;

    case RETRY_LATER:
      /* Keep going! */
      debug("RETRY_LATER readFirst");
      swiWaitForVBlank();
      swiWaitForVBlank();
      m_connectAttempts++;
      if (m_connectAttempts == m_maxConnectAttempts) {
        m_state = FAILED;
      }
      break;

    default:
      m_state = READING_ALL;
      m_connectAttempts = 0;
      break;
  }

}

void HttpClient::readAll()
{
  int read = this->read();
  switch (read)
  {
    case READ_ERROR:
      m_state = FINISHED;
      break;
    case CONNECTION_CLOSED:
      debug("readAll returned 0 - FINISH or wait?");
      m_state = FINISHED;
      //swiWaitForVBlank();
      //swiWaitForVBlank();
      break;

    case RETRY_LATER:
      /* Keep going */
      debug("RETRY_LATER readAll");
      m_connectAttempts++;
      if (m_self->m_document->uri() != m_uri.asString())
      {
        // redirected.
        debug("Redirect:");
        debug(m_self->m_document->uri().c_str());
        m_state = FINISHED;
      }
      if (m_connectAttempts == m_maxConnectAttempts) {
        debug("m_maxConnectAttempts reached - surely it is done...");
        m_state = FINISHED;
      } else {
        swiWaitForVBlank();
        swiWaitForVBlank();
      }
      break;

    default:
      // ok - there may be more bytes.
      m_state = READING_ALL;
      m_connectAttempts = 0;
      break;
  }
}

HttpClient::ConnectionState HttpClient::state() const
{
  return m_state;
}
