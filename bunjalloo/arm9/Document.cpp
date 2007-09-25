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
#include "Document.h"
#include "HtmlDocument.h"
#include "HeaderParser.h"
#include "HtmlElement.h"
#include "CookieJar.h"

using namespace std;

Document::Document():
  m_amount(0),
  m_cookieJar(new CookieJar),
  m_htmlDocument(new HtmlDocument),
  m_headerParser(new HeaderParser(m_htmlDocument,m_cookieJar))
{
  m_history.clear();
  m_historyPosition = m_history.begin();
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
  if (m_history.empty() or uriString != *m_historyPosition)
  {
    // refreshing the same page
    vector<string>::iterator currPosition(m_historyPosition);
    if (hasNextHistory()) {
      ++currPosition;
      m_history.erase(currPosition, m_history.end());
    }
    m_history.push_back(uriString);
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

const std::string & Document::uri() const
{
  return *m_historyPosition;
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
      *m_historyPosition = m_headerParser->redirect();
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
  vector<string>::iterator currPosition(m_historyPosition);
  return currPosition != m_history.begin();
}
bool Document::hasNextHistory() const
{
  vector<string>::iterator currPosition(m_historyPosition);
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
    --m_historyPosition;
    return *m_historyPosition;
  }
  return "";
}

std::string Document::gotoNextHistory()
{
  if (hasNextHistory())
  {
    ++m_historyPosition;
    return *m_historyPosition;
  }
  return "";
}

CookieJar * Document::cookieJar() const
{
  return m_cookieJar;
}
