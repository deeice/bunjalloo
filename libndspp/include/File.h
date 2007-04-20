#ifndef File_h_seen
#define File_h_seen

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

      /** Get the size of the file in bytes.
       */
      int size();


      /** Closes the file.*/
      void close();

    private:
      FileImplementation * m_details;
  };
}

#endif
