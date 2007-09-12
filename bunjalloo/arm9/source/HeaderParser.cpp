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
#include <assert.h>
#include "HeaderParser.h"
#include "HtmlElement.h"
#include "HtmlParser.h"
#include "CookieJar.h"
#include "URI.h"

using namespace std;
static const string HTTP1("HTTP/1.");
static const int HTTP1_LEN = HTTP1.length();

HeaderParser::HeaderParser(HtmlParser * htmlParser, CookieJar * cookieJar):
  m_uri(*(new URI())),
  m_expected(0),
  m_htmlParser(htmlParser),
  m_cookieJar(cookieJar)
{
  reset();
}
HeaderParser::~HeaderParser()
{ }

void HeaderParser::reset()
{
  m_state = HTTP_RESPONSE;
  m_redirect = "";
  m_chunked = false;
  m_chunkLength = 0;
  m_chunkLengthString = "";
  m_htmlParser->setToStart();
  m_uri.setUri("");
}

void HeaderParser::setUri(const std::string & uri)
{
  m_uri.setUri(uri);
}

void HeaderParser::rewind()
{
  m_position = m_lastPosition;
}

void HeaderParser::next()
{
  m_value = *m_position;
  m_lastPosition = m_position;
  m_position++;
}

void HeaderParser::feed(const char * data, unsigned int length)
{
  //cout << "\nFeed " << length << " bytes." << endl;
  m_position = data;
  m_end = data+length;
  while (m_position < m_end)
  {
    next();
    switch (m_state)
    {
      case HTTP_RESPONSE:
        httpResponse();
        break;
      case BEFORE_FIELD:
        beforeField();
        break;
      case FIELD:
        field();
        break;
      case AFTER_FIELD:
        afterField();
        break;
      case BEFORE_VALUE:
        beforeValue();
        break;
      case VALUE:
        value();
        break;
      case ENDING_HEADERS:
        endingHeaders();
        break;
      case CHUNK_LINE:
        chunkLine();
        break;
      case PARSE_ERROR:
        parseError();
        break;
      case DATA:
        fireData();
        break;
    }
  }
}

const std::string HeaderParser::redirect() const
{
  return m_redirect;
}


unsigned int HeaderParser::expected() const
{
  return m_expected;
}


void HeaderParser::handleHeader(const std::string & field, const std::string & value)
{
  if (field == "transfer-encoding" and value == "chunked") {
    m_chunked = true;
  } 
  else if (field == "location" and m_httpStatusCode >= 300 and m_httpStatusCode < 400)
  {
    m_redirect = value;
  }
  else if (field == "content-length") {
    m_expected = strtol(value.c_str(), 0 , 0);
  } 
  else if (field == "content-type") {
    m_htmlParser->parseContentType(value);
  }
  else if (field == "refresh") {
    m_htmlParser->parseRefresh(value);
  }
  else if (field == "set-cookie")
  {
    m_cookieJar->addCookieHeader(m_uri, value);
  }
}

void HeaderParser::parseError()
{
  if (m_chunked and m_chunkLength == 0) {
    return;
  }
  // woops "this should never happen" so catch it when it does.
  assert(m_state != PARSE_ERROR);
  m_position = m_end;
}

void HeaderParser::beforeField()
{
  if (::isalpha(m_value)) {
    m_value = ::tolower(m_value);
    m_field = m_value;
    m_state = FIELD;
  } else {
    if (m_value == '\r') {
      next();
    }
    if (m_value == '\n') {
      m_state = ENDING_HEADERS;
      rewind();
    }
  }
  // else parse error, but keep going...
}

void HeaderParser::endingHeaders()
{
  if (m_chunked) {
    m_state = CHUNK_LINE;
    m_chunkLengthString = "0x";
  }
  else
  {
    m_state = DATA;
  }
}

bool HeaderParser::isFieldValue() const
{
  return (m_value == '-' 
      or m_value == '_' 
      or ::isalnum(m_value));

}

void HeaderParser::field()
{
  if (isFieldValue()) {
    m_value = ::tolower(m_value);
    m_field += m_value;
  } else {
    switch (m_value) {
      case ' ':
        m_state = AFTER_FIELD;
        break;
      case ':':
        m_state = BEFORE_VALUE;
        break;
      default:
        // parse error?
        m_state = PARSE_ERROR;
        // printf("%s",m_position-1);
        assert(m_state != PARSE_ERROR);
        break;
    }
  }
}
void HeaderParser::afterField()
{
  if (m_value == ':') {
    m_state = BEFORE_VALUE;
  } else {
    switch (m_value) {
      case ' ':
        // keep looking for :
        m_state = AFTER_FIELD;
        break;
      default:
        // parse error.
        m_state = PARSE_ERROR;
        assert(m_state != PARSE_ERROR);
        break;
    }
  }
}
void HeaderParser::beforeValue()
{
  switch (m_value) {
    case ' ':
      break;
    case '\n':
      m_state = PARSE_ERROR;
      assert(m_state != PARSE_ERROR);
      break;
    default:
      m_headerValue = m_value;
      m_state = VALUE;
      break;
  }
}
void HeaderParser::value()
{
  switch (m_value) {
    case '\r':
      m_state = BEFORE_FIELD;
      next();
      // allow \n or \r\n 
      if (m_value != '\n') {
        rewind();
      } 
      break;
    default:
      m_headerValue += m_value;
      break;
  }
  if (m_state != VALUE)
  {
    handleHeader(m_field,m_headerValue);
  }
}

void HeaderParser::chunkLine()
{
  if (::isxdigit(m_value) or m_value == ' ')
  {
    m_chunkLengthString += m_value;
  }
  else
  {
    m_state = PARSE_ERROR;
    if (m_value == '\r') {
      next();
    }
    if (m_value == '\n') {
      m_chunkLength = strtol(m_chunkLengthString.c_str(),0,0);
      m_expected = m_chunkLength;
      m_state = DATA;
    }
  }
}

void HeaderParser::setDataState()
{
  m_state = DATA;
}

unsigned int HeaderParser::httpStatusCode() const
{
  return m_httpStatusCode;
}

void HeaderParser::httpResponse()
{
  string response;
  while (m_value != '\n') {
    response += ::toupper(m_value);
    next();
  }
  if (response.substr(0,HTTP1_LEN) == HTTP1 and response[HTTP1_LEN+1] == ' ') {
    m_httpStatusCode = strtol(response.substr(9,3).c_str(), 0, 0);
    m_state = BEFORE_FIELD;
  } else {
    m_state = PARSE_ERROR;
    assert(m_state != PARSE_ERROR);
  }
}
  
void HeaderParser::fireData()
{
  rewind();
  int length = (m_end - m_position);
  if (m_chunked) {
    if (length >= m_chunkLength) {
      length = m_chunkLength;
      m_state = BEFORE_FIELD;
      m_chunkLengthString = "";
      m_chunkLength = 0;
    }
    else 
    {
      m_chunkLength -= length;
    }
  }
  // once done, feed the data to the html parser.
  m_htmlParser->feed(m_position, length);
  m_position += length;
}
