#include <nds.h>
#include "ndspp.h"
#include "Casting.h"
#include "SpellData.h"
#include "Arena.h"
#include "Wizard.h"
#include "Misc.h"
#include "Text16.h"

using namespace nds;

int Casting::s_worldChaos(0);
bool Casting::s_castSuccess(false);

int Casting::getWorldChaos() {
  return s_worldChaos;
}
void Casting::show()
{
  Arena & arena(Arena::instance());
  arena.resetAnimFrames();
  arena.display();
  arena.currentPlayer(0);
  arena.setBorderColour(0);
  arena.initialiseCursor(0,0,Arena::CURSOR_NORMAL_GFX);
  Video::instance().fade(false);
  startCastRound();
}
void Casting::animate()
{
  Arena::instance().drawCreatures();
}

CurrentScreen_t Casting::getId() const
{
  return SCR_CASTING;
}
void Casting::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  Arena & arena(Arena::instance());
  if (keysSlow & KEY_UP) {
    arena.cursorUp();
  }
  if (keysSlow & KEY_DOWN) {
    arena.cursorDown();
  }
  if (keysSlow & KEY_LEFT) {
    arena.cursorLeft();
  }
  if (keysSlow & KEY_RIGHT) {
    arena.cursorRight();
  }
  // cast...
  if (keysSlow & KEY_A) {
    execute();
  }
  // cancel...
  if (keysSlow & KEY_B) {
    cancel();
  }
}


// approximately at 95ee
void Casting::startCastRound() 
{
  Arena & arena(Arena::instance());
  int currentPlayer(arena.currentPlayer());
  int playerCount(arena.players());
  if (currentPlayer < playerCount) 
  {
    Wizard & player(Wizard::getCurrentPlayer());
    // temp_cast_amount = 0; // moved to Wizard - the amount of goes this spell has
    if (player.isDead()) {
      // player is dead...
      player.updateCreatureCount();
      arena.currentPlayer(currentPlayer+1);
      startCastRound();
    } else {
      // this moves to the player, even if they don't have a spell to cast
      arena.setCurrentPlayerIndex();
      if (player.isCpu()) {
        // cpu spell casting...
        // jump 96f3
        arena.enableCursor(false);
        player.doAISpell();
        // delay(10); //FIXME
        player.updateCreatureCount();
        arena.currentPlayer(currentPlayer+1);
        startCastRound();
      } else {
        int currentSpellId(player.getSelectedSpellId());
        if (currentSpellId == 0) {
          player.updateCreatureCount();
          arena.currentPlayer(currentPlayer+1);
          startCastRound();
        } else {
          // sets the success flag, etc
          //   setup_human_player_cast();
          player.setupHumanPlayerCast();
          
          // auto cast certain spells...
          if (currentSpellId == SPELL_MAGIC_WOOD or currentSpellId == SPELL_TURMOIL 
              or (currentSpellId >= SPELL_MAGIC_SHIELD and currentSpellId <= SPELL_SHADOW_FORM) 
             )
          {
            arena.enableCursor(false);
            s_spellData[currentSpellId].spellFunction();
            nextPlayerCast();
          } 
          else if (currentSpellId >= SPELL_VENGEANCE and currentSpellId <= SPELL_JUSTICE) 
          {
            // set up the casting chance first... if fails go to the next player
            setSpellSuccess();
            if (not s_castSuccess) {
              // print spell success/fail message...
              printSuccessStatus();
#if 0
              delay(20);
              temp_cast_amount = 0;
#endif
              nextPlayerCast();
            } 
          } else {
            // creature spells
            arena.drawCursor(Arena::CURSOR_SPELL_GFX);
            arena.enableCursor();
          }
        }
      }
    } 
  } 
  else 
  {
#if 0
    // start movement round..
    unset_moved_flags();
    round_count++;
    clear_message();
    current_player = 0;
    spread_fire_blob();
    destroy_castles();
    random_new_spell();
    // here, check that there are enough wizards left to carry on
    
    if (dead_wizards == (playercount-1)) {
      // uh oh -  no wizards left, do winner screen
      win_contest();
      return;
    }
    start_movement_round();
#endif  
  }
}

void Casting::cancel() {
}

void Casting::execute() 
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::getCurrentPlayer());
  int currentSpellId(player.getSelectedSpellId());
  const SpellData & spellData(s_spellData[currentSpellId]);
  
  arena.enableCursor(false);
  spellData.spellFunction();
  arena.enableCursor();
  
  if (player.castAmount() == 0 ) {
    // next players turn
    nextPlayerCast();
  }
}

void Casting::nextPlayerCast() 
{
  // need checks here for if CPU, if less than playercount, etc,
  Arena & arena(Arena::instance());
  int currentPlayer(Arena::instance().currentPlayer());
  int playerCount(Arena::instance().players());
  if (currentPlayer < playerCount) {
    Wizard & player(Wizard::getCurrentPlayer());

    player.updateCreatureCount();
    // set the current spell to 0 if it isn't disblv.
    player.removeSelectedSpell();

    arena.currentPlayer(currentPlayer+1);
    arena.drawCursor(Arena::CURSOR_NORMAL_GFX);
    startCastRound();
  } 
  /* else  // end round */
}

void Casting::printSuccessStatus() 
{
  // based on 97a3
  Text16 & text16(Text16::instance());
  text16.clearMessage();
  if (not s_castSuccess) {
    // print spell fails in purple
    text16.setColour(12, Color(31,0,30)); // purple
    text16.displayMessage("SPELL FAILS");
  } else {
    text16.setColour(12, Color(31,31,31)); // white
    text16.displayMessage("SPELL SUCCEEDS");
  }
#if 0 
  delay(30);
#endif

}
void Casting::setSpellSuccess() 
{
  s_castSuccess = false;
  Wizard & player(Wizard::getCurrentPlayer());
  if (player.illusionCast()) {
    s_castSuccess = true;
  } else {
    int r = Misc::getRand(10);
    int currentSpellId(player.getSelectedSpellId());
    const SpellData & spellData(s_spellData[currentSpellId]);
    int currentSpellChance(spellData.getCastChance()+1);
    if (r < currentSpellChance) {
      s_castSuccess = true;
      s_worldChaos += spellData.chaosRating;
    } 
  } 
#ifdef DEBUG
  s_castSuccess = true;
#endif
}
