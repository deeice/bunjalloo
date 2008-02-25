/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "Cache.h"
#include "Document.h"
#include "URI.h"
#include "File.h"

const char * Cache::CACHE_DIR("/"DATADIR"/cache");

Cache::Cache(Document & document, bool useCache, bool clearCache)
: m_document(document), m_useCache(useCache)
{
  m_fileIds.clear();
  if (m_useCache)
  {
    if (clearCache and nds::File::exists(CACHE_DIR) != nds::File::F_NONE)
    {
      nds::File::rmrf(CACHE_DIR);
    }
    if (nds::File::exists(CACHE_DIR) == nds::File::F_NONE and not nds::File::mkdir(CACHE_DIR) )
    {
      // didn't exist, nor can we make it..
      m_useCache = false;
    }
  }
}

std::string uri2CacheFile(const URI & uri)
{
  // strip #internal from URI
  std::string crc = uri.navigateTo(uri.fileName()).crc32();
  std::string cacheFile = Cache::CACHE_DIR;
  // Cache name is 8 letter Hex crc32
  // Headers are stored in cacheFile.hdr
  cacheFile += "/"+crc;
  return cacheFile;
}


bool Cache::contains(const URI & uri, bool stripInternal) const
{
  if (m_useCache)
  {
    CachedMap::const_iterator pos;
    if (stripInternal and not uri.internalLink().empty())
    {
      // has an internal link... strip it, since we are interested in 
      // real page hits
      pos = m_fileIds.find(uri.navigateTo(uri.fileName()).crc32int());
    }
    else
    {
      pos = m_fileIds.find(uri.crc32int());
    }
    return (pos != m_fileIds.end());
  }
  return false;
}

void Cache::feed(const std::string & filename)
{
  nds::File file;
  file.open(filename.c_str());
  // read the lot
  if (file.is_open())
  {
    char buffer[256];
    int read;
    while ( (read = file.read(buffer, 256)) != 0)
    {
      m_document.appendData(buffer, read);
    }
  }
}

void Cache::add(const URI & uri)
{
  m_fileIds[uri.crc32int()] = 1;
}

void Cache::remove(const URI & uri)
{
  if (m_useCache)
  {
    m_document.setCacheFile("");
    std::string cacheFile(uri2CacheFile(uri));
    if (nds::File::exists(cacheFile.c_str()) == nds::File::F_REG)
    {
      nds::File::unlink(cacheFile.c_str());
    }
    if (nds::File::exists(cacheFile.c_str()) == nds::File::F_REG)
    {
      nds::File::unlink((cacheFile+".hdr").c_str());
    }
    m_fileIds.erase(uri.crc32int());
  }
}

bool Cache::load(const URI & uri)
{
  if (m_useCache)
  {
    m_document.setCacheFile("");
    std::string cacheFile(uri2CacheFile(uri));
    if (contains(uri) and nds::File::exists(uri2CacheFile(uri).c_str()) == nds::File::F_REG)
    {
      add(uri);
      m_document.reset();
      feed(cacheFile+".hdr");
      m_document.appendData("\r\n", 2);
      feed(cacheFile);
      return true;
    }
    else
    {
      add(uri);
      m_document.setCacheFile(cacheFile);
    }
  }
  return false;
}

std::string Cache::fileName(const URI & uri) const
{
  if (m_useCache and contains(uri))
  {
    return uri2CacheFile(uri);
  }
  return "";
}
