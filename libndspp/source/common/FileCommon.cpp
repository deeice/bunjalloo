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
