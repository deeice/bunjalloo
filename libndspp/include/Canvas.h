#ifndef Canvas_h_seen
#define Canvas_h_seen

namespace nds
{
  class Background;

  class Canvas
  {
    public:
      static Canvas & instance();
      void drawPixel(int x, int y, int color);
      void fillRectangle(int x, int y, int w, int h, int color);
      void drawRectangle(int x, int y, int w, int h, int color);
      void verticalLine(int x, int y, int length, int colour);
      void horizontalLine(int x, int y, int length, int colour);
      int width() const;
      int height() const;
      void endPaint();

    private:
      Background * m_bgMain;
      Background * m_bgSub;
      unsigned short * m_frontMain;
      unsigned short * m_backMain;
      unsigned short * m_frontSub;
      unsigned short * m_backSub;

      Canvas();
      ~Canvas();

      Canvas(const Canvas &);
      const Canvas operator=(const Canvas &);
      

  };
}
#endif
