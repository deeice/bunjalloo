#include "Client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#ifndef ARM9
#include <arpa/inet.h>
#endif
#include <string.h>
#if 0
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
  m_connected(false),
  m_timeout(1)
{ }

Client::~Client()
{
  if (isConnected())
  {
    ::closesocket(m_tcp_socket);
  }
}

bool Client::connect(sockaddr_in & socketAddress)
{
  int addrlen = sizeof(struct sockaddr_in);
  int i(1);
  int iotclResult = ::ioctl(m_tcp_socket, FIONBIO, &i);
  if (iotclResult == -1) {
    debug("iotcl non blocking failed");
    return false;
  }
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
          timeval tv;
          int retval;
          FD_ZERO(&wfds);
          FD_SET(m_tcp_socket, &wfds);
          while (not m_connected) {
            tv.tv_sec = m_timeout;
            tv.tv_usec = 0;
            retval = select(m_tcp_socket+1, NULL, &wfds, NULL, &tv);
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
              debug("No data within 1 second.");
              // keep trying - let the client know what is happening.
              if ( connectCallback() == false ) 
              {
                break;
              }
            }
          }
        }
        break;
      default:
        {
#if 0
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
#if 0
      stringstream dbg;
      dbg << "About to send " << length << " bytes of data" << endl;
      debug(dbg.str().c_str());
#endif
    writeCallback();
    fd_set wfds;
    timeval tv;
    int retval;
    FD_ZERO(&wfds);
    FD_SET(m_tcp_socket, &wfds);
    tv.tv_sec = m_timeout;
    tv.tv_usec = 0;
    retval = select(m_tcp_socket+1, NULL, &wfds, NULL, &tv);
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
#if 0
    {
      stringstream dbg;
      dbg << "Remaining: " << length << " bytes of data" << endl;
      debug(dbg.str().c_str());
    }
#endif
  } while (length);
  debug("Done write\n");
  return total;
}

void Client::read()
{
  if (!isConnected()) {
    debug("read(), Not connected\n");
    return;
  }
  const static int bufferSize(BUFSIZ);
  char buffer[bufferSize];
  int total = 0;
  int notReadyCount = 0;
  while (true)
  {
    readCallback();
    fd_set rfds;
    timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(m_tcp_socket, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    retval = select(m_tcp_socket+1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      debug("Select error");
      break;
    } 
    else if (retval) {
      debug("Data available");
    }
    else
    {
      debug("not ready");
      notReadyCount++;
      if (notReadyCount == 2)
        break;
      continue;
    }
    notReadyCount = 0;
    int amountRead = ::recv(m_tcp_socket, buffer, bufferSize, 0 /*MSG_DONTWAIT*/);
    int tmpErrno = errno;
    if (amountRead <= 0) {
      debug("Error on recv");
      break;
    }
    if (tmpErrno == EWOULDBLOCK) {
      break;
    }
    handle(buffer, amountRead);
    total += amountRead;
#if 0
    stringstream dbg;
    dbg << "Read " << total << " bytes" << endl;;
    debug(dbg.str().c_str());
#endif
    if (finished())
      break;
  }
#if 0
  stringstream dbg;
  dbg << "Done: " << total << " bytes";
  debug(dbg.str().c_str());
#endif
  finish();
}
