#include "ndspp.h"
#include "SpellData.h"
#include "Wizard.h"
#include "WizardCPU.h"
#include "Arena.h"
#include "Misc.h"
#include "Casting.h"
#include "Graphics.h"
#include "magic.h"
#include "Text16.h"

using nds::Color;
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
  // if CPU and no square, set a player.setHasTargetSquare(false) and return withough doing anything.
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
  Wizard & player(Wizard::currentPlayer());
  int currentSpell(player.selectedSpellId());
  if (player.castAmount() == 0) {
    if (currentSpell >= SPELL_MAGIC_CASTLE) {
      player.setCastAmount(1);
    } else { 
      player.setCastAmount(8);
    }
  }
  if (player.isCpu())
  {
    // jump 9b00
    if (currentSpell < SPELL_MAGIC_CASTLE) {
      // shadow wood or magic wood - uses the same routine
      // jump 9b1c (print player name and call magic wood casting...)
      player.setCastAmount(8);
      player.printNameSpell();
      Misc::delay(80);
      player.aiCast(WizardCPU::TREES);
      
    } else {
      // check if spell is worth casting...
      Arena & arena(Arena::instance());
      int creature(arena.at(0,arena.startIndex()));
      if (creature < SPELL_MAGIC_CASTLE or creature > SPELL_DARK_CITADEL)
      {
        player.setCastAmount(1);
        player.printNameSpell();
        Misc::delay(80);
        player.aiCast(WizardCPU::TREES);
        player.setHasTargetSquare(true);
      } else {
        player.setHasTargetSquare(false);
      }
    }
    
  } else {
    // not CPU
    if (currentSpell == SPELL_MAGIC_WOOD) {
      // do magic wood spell cast...
      player.setCastAmount(8);
      player.aiCast(WizardCPU::TREES);
    } else {
      // do same checks as for creatures
      if (not player.isCastAllowed()) {
        return;
      }
      Casting::spellAnimation();
      
      if (Casting::spellSuccess()) {
        Arena::instance().creatureSpellSucceeds();
        player.setCastAmount(player.castAmount()-1);
      } else {
        player.setCastAmount(0);
      }
      
      if (player.castAmount() == 0)
        Casting::printSuccessStatus();
      
      Arena::instance().drawCreatures();
      Misc::delay(10);
    }
  } 
}

void cast_wall()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.castAmount() == 0) 
    player.setCastAmount(4);

  if (player.isCpu()) {
    // 9b85 = wall ai casting
    player.aiCast(WizardCPU::WALL);
    if (player.hasTargetSquare()) {
      Casting::printSuccessStatus();
      Misc::delay(30);
    }
  }
  else {
    if (not player.isCastAllowed()) {
      return;
    }
   
    doWallCast();
    
    if (player.castAmount() == 0) {
      Casting::printSuccessStatus();
      Misc::delay(30);
    }
  }
}

void cast_magic_missile()
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  player.setCastAmount(1);
  if (player.isCpu()) {
    player.aiCast(WizardCPU::MAGIC_MISSILE);
  }
  else {
    // check this is a vlid square for human casting
    if (arena.at(0, arena.targetIndex()) == 0 or arena.targetIndex() == arena.wizardIndex() ) {
      // if no creature, or attacking self, return
      return;
    }
    
    // in range?
    const SpellData & currentSpell(s_spellData[player.selectedSpellId()]);
    if (not currentSpell.isSpellInRange()) {
      Text16 & text16(Text16::instance());
      text16.clearMessage();
      text16.displayMessage("OUT OF RANGE", Color(30,31,0)); // yellow
      return;
    }
    
    // do LOS check...
    if (arena.isBlockedLOS()) {
      Text16 & text16(Text16::instance());
      text16.clearMessage();
      text16.displayMessage("NO LINE OF SIGHT", Color(31,30,0)); // yellow
      return;
    }
    
    // got to here? well cast the spell!
    doMagicMissile();
    Misc::delay(30);
    player.setCastAmount(0);
  }
}

void cast_justice()
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  if (player.castAmount() == 0) {
    // setup cast amount
    player.setCastAmount(1);
    if (player.selectedSpellId() >= SPELL_DARK_POWER) 
      player.setCastAmount(3);
  }
  
  if (player.isCpu()) {
    // this will always be chosen, but might not actually be any good
    ai_cast_justice();
    
  }
  else {
    // code from 9df4 - spell must have passed initial cast rate to get this far
    Text16 & text16(Text16::instance());
    text16.clearMessage();
    if (Casting::spellSuccess() == 0) {
      // not strictly needed ?
      player.setCastAmount(0);
      return;
    }
    
    // ok, got here then check spell is OK to cast here...
    int target(arena.at(0, arena.targetIndex()));
    if (target == 0)
      return;
    
    // 9e27 - only allow on proper creatures and wizards
    if (target < Arena::WIZARD_INDEX and target >= SPELL_GOOEY_BLOB)
      return;
    
    do_justice_cast();
    player.setCastAmount(player.castAmount()-1);
    
  }
}

