/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "Language.h"
#include "Controller.h"
#include "Document.h"
#include "HtmlDocument.h"
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
"<html> <meta http-equiv='Content-Type' content='text/html; charset=utf-8' />"
};

const static char * LICENCE_URL = "file:///licence";
const static char * UNABLE_TO_LOAD = "cannot_load";
const static int MAX_REDIRECTS(7);

Controller::Controller()
  : m_document(new Document()),
  m_httpClient(new HttpClient),
  m_wifiInit(false),m_redirected(0),m_maxRedirects(MAX_REDIRECTS)
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
  m_config->resource("redirects", m_maxRedirects);
  m_httpClient->setController(this);
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
    m_document->setPosition(-1);
  }
}

void Controller::reload()
{
  m_cache->remove(m_document->uri());
  doUri(m_document->uri());
}

void Controller::cancelSaveAs()
{
  if (m_saveAs == SAVE_NEEDS_DOWNLOADING)
  {
    // meh.
    m_document->setStatus(Document::LOADED);
  }
}

void Controller::saveAs(const char * fileName)
{
  // 2 types of "save as" - save the current displayed file, or save a file not yet downloaded.
  switch (m_saveAs)
  {
    case SAVE_CURRENT_FILE:
      saveCurrentFileAs(fileName);
      break;

    default:
      // download the file...
      downloadAndSaveAs(fileName);
      break;
  }
}

void Controller::downloadAndSaveAs(const char * fileName)
{
  // ulp
  URI tmp(m_document->uri());
  tmp.setMethod("GET");
  m_document->setCacheFile(fileName);
  m_httpClient->setUri(tmp);
  fetchHttp2(tmp);
  if (m_httpClient->hasPage())
  {
    finishFetchHttp(tmp);
  }
}

void Controller::saveCurrentFileAs(const char * fileName)
{
  // save the current document as fileName
  // simply copy from the cache.
  string cachedFile = m_cache->fileName(m_document->uri());
  if (nds::File::exists(cachedFile.c_str()) == nds::File::F_REG
      and nds::File::exists(fileName) == nds::File::F_NONE)
  {
    /*bool ok = */nds::File::cp(cachedFile.c_str(), fileName);
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
  string errorStr(unicode2string(T(UNABLE_TO_LOAD), true));
  m_document->appendLocalData(errorStr.c_str(), errorStr.length());
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

void Controller::fetchHttp2(URI & uri)
{
  m_stop = false;
  while (not m_httpClient->finished())
  {
    m_httpClient->handleNextState();
    if (m_httpClient->state() > HttpClient::WIFI_OFF)
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
}

void Controller::fetchHttp(const URI & uri)
{
  /* this works as follows:
   *
   * if not in the cache
   *   do head request to find mime type
   *   if not a friendly mime type
   *      get save name and wait for saveAs callback
   *      saveAs really gets the file, saving it as needed
   *   else 
   *      really get the file.
   *   endif
   * else
   *   load from the cache
   * endif
   * if have a page
   *   finishFetchHttp 
   * endif
   *
   */
  bool hasPage = false;
  m_saveAs = m_cache->useCache()?SAVE_CURRENT_FILE:SAVE_NEEDS_DOWNLOADING;
  if (not m_cache->load(uri))
  {
    // loop one, if get, then head
    // if that is ok, then get again
    URI tmp(uri);
    if (uri.method() == "GET")
    {
      tmp.setMethod("HEAD");
      // don't cache the head request, it screws stuff up.
      m_cache->remove(uri);
      m_document->setCacheFile("");
    }

    m_httpClient->setUri(tmp);
    m_httpClient->reset();
    for (;;)
    {
      fetchHttp2(tmp);
      if (m_stop)
        return;

      hasPage = m_httpClient->hasPage();
      if (m_httpClient->uri().method() == "HEAD")
      {
        // what is the mime type of the file?
        bool download = true;
        switch (m_document->htmlDocument()->mimeType())
        {
          case HtmlParser::UNINITIALISED:
          case HtmlParser::OTHER:
            break;

          default:
            // can see it
            download = false;
            break;
        }
        if (download)
        {
          // m_httpClient->disconnect();
          // need to get file name...
          hasPage = false;
          // save as something
          m_saveAs = SAVE_NEEDS_DOWNLOADING;
          m_view->saveAs();
          break;
        }
        else
        {
          tmp.setMethod("GET");
          m_document->reset();
          m_httpClient->setUri(tmp);
          // load doesn't actually load now, it sets up the caching name and
          // registers the URL in the cache.
          m_cache->load(uri);
        }
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    // page was cached.
    hasPage = true;
  }

  if (hasPage)
  {
    finishFetchHttp(uri);
  }
}

void Controller::finishFetchHttp(const URI & uri)
{
  // check the caching status
  if (not m_document->shouldCache())
  {
    // oops, remove it
    m_cache->remove(uri);
  }
  URI docUri(m_document->uri());
  if (docUri != uri and m_redirected < m_maxRedirects)
  {
    // redirected.
    m_redirected++;
    swiWaitForVBlank();
    swiWaitForVBlank();
    m_document->setStatus(Document::REDIRECTED);
    m_document->reset();
    doUri(uri.navigateTo(m_document->uri()));
  }
  else
  {
    m_redirected = 0;
    m_document->setStatus(Document::LOADED);
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

void Controller::setReferer(const URI & referer)
{
  m_httpClient->setReferer(referer);
}

void Controller::clearReferer()
{
  m_httpClient->clearReferer();
}
