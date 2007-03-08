#include "Wizard.h"
#include "Arena.h"
#include "Misc.h"
#include "Casting.h"

void cast_disbelieve()
{
  Wizard & player(Wizard::currentPlayer());
  player.setCastAmount(1);
  
  if (player.isCpu()) {
#if 0 
    ai_cast_disbelieve();
    
    if (target_square_found)
      doDisbelieveCast();
#endif
  } else {
    Misc::waitForLetgo();
#if 0
    if (arena[0][target_index] == 0 || arena[0][target_index] >= SPELL_GOOEY_BLOB)
      return;
    
    doDisbelieveCast();
#endif
    player.setCastAmount(0);
  }
  
}

void cast_creature()
{  
  // if human just do the check on target_index, if CPU find the optimum square to cast to
  // if CPU and no square, set a target_square_found = 0 and return withough doing anything.
  // temp_cast_amount = 1;  // set this in Wiz
  Wizard & player(Wizard::currentPlayer());
  player.setCastAmount(1);
  if (player.isCpu()) {
    // do the cpu creature cast ai routine 
    player.aiCastCreature();
  } else {
    // call code at 9856 - validates player's spell cast
    if (not player.isCastAllowed()) {
      return;
    }
    Casting::spellAnimation();
    if (Casting::spellSuccess()) {
      Arena::instance().creatureSpellSucceeds();
    }
    Casting::printSuccessStatus();
    player.setCastAmount(0);
    Arena::instance().drawCreatures();
    Misc::delay(30);
  }
}

void cast_chaos_law()
{}
void cast_trees_castles()
{}
void cast_fire_goo()
{}
void cast_wall()
{}
void cast_magic_missile()
{}
void cast_justice()
{}

void cast_raise_dead()
{}
void cast_subversion()
{}
void cast_magic_shield()
{}
void cast_magic_armour()
{}
void cast_magic_sword()
{}
void cast_magic_knife()
{}
void cast_magic_bow()
{}
void cast_magic_wings()
{}
void cast_shadow_form()
{}

void cast_turmoil()
{}


///////////////////////////////////////////////////////////////////////////////

// implements the casting of disb... 
// code from 9a21 and used by AI and humans
void doDisbelieveCast(void) {
  
  Casting::spellAnimation();
#if 0
  temp_success_flag = 0;
  for (int i = 0; i < 4; ++i)
    swiWaitForVBlank();
  // check arena 3 value, bit 4
  if (IS_ILLUSION(arena[3][target_index])) {
    // place the arena 4 riding wizard, or 0, in arena 0
    arena[0][target_index] = arena[4][target_index];
    arena[4][target_index] = 0;
    if (not Options::instance().option(OLD_BUGS)
        and arena[5][target_index] != 0
        and arena[0][target_index] == 0) 
    {
      // what about dead bodies?
      // but only if arena 4 was empty
      // bug fix v0.7a (disbelieve failed with old bugs turned off)
      arena[0][target_index] = arena[5][target_index]; //creature in arena 5
      arena[2][target_index] = 4; // dead
      arena[5][target_index] = 0; //clear creature in arena 5 
    }
    
    pop_animation();
    temp_success_flag = 1;
  }
  Misc::delay(10);
  Casting::printSuccessStatus();
  // set bit 5 for this creature (won't matter if disbelieved already...)
  // bit 5 is "has been disbelieved" and is used for the AI to know which creatures
  // are known to be real and which are not known... 
  arena[3][target_index] |= 0x20;
#endif
}

