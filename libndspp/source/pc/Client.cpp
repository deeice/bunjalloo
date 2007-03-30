#include "libnds.h"
#include "Client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
/*
 * Simple TCP/IP client. 
 * */
using namespace std;
using namespace nds;
#define BUFFER_SIZE 128

Client::Client(const char * ip, int port):
  m_ip(ip),
  m_port(port),
  m_connected(false)
{ }

Client::~Client()
{
  if (isConnected())
  {
    ::close(m_tcp_socket);
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

  int result = ::connect(m_tcp_socket, (struct sockaddr*)&socketAddress,sizeof(socketAddress));
  if (result == 0)
  {
    m_connected = true;
    //cerr << "Connected to " << m_ip << ":"<<m_port<< endl;
    stringstream dbg;
    dbg << "Connected to " << m_ip << ":" << m_port;
    debug(dbg.str().c_str());
  }
  else
  {
    stringstream dbg;
    dbg << "Unable to connect to\n" << m_ip << ":" << m_port << "\nError:"<<result;
    debug(dbg.str().c_str());
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
    int sent = ::send(m_tcp_socket, cdata, SEND_SIZE, 0);
    if (sent <= 0)
      break;
    cdata += sent;
    length -= sent;
    total += sent;
    // wait for the cores to sync
    for (int i = 0; i < 20; ++i)
      swiWaitForVBlank();
  } while (length);
  return total;
}

void Client::read()
{
  if (!isConnected())
    return;
  const static int bufferSize(BUFFER_SIZE);
  char buffer[bufferSize];
  int amountRead;
  int total = 0;
  while ( (amountRead = ::recv(m_tcp_socket, buffer, bufferSize,0)) != 0)
  {
    handle(buffer, amountRead);
    total += amountRead;
  }
  //cerr << "Read " << total << " bytes" << endl;
  stringstream dbg("Read ");
  dbg << total << " bytes";
  debug(dbg.str().c_str());
  finish();
}
