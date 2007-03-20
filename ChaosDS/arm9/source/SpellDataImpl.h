
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

  /** Returns if the spell is in range of the wizard. Uses the wizard index and target index from arena.
   * @return true if the spell is in range, false otherwise.
   */
  bool isSpellInRange() const;

  void drawJusticeGfx(int frame) const;

private:
  void fullText(char * str) const;
// }
