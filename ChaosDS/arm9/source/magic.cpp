#include <stdio.h>
#include "Wizard.h"
#include "Arena.h"
#include "Misc.h"
#include "Casting.h"

void cast_disbelieve()
{}

void cast_creature()
{  
  // if human just do the check on target_index, if CPU find the optimum square to cast to
  // if CPU and no square, set a target_square_found = 0 and return withough doing anything.
  // temp_cast_amount = 1;  // set this in Wiz
  Wizard & player(Wizard::getCurrentPlayer());
  player.setCastAmount(1);
  if (player.isCpu()) {
    // do the cpu creature cast ai routine 
    player.aiCastCreature(); // ai_cast_creature();
  } else {
    
    // call code at 9856 - validates player's spell cast
    if (not player.castAllowed()) {
      return;
    }
    
    Casting::spellAnimation();
#if 0
    spell_animation();
    if (temp_success_flag)
      creature_spell_succeeds(target_index);
    print_success_status();
#endif
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

