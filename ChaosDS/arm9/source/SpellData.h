#ifndef SpellData_h_seen
#define SpellData_h_seen
#include <nds/jtypes.h>
typedef void (*FunctionPtr_t)(void);
struct SpellData {
  const char * spellName;
  s8 chaosRating;
  u8 castChance;
  u8 castRange;
  u8 castPriority;
  u8 palette;
  u8 combat;
  u8 rangedCombat;
  u8 rangedCombatRange;
  u8 defence;
  u8 movement;
  u8 manvr;
  u8 magicRes;
  FunctionPtr_t spellFunction;
  const u16 * gfxData;
  const u16 * mapData;
#include "SpellDataImpl.h"
};
extern const SpellData s_spellData[];
#endif
