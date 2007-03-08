#include <nds/jtypes.h>
#include <nds/arm9/sound.h>
#include "sounds.h"
#include "SoundEffect.h"
#include "Options.h"

const SoundEffect s_soundData[] = {
  {_binary_attack_snd_start, (u32)_binary_attack_snd_size},
  {_binary_beam_snd_start, (u32)_binary_beam_snd_size},
  {_binary_chosen_snd_start, (u32)_binary_chosen_snd_size},
  {_binary_cpustart_snd_start, (u32)_binary_cpustart_snd_size},
  {_binary_electro_snd_start, (u32)_binary_electro_snd_size},
  {_binary_fire_snd_start, (u32)_binary_fire_snd_size},
  {_binary_gooey_snd_start, (u32)_binary_gooey_snd_size},
  {_binary_justice_snd_start, (u32)_binary_justice_snd_size},
  {_binary_menu_snd_start, (u32)_binary_menu_snd_size},
  {_binary_range_snd_start, (u32)_binary_range_snd_size},
  {_binary_scream_snd_start, (u32)_binary_scream_snd_size},
  {_binary_spellstep_snd_start, (u32)_binary_spellstep_snd_size},
  {_binary_spellsuccess_snd_start, (u32)_binary_spellsuccess_snd_size},
  {_binary_urgh_snd_start, (u32)_binary_urgh_snd_size},
  {_binary_walk_snd_start, (u32)_binary_walk_snd_size},
};

class SoundEngine {
  public:
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