void cast_raise_dead()
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  if (player.castAmount() == 0) {
    // setup cast amount
    player.setCastAmount(1);
  }
  
  if (player.isCpu()) {
    
    // checks if it is worht casting, and casts if so
    // otherwise sets relevent flags to ignore this spell
    ai_cast_raisedead();
    
  }
  else {
    // check that can be cast on this square
    // code from 861a
    if (arena.at(0, arena.targetIndex()) == 0) 
      return;
    if (not arena.isDead(arena.targetIndex())) 
      return;
    
    const SpellData & currentSpell(s_spellData[player.selectedSpellId()]);
    if (not currentSpell.isSpellInRange()) {
      Text16 & text16(Text16::instance());
      text16.clearMessage();
      text16.displayMessage("OUT OF RANGE", Color(30,31,0)); // lblue
      return;
    }
    
    // do LOS check...
    if (arena.isBlockedLOS()) {
      Text16 & text16(Text16::instance());
      text16.clearMessage();
      text16.displayMessage("NO LINE OF SIGHT", Color(31,30,0)); // lblue
      return;
    }
    do_raisedead_cast();
    
    Casting::printSuccessStatus();
    
  }
  
}

void cast_subversion()
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  player.setCastAmount(1);
  if (player.isCpu()) {
    ai_cast_subversion();
    if (player.hasTargetSquare()) {
      do_subversion();
      player.setCastAmount(0);
    }
  }
  else {
    // human...
    int target(arena.at(0, arena.targetIndex()));
    if (target >= SPELL_GOOEY_BLOB or target == 0
        or arena.at(4, arena.targetIndex()) != 0 ) 
    {
      // can't cast here, as the creature is ridden by a wizard, 
      // is not a proper creature or there's nothing here
      return;
    }
    
    const SpellData & currentSpell(s_spellData[player.selectedSpellId()]);
    if (not currentSpell.isSpellInRange()) {
      Text16 & text16(Text16::instance());
      text16.clearMessage();
      text16.displayMessage("OUT OF RANGE", Color(30,31,0)); // lblue
      return;
    }
    
    // do LOS check...
    if (arena.isBlockedLOS()) {
      Text16 & text16(Text16::instance());
      text16.clearMessage();
      text16.displayMessage("NO LINE OF SIGHT", Color(31,30,0)); // lblue
      return;
    }
    
    // got to here then spell is valid
    player.setCastAmount(0);
    do_subversion();
  }
  
}

void cast_magic_shield()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if ( player.hasMagicShield() ) {
      // already have something similar...
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_shield_cast();
  }
  else {
    do_shield_cast();
    player.setCastAmount(0);
  }
  
}

void cast_magic_armour()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if ( player.hasMagicArmour() ) {
      // already have something similar...
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_armour_cast();
  }
  else {
    do_armour_cast();
    player.setCastAmount(0);
  }
}

void cast_magic_sword()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if (player.hasMagicSword()) {
      // already have something similar...
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_sword_cast();
  }
  else {
    do_sword_cast();
    player.setCastAmount(0);
  }
}

void cast_magic_knife()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if ( player.hasMagicKnife() ) {
      // already have something similar...
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_knife_cast();
  }
  else {
    do_knife_cast();
    player.setCastAmount(0);
  }
}

void cast_magic_bow()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if ( player.rangedCombat() ) {
      // already fitted with the bow
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_bow_cast();
  }
  else {
    do_bow_cast();
    player.setCastAmount(0);
  }
}

void cast_magic_wings()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if ( player.hasMagicWings() ) {
      // already fitted with wings
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_wings_cast();
  }
  else {
    do_wings_cast();
    player.setCastAmount(0);
  }
}

void cast_shadow_form()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    if (player.hasShadowForm()) { 
      // already have
      player.setHasTargetSquare(false);
      return;
    }
    player.setHasTargetSquare(true);
    do_shadowform_cast();
  }
  else {
    do_shadowform_cast();
    player.setCastAmount(0);
  }
}

void cast_turmoil()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    
    ai_cast_turmoil();
    
    // // computer should never cast turmoil!
    // player.setHasTargetSquare(false);
    // player.setCastAmount(0);
  } else {
    // humans can cast turmoil at their own risk!
    player.setCastAmount(1);
    do_turmoil_cast();
  }
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


