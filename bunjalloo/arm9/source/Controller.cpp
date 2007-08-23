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
#include "libnds.h"
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

const static char * LICENCE_URL = "file:///licence";

Controller::Controller()
  : m_document(new Document())
{
  m_config = new Config(*m_document);
  m_view = new View(*m_document, *this);
}

Controller::~Controller()
{
  delete m_document;
  delete m_view;
  delete m_config;
}

void Controller::showLicence()
{
  m_document->reset();
  m_document->setUri(LICENCE_URL);
  m_document->appendLocalData(s_licenceText, strlen(s_licenceText));
  m_document->setStatus(Document::LOADED);
}

const Config & Controller::config() const
{
  return *m_config;
}

void Controller::handleUri(const URI & uri)
{
  switch (uri.protocol())
  {
    case URI::FILE_PROTOCOL:
      if (uri.asString() == LICENCE_URL)
      {
        showLicence();
      }
      else
      {
        localFile(uri.fileName());
      }
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
  // cout << uri.asString() << endl;
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
    m_config->postConfiguration(postedUrl);
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
  HttpClient client(uri.server().c_str(), uri.port(), uri);
  client.setController(this);
  while (not client.finished())
  {
    client.handleNextState();
    m_view->tick();
    swiWaitForVBlank();
  }

  if (client.hasPage())
  {
#if 0
    // redirect:
    URI docUri(m_document->uri());
    if (docUri != uri)
    {
      // redirected
      client.disconnect();
      swiWaitForVBlank();
      swiWaitForVBlank();
      m_document->reset();
      doUri(uri.navigateTo(m_document->uri()));
    }
    else
    {
      m_document->setStatus(Document::LOADED);
    }
#endif
    m_document->setStatus(Document::LOADED);
  }
  /*
  nds::Wifi9::instance().connect();
  if (nds::Wifi9::instance().connected()) {
    // open a socket to the server.
    HttpClient client(uri.server().c_str(), uri.port());
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
        doUri(uri.navigateTo(m_document->uri()));
      }
      client.disconnect();
    }
    else
    {
      loadError();
    }
  } else {
    char * woops = "Woops, wifi not done";
    m_document->appendData(woops, strlen(woops));
  }
  */
}

