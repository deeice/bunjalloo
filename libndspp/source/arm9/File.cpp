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
#include <string>
#include <fat.h>
#include <stdio.h>

using namespace nds;

class FatLibrary
{
  public:
    static FatLibrary & instance();
  private:
    FatLibrary();
    ~FatLibrary();
};

FatLibrary & FatLibrary::instance()
{
  static FatLibrary s_instance;
  return s_instance;
}
FatLibrary::FatLibrary()
{
  fatInitDefault();
}
FatLibrary::~FatLibrary()
{
  // Shut down fat? "should never happen"...
}

class nds::FileImplementation
{
  public:
    FileImplementation();
    void open(const char * name, const char * mode);
    ~FileImplementation();

    int read(char * buffer, int amount=-1);
    int write(const char * buffer, int amount=-1);
    int size();
    bool is_open();
    void close();

  private:
    FILE * m_stream;
  
    FileImplementation(const FileImplementation&);
    const FileImplementation operator=(const FileImplementation&);
};

// delegate class:
FileImplementation::FileImplementation(): m_stream(0)
{
  // make sure we have initialised the fat library.
  FatLibrary::instance();
}

void FileImplementation::open(const char * name, const char * mode)
{
  if (m_stream) {
    close();
  }
  m_stream = fopen(name, mode);
}


FileImplementation::~FileImplementation()
{
  if (m_stream)
    fclose(m_stream);
}

int FileImplementation::read(char * buffer, int amount)
{
  if (not m_stream)
  {
    return 0;
  }

  int bytesExpected = amount;
  if (amount < 0) {
    // read all bytes
    rewind(m_stream);
    bytesExpected = this->size();
  }
  return fread(buffer, 1, bytesExpected, m_stream);
}

int FileImplementation::write(const char * buffer, int amount)
{
  if (not m_stream)
  {
    return 0;
  }

  int bytesExpected = amount;
  if (amount < 0) {
    bytesExpected = strlen(buffer);
  }
  return fwrite(buffer, 1, bytesExpected, m_stream);
}

int FileImplementation::size()
{
  if (not m_stream)
    return 0;

  int current = ftell(m_stream);
  fseek(m_stream, 0, SEEK_END);
  int fileSize = ftell(m_stream);
  fseek(m_stream, current, SEEK_SET);
  return fileSize;
}

bool FileImplementation::is_open()
{
  return m_stream != 0;
}
void FileImplementation::close()
{
  fclose(m_stream);
  m_stream = 0;
}

// File proxy as follows:
File::File():m_details(new FileImplementation())
{}

void File::open(const char * name, const char * mode)
{
  m_details->open(name, mode);
}

File::~File()
{
  delete m_details;
}

int File::read(char * buffer, int amount)
{
  return m_details->read(buffer, amount);
}

int File::write(const char * buffer, int amount)
{
  return m_details->write(buffer, amount);
}

int File::size()
{
  return m_details->size();
}

bool File::is_open()
{
  return m_details->is_open();
}

void File::close()
{
  m_details->close();
}
