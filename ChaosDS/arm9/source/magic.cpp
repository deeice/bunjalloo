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
{
  // no need for AI on this one... the spell is really scraping the barrel!
  Wizard & player(Wizard::currentPlayer());
  // if cpu, print player name and spell
  if (player.isCpu()) {
    player.printNameSpell();
    Misc::delay(80);
  }
  
  // do cast chance and update world chaos
  Casting::calculateSpellSuccess();
  
  // print spell success/fail message...
  Casting::printSuccessStatus();
  
  player.setHasTargetSquare(true);
  player.setCastAmount(0);
  
}

void cast_fire_goo(void){
  // these are treated the same as normal creatures
  cast_creature();
}

void cast_trees_castles()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_wall()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}
void cast_magic_missile()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_justice()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_raise_dead()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_subversion()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_magic_shield()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_magic_armour()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_magic_sword()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_magic_knife()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_magic_bow()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_magic_wings()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_shadow_form()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

void cast_turmoil()
{
  Wizard::currentPlayer().setHasTargetSquare(false);
}

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

