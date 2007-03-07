#include <string.h>
#include <stdio.h>
#include "ndspp.h"
#include "Movement.h"
#include "Arena.h"
#include "Misc.h"
#include "Wizard.h"
#include "Text16.h"

using nds::Color;

void Movement::show()
{
  iprintf("show Movement\n");
  startMovementRound();
}

void Movement::startMovementRound()
{
  iprintf("startMovementRound\n");
  // start the movement round
  // code is very roughly based on speccy chaos...
  // code begins at ac36
  m_selectedCreature = 0;
  m_attacker = 0;
  m_rangeAttack = 0;
  m_engagedFlag = 0;
  m_creatureId = 0;
  m_rangeAttackVal = 0;
  m_movementAllowance = 0;
  m_isFlying = 0;
  m_wizardMovementAllowance = 0;

  Arena & arena(Arena::instance());
  int currentPlayer(arena.currentPlayer());
  int playerCount(arena.players());
  if (currentPlayer < playerCount) {
    Wizard & player(Wizard::currentPlayer());
    if (player.isDead()) {
      // player is dead...
      arena.setCurrentPlayer(currentPlayer+1);
      startMovementRound();
    } 
    else 
    {
      // player is alive... 
      // print "player'S TURN" string...
      m_highlightItem = 0;
      arena.setCurrentPlayerIndex();
      arena.enableCursor(false);
      int x,y;
      arena.currentPlayerXY(x,y);
      arena.setCursor(x-1,y-1);
      arena.setBorderColour(currentPlayer);
      
      Text16::instance().clearMessage();
#if 0
      highlight_players_stuff(current_player);
#endif
      
      if (player.isCpu()) {
#if 0 
        PlaySoundFX(SND_CPUSTART);
#endif
      }
      char str[30];
      strcpy(str, player.name());
      strcat(str, "'S TURN");
      
      Text16::instance().setColour(12, Color(30,31,0));
      Text16::instance().displayMessage(str);

      iprintf("Misc::delay\n");
      Misc::delay(1); //?
      iprintf("Misc::after delay\n");
      /*for (int i = 0; i < 24; i++) {
        swiWaitForVBlank();
      }*/
      
      // highlight the current player's creatures
      if (player.isCpu()) {
        // cpu movement round
        // jump 96f3
        Misc::delay(10);
#if 0
        do_ai_movement();
#endif
        arena.setCurrentPlayer(currentPlayer+1);
        startMovementRound();
      } else {
        arena.enableCursor();
       // human movement selection...
      }
      iprintf("here\n");
      
    }
  } else {
    
#if 0
    // here, check that there are enough wizards left to carry on
    // and that we haven't been playing for players*2+15 rounds yet
    
    if (dead_wizards == (playercount-1)) {
      // uh oh -  no wizards left, do winner screen
      win_contest();
      return;
    }
    if (Options[OPT_ROUND_LIMIT] == DEFAULT_ROUNDS) {
      if (round_count >= playercount*2+15) {
        // we have been playing for ages... no one is gonna win
        drawn_contest();
        return;
      }
    } else if (Options[OPT_ROUND_LIMIT] != 0) {
      if (round_count >= Options[OPT_ROUND_LIMIT]) {
        // we have been playing for more than the optional round limit... draw the game
        drawn_contest();
        return;
      }
    } // else if round limit is 0, then the game can go on for ever!
    
    
    // return to spell selection/cast round
    current_player = 0;
    
    // check if we need to show game menu...
    if (IS_CPU(0)|| IS_WIZARD_DEAD(players[0].modifier_flag)) 
      current_player = get_next_human(0);
    else
      current_player = 0;
    remove_cursor();
    delay(64);
    if (current_player == 9) {
      // there is no human player!
      end_game_option();
      if (current_player == 9) {
        
        drawn_contest();
        return;
      } else {
        continue_game();
      }
      
    } else {
      fade_down();
      show_game_menu();
      fade_up();
    }  
#endif
  }
}

void Movement::animate()
{
}

CurrentScreen_t Movement::screenId() const
{
  return SCR_MOVEMENT;
}

void Movement::handleKeys()
{
}

