#ifndef File_h_seen
#define File_h_seen

#include <string>
#include <vector>

namespace nds
{
  class FileImplementation;
  class File
  {
    public:
      
      static const char * base(const char * path);
      //! Create a file object.
      File();

      /** Open a file using the given mode.
       * @param name the file name.
       * @param mode the open mode, "w", "r", etc.
       */
      void open(const char * name, const char * mode="r");

      /** @return true if the file is open.*/
      bool is_open();

      /** closes the file.
       */
      ~File();

      /** Read at most size bytes, returned as an array of bytes.
       * @param buffer An area to read the data into.
       * @param size the amount to read. If ommitted or negative, reads until EOF.
       */
      int read(char * buffer, int size=-1);

      /** Write some bytes to the file.
       * @param buffer the data to write
       * @param size the amount to write. If ommitted or negative, writes until 0 byte.
       */
      int write(const char * buffer, int size=-1);

      /** Get the size of the file in bytes.
       */
      int size();

      /** Read the file as lines.
       * @param lines the lines of the file.
       */
      void readlines(std::vector<std::string> & lines);

      /** Closes the file.*/
      void close();

    private:
      FileImplementation * m_details;
      File(const File &);
      const File operator=(const File &);
  };
}

#endif
