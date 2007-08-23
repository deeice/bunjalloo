/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

// struct SpellData {
public:
  /*! @brief print the name of this spell at the given location
   * @param x the x position
   * @param y the y position
   * @param pal the palette to use. If -1, then uses the palette for the
   * adjusted casting chance.
   */
  void printName(int x, int y, int pal=-1) const;
  /*! @brief get the adjusted casting chance.
   * @return the casting chance taking world chaos and player ability into account
   */
  int realCastChance() const;

  /*! @brief Work out the main colour for this spell/creature.
   * @param frame The frame to use
   * @return the index of the colour that is most used by this creature.
   */
  int mainColour(int frame) const;

  /** Draw the justice graphic frame for the spell. Draws a multicoloured silhoutte.
   * @param frame the frame number to use.
   */
  void drawJusticeGfx(int frame) const;

  /** Is this spell a new feature or was it in the original Chaos?.
   * @return true if the spell is new for the remake. 
   */
  bool isNewFeature() const;

private:
  void fullText(char * str) const;
// }
