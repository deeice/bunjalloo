#ifndef MiniMessage_h_seen
#define MiniMessage_h_seen

#include "Background.h"
namespace nds
{
  class MiniMessage
  {
    public:
      MiniMessage(const char * msg);
      void failed();
      void ok();

    private:
      static int s_lastRow;
      int m_x;
      int m_y;
      int m_max;
      int m_pal;
      Background m_background;

      void init();
      void print(const char * msg, int x, int y);
  };
}
#endif
