#ifndef Misc_h_seen
#define Misc_h_seen
// Hurray! Misc functions

namespace Misc
{
  /*! @brief get a random number
   * @param maximum the range of the random number is 0-maximum
   * @returns a random number 0-@arg maximum
   */
  unsigned int rand(unsigned int maximum);
  /*! @brief set the random number seed
   * @param s the seed to use
   */
  void seedRand(unsigned int s);
  /*! @brief churn the random number once */
  void churnRand();

  /*! @brief trim white space from a string in place.
   * @param str the string to trim
   * @returns pointer to the passed in character array
   */
  char * trim(char * str);
  /*! @brief Order a table of values by the priority in place.  Based on code
   * at c64c.
   * @param count the number of element pairs in the table. i.e a table of 40
   * bytes would have 20 pairs.
   * @param table the table to order
   */
  void orderTable(int count, unsigned char * table);

  bool keypressWait();

  void delay(int time);
}
#endif
