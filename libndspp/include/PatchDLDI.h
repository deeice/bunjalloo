/*
  Copyright (C) 2008 Richard Quirk

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

      /** Free the buffer.
       */
      static void freeBuffer(unsigned short * buffer);

    private:
      class PatchDLDI_Impl;
      PatchDLDI_Impl * m_impl;
  };
}
#endif
