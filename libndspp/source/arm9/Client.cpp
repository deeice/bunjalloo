#include "Client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#ifndef ARM9
#include <arpa/inet.h>
#endif
#include <string.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <string>
/*
 * Simple TCP/IP client. 
 * */
using namespace std;
using namespace nds;
#define BUFFER_SIZE 256

Client::Client(const char * ip, int port):
  m_ip(ip),
  m_port(port),
  m_connected(false)
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
  int result = ::connect(m_tcp_socket, (struct sockaddr*)&socketAddress, addrlen);
  if (result != -1)
  {
    m_connected = true;
    stringstream dbg;
    dbg << "Connected to " << m_ip << ":" << m_port;
    debug(dbg.str().c_str());
    return m_connected;
  }
  else
  {
    stringstream dbg;
    dbg << "Unable to connect to\n" << m_ip << ":" << m_port << "\nError:"<<result;
    debug(dbg.str().c_str());
    return false;
  }
}

void
Client::connect()
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
    while (host->h_addr_list[i] != NULL) {
      memcpy(&socketAddress.sin_addr, host->h_addr_list[i], sizeof(struct in_addr));
      stringstream dbg;
      dbg << "Trying: " << host->h_aliases[i] << endl;
      debug(dbg.str().c_str());
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
    {
      stringstream dbg;
      dbg << "About to send " << length << " bytes of data" << endl;
      debug(dbg.str().c_str());
    }
    int sent = ::send(m_tcp_socket, cdata, length, 0);
    if (sent <= 0)
      break;
    cdata += sent;
    length -= sent;
    total += sent;
    // wait for the cores to sync
    {
      stringstream dbg;
      dbg << "Remaining: " << length << " bytes of data" << endl;
      debug(dbg.str().c_str());
    }
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
  const static int bufferSize(BUFFER_SIZE);
  char buffer[bufferSize];
  int total = 0;
  while (true)
  {
    // after the first read, check the end condition
    // this requires a patch to dswifi :-(
    if (total) {
      int potential = ::recv(m_tcp_socket, buffer, bufferSize,MSG_PEEK);
      if (potential == 0) 
      {
        debug("PEEK 0 bytes. Aborting\n");
        break;
      }
    }
    int amountRead = ::recv(m_tcp_socket, buffer, bufferSize,0);
    if (amountRead == -1 or amountRead == 0)
      break;
    handle(buffer, amountRead);
    total += amountRead;
    stringstream dbg;
    dbg << "Read " << total << " bytes" << endl;;
    debug(dbg.str().c_str());
    if (finished())
      break;
  }
  stringstream dbg;
  dbg << "Done: " << total << " bytes";
  debug(dbg.str().c_str());
  finish();
}
