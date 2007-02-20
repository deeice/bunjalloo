#include <nds.h>
#include "ndspp.h"
#include "Casting.h"
#include "SpellData.h"
#include "Arena.h"
#include "Wizard.h"

using namespace nds;

int Casting::s_worldChaos(0);
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
  startCastRound();
  Video::instance().fade(false);
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
        arena.removeCursor();
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
          
          // now remove the spell from the player's collection...
          if (currentSpellId != SPELL_DISBELIEVE) {
            // set the current spell to 0 if it isn't disblv.
            player.removeSelectedSpell();
          }
          player.setNoSpell();
          // auto cast certain spells...
          if (currentSpellId == SPELL_MAGIC_WOOD or currentSpellId == SPELL_TURMOIL 
              or (currentSpellId >= SPELL_MAGIC_SHIELD and currentSpellId <= SPELL_SHADOW_FORM) 
             )
          {
            arena.removeCursor();
            s_spellData[currentSpellId].spellFunction();
            /* next_player_cast(); */
          } 
          else if (currentSpellId >= SPELL_VENGEANCE and currentSpellId <= SPELL_JUSTICE) 
          {
#if 0
            // set up the casting chance first... if fails go to the next player
            set_spell_success();
            if (temp_success_flag == 0) {
              // print spell success/fail message...
              print_success_status();
              delay(20);
              temp_cast_amount = 0;
              next_player_cast();
            } 
#endif
          } else {
#if 0
            draw_cursor(CURSOR_SPELL_GFX);
            redraw_cursor();
#endif
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
}
