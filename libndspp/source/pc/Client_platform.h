#ifndef Client_platform_h_seen
#define Client_platform_h_seen
#include <sys/ioctl.h>
#include <arpa/inet.h>
#define TIMEOUT 1
#define READ_WAIT_SEC 1
#define READ_WAIT_USEC 0
#define closesocket_platform close
#endif
