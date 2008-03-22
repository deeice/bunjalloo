#include <dirent.h>
#include <string>
#include <stdlib.h>
#include "PatchDLDI.h"

const unsigned char * nds::PatchDLDI::dldiPatch()
{
  static unsigned char * s_data(0);
  delete[] s_data;
  // Read a file from DLDIPATH/*.dldi
  char * dldiPath = getenv("DLDIPATH");
  if (dldiPath == 0)
  {
    return 0;
  }

  DIR * dir = ::opendir(dldiPath);
  if (dir == 0)
  {
    return 0;
  }
  struct dirent * ent;
  std::string filename;
  while ( (ent = ::readdir(dir)) != 0)
  {
    if (strcmp( ent->d_name, ".") != 0 and strcmp(ent->d_name, "..") != 0 )
    {
      filename = (ent->d_name);
      static const std::string dldiExt(".dldi");
      size_t pos = filename.rfind(dldiExt);
      if (pos == (filename.length() - dldiExt.length()))
      {
        // this one
        break;
      }
      filename.clear();
    }
  }
  ::closedir(dir);
  if (not filename.empty())
  {
    filename = std::string(dldiPath) + "/" + filename;
    FILE * fp = fopen(filename.c_str(), "r");
    if (not fp)
      return 0;
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char * data = new unsigned char[fileSize];
    s_data = data;
    /*size_t read = */ fread(data, 1, fileSize, fp);
    return data;
  }
  return 0;
}

unsigned short * nds::PatchDLDI::buffer()
{
  static unsigned short buffer[1024*64];
  return buffer;
}

void nds::PatchDLDI::freeBuffer(unsigned short * /*buffer*/)
{
  // nothing
}
