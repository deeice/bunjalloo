#include "matrixSsl.h"
#include "File.h"

extern "C" {
  /* message should contain one %s */
  void matrixStrDebugMsg(char *message, char *value)
  {
    nds::File log;
    log.open("bunjalloo.log", "a");

    log.write("matrix:");
    if (value) {
      char buffer[1024];
      snprintf(buffer, 1024, message, value);
      log.write(buffer);
    } else {
      log.write(message);
    }
  }

  /* message should contain one %d */
  void matrixIntDebugMsg(char *message, int32 value)
  {
    nds::File log;
    log.open("bunjalloo.log", "a");
    char buffer[1024];
    snprintf(buffer, 1024, message, value);
    log.write("matrix:");
    log.write(buffer);
  }

  /* message should contain one %p */
  void matrixPtrDebugMsg(char *message, void *value)
  {
    nds::File log;
    log.open("bunjalloo.log", "a");
    char buffer[1024];
    snprintf(buffer, 1024, message, value);
    log.write("matrix:");
    log.write(buffer);
  }

}
