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
#include "libnds.h"
#include <vector>
#include "Cache.h"
#include "Config.h"
#include "Controller.h"
#include "Document.h"
#include "File.h"
#include "Font.h"
#include "HttpClient.h"
#include "TextAreaFactory.h"
#include "URI.h"
#include "View.h"

using namespace std;

static const char s_licenceText[] = {
#include "licence"
};
static const char s_errorText[] = {
#include "error.txt"
};

const static char * LICENCE_URL = "file:///licence";

Controller::Controller()
  : m_document(new Document()), m_wifiInit(false)
{
  m_config = new Config(*m_document);
  string font;
  m_config->resource(Config::FONT_STR, font);
  TextAreaFactory::setFont(new Font(font));
  TextAreaFactory::usePaletteName(font+".pal");
  m_view = new View(*m_document, *this);
  bool useCache(false);
  bool clearCache(false);
  m_config->resource(Config::USECACHE, useCache);
  m_config->resource(Config::CLEARCACHE, clearCache);

  m_cache = new Cache(*m_document, useCache, clearCache);
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
  m_document->setCacheFile("");
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

    case URI::HTTPS_PROTOCOL:
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

void Controller::reload()
{
  m_cache->clean(m_document->uri());
  doUri(m_document->uri());
}

void Controller::saveAs()
{
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
    //m_config->postConfiguration(postedUrl);
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
  bool hasPage = false;
  if (not m_cache->load(uri))
  {
    HttpClient client(uri.server().c_str(), uri.port(), uri);
    client.setController(this);
    m_stop = false;
    while (not client.finished())
    {
      client.handleNextState();
      if (client.state() > HttpClient::WIFI_OFF)
      {
        m_wifiInit = true;
      }
      m_view->tick();
      if (m_stop)
      {
        loadError();
        return;
      }
      swiWaitForVBlank();
    }
    hasPage = client.hasPage();
  }
  else
  {
    // page was cached.
    hasPage = true;
  }

  if (hasPage)
  {
    URI docUri(m_document->uri());
    if (docUri != uri)
    {
      //m_redirects[uri.asString()] = docUri.asString();
      // redirected
      //client.disconnect(); // < check that there are no regressions from this change for caching!
      swiWaitForVBlank();
      swiWaitForVBlank();
      m_document->reset();
      doUri(uri.navigateTo(m_document->uri()));
    }
    else
    {
      m_document->setStatus(Document::LOADED);
    }
  }
}

bool Controller::wifiInitialised() const
{
  return m_wifiInit;
}

void Controller::stop()
{
  m_stop = true;
}

Cache * Controller::cache() const
{
  return m_cache;
}
