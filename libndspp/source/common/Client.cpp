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
#define DEBUG_WITH_SSTREAM 0
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

using namespace std;
using namespace nds;
const int Client::BUFFER_SIZE(1024+512);

Client::Client(const char * ip, int port):
  m_ip(0),
  m_port(port),
  m_tcp_socket(0),
  m_connected(false),
  m_timeout(TIMEOUT)
{ 
  setConnection(ip, port);
}

Client::~Client()
{
  if (isConnected())
  {
    ::closesocket_platform(m_tcp_socket);
  }
  free(m_ip);
}

void Client::setConnection(const char * ip, int port)
{
  if (m_ip)
    free(m_ip);
  m_ip = (char*)malloc(strlen(ip)+1);
  //printf("%s\n",ip);
  memcpy(m_ip, ip, strlen(ip));
  m_ip[strlen(ip)] = 0;
  //printf("%s\n",m_ip);
  m_port = port;
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
  errno = 0;
  int result = ::connect(m_tcp_socket, (struct sockaddr*)&socketAddress, addrlen);
  int tmperr = errno;
  if (result == 0) {
    // connected immediately
#if DEBUG_WITH_SSTREAM
    stringstream dbg;
    dbg << "immediately connected w/ errno:" << tmperr;
    debug(dbg.str().c_str());
#endif
    m_connected = true;
  }
  else {
    debug("connect result != 0");
    switch (tmperr) {
      case EINPROGRESS:
        {
          debug("connect - EINPROGRESS");
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
              debug("connect - Data is available now.");
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
  debug("Client ip:");
  debug(m_ip);
  // DS - must create the socket moments before using it!
  m_tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in socketAddress = {
    AF_INET,          /* address family */
    htons(m_port),    /* port in network byte order */
    {inet_addr(m_ip)} /* internet address - network byte order */
  };
  debug(m_ip);

  // is it an ip address?
  std::string serverName(m_ip);
  std::string::const_iterator end = std::find_if(serverName.begin(), serverName.end(), ::isalpha);
  if (end != serverName.end())
  {
    // it is not an IP address, it contains letters
    errno = 0;
    struct hostent * host = gethostbyname(m_ip);
#if DEBUG_WITH_SSTREAM
    int tmperrno = errno;
    {
      stringstream dbg;
      dbg << "gethostbyname errno: " << tmperrno << " ";
    }
#endif
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

int Client::read()
{
  fd_set rfds;
  timeval timeout;
  int retval;
  FD_ZERO(&rfds);
  FD_SET(m_tcp_socket, &rfds);

  timeout.tv_sec = m_timeout;
  timeout.tv_usec = 0;

  errno = 0;
  retval = select(m_tcp_socket+1, &rfds, NULL, NULL, &timeout);
  int tmperr = errno;
  if (retval == -1) {
#if DEBUG_WITH_SSTREAM
    stringstream dbg;
    dbg << "select error: " << tmperr;
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
    // on the DS this plays tricks with us.
    // May return not ready when the socket has actually been closed.
    // In that case, recv will return -1 with errno == ESHUTDOWN.
#ifndef ARM9
    return RETRY_LATER;
#endif
  }
  char buffer[BUFFER_SIZE];
  errno = 0;
  int amountRead = ::recv(m_tcp_socket, buffer, BUFFER_SIZE, 0 /*MSG_DONTWAIT*/);
  tmperr = errno;
  if (amountRead < 0) {
    debug("Error on recv");
#if DEBUG_WITH_SSTREAM
    stringstream dbg;
    dbg << "Error on recv errno: " << tmperr;
    debug(dbg.str().c_str());
#endif
    // if we read and there is an error
    if (tmperr == EAGAIN)
    {
      // not ready yet - from man recv:
      // EAGAIN The socket is marked non-blocking and the receive operation
      // would block.
      return RETRY_LATER;
    }
    return READ_ERROR;
  }
  if (amountRead == 0)
  {
    // man recv(2):
    // The return value will be 0 when the peer has performed an orderly shutdown.
    debug("Read 0 bytes");
#if DEBUG_WITH_SSTREAM
    stringstream dbg;
    dbg << "Read 0 errno: " << tmperr;
    debug(dbg.str().c_str());
#endif
    if (tmperr == EINVAL or tmperr == ESHUTDOWN) {
      debug("EINVAL or ESHUTDOWN");
      return READ_ERROR;
    }
    return amountRead;
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
