#include "libnds.h"
#include "Wifi9.h"
#include <vector>
#include "Controller.h"
#include "Document.h"
#include "View.h"
#include "URI.h"
#include "HttpClient.h"
#include "Config.h"
#include "File.h"

using namespace std;

static const char s_licenceText[] = {
#include "licence"
};
static const char s_errorText[] = {
#include "error.txt"
};


Controller::Controller()
  : m_document(new Document())
{
  Config::instance().initialise(m_document, this);
  m_view = new View(*m_document, *this);
}

Controller::~Controller()
{
  delete m_document;
  delete m_view;
}

void Controller::showLicence()
{
  m_document->reset();
  m_document->setUri("file:///licence");
  m_document->appendLocalData(s_licenceText, strlen(s_licenceText));
  m_document->setStatus(Document::LOADED);
}

void Controller::doUri(const std::string & uriString)
{
  doUri(URI(uriString));
}

void Controller::handleUri(const URI & uri)
{
  switch (uri.protocol())
  {
    case URI::FILE_PROTOCOL:
      localFile(uri.fileName());
      break;

    case URI::HTTP_PROTOCOL:
      fetchHttp(uri);
      break;

    case URI::CONFIG_PROTOCOL:
      configureUrl(DATADIR"/config/" + uri.fileName());
      break;

    default:
      break;
  }
}

void Controller::doUri(const URI & uri)
{
  //cout << uriString << endl;
  if (uri.isValid()) {
    m_document->setUri(uri.asString());
    handleUri(uri);
  }
}

void Controller::previous() 
{
  string ph = m_document->gotoPreviousHistory();
  if (not ph.empty())
  {
    URI uri(ph);
    handleUri(uri);
  }
}

void Controller::next() 
{
  string ph = m_document->gotoNextHistory();
  if (not ph.empty())
  {
    URI uri(ph);
    handleUri(uri);
  }
}

void Controller::mainLoop()
{
  for (;;) {
    m_view->tick();
    swiWaitForVBlank();
  }
}

void Controller::loadError()
{
  m_document->reset();
  m_document->appendLocalData(s_errorText, strlen(s_errorText));
  string href("<a href='");
  href += m_document->uri();
  href += "'>";
  href += m_document->uri();
  href += "</a>";
  m_document->appendLocalData(href.c_str(), href.length());
  m_document->setStatus(Document::LOADED);
}

void Controller::configureUrl(const std::string & fileName)
{
  // if file exists, localfile it
  // search for ? - if found, send the encoding to configure
  // otherwise localFile the fileName

  unsigned int position = fileName.find_first_of("?");
  if (position != string::npos)
  {
    string postedUrl = fileName.substr(position+1, fileName.length() - position - 1);
    Config::instance().postConfiguration(postedUrl);
  }
  else
  {
    localFile(fileName);
  }
}

void Controller::localFile(const std::string & fileName)
{
  nds::File uriFile;
  uriFile.open(fileName.c_str());
  // read the lot
  if (uriFile.is_open())
  {
    int size = uriFile.size();
    char * data = new char[size+2];
    uriFile.read(data);
    data[size] = 0;
    m_document->reset();
    m_document->appendLocalData(data, size);
    m_document->setStatus(Document::LOADED);
    delete [] data;
    uriFile.close();
  }
  else
  {
    // could not load file.
    loadError();
  }

}

void Controller::fetchHttp(const URI & uri)
{
  nds::Wifi9::instance().connect();
  if (nds::Wifi9::instance().connected()) {
    // open a socket to the server.
    // FIXME - hardcoded 80 port.
    HttpClient client(uri.server().c_str(), uri.port());
    // for a proxy, do this:
    // HttpClient client(proxy_ip, uri.port());
    client.setController(this);
    client.connect();
    if (client.isConnected())
    {
      client.get(uri);
      client.read();

      URI docUri(m_document->uri());
      if (docUri != uri)
      {
        // redirected
        doUri(uri.navigateTo(m_document->uri()).asString());
      }
    }
    else
    {
      loadError();
    }
  } else {
    char * woops = "Woops, wifi not done";
    m_document->appendData(woops, strlen(woops));
  }
}

