#include <string>
#include "libnds.h"
#include "System.h"
#include "HttpClient.h"
#include "Controller.h"
#include "Document.h"
#include "URI.h"

using namespace std;

static const int MAX_CONNECT_ATTEMPTS(60);
extern const char * VERSION;

HttpClient::HttpClient(const char * ip, int port) : 
  nds::Client(ip,port), m_total(0), m_finished(false), m_connectAttempts(0)
{}

void HttpClient::setController(Controller * c)
{
  m_self = c;
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

bool HttpClient::connectCallback() {
  //printf("Connect?...\n");
  swiWaitForVBlank();
  m_connectAttempts++;
  return m_connectAttempts < MAX_CONNECT_ATTEMPTS;
}

void HttpClient::writeCallback() {
}
void HttpClient::readCallback() {
}

bool HttpClient::finished() 
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
  // printf("\ndebug:%s\n",s);
}

// GET stuff
void HttpClient::get(const URI & uri)
{
  if (isConnected())
  {
    string s("GET ");
    s += uri.fileName();
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
    s += "\r\n";
    write(s.c_str(), s.length());
    m_finished = false;
  }
  // reset the document for downloading
  m_self->m_document->reset();
}
