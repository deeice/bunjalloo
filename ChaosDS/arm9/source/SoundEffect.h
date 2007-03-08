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
  const unsigned short * const data;
  unsigned int size;
  /** Plays the sound effect. For example:
   */
  void play() const;
  static inline void play(SoundEffect_t sfx) {
    s_soundData[sfx].play();
  }
};

#endif
