#include "SpellData.h"
#include "Wizard.h"
#include "WizardCPU.h"
#include "Arena.h"
#include "Misc.h"
#include "Casting.h"

static void doDisbelieveCast(void);

void cast_disbelieve()
{
  Wizard & player(Wizard::currentPlayer());
  player.setCastAmount(1);
  
  if (player.isCpu()) {
    player.aiCast(WizardCPU::DISBELIEVE);
    
    if (player.hasTargetSquare())
      doDisbelieveCast();
  } else {
    Misc::waitForLetgo();
    Arena & arena(Arena::instance());
    int targetIndex(arena.targetIndex());
    if (arena.at(0,targetIndex) == 0 
        or arena.at(0,targetIndex) >= SPELL_GOOEY_BLOB)
    {
      return;
    }

    doDisbelieveCast();
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
    player.aiCast(WizardCPU::CREATURE);
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
static void doDisbelieveCast(void) 
{
  
  Casting::spellAnimation();
  Casting::setSpellSuccess(false);
  Misc::delay(4, false);
  Arena & arena(Arena::instance());
  int targetIndex(arena.targetIndex());
  if (arena.isIllusion(targetIndex)) {
    arena.doDisbelieve();
    Casting::setSpellSuccess(true);
  }
  Misc::delay(10);
  Casting::printSuccessStatus();
  // set bit 5 for this creature (won't matter if disbelieved already...)
  // bit 5 is "has been disbelieved" and is used for the AI to know which creatures
  // are known to be real and which are not known... 
  arena.setDisbelieved(targetIndex);
}