void doWallCast()
{
  Casting::spellAnimation();
  Wizard & player(Wizard::currentPlayer());

  if (Casting::spellSuccess())
    Arena::instance().creatureSpellSucceeds();
  else 
    player.setCastAmount(0);
  if (player.castAmount() > 0)
    player.setCastAmount(player.castAmount()-1);
  Arena::instance().drawCreatures();
  Misc::delay(2);
  Graphics::loadAllPalettes();
}

// the implementation of a magic missile
// 9ca9 
void doMagicMissile()
{
  // wait for redraw
  Misc::delay(4); 
  
  // spell anim
  u8 anim_type = 5;
  if (current_spell == SPELL_LIGHTNING)
    anim_type = 6;
  SoundEffect::play(SND_BEAM);
  los_blocked(target_index, anim_type);
  
  if (current_spell == SPELL_LIGHTNING) {
    SoundEffect::play(SND_ELECTRO);
  } else {
    // a splat sound?
    SoundEffect::play(SND_SPELLSUCCESS);
  }
  
  // do "splat" animation
  splat_animation();
  
  if (arena.at(0, target_index) == SPELL_MAGIC_FIRE or
    ( arena.at(0, target_index) >= SPELL_MAGIC_CASTLE and arena.at(0, target_index) <= SPELL_WALL)) {
    // no effect
    return;
  }
  
  // get the defence of the creature here
  u8 defence;
  if (arena.at(0, target_index) >= WIZARD_INDEX) {
    // wiz
    u8 plyr = arena.at(0, target_index) -WIZARD_INDEX;
    defence = players[plyr].defence;
    if ((players[plyr].modifier_flag & 0xc0) == 0x40) {
      // has magic shield
      defence += 2;
    } else
    if ((players[plyr].modifier_flag & 0xc0) == 0x80) {
      // has magic armour
      defence += 4;
    }
    
  } else {
    // creature
    defence = CHAOS_SPELLS.pSpellDataTable[arena.at(0, target_index)]->defence;
  }
  
  defence += GetRand(10);
  u8 attack = 3 + GetRand(10);
  
  if (current_spell == SPELL_LIGHTNING) {
    attack += 3;
   
  }
  
  
  if (attack < defence)
    return;
  
  wait_vsync_int();
  wait_vsync_int();
  
  // do the pop animation...
  pop_animation();
  
  // new code...
  if (arena[4][target_index] == 0) {
    // nothing in arena 4...
    if (arena[0][target_index] >= WIZARD_INDEX) {
      // was a wizard, do wizard death anim...
      kill_wizard();
    }  else {
      // remove the creature
      arena[0][target_index] = 0;
      if (!Options[OPT_OLD_BUGS] and arena[5][target_index] != 0) {
        // what about dead bodies?
        // but only if arena 4 was empty
        // bug fix v0.7a (disbelieve failed with old bugs turned off, fixed here too)
        arena[0][target_index] = arena[5][target_index]; //creature in arena 5
        arena[2][target_index] = 4; // dead
        arena[5][target_index] = 0; //clear creature in arena 5 
      }
    }
    
  } else {
    // arena 4 had something in it
    u8 arena4 = arena[4][target_index];
    arena[4][target_index] = 0;
    if (!Options[OPT_OLD_BUGS]) {
      // an old bug was destroying gooey blob results in creature under blob 
      // taking the same owner as the blob... 
      // e.g. wizard 1 blob covers wizard 2 creature
      // someone kills the blob, wizard 2's creature now belongs to wizard 1!
      if (arena[0][target_index] == SPELL_GOOEY_BLOB) {
        arena[3][target_index] = arena[5][target_index];
      }
      else {
        // the famous "undead wizard" bug is caused by not updating the arena[3] flag properly
        if (arena4 >= WIZARD_INDEX) {
          arena[3][target_index] = arena4-WIZARD_INDEX;
        }
      }
    }
    
    arena[0][target_index] = arena4;
    arena[5][target_index] = 0;
    
  }
  
  /* old code
  if (arena[0][target_index] >= WIZARD_INDEX) {
      // jump to wizard death stuff..
    kill_wizard();
  }
  else {
    arena[0][target_index] = arena[4][target_index];
    arena[4][target_index] = 0;
    if (!Options[OPT_OLD_BUGS] and arena[5][target_index] != 0
    and arena[0][target_index] == 0) {
      // what about dead bodies?
      // but only if arena 4 was empty
      // bug fix v0.7a (disbelieve failed with old bugs turned off, fixed here too)
      arena[0][target_index] = arena[5][target_index]; //creature in arena 5
      arena[2][target_index] = 4; // dead
      arena[5][target_index] = 0; //clear creature in arena 5 
    }
    
  }
  */
}
