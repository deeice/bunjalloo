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
#ifndef Image_h_seen
#define Image_h_seen

class Image
{
  public:
    Image(const char * filename);
    ~Image();

    bool isValid() const;

    unsigned int width() const;
    unsigned int height() const;
    unsigned int paletteSize() const;
    const unsigned short * palette() const;
    

    // RGB data.
    const unsigned char * data() const;

  private:
    bool m_valid;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_paletteSize;
    unsigned char * m_data;
    unsigned short * m_palette;
    
    void readPng(const char *filename);

};
#endif
