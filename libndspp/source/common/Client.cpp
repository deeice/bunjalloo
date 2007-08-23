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
#define DEBUG_WITH_SSTREAM 1
#include "Client.h"
#include "Client_platform.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#if DEBUG_WITH_SSTREAM
#include <sstream>
#endif
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <string>
#include <errno.h>
/*
 * Simple TCP/IP client. 
 * */
using namespace std;
using namespace nds;

Client::Client(const char * ip, int port):
  m_ip(ip),
  m_port(port),
  m_tcp_socket(0),
  m_connected(false),
  m_timeout(TIMEOUT)
{ }

Client::~Client()
{
  if (isConnected())
  {
    ::closesocket_platform(m_tcp_socket);
  }
}

void Client::disconnect()
{
  if (isConnected())
    ::closesocket_platform(m_tcp_socket);
  m_connected = false;
}

void Client::makeNonBlocking() {
  int i(1);
  int iotclResult = ::ioctl(m_tcp_socket, FIONBIO, &i);
  if (iotclResult == -1) {
    debug("iotcl non blocking failed");
    // never happens on DS...
  }
}

bool Client::connect(sockaddr_in & socketAddress)
{
  makeNonBlocking();
  socklen_t addrlen = sizeof(struct sockaddr_in);
  int result = ::connect(m_tcp_socket, (struct sockaddr*)&socketAddress, addrlen);
  if (result == 0) {
    // connected immediately
    m_connected = true;
  }
  else {
    switch (errno) {
      case EINPROGRESS:
        {
          // select connect for write
          fd_set wfds;
          timeval timeout;
          int retval;
          FD_ZERO(&wfds);
          FD_SET(m_tcp_socket, &wfds);
          int tries = 0;
          while (not m_connected) {
            timeout.tv_sec = m_timeout;
            timeout.tv_usec = 0;
            retval = select(m_tcp_socket+1, NULL, &wfds, NULL, &timeout);
            if (retval == -1) {
              debug("Select error");
              m_connected = false;
              return m_connected;
            }
            else if (retval) {
              debug("Data is available now.");
              m_connected = true;
              break;
            }
            else {
              debug("No data within m_timeout seconds.");
              // keep trying 
              tries++;
              if (tries == 3)
              {
                break;
              }
            }
          }
        }
        break;
      default:
        {
#if DEBUG_WITH_SSTREAM
          stringstream dbg;
          dbg << "Unable to connect to\n" << m_ip << ":" << m_port << "\nError:"<<result;
          debug(dbg.str().c_str());
#endif
          return false;
        }
        break;
    }
  }
  return m_connected;
}

void Client::connect()
{
  if (!m_ip)
    return;
  // DS - must create the socket moments before using it!
  m_tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in socketAddress = {
    AF_INET,          /* address family */
    htons(m_port),    /* port in network byte order */
    {inet_addr(m_ip)} /* internet address - network byte order */
  };

  // is it an ip address?
  std::string serverName(m_ip);
  std::string::const_iterator end = std::find_if(serverName.begin(), serverName.end(), ::isalpha);
  if (end != serverName.end())
  {
    // it is not an IP address, it contains letters
    struct hostent * host = gethostbyname(m_ip);
    int i = 0;
    while (host and host->h_addr_list[i] != NULL) {
      memcpy(&socketAddress.sin_addr, host->h_addr_list[i], sizeof(struct in_addr));
#if DEBUG_WITH_SSTREAM
      stringstream dbg;
      dbg << "Trying: " << i << " ";
#ifdef ARM9
      dbg << "(" << m_ip;
#else
      dbg << "(" << inet_ntoa( *( struct in_addr*)( host->h_addr_list[i]));
#endif
      dbg << ":" << m_port << ")" << endl;
      debug(dbg.str().c_str());
#endif
      if ( this->connect(socketAddress) ) {
        break;
      }
      i++;
    }
  } else {
    this->connect(socketAddress);
  }
}


unsigned int Client::write(const void * data, unsigned int length)
{
  // can only send "buffer" amount of data, and when the buffer is full we need to wait
  unsigned int total(0);
  char * cdata = (char*)data;
#define SEND_SIZE 2048
  do
  {
#if DEBUG_WITH_SSTREAM
    {
      stringstream dbg;
      dbg << "About to send " << length << " bytes of data";
      debug(dbg.str().c_str());
    }
#endif
    //writeCallback();
    fd_set wfds;
    timeval timeout;
    int retval;
    FD_ZERO(&wfds);
    FD_SET(m_tcp_socket, &wfds);
    timeout.tv_sec = m_timeout;
    timeout.tv_usec = 0;
    retval = select(m_tcp_socket+1, NULL, &wfds, NULL, &timeout);
    if (retval < 0) {
      continue;
    }
    int sent = ::send(m_tcp_socket, cdata, length, 0);
    if (sent <= 0)
      break;
    cdata += sent;
    length -= sent;
    total += sent;
    // wait for the cores to sync
#if DEBUG_WITH_SSTREAM
    {
      stringstream dbg;
      dbg << "Remaining: " << length << " bytes of data";
      debug(dbg.str().c_str());
    }
#endif
  } while (length);
  debug("Done write");
  return total;
}

const int Client::CONNECTION_CLOSED(0);
const int Client::READ_ERROR(-1);

int Client::read()
{
  const static int bufferSize(BUFSIZ);
  char buffer[bufferSize];
  fd_set rfds;
  timeval timeout;
  int retval;
  FD_ZERO(&rfds);
  FD_SET(m_tcp_socket, &rfds);
  // The problem here is that Linux triggers the select when the 
  // connection is closed by the peer.
  // The DS does not, so when all data has been returned, it will wait an extra select
  // (until timeout) to discover that the peer has shut the connection.
  //timeout.tv_sec = READ_WAIT_SEC;
  //timeout.tv_usec = READ_WAIT_USEC;
  timeout.tv_sec = m_timeout;
  timeout.tv_usec = 0;
  retval = select(m_tcp_socket+1, &rfds, NULL, NULL, &timeout);
  if (retval == -1) {
#if DEBUG_WITH_SSTREAM
    stringstream dbg;
    dbg << "select error: " << errno;
    debug(dbg.str().c_str());
#endif
    return READ_ERROR;
  } 
  else if (retval) {
    debug("Data is readable");
  }
  else
  {
    debug("not ready");
    return READ_ERROR;
  }
  int amountRead = ::recv(m_tcp_socket, buffer, bufferSize, 0 /*MSG_DONTWAIT*/);
  int tmpErrno = errno;
  if (amountRead < 0) {
    debug("Error on recv");
    return READ_ERROR;
  }
  if (tmpErrno == EWOULDBLOCK) {
    debug("EWOULDBLOCK");
    return READ_ERROR;
  }
  if (amountRead == 0)
  {
    // man recv(2):
    // The return value will be 0 when the peer has performed an orderly shutdown.
    debug("Read 0 bytes");
    return CONNECTION_CLOSED;
/*
#ifdef ARM9
    return amountRead;
#else
    // needs to be different!
    return -1;
#endif
*/
  }
  handle(buffer, amountRead);
#if DEBUG_WITH_SSTREAM
  stringstream dbg;
  dbg << "Read " << amountRead << " bytes";
  debug(dbg.str().c_str());
#endif
  return amountRead;
}

void Client::setTimeout(int timeout)
{
  m_timeout = timeout;
}
