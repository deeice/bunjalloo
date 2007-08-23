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
  nds::Client(ip,port), m_total(0), m_finished(false), m_connectAttempts(0),
  m_uri(uri),
  m_state(WIFI_OFF)
{}

void HttpClient::setController(Controller * c)
{
  m_self = c;
  if (m_self->config().useProxy())
  {
    URI uri(m_self->config().proxy());
    setConnection(uri.server().c_str(), uri.port());
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
  } 
  else {
    swiWaitForVBlank();
    swiWaitForVBlank();
  }
  printf("debug:%s\n",s);
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
        if (m_connectAttempts == MAX_CONNECT_ATTEMPTS)
        {
          debug("FAILED m_connectAttempts == MAX_CONNECT_ATTEMPTS");
          m_state = FAILED;
        }
      }
      break;

    case GET_URL:
      setTimeout(2);
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
  if (read == CONNECTION_CLOSED)
  {
    // worrying. resend the url request?
    //swiWaitForVBlank();
    //swiWaitForVBlank();
    //m_state = GET_URL;
    debug("readFirst returned 0 - FINISH or wait?");
    m_state = FAILED;
  }
  else if (read == READ_ERROR)
  {
    // not be ready yet. This is the select() returning early.
    m_connectAttempts++;
    if (m_connectAttempts >= MAX_CONNECT_ATTEMPTS)
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
  else
  {
    m_state = READING_ALL;
    m_connectAttempts = 0;
  }

}

void HttpClient::readAll()
{
  int read = this->read();
  if (read == READ_ERROR)
  {
    m_state = FINISHED;
  }
  else if (read == CONNECTION_CLOSED)
  {
    debug("readAll returned 0 - FINISH or wait?");
    m_state = FINISHED;
    //swiWaitForVBlank();
    //swiWaitForVBlank();
  }
  else
  {
    // ok - there may be more bytes.
    m_state = READING_ALL;
  }
}
