#include "ndspp.h"
#include "Casting.h"
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
{}
CurrentScreen_t Casting::getId() const
{
  return SCR_CASTING;
}
void Casting::handleKeys()
{}


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
#if 0
        current_spell = players[current_player].spells[players[current_player].selected_spell];
        if (current_spell == 0) {
          update_creaturecount();
          current_player++;
          startCastRound();
        } else {
          
          if (current_spell != SPELL_DISBELIEVE) {
            // set the current spell to 0 if it isn't disblv.
            players[current_player].spells[players[current_player].selected_spell] = 0;
          }
          players[current_player].selected_spell = 0; // set to 0 for "no spell selected"
          
          // sets the success flag, etc
          setup_human_player_cast();
          
          // auto cast certain spells...
          if (current_spell == SPELL_MAGIC_WOOD || current_spell == SPELL_TURMOIL ||
          (current_spell >= SPELL_MAGIC_SHIELD && current_spell <= SPELL_SHADOW_FORM) ) {
            remove_cursor();
            CHAOS_SPELLS.pSpellDataTable[current_spell]->pFunc();
            next_player_cast();
            
          } else if (current_spell >= SPELL_VENGEANCE && current_spell <= SPELL_JUSTICE) {
            // set up the casting chance first... if fails go to the next player
            set_spell_success();
            if (temp_success_flag == 0) {
              // print spell success/fail message...
              print_success_status();
              delay(20);
              temp_cast_amount = 0;
              next_player_cast();
            } 
          } else {
            draw_cursor(CURSOR_SPELL_GFX);
            redraw_cursor();
          }
        }
        
#endif
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
