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
