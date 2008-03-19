#ifndef PatchDLDI_h_seen
#define PatchDLDI_h_seen

namespace nds
{
  class PatchDLDI
  {
    public:
      PatchDLDI(const char * filename);

      /** Patch the file.
       * @returns true if patched OK, false otherwise.
       */
      bool patch();

      /** Get the address of the current DLDI patch.
       * @return a pointer to the DLDI data in the current NDS file, or from a file on the PC.
       */
      static const unsigned char * dldiPatch();

      /** Get the address of a 1024 * 128 byte buffer to mess with.
       * @return a pointer to some u16 buffer.
       */
      static unsigned short * buffer();

    private:
      class PatchDLDI_Impl;
      PatchDLDI_Impl * m_impl;
  };
}
#endif
