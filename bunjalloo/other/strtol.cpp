#include <string>
#include <iostream>

int main(int arg , char * argv[])
{
  std::string chunkLengthString(argv[1]);
  chunkLengthString += "   ";
  int l = strtol(chunkLengthString.c_str(), 0 , 0);
  std::cout << "length: " << l << std::endl;
  return 0;
}
