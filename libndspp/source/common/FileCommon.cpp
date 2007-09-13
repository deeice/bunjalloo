/*
  Copyright (C) 2007 Richard Quirk

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
#include "File.h"

void nds::File::readlines(std::vector<std::string> & lines)
{
  lines.clear();
  if (is_open())
  {
    int size = this->size();
    char * data = new char[size+2];
    read(data);
    int startOfLine = 0;
    for (int i = 0; i < size; ++i)
    {
      if (data[i] == '\n')
      {
        lines.push_back( std::string(&data[startOfLine], i-startOfLine));
        startOfLine = i+1;
      }
    }
    delete [] data;
  }
}

const char * nds::File::base(const char * path)
{
  static std::string str;
  str = path;
  unsigned int pos = str.rfind("/");
  if (pos == std::string::npos)
    return path;
  else
    return &path[pos+1];
}

