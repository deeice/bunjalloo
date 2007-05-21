#ifndef Rectangle_h_seen
#define Rectangle_h_seen

struct Rectangle
{
  int x;
  int y;
  int w;
  int h;
  bool hit(int x, int y) const;
};

#endif
