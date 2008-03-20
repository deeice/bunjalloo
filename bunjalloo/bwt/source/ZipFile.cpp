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

  This file incorporates work covered by the following copyright and
  permission notice:

    Copyright (C) 1998-2005 Gilles Vollant

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

*/
#include "unzip.h"
#include "ZipFile.h"
#include "File.h"
using std::string;

class ZipFileImpl
{
  public:
    void open(const char * filename)
    {
      m_filename = filename;
      m_file = unzOpen(filename);
    }

    bool is_open() const
    {
      return m_file != 0;
    }

    void list(std::vector<std::string> & ls)
    {
      unz_global_info gi;
      int err = unzGetGlobalInfo (m_file,&gi);
      if (err!=UNZ_OK)
      {
        printf("error %d with zipfile in unzGetGlobalInfo \n",err);
        return;
      }

      for (int i=0; i < gi.number_entry; i++)
      {
        char filename_inzip[256];
        unz_file_info file_info;
        err = unzGetCurrentFileInfo(m_file,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
        if (err!=UNZ_OK)
        {
          printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
          break;
        }
        ls.push_back(filename_inzip);
        if ((i+1)<gi.number_entry)
        {
          err = unzGoToNextFile(m_file);
          if (err!=UNZ_OK)
          {
            printf("error %d with zipfile in unzGoToNextFile\n",err);
            break;
          }
        }
      }
    }

  private:
    string m_filename;
    unzFile m_file;
};


ZipFile::ZipFile() :
  m_impl(new ZipFileImpl)
{
}
ZipFile::~ZipFile()
{
}

void ZipFile::open(const char * filename)
{
  m_impl->open(filename);
}

void ZipFile::list(std::vector<std::string> & ls)
{
  m_impl->list(ls);
}

void ZipFile::close()
{
}

bool ZipFile::is_open() const
{
  return m_impl->is_open();
}
