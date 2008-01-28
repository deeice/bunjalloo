#include "VideoGL.h"
#ifdef ARM9
#include <nds/arm9/videoGL.h>
#else
#include <GL/gl.h>
#endif

void nds::VideoGL::glLoadIdentity()
{
  ::glLoadIdentity();
}
void nds::VideoGL::glEnd()
{
  ::glEnd();
}

void nds::VideoGL::glPushMatrix()
{
  ::glPushMatrix();
}

void nds::VideoGL::glBindTexture2D(int texture)
{
  ::glBindTexture(
#ifdef GL_TEXTURE_2D
      GL_TEXTURE_2D,
#else
      0,
#endif
      texture);
}


int nds::VideoGL::glGenTexture()
{
#ifdef ARM9
  int i;
#else
  GLuint i;
#endif
  ::glGenTextures(1, &i);
  return i;
}
