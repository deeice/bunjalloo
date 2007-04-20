#include "URI.h"
#include <algorithm>
#include <functional>

using namespace std;

URI::URI()
  : m_protocol(""),m_address("")
{
}

URI::URI(const std::string & uriString, bool fix):
  m_protocol(""), m_address(""), m_fix(fix)
{
  setUri(uriString);
}

void URI::setUri(const std::string & uriString)
{
  string tmpUri = uriString;
  int sep(tmpUri.find(":"));
  if (sep == -1) {
    if (not m_fix) {
      return;
    }
    tmpUri = "http://" + uriString;
    sep = tmpUri.find(":");
  }
  if (sep != -1) {
    m_protocol = tmpUri.substr(0,sep);
    transform(m_protocol.begin(), m_protocol.end(), m_protocol.begin(), tolower);
    m_address = tmpUri.substr(sep+3, tmpUri.length());
  }
}

bool URI::isFile() const
{
  return m_protocol == "file";
}

bool URI::isValid() const
{
  return m_address != "" and (m_protocol == "http" or m_protocol == "file");
}

int URI::port() const
{
  int firstSlash(m_address.find("/"));
  int portDots(m_address.find(":"));
  if (portDots == -1)
    return 80;
  if (firstSlash == -1 or (firstSlash > portDots))
  {
    int amount = firstSlash==-1?m_address.length():firstSlash;
    amount -= portDots;
    if (amount == 1) {
      return 80;
    }
    string portStr = m_address.substr(portDots+1, amount);
    return strtol(portStr.c_str(), 0, 0);
  }
  return 80;
}

std::string URI::server() const
{
  if (isValid() and not isFile())
  {
    int firstSlash(m_address.find("/"));
    int portDots(m_address.find(":"));
    if (firstSlash == -1 and portDots == -1) {
      return m_address;
    }
    // has / but not : -> http://server/
    if (firstSlash != -1 and portDots == -1)
    {
      return m_address.substr(0, firstSlash);
    }
    // has : but not / ->  http://server:8080
    if (portDots != -1 and (firstSlash == -1 or portDots < firstSlash)) {
      return m_address.substr(0, portDots);
    }
    if (firstSlash != -1) {
      return m_address.substr(0, firstSlash);
    }
    return m_address.substr(0, m_address.length());
  }
  return "";
}

const std::string URI::fileName() const
{
  if (isFile()) {
    return m_address;
  } else {
    // strip off server
    int firstSlash(m_address.find("/"));
    if (firstSlash == -1)
    {
      return "/";
    }
    else
    {
      return m_address.substr(firstSlash, m_address.length()-firstSlash);
    }
    string serverName(server());
    if (m_address.length() == serverName.length()) {
      return "/";
    }
    return m_address.substr(serverName.length(), m_address.length());
  }
}
void URI::navigateTo(const std::string & newFile )
{
  if (newFile.length() == 0)
    return;

  // first see if the newFile is a valid URI
  URI tmp(*this);
  m_address = "";
  m_protocol = "";
  m_fix = false;

  setUri(newFile);
  if (isValid() and m_protocol == tmp.m_protocol)
  {
    // that is all, nothing else to do
    m_fix = tmp.m_fix;
    return;
  }
  *this = tmp;
  // first change this: www.server.com/foo/bar -> navigateTo("/path/to/file") 
  // should go to www.server.com/path/to/file
  if (newFile[0] == '/')
  {
    // ok, so strip off the last part and try again
    string newURI(m_protocol);
    newURI += "://";
    int firstSlash(m_address.find("/"));
    if (firstSlash == -1)
    {
      newURI += m_address + newFile;
    }
    else
    {
      newURI += m_address.substr(0,firstSlash) + newFile;
    }
    setUri(newURI);
  }
  else
  {
    // strip off last part of file and go here.
    string newURI(m_protocol);
    newURI += "://";
    int lastSlash(m_address.rfind("/"));
    if (lastSlash == -1)
    {
      newURI += m_address + "/" + newFile;
    }
    else
    {
      newURI += m_address.substr(0,lastSlash) +"/"+ newFile;
    }
    setUri(newURI);
  }
}

const std::string URI::asString() const
{
  return m_protocol+"://"+m_address;
}

bool URI::operator==(const URI & other)
{
  return m_protocol == other.m_protocol and m_address==other.m_address;
}
bool URI::operator!=(const URI & other)
{
  return not operator==(other);
}
