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
#include "SoundEffect.h"
#include "Line.h"
#include "Interrupt.h"

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
    if (arena.atTarget() == 0 
        or arena.atTarget() >= SPELL_GOOEY_BLOB)
    {
      return;
    }

    doDisbelieveCast();
    player.setCastAmount(0);
  }
  
}

void cast_meditate()
{
  // If a Wizard does not cast a spell or move for one turn, there could be
  // a 10% chance of the Wizard gaining another spell.
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu())
  {
    player.setHasTargetSquare(true);
    doMeditateCast();
  }
  else
  {
    doMeditateCast();
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
    if (arena.atTarget() == 0 or arena.targetIndex() == arena.wizardIndex() ) {
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
    player.aiCast(WizardCPU::JUSTICE);
    
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
    int target(arena.atTarget());
    if (target == 0)
      return;
    
    // 9e27 - only allow on proper creatures and wizards
    if (target < Arena::WIZARD_INDEX and target >= SPELL_GOOEY_BLOB)
      return;
    
    doJusticeCast();
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
    player.aiCast(WizardCPU::RAISEDEAD);
    
  }
  else {
    // check that can be cast on this square
    // code from 861a
    if (arena.atTarget() == 0) 
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
    doRaisedeadCast();
    
    Casting::printSuccessStatus();
    
  }
  
}

void cast_subversion()
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  player.setCastAmount(1);
  if (player.isCpu()) {
    player.aiCast(WizardCPU::SUBVERSION);
    if (player.hasTargetSquare()) {
      doSubversion();
      player.setCastAmount(0);
    }
  }
  else {
    // human...
    int target(arena.atTarget());
    if (target >= SPELL_GOOEY_BLOB or target == 0
        or arena.underTarget() != 0 ) 
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
    doSubversion();
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
    doShieldCast();
  }
  else {
    doShieldCast();
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
    doArmourCast();
  }
  else {
    doArmourCast();
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
    doSwordCast();
  }
  else {
    doSwordCast();
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
    doKnifeCast();
  }
  else {
    doKnifeCast();
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
    doBowCast();
  }
  else {
    doBowCast();
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
    doWingsCast();
  }
  else {
    doWingsCast();
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
    doShadowformCast();
  }
  else {
    doShadowformCast();
    player.setCastAmount(0);
  }
}

void cast_turmoil()
{
  Wizard & player(Wizard::currentPlayer());
  if (player.isCpu()) {
    
    player.aiCast(WizardCPU::TURMOIL);
    
    // // computer should never cast turmoil!
    // player.setHasTargetSquare(false);
    // player.setCastAmount(0);
  } else {
    // humans can cast turmoil at their own risk!
    player.setCastAmount(1);
    doTurmoilCast();
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
  
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  // spell anim
  Line::Line_t anim_type = Line::BOLT;
  int currentSpell(player.selectedSpellId());
  if (currentSpell == SPELL_LIGHTNING)
  {
    anim_type = Line::LIGHTNING;
  }
  else if (currentSpell == SPELL_MAGIC_SLEEP or currentSpell == SPELL_BLIND)
  {
    anim_type = Line::SPELL;
  }
  SoundEffect::play(SND_BEAM);
  Line::doLine(anim_type);
  
  if (currentSpell == SPELL_LIGHTNING) {
    SoundEffect::play(SND_ELECTRO);
  } else {
    // a splat sound?
    SoundEffect::play(SND_SPELLSUCCESS);
  }
  
  // do "splat" animation
  arena.splatAnimation();

  if (arena.atTarget() == SPELL_MAGIC_FIRE or
    ( arena.atTarget() >= SPELL_MAGIC_CASTLE 
      and arena.atTarget() <= SPELL_WALL)) {
    // no effect
    return;
  }
  
  int defence;
  int magicResistance; // for blind/sleep
  // get the defence of the creature here
  if (arena.atTarget() >= Arena::WIZARD_INDEX) {
    // wiz
    int plyr = arena.atTarget() - Arena::WIZARD_INDEX;
    Wizard & targetPlayer(Wizard::player(plyr));
    defence = targetPlayer.defence();
    magicResistance = targetPlayer.magicResistance(); // blind/sleep
    if (targetPlayer.hasMagicShield()) {
      // has magic shield
      defence += 2;
    } else
    if ( targetPlayer.hasMagicArmour()) {
      // has magic armour
      defence += 4;
    }
  } else {
    // creature
    defence = s_spellData[arena.atTarget()].defence;
    magicResistance = s_spellData[arena.atTarget()].magicRes; // blind/sleep
  }
  
  defence += Misc::rand(10);
  magicResistance += Misc::rand(10); // blind/sleep
  int attack = 3 + Misc::rand(10);
  
  if (currentSpell == SPELL_LIGHTNING) {
    attack += 3;
  }
  
  if (currentSpell == SPELL_MAGIC_SLEEP or currentSpell == SPELL_BLIND)
  {
    // for blind/sleep
    Casting::setSpellSuccess(false);
    if (attack > magicResistance)
    {
      Casting::setSpellSuccess(true);
      arena.sleepBlind();
    }
    Casting::printSuccessStatus();
  }
  else
  {
    if (attack < defence)
      return;
    Misc::delay(2);

    arena.magicMissileEnd();
  }
}

void doJusticeCast()
{
  // code from 9e38 onwards
  // do sound fx, flashing graphics
  Arena & arena(Arena::instance());
  SoundEffect::play(SND_JUSTICE);
  for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 3; k++) {
      for (int j = 0; j < 8; j++) {
        // do the flashing gfx...
        Misc::delay(2, false);
        if (arena.atTarget() >= Arena::WIZARD_INDEX) {
          int id(arena.atTarget()-Arena::WIZARD_INDEX);
          Wizard::player(id).drawJusticeGfx(j);
        }
        else {
          s_spellData[arena.atTarget()].drawJusticeGfx(j);
        }
        
      }
      
    }
    
    
  }
  
  
  // 9e96
  // refresh arena
  // set the has cast spell flag
  
  // get magic resistance of creature...
  u8 magres;
  if (arena.atTarget() >= Arena::WIZARD_INDEX) {
    // wizard res
    magres = Wizard::player(arena.atTarget() - Arena::WIZARD_INDEX).magicResistance();
  } else {
    // creature res
    magres = s_spellData[arena.atTarget()].magicRes;
  }
  magres++;
  
  u8 r = Misc::rand(10);
  
  if (r >= magres) {
    // spell succeeded...
    // do pop anim
    
    arena.popAnimation();
    
    if (arena.atTarget() >= Arena::WIZARD_INDEX) {
      // wizard - destroy all his creations!
      Misc::delay(4);
      arena.destroyAllCreatures(arena.atTarget() - Arena::WIZARD_INDEX);
    }
    else {
      arena.justice();
    }
    Misc::delay(4);
  }
  
}

