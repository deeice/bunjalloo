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
#ifndef Options_h_seen
#define Options_h_seen

#include "TouchScreen.h"


//! object that represents a single option
class Option
{
  public:
    //! Type of options available
    enum Option_t {
      ON_OFF,  //!< can be either on or off
      NUMBER,  //!< is a number
      ROUND_LIMIT, //!< is a number (>2) or off (0) or default behaviour (1)
      NONE         //!< is a simple option with no value associated
    };
    //! For ON_OFF types Option_t
    enum OptionChoice_t {
      OFF,
      ON
    };

    /** @brief Encapsulate an option
     * @param name the name to be displayed
     * @param type the type @see Option_t
     * @param min the minimum value
     * @param max the maximum value
     */
    Option(const char * name, 
        Option_t type,
        unsigned short min,
        unsigned short max);

    /** Retrieve the value as a textual representation
     * @param buffer a text buffer to store the return value in
     */
    void asText(char * buffer) const;

    /** Set the value
     * @param value the new value
     */
    void setValue(unsigned short value);
    /** Increase the current value */
    unsigned short operator++(int);
    /** Decrease the current value */
    unsigned short operator--(int);

    /** Get the current value */
    operator int() const;

    /** Get the current value as a boolean. */
    operator bool() const;

    //! text value of the option on screen
    const char * m_name;
  private:
    //! on off, etc
    Option_t m_type;
    //! min value
    unsigned short m_min;
    //! max value
    unsigned short m_max;

    //! value held by the option
    unsigned short m_value;

    static const char * const ON_STRING;
    static const char * const OFF_STRING;
    static const char * const DEFAULT_STRING;

};

//! screen to show the options
class OptionScreen : public TouchScreen
{
  public:
    OptionScreen();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();

  private:
    int m_hilightItem;
    static void incrOptionCb(void * arg);
    static void decrOptionCb(void * arg);
    static void activateOptionCb(void * arg);
    void drawOptions() const;
    void drawOption(Option & opt, int x, int y, int pal) const;

    void deselectOption(int item) const;
    void selectOption(int item) const;

    void a();
    void up();
    void down();
    void left();
    void right();
    void back();

    void selectFromTouch(); 

};

//! Options class helps to fiddle about with the list of Option values.
class Options
{
  public:
    //! A value that corresponds to using the default round calculation.
    static const int DEFAULT_ROUNDS;
    //! Total number of options.
    static const int OPTION_COUNT;
    //! Position of the options on screen.
    enum OptionPosition_t
    {
      ROUND_LIMIT,
      OLD_BUGS,
      NEW_FEATURES,
      ONE_TOUCH, //!< touch to move, instead of touch to select.
      SOUND_ENABLE,
      SOUND_TEST,
      BACK
    };
    //! Get the options instance
    static Options & instance();
    //! Get a pointer to the options
    Option * options() const;
    //! Get a singe option
    Option & option(OptionPosition_t) const;

  private:
    static Option s_option[];
    Options();
    ~Options();
};

#endif
