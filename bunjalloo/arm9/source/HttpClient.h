#ifndef HttpClient_h_seen
#define HttpClient_h_seen

#include "Client.h"

class Controller;
class URI;

class HttpClient: public nds::Client
{
  public:
    HttpClient(const char * ip, int port);
    void setController(Controller * c);

    void handle(void * bufferIn, int amountRead);
    
    bool connectCallback();

    void writeCallback();
    void readCallback();
    bool finished();

    void finish();

    void debug(const char * s);

    void get(const URI & uri);

  private:
    int  m_total;
    bool m_finished;
    int  m_connectAttempts;
    Controller * m_self;
};

#endif
