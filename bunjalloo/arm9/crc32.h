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

  This file incorporates work covered by the following copyright and  
  permission notice:  

     crc32.h

     header file for crc32 checksum

     C implementation of CRC-32 checksums for NAACCR records.  Code is based
     upon and utilizes algorithm published by Ross Williams.

     This file contains:
        CRC lookup table
        function CalcCRC32 for calculating CRC-32 checksum
        function AssignCRC32 for assigning CRC-32 in NAACCR record
        function CheckCRC32 for checking CRC-32 in NAACCR record

     Provided by:
        Eric Durbin
        Kentucky Cancer Registry
        University of Kentucky
        October 14, 1998

     Status:
        Public Domain
*/
#ifndef crc32_h_seen
#define crc32_h_seen
#ifdef __cplusplus
extern "C" {
#endif

#define CRC32_XINIT 0xFFFFFFFFL	/* initial value */
#define CRC32_XOROT 0xFFFFFFFFL	/* final xor value */

#define MINIMUM_CHECKSUM_LEN     8
#define MAXIMUM_CHECKSUM_LEN    99

/* NAACCR 6.0 Specifications */
#define NAACCR_60_CHECKSUM_POS  942
#define NAACCR_60_CHECKSUM_LEN  10

/* function prototypes */
unsigned long CalcCRC32 (unsigned char const *, unsigned long, unsigned long,
			 unsigned long);
#if 0
int AssignCRC32 (unsigned char *, unsigned long, unsigned long, unsigned long);
int CompareCRC32 (unsigned char *, unsigned long, unsigned long, unsigned long);
#endif
#ifdef __cplusplus
}
#endif
#endif
