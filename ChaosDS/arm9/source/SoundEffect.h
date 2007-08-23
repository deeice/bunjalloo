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
#ifndef SoundData_h_seen
#define SoundData_h_seen

enum SoundEffect_t
{
  SND_ATTACK,
  SND_BEAM,
  SND_CHOSEN,
  SND_CPUSTART,
  SND_ELECTRO,
  SND_FIRE,
  SND_GOOEY,
  SND_JUSTICE,
  SND_MENU,
  SND_RANGE,
  SND_SCREAM,
  SND_SPELLSTEP,
  SND_SPELLSUCCESS,
  SND_URGH,
  SND_WALK
};

// forward declare the struct...
struct SoundEffect;
extern const SoundEffect s_soundData[];

struct SoundEffect {
  //! The converted sample data.
  const unsigned short * const data;
  //! Size of the sample data.
  unsigned int size;
  /** Plays the sound effect. 
   */
  void play() const;
  /** Helper function to play a sound effect.
   * @param sfx the sound effect to play.
   */
  static inline void play(SoundEffect_t sfx) {
    s_soundData[sfx].play();
  }
};

#endif
