#!/usr/bin/env python
""" Convert a True Type font file to png and map file """
import sys
import struct
import os.path
import Image, ImageDraw, ImageFont

## Align value to the given alignment
# @param value the value to align
# @param alignment the even value to align to
# @return aligned value
# 
def align(value, alignment):
  if value & (alignment-1):
    value += alignment - (value&(alignment-1))
  return value

## Crappy file wrapper that can write raw bytes.
class FileWrapper:
  ## Constructor.
  # @param self the self reference.
  # @param fileName the name of the file to open
  # @param mode the open mode
  def __init__(self, fileName, mode='r'):
    self.__fp = open(fileName, mode)

  ## Write a string or whatever to the file
  # @param self the self reference.
  # @param whatever the stuff to write
  def write(self, whatever):
    self.__fp.write(whatever)

  ## Write a value as an 8 bit "byte"
  # @param self the self reference.
  # @param uint8 the value to write, it is truncated to 8 bits
  def write8(self, uint8):
    self.__fp.write(struct.pack('B',uint8&0xff))

  ## Write a value as a 16 bit halfword
  # @param self the self reference.
  # @param uint16 the value to write, it is truncated to 16 bits
  def write16(self, uint16):
    assert uint16 < 0x10000
    self.write8(uint16>>8)
    self.write8(uint16)

  ## Write a value as a 32 bit word
  # @param self the self reference.
  # @param uint32 the value to write
  def write32(self, uint32):
    self.write8(uint32>>24)
    self.write8(uint32>>16)
    self.write8(uint32>>8)
    self.write8(uint32)

  ## Close the file
  def close(self):
    self.__fp.close()


## SubFont class to wrap a TrueType Font
class SubFont:
  ## Constructor.
  # @param self the self reference.
  # @param fileName the name of the TTF file
  # @param size the height of the font
  def __init__(self, fileName, size):
    self.__fileName = fileName
    self.__ranges = []
    self.__height = 0
    self.__width = 0
    self.__totalWidth = 0
    self.__ttf = ImageFont.truetype(fileName, size)

  ## Set the height attribute of the font
  def setHeight(self, height):
    self.__height = height

  ## Add a range of glyphs to export.
  # @param self the self reference.
  # @param lower the lower boundary of the range.
  # @param upper the upper boundary of the range.
  def addRange(self, lower, upper):
    self.__ranges.append((lower, upper))
    self.recalculate()

  ## Fetch the current ranges in one big range
  # @return list of integer values representing glyph indices.
  def glyphRanges(self):
    ranges = []
    for rangeTuplet in self.__ranges:
      ranges.extend(range(rangeTuplet[0], rangeTuplet[1]))
    return ranges

  ## Fetch the current height.
  def height(self):
    return self.__height

  ## Fetch the basename of the font file.
  def basename(self):
    return os.path.basename(self.__fileName)

  ## Fetch the font file name.
  def fileName(self):
    return self.__fileName

  ## Fetch the total width of the font (used ranges).
  def totalWidth(self):
    return self.__totalWidth

  ## Recalculate the font image layout based on the ranges.
  def recalculate(self):
    self.__totalWidth = 0
    for i in self.glyphRanges():
      w, h = self.__ttf.getsize(unichr(i))
      if (w > self.__width):
        self.__width = w
      if (h > self.__height):
        self.__height = h
      # align 2 bytes
      self.__totalWidth += align(w, 2)
    self.__totalWidth = align(self.__totalWidth, 8)

  ## Get the TrueType Font.
  def ttf(self):
    return self.__ttf

  ## Save the font map.
  # Map file format as follows:
  #   u16 height;
  #   u16 totalWidth;
  #   u16 glyphCount;
  #   u16 rangeCount;
  #   struct {
  #     u32 low;
  #     u32 high;
  #   } ranges[rangeCount]
  #   struct {
  #     u8 width;
  #     u16 position;
  #   } glyph[glyphCount];
  # glyphCount is the sum of all the ranges;
  def save(self):
    fp = FileWrapper(self.basename()+'.map', 'w')
    fp.write16(self.__height)
    fp.write16(self.__totalWidth)
    fp.write16(len(self.glyphRanges()))
    # write the glyph ranges:
    fp.write16(len(self.__ranges))
    for rangeTuplet in self.__ranges:
      fp.write32(rangeTuplet[0])
      fp.write32(rangeTuplet[1])

    position = 0
    # save sizes of the glyphs - all have the same fixed height, so just save the width.
    for i in self.glyphRanges():
      #fp.write('%2d // h: %2d val:'%self.glyphSize(unichr(i)))
      w, h = self.__ttf.getsize(unichr(i))
      w = align(w, 2)
      #print unichr(i) +' '+ str(position) +' ' +str(w)
      fp.write8(w)
      fp.write16(position)
      position += w
      #fp.write('0x%03X\n'%i)
    fp.close()

## Create a bitmap font.
class BitmapFont:
  ## Constructor.
  # @param self the self reference.
  # @param fileName the name of the TTF file.
  # @param size the height of the font.
  def __init__(self, fileName, size=8):
    self._subfont = SubFont(fileName, size)
    self._draw = None
    self._image = None

  ## Save the bitmap image to a file. The filename is basename+.png.
  # Also saves the subfont details.
  def save(self):
    self._image = Image.new("RGB", (self._subfont.totalWidth(), self._subfont.height()), 'white')
    self._draw = ImageDraw.Draw(self._image)
    self._draw.setfont(self._subfont.ttf())
    position = 0
    for i in self._subfont.glyphRanges():
      self._draw.text((position, 0), unichr(i), fill='black')
      w, h = self._subfont.ttf().getsize(unichr(i))
      position += align(w, 2)
    self._image.save(self._subfont.basename()+'.png','PNG')

    self._subfont.save()

  ## Add a range of glyphs to the sub font.
  # @param self the self reference.
  # @param rangeArg the range, a string like "0,100"
  def addRange(self, rangeArg):
    bounds = rangeArg.split(',')
    self._subfont.addRange(int(bounds[0],0),int(bounds[1],0))

if __name__ == '__main__':

  if len(sys.argv) == 0:
    print "Usage ttf2png.py ttf height [range,range ...]"

  if len(sys.argv) > 2:
    f = BitmapFont(sys.argv[1], int(sys.argv[2]))
  elif len(sys.argv) > 1:
    f = BitmapFont(sys.argv[1])

  if len(sys.argv) > 3:
    for r in sys.argv[3:]:
      f.addRange(r)
  else:
    # If no range given, export basic ASCII, extended ASCII and Euro symbol
    f.addRange('0x1f,0x7f')
    f.addRange('0xa0,0x100')
    f.addRange('0x20AC,0x20AD')
    f.addRange('8211,8212')
    f.addRange('8216,8218')
    f.addRange('8220,8221')
    f.addRange('8226,8226')
  f.save()
