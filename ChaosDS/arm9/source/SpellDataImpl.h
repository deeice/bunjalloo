
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
   * @returns the casting chance taking world chaos and player ability into account
   */
  int realCastChance() const;

private:
  void fullText(char * str) const;
// }
