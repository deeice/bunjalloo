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
#include <assert.h>
#include "Document.h"
#include "HtmlDocument.h"
#include "HtmlConstants.h"
#include "HeaderParser.h"
#include "HtmlElement.h"
#include "CookieJar.h"
#include "URI.h"

using namespace std;

Document::Document():
  m_amount(0),
  m_cookieJar(new CookieJar),
  m_htmlDocument(new HtmlDocument),
  m_headerParser(new HeaderParser(m_htmlDocument,m_cookieJar)),
  m_historyEnabled(true)
{
  m_history.clear();
  m_historyPosition = m_history.begin();
  reset();
}

Document::~Document()
{
  delete m_htmlDocument;
  delete m_headerParser;
  delete m_cookieJar;
}

void Document::setUri(const std::string & uriString)
{
  // m_uri = uriString;
  if (m_historyEnabled and (m_history.empty() or uriString != currentHistoryUri()))
  {
    // not refreshing the same page
    HistoryVector::iterator currPosition(m_historyPosition);
    if (hasNextHistory()) {
      ++currPosition;
      m_history.erase(currPosition, m_history.end());
    }
    m_history.push_back(HistoryEntry(uriString,-1));
    m_historyPosition = m_history.end();
    --m_historyPosition;
  }
  m_headerParser->setUri(uriString);
#if 0
  printf("%s %d\n", m_historyPosition->c_str(), m_history.size());
  vector<string>::iterator it(m_history.begin());
  for (; it != m_history.end(); ++it)
  {
    printf("%s\n",it->c_str());
  }
#endif
}

void Document::setHistoryEnabled(bool enable)
{
  m_historyEnabled = enable;
}

const std::string & Document::uri() const
{
  return currentHistoryUri();
}

// const char * Document::asText() const
const UnicodeString & Document::asText() const
{
  return m_htmlDocument->data();
}


void Document::dumpDOM()
{
  m_htmlDocument->dumpDOM();
}

const HtmlElement * Document::rootNode() const
{
  // m_htmlDocument->dumpDOM();
  return m_htmlDocument->rootNode();
}

const HtmlElement * Document::titleNode() const
{
  const HtmlElement * title(0);
  if (m_htmlDocument->mimeType() == HtmlDocument::TEXT_HTML)
  {
    const HtmlElement * root(rootNode());
    assert(root->isa(HtmlConstants::HTML_TAG));
    assert(root->hasChildren());
    // firstChild is <HEAD> node
    const ElementList titles = root->firstChild()->elementsByTagName(HtmlConstants::TITLE_TAG);
    if (not titles.empty())
    {
      title = titles.front();
    }
  }
  return title;
}

void Document::reset()
{
  m_status = NOTHING;
  m_htmlDocument->reset();
  m_headerParser->reset();
}

void Document::registerView(ViewI * view)
{
   vector<ViewI*>::iterator it = find(m_views.begin(), m_views.end(), view);
   if (it == m_views.end())
     m_views.push_back(view);
}

void Document::unregisterView(ViewI * view)
{
   vector<ViewI*>::iterator it = find(m_views.begin(), m_views.end(), view);
   if (it != m_views.end())
     m_views.erase(it);
}

unsigned int Document::percentLoaded() const
{
    unsigned int dataExpected = m_headerParser->expected();
    if (dataExpected) {
      return (m_htmlDocument->dataGot()*100) / dataExpected;
    }
    return 0;
}

void Document::appendLocalData(const char * data, int size)
{
  m_headerParser->setDataState();
  if (m_status == NOTHING)
  {
    // need to get mime type from the data...
    magicMimeType(data, size);
  }
  appendData(data, size);
}

void Document::appendData(const char * data, int size)
{
  m_status = INPROGRESS;
  if (size) {
    m_headerParser->feed(data,size);
    unsigned int dataExpected(m_headerParser->expected());
    unsigned int dataGot(m_htmlDocument->dataGot());
    if (dataExpected < dataGot)
    {
      m_htmlDocument->setDataGot(0);
    }
    if (not m_headerParser->redirect().empty())
    {
      currentHistoryUri() = m_headerParser->redirect();
    }
  }
  notifyAll();
}

void Document::notifyAll() const
{
  for_each(m_views.begin(), m_views.end(), mem_fun(&ViewI::notify));
}

void Document::setStatus(Document::Status status)
{
  m_status = status;
  if (m_status == LOADED)
  {
    m_htmlDocument->handleEOF();
    //m_htmlDocument->dumpDOM();
  }
  notifyAll();
}

void Document::refresh(std::string & refreshUrl, int & time) const
{
  m_headerParser->refresh(refreshUrl, time);
}

Document::Status Document::status() const
{
  return m_status;
}


bool Document::hasPreviousHistory() const
{
  HistoryVector::iterator currPosition(m_historyPosition);
  return currPosition != m_history.begin();
}
bool Document::hasNextHistory() const
{
  HistoryVector::iterator currPosition(m_historyPosition);
  if (currPosition == m_history.end())
  {
    return false;
  }
  ++currPosition;
  return currPosition != m_history.end();
}

std::string Document::gotoPreviousHistory()
{
  if (hasPreviousHistory())
  {
    if (m_historyEnabled) {
      --m_historyPosition;
    }
    return currentHistoryUri();
  }
  return "";
}

std::string Document::gotoNextHistory()
{
  if (hasNextHistory())
  {
    if (m_historyEnabled) {
      ++m_historyPosition;
    }
    return currentHistoryUri();
  }
  return "";
}

CookieJar * Document::cookieJar() const
{
  return m_cookieJar;
}

void Document::setCacheFile(const std::string & cacheFile)
{
  m_htmlDocument->setCacheFile(cacheFile);
  if (not cacheFile.empty())
  {
    m_headerParser->setCacheFile(cacheFile+".hdr");
  }
  else
  {
    m_headerParser->setCacheFile(cacheFile);
  }
}

#include <png.h>
#include <gif_lib.h>
void Document::magicMimeType(const char * data, int length)
{
  // this is only for local data - data from http should already have
  // content-type in the headers or in meta or whatever.
  if (length < 3)
  {
    // assume plain text we only have 2 bytes...
    m_htmlDocument->parseContentType("text/plain");
    return;
  }
  else
  {
    if (!png_sig_cmp((png_byte*)data, (png_size_t)0, 3))
    {
      m_htmlDocument->parseContentType("image/png");
    }
    else if (strncmp(GIF_STAMP, data, GIF_VERSION_POS) == 0)
    {
      m_htmlDocument->parseContentType("image/gif");
    }
    else if ((unsigned char)data[0] == 0xFF and (unsigned char)data[1] == 0xD8)
    {
      // naive check, but if it isn't a jpeg, then the proper
      // isJpeg call will catch it...
      m_htmlDocument->parseContentType("image/jpeg");
    }
    // lame hack to get html type
    else if (strncmp(data, "<html>", 6) == 0)
    {
      m_htmlDocument->parseContentType("text/html");
    }
  }
}

void Document::setPosition(int position)
{
  currentHistoryPosition() = position;
}

int Document::position() const
{
  return currentHistoryPosition();
}

std::string & Document::currentHistoryUri()
{
  return m_historyPosition->first;
}

const std::string & Document::currentHistoryUri() const
{
  return m_historyPosition->first;
}

int & Document::currentHistoryPosition()
{
  return m_historyPosition->second;
}

const int & Document::currentHistoryPosition() const
{
  return m_historyPosition->second;
}

bool Document::shouldCache() const
{
  return m_headerParser->shouldCache();
}

