/*
  Copyright (C) 2008 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