// code from 86c3
void doRaisedeadCast()
{
  // implementation of the raise dead spell
  // used by human and cpu players
  
  // spell animation to target square
  Casting::spellAnimation();
  Wizard & player(Wizard::currentPlayer());  
  if (not Casting::calculateSpellSuccess()) {
    player.setCastAmount(0);
    return;
  }
  Arena::instance().raiseDead();
  player.setCastAmount(0);
  // temp_cast_amount--;
}


void doSubversion()
{
  Wizard & player(Wizard::currentPlayer());  
  Arena & arena(Arena::instance());
  // do the actual casting of subvert - target_index is valid 859c
  if (player.isCpu()) {
    player.printNameSpell();
    Misc::delay(80);    
  }
 
  Casting::spellAnimation();
   
  Casting::setSpellSuccess(false);
  if ( not arena.isIllusion(arena.targetIndex())) {
    // if BIT 4 is set, the spell would fail 
    // i.e. we are in (if !illusion)...
    
    u8 magic_res = s_spellData[arena.atTarget()].magicRes;
    magic_res++;
    
    u8 r = Misc::rand(10);
    
    // A possible bug in the original: Subversion succeeded if the random value
    // was less than magic resistance! Fixed here. See code at 85c7. 
    if (r > magic_res ) {
      if (player.selectedSpellId() == SPELL_MUTATION) {
        Casting::setSpellSuccess(arena.mutate());
      } else {
        arena.subvert();
        Casting::setSpellSuccess(true);
      }
    } 
  }
  Casting::printSuccessStatus();
}


void doShieldCast()
{
  
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasMagicShield();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
  
}

void doArmourCast()
{
  
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasMagicArmour();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
  
}


void doSwordCast()
{
  
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasMagicSword();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
  
}

void doKnifeCast()
{
  
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasMagicKnife();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
  
}

// 8486
void doBowCast()
{
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasMagicBow();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
}

// 
void doWingsCast()
{
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasMagicWings();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
  
}

void doShadowformCast()
{
  Wizard::currentPlayer().setupRange0Spell();
  if (Casting::spellSuccess()) {
    Wizard::currentPlayer().setHasShadowForm(true);
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
}


void doTurmoilCast()
{
  int tsi = Arena::instance().startIndex();
  Casting::calculateSpellSuccess();
  Interrupt::disable();
  Casting::setSpellSuccess(true);
  if (Casting::spellSuccess()) {
    // unset bit 7 of all things
    Arena::instance().turmoil();
  }
  Interrupt::enable();
  Arena::instance().setStartIndex(tsi);
  Casting::printSuccessStatus();
  Wizard::currentPlayer().setCastAmount(0);
}

void doMeditateCast()
{
  Wizard & player(Wizard::currentPlayer());
  player.setupRange0Spell();
  if (Casting::spellSuccess()) {
    // new spell
    player.newRandomSpell();
    Misc::delay(4);
  }
  Casting::printSuccessStatus();
}
