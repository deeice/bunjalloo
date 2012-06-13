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
*/
#include "libnds.h"
#include "sounds.h"
#include "Options.h"
#include "SoundEffect.h"

const SoundEffect s_soundData[] = {
  {_binary_attack_snd_bin_start, (u32)_binary_attack_snd_bin_size},
  {_binary_beam_snd_bin_start, (u32)_binary_beam_snd_bin_size},
  {_binary_chosen_snd_bin_start, (u32)_binary_chosen_snd_bin_size},
  {_binary_cpustart_snd_bin_start, (u32)_binary_cpustart_snd_bin_size},
  {_binary_electro_snd_bin_start, (u32)_binary_electro_snd_bin_size},
  {_binary_fire_snd_bin_start, (u32)_binary_fire_snd_bin_size},
  {_binary_gooey_snd_bin_start, (u32)_binary_gooey_snd_bin_size},
  {_binary_justice_snd_bin_start, (u32)_binary_justice_snd_bin_size},
  {_binary_menu_snd_bin_start, (u32)_binary_menu_snd_bin_size},
  {_binary_range_snd_bin_start, (u32)_binary_range_snd_bin_size},
  {_binary_scream_snd_bin_start, (u32)_binary_scream_snd_bin_size},
  {_binary_spellstep_snd_bin_start, (u32)_binary_spellstep_snd_bin_size},
  {_binary_spellsuccess_snd_bin_start, (u32)_binary_spellsuccess_snd_bin_size},
  {_binary_urgh_snd_bin_start, (u32)_binary_urgh_snd_bin_size},
  {_binary_walk_snd_bin_start, (u32)_binary_walk_snd_bin_size},
};

class SoundEngine {
  public:
    //! Returns the global instance of the sound enegine.
    static SoundEngine & instance();
  private:
    SoundEngine();
};

SoundEngine::SoundEngine()
{
  setGenericSound(16000, // sample rate
                  127,   // volume
                  64,    // panning
                  1 );   // sound format 1 == 8 bit, 0 == 16 bit

}
SoundEngine & SoundEngine::instance()
{
  static SoundEngine s_soundEngine;
  return s_soundEngine;
}

void SoundEffect::play() const
{
  if (Options::instance().option(Options::SOUND_ENABLE)) {
    SoundEngine::instance();
    playGenericSound(this->data, this->size);
  }
}

