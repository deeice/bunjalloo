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
#include "ZipFile.h"

class ZipFileImpl
{
  public:
    void open(const char * filename)
    {
    }

    bool is_open() const
    {
      return false;
    }
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

void ZipFile::list(std::vector<std::string> ls)
{
}

void ZipFile::close()
{
}

bool ZipFile::is_open() const
{
  return m_impl->is_open();
}
