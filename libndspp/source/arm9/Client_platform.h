#ifndef Client_platform_h_seen
#define Client_platform_h_seen
#include "nds.h"
#define TIMEOUT 2
#define READ_WAIT_SEC 0
#define READ_WAIT_USEC 500000
#define closesocket_platform closesocket
#define socklen_t int
#endif
