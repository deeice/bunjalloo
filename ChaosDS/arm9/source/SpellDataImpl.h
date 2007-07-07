
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
