#include "Cache.h"
#include "Document.h"
#include "URI.h"
#include "File.h"

const char * Cache::CACHE_DIR("/"DATADIR"/cache");

Cache::Cache(Document & document, bool useCache):m_document(document),m_useCache(useCache)
{
  if (m_useCache)
  {
    if ( nds::File::exists(CACHE_DIR) != nds::File::F_NONE)
    {
      bool result = nds::File::rmrf(CACHE_DIR);
      printf("Cleared cache... %s\n", result?"OK":"NOK");
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
  std::string crc = uri.crc32();
  std::string cacheFile = Cache::CACHE_DIR;
  // Cache name is 8 letter Hex crc32
  // Headers are stored in cacheFile.hdr
  cacheFile += "/"+crc;
  return cacheFile;
}


bool Cache::contains(const URI & uri) const
{
  if (m_useCache)
  {
    std::string cacheFile = "";
    if (m_useCache)
    {
      if (nds::File::exists(uri2CacheFile(uri).c_str()) == nds::File::F_REG)
      {
        return true;
      }
    }
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

bool Cache::load(const URI & uri)
{
  if (m_useCache)
  {
    m_document.setCacheFile("");
    std::string cacheFile(uri2CacheFile(uri));
    if (contains(uri))
    {
      printf("Cache hit %s\n", cacheFile.c_str());
      m_document.reset();
      feed(cacheFile+".hdr");
      m_document.appendData("\r\n", 2);
      feed(cacheFile);
      return true;
    }
    else
    {
      m_document.setCacheFile(cacheFile);
    }
  }
  return false;
}

void Cache::clean(const URI & uri)
{
  if (m_useCache)
  {
    std::string cacheFileName(uri2CacheFile(uri));
    nds::File::unlink(cacheFileName.c_str());
    nds::File::unlink((cacheFileName+".hdr").c_str());
  }
}
