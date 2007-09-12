/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string.h>
#include "libnds.h"
#include "ndspp.h"
#include "images.h"
#include "Arena.h"
#include "ExamineSquare.h"
#include "GameMenu.h"
#include "GameState.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Line.h"
#include "Misc.h"
#include "Movement.h"
#include "Options.h"
#include "SoundEffect.h"
#include "SpellData.h"
#include "Text16.h"
#include "VictoryScreen.h"
#include "WizardCPU.h"
#include "Wizard.h"

using namespace nds;

Movement::Movement(bool start):
  ArenaTouchScreen(),
  m_start(start)
{
}

void Movement::show()
{
  if (m_start)
    startMovementRound();
  else {
    Arena & arena(Arena::instance());
    arena.resetAnimFrames();
    arena.display();
    arena.enableCursor();
    arena.cursorSet();
    arena.setBorderColour(arena.currentPlayer());
    Video::instance().fade(false);
  }
}

void Movement::startMovementRound()
{
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
      arena.setCursor(x,y);
      arena.setBorderColour(currentPlayer);
      
      Text16::instance().clearMessage();
      // highlight the current player's creatures
      arena.highlightCreatures(currentPlayer);
      
      if (player.isCpu()) {
        SoundEffect::play(SND_CPUSTART);
      }
      char str[30];
      strcpy(str, player.name());
      strcat(str, "'S TURN");
      
      Text16::instance().displayMessage(str, Color(30,31,0));

      Misc::delay(30,false);
      
      if (player.isCpu()) {
        // cpu movement round
        // jump 96f3
        Misc::delay(10);
        player.doAiMovement();
        arena.setCurrentPlayer(currentPlayer+1);
        startMovementRound();
      } else {
        arena.enableCursor();
       // human movement selection...
      }
    }
  } else {
    
    // here, check that there are enough wizards left to carry on
    // and that we haven't been playing for players*2+15 rounds yet
    Arena & arena(Arena::instance());
    if (Wizard::deadWizards() == (arena.players()-1)) {
      // uh oh -  no wizards left, do winner screen
      GameState::instance().setNextScreen(new VictoryScreen(VictoryScreen::WIN));
      return;
    }
    if (Options::instance().option(Options::ROUND_LIMIT) == Options::DEFAULT_ROUNDS) {
      if (arena.roundNumber() >= arena.players()*2+15) {
        // we have been playing for ages... no one is gonna win
        GameState::instance().setNextScreen(new VictoryScreen(VictoryScreen::DRAW));
        return;
      }
    } else if (Options::instance().option(Options::ROUND_LIMIT) != 0) {
      if (arena.roundNumber() >= Options::instance().option(Options::ROUND_LIMIT)) {
        // we have been playing for more than the optional round limit... draw the game
        GameState::instance().setNextScreen(new VictoryScreen(VictoryScreen::DRAW));
        return;
      }
    } // else if round limit is 0, then the game can go on for ever!
    
    // return to spell selection/cast round
    arena.setCurrentPlayer(0);
    Wizard & player(Wizard::currentPlayer());
    // check if we need to show game menu...
    if (player.isCpu() or player.isDead())
      arena.setCurrentPlayer(Wizard::nextHuman());
    arena.enableCursor(false);
    Misc::delay(64);
    if (arena.currentPlayer() == 9) {
      // there is no human player!
      VictoryScreen::endGameOption();
      if (arena.currentPlayer() == 9) {
        GameState::instance().setNextScreen(new VictoryScreen(VictoryScreen::DRAW));
        return;
      } else {
        GameMenu::continueGame();
      }
    } else {
      Video::instance().fade();
      GameState::instance().setNextScreen(new GameMenu());
    }  
  }
}

void Movement::animate()
{
  Arena::instance().drawCreatures();
  if (m_examineScreen)
  {
    m_examineScreen->animate();
  }
}

void Movement::vblank()
{
  Arena::instance().countdownAnim();
}

void Movement::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  u16 keys= keysDown();
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

  if (keys & KEY_L) {
    arena.highlightTargetCreations();
  }
  if (keys & KEY_R) {
    examine();
  }

  if (keysSlow & KEY_A) {
    execute();
  }
  if (keysSlow & KEY_B) {
    cancel();
  }
  if (keysSlow & KEY_START) {
    start();
  }
  if (keysSlow & KEY_Y) {
    y();
  }
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  }

#if 0
  if (keysSlow & KEY_X) {
    int theCreature = Arena::instance().cursorContents();
    if (theCreature >= Arena::WIZARD_INDEX)
    {
      Wizard::player(theCreature-Arena::WIZARD_INDEX).kill();
    }
  }
#endif
}

bool Movement::isFlying() const
{
  return (m_isFlying != 0);
}
void Movement::setMovementAllowance(int ma)
{
  m_movementAllowance = ma;
}

void Movement::start()
{
  if (m_selectedCreature == 0) {
    Arena::instance().setCurrentPlayer(
        Arena::instance().currentPlayer() + 1
        );
    startMovementRound();
  }
}

void Movement::next()
{
  // if no creature selected, end
  if (m_selectedCreature != 0) {
    cancel();
  }
  else
  {
    start();
  }
}

// When Y is pressed, move the cursor to the next moveable creature.
void Movement::y() {
  if (m_selectedCreature == 0) {
    int newIndex = -1;
    Arena & arena(Arena::instance());
    for (int j = 0; j < 2 && newIndex == -1; j++)
    {
      int lowerBounds, upperBounds;
      if (j == 0) {
        // first time through, go from current target to the end
        lowerBounds = arena.targetIndex()+1;
        upperBounds = Arena::ARENA_SIZE;
      } else {
        // second time through, go from start to current target.
        lowerBounds = 0;
        upperBounds = arena.targetIndex()+1;
      }
      // search the rest of the arena for creatures...
      for (int i = lowerBounds; i < upperBounds; i++)
      {
        // if something here and it is not dead and it is...
        // (a creature, or a wizard, or a wizard inside a magic wood or castle) and
        // it hasn't moved yet AND it is our creature or wizard...
        int creature(arena.at(0,i));
        int underneath(arena.at(4,i));
        if ( creature 
             and not arena.isDead(i)
             and 
               ( creature < SPELL_GOOEY_BLOB
                 or creature >= Arena::WIZARD_INDEX
                 or creature == SPELL_SHADOW_WOOD
                 or ( creature >= SPELL_MAGIC_WOOD and underneath >= Arena::WIZARD_INDEX )
               ) 
             and not arena.hasMoved(i)
           )
        {
          // get the owner - low 3 bits of arena 3 for creatures, 
          // (creatureid - WIZARD_INDEX) for wizards
          int owner = -1;
          if (creature >= Arena::WIZARD_INDEX) {
            owner = creature - Arena::WIZARD_INDEX;
          } else {
            // a creature - but is it a mount or a wood/castle?
            if (creature < SPELL_MAGIC_WOOD) {
              // a mount...
              owner = arena.owner(i);
            }
            else {
              // a castle/wood - could be anyones, so base the owner on the wizard inside
              owner = underneath - Arena::WIZARD_INDEX;
            }
          }
          // is it ours?
          if (owner == arena.currentPlayer()) {
            newIndex = i;
            break;
          }
        }
      }
    }
    if (newIndex != -1) {
      int x,y;
      Arena::getXY(newIndex, x, y);
      arena.setCursor(x-1,y-1);
    } else {
      Text16::instance().clearMessage();
      Text16::instance().displayMessage("PRESS START TO END TURN",Color(31,30,0));
    }
  }
}

void Movement::execute() 
{
  // A pressed in movement round
  // check in the actual arena...
  int x,y;
  Arena::instance().targetXY(x, y);
  if (x >= 16) {
    return;
  }
  if (m_selectedCreature == 0) {
    // select a creature
    selectCreature();
    if (m_selectedCreature != 0) {
      SoundEffect::play(SND_CHOSEN);
      if (m_isFlying)
        Arena::instance().drawCursor(Arena::CURSOR_FLY_GFX);
      else if (m_engagedFlag == 1) {
        Arena::instance().drawCursor(Arena::CURSOR_ENGAGED_GFX);
      } else {
        Arena::instance().drawCursor(Arena::CURSOR_GROUND_GFX);
      }
    }
    
  } else if ( (m_rangeAttack == 0) and m_selectedCreature !=0 ) {
    // try moving the selected creature
    Interrupt::disable();
    if (m_isFlying)
      moveFlyingCreature();
    else
      moveWalkingCreature();
    Interrupt::enable();
    
    if (m_engagedFlag == 1) {
      Arena::instance().drawCursor(Arena::CURSOR_ENGAGED_GFX);
    }
    
    
  } else if (m_rangeAttack != 0) {
    Interrupt::disable();
    doRangeAttack();
    Interrupt::enable();
  }
  if (m_selectedCreature == 0) {
    Arena::instance().drawCursor(Arena::CURSOR_NORMAL_GFX);
  }
}

void Movement::cancel()
{
  if (m_selectedCreature != 0) {
    Arena & arena(Arena::instance());
    if (m_rangeAttack == 0) {
      end();
    } else {
      int currentPlayer(arena.currentPlayer());
      arena.setBorderColour(currentPlayer);
      m_rangeAttack = 0;
      m_selectedCreature = 0;
    }
    
    if (m_selectedCreature == 0) {
      arena.drawCursor(Arena::CURSOR_NORMAL_GFX);
    }  
    
  }
}

void Movement::end()
{
  WizardCPU * cpu  = static_cast<WizardCPU*>(Wizard::currentPlayer().computer());
  cpu->endMove();
  m_attacker = 0;
  // set has moved flag...
  Arena & arena(Arena::instance());
  int startIndex(arena.startIndex());
  arena.setHasMoved(startIndex);
  Text16::instance().clearMessage();
  if (arena.at(0,startIndex) == SPELL_SHADOW_WOOD) 
  {
    m_attacker = 0;
    m_rangeAttack = 0;
    m_selectedCreature = 0;
    return; // if shadow wood
  }
  
  // check range attack...
  if (m_rangeAttack) {
    m_rangeAttack = 0;
    m_selectedCreature = 0;
    return;
  }
  
  if (arena.at(0,startIndex) == 0) {  
    m_selectedCreature = 0;
    return;
  }
  
  bool hasRangedCombat(false);
  int creature(arena.at(0,startIndex));
  // code from b8dd - init range attacks
  if (creature >= Arena::WIZARD_INDEX) { 
    hasRangedCombat = Wizard::player(creature-Arena::WIZARD_INDEX).rangedCombat();
    m_rangeAttack = Wizard::player(creature-Arena::WIZARD_INDEX).range();
  }  else {
    hasRangedCombat = (s_spellData[creature].rangedCombat != 0);
    m_rangeAttack = s_spellData[creature].rangedCombatRange*2+1;
    // blinded creatures lose their ranged combat
    if (arena.isBlind(startIndex))
    {
      hasRangedCombat = false;
    }
  }
  
  if (hasRangedCombat) {
    // draw the border a funny colour...
    // print the message RANGED COMBAT,RANGE= range val
    SoundEffect::play(SND_RANGE);
    char str[3];
    Text16::int2a(m_rangeAttack/2, str);
    
    Text16 & text16(Text16::instance());
    text16.displayMessage("RANGED COMBAT,RANGE=", Color(0,30,0)); // green
    text16.print(str,Text16::MESSAGE_X+20, Text16::MESSAGE_Y, 13);  
    text16.setColour(13, Color(30,30,0)); // yellow
    Arena::instance().drawCursor(Arena::CURSOR_FIRE_GFX);
    // set the border colour too
    Palette borderPal(0, 11);
    borderPal[1] = Color(0,0,0);
    borderPal[2] = Color(31,0,31);
    
    // seperate out the cpu stuff...  originally looped on keyboard here,
    // moving cursor about then waited for S to be pressed. I moved that code to
    // doRangeAttack(), which is triggered when you press A again on the
    // joypad. Also, there was some CPU specific code here, which has gone into
    // the cpu class but the initial cpu target table creation is easiest to leave
    // in here...
    if (Wizard::currentPlayer().isCpu())
    {
      WizardCPU * cpu = static_cast<WizardCPU*>(Wizard::currentPlayer().computer());
      cpu->createAllEnemiesTable();
      Interrupt::enable();
      Misc::delay(2);
      Interrupt::disable();
    }
  } else {
    m_rangeAttack = 0;
    m_selectedCreature = 0;  
  }
}

// taken from aced 
void Movement::selectCreature()
{
  // see where the cursor is in the arena..
  // target_index contains the current cursor index.
  Arena & arena(Arena::instance());
  int startIndex(arena.targetIndex());
  arena.setStartIndex(startIndex);
  m_selectedCreature = arena.atTarget();
  m_rangeAttack = 0;
  m_engagedFlag = 0;
  m_creatureId = 0;
  m_rangeAttackVal = 0;
  m_movementAllowance = 0;
  m_isFlying = 0;
  m_wizardMovementAllowance = 0;

  WizardCPU * cpu  = static_cast<WizardCPU*>(Wizard::currentPlayer().computer());
  cpu->setFlyingTargetFound(false);

  // is there a creature here?
  if (m_selectedCreature == 0) 
    return;

  // check if we have moved already HAS_MOVED(arena.at(3, start_index))
  if (arena.hasMoved(startIndex)) {
    m_selectedCreature = 0;
    return;
  }
  
  
  int currentPlayer(arena.currentPlayer());
  if (m_selectedCreature >= Arena::WIZARD_INDEX) {
    // wizard - check it is ours
    int selectedPlayer = m_selectedCreature - Arena::WIZARD_INDEX;
    if (selectedPlayer != currentPlayer) {
      m_selectedCreature = 0;
      return;
    }
    
    // default movement for wizards
    m_movementAllowance = 2;
    if (Wizard::player(selectedPlayer).hasMagicWings()) 
    {
      m_movementAllowance = 13;
      m_isFlying = 1;
    } else if (Wizard::player(selectedPlayer).hasShadowForm()) {
      m_movementAllowance = 6;
    }
    
  } else {
    // a creature is selected
    // set the movement value
    m_movementAllowance = s_spellData[m_selectedCreature].movement*2;
    if (m_selectedCreature >= SPELL_PEGASUS
        and m_selectedCreature < SPELL_SPECTRE)
    {
      // between pegasus and vampire
      m_isFlying = 1;
      m_movementAllowance++;
    }
    // blindness reduces movement to 1 square.
    if (arena.isBlind(startIndex))
    {
      m_movementAllowance = 2;
    }

    // check if there is something inside
    int inside_creature = arena.at(4,startIndex);
    bool yes_pressed(false);
    if (inside_creature >= Arena::WIZARD_INDEX) {
      inside_creature -= Arena::WIZARD_INDEX;
      // it is a wizard id 
      if (inside_creature != currentPlayer) {
        m_selectedCreature = 0;
        return;
      }
      inside_creature += Arena::WIZARD_INDEX;
      // check if the covering creature is a wood or up
      if (m_selectedCreature < SPELL_MAGIC_WOOD)
      {
        // is not a wood, so carry on
        // display "DISMOUNT WIZARD? (Y OR N)" message
        // loop until Y or N pressed...
        if (Wizard::currentPlayer().isCpu()) {
	  // room for improvement here - e.g. what if stood next to a wood?
	  // traditional:
	  /*
	  if (Options[OPT_OLD_BUGS]) {
	    yes_pressed = 0;
	  } else 
	  {*/
	    // "improved"
          WizardCPU * cpu  = static_cast<WizardCPU*>(Wizard::currentPlayer().computer());
          yes_pressed = cpu->dismount();
	  //}
	}
        else {
          Text16 & text16(Text16::instance());
          text16.clearMessage();
          text16.displayMessage("DISMOUNT WIZARD? A=YES B=NO", Color(31,30,0)); // yellow
          yes_pressed = Misc::confirm();
        }
        
         
      } else {
        // bit of a hack this...
        yes_pressed = true;
        // jump add2
      }
      if (yes_pressed)
      {
        // dismount the wizard
        m_movementAllowance = 3;
        m_wizardMovementAllowance = 1;
        m_isFlying = 0;
        // make sure we move the wizard
        m_selectedCreature = inside_creature;
      }
      
    } // else inside creature < WIZARD_INDEX
    if (not yes_pressed) {
      // if not dismounting...
      // adb3 
      if (m_selectedCreature != SPELL_SHADOW_WOOD) {
        // not shaodw wood..
        if (m_selectedCreature >= SPELL_GOOEY_BLOB) {
          // blob etc, so do nothing
          m_selectedCreature = 0;
          return;
        }
      }
      if (arena.owner(startIndex) != currentPlayer) {
        m_selectedCreature = 0;
        return;
      }
      
      if (arena.isDead(startIndex))
      {
        // dead 
        m_selectedCreature = 0;
        return;
      }
      
    }
  }
  // add2
  // check the engaged to enemy stuff
  checkEngaged(startIndex);
  if (m_engagedFlag == 0) {
    // ae32
    char str[3];
    Text16 & text16(Text16::instance());
    text16.clearMessage();
    Text16::int2a(m_movementAllowance / 2, str);
    text16.displayMessage("MOVEMENT RANGE=",Color(0,30,0)); // green
    text16.print(str,Text16::MESSAGE_X+15,Text16::MESSAGE_Y, 13);  
    text16.setColour(13, Color(30,30,0)); // yellow
    
    if (m_isFlying) {
      text16.print("(FLYING)",Text16::MESSAGE_X+17,Text16::MESSAGE_Y, 14);  
      text16.setColour(14, Color(0,30,31)); // yellow
    }
    
    Misc::waitForLetgo();
  }
}

void Movement::moveFlyingCreature()
{
  // based on b0a8
  if (m_engagedFlag) {
    m_movementAllowance = 0x3;
  }
//  if (IS_CPU(current_player)) {
//    s16 in_range = create_range_table(start_index, m_movementAllowance);
//    // get best value in LUT
//    
//  } 
  Arena & arena(Arena::instance());
  if (arena.startIndex() == arena.targetIndex())
    return;
  // check in range...
  int dist = Arena::distance(arena.startIndex(), arena.targetIndex());
  
  if (m_movementAllowance - dist >= 0) {
    // JP b148
    moveCreature(dist);
  } else {
    // print out of range
    Text16::instance().clearMessage();
    Text16::instance().displayMessage("OUT OF RANGE",Color(0,30,30));
//    Misc::waitForLetgo();
//    Misc::waitForKeypress();
    return;
  }
  
  
}

void Movement::moveWalkingCreature(void) {
  int targetIndex(Arena::instance().targetIndex());
  int startIndex(Arena::instance().startIndex());
  int movementAmount = Arena::distance(targetIndex, startIndex);
  if (m_isFlying == 0 and movementAmount > 3)
  {
    Text16::instance().clearMessage();
    Text16::instance().displayMessage("OUT OF RANGE",Color(0,30,30));
    return;
  } else if (targetIndex == startIndex) {
    return;
  }
  
  moveCreature(movementAmount);
}

// after selecting a creature, pressing A again moves it to its target square
void Movement::moveCreature(int distanceMoved)
{
  // code roughly based on that at aea7 onwards
  // get the creature in the target square
  Arena & arena(Arena::instance());
  int targetIndex(arena.targetIndex());
  // something here and is it not dead?
  if (arena.atTarget() != 0 and not arena.isDead(targetIndex))
  {
    int owner = arena.owner(targetIndex);
    int creature(arena.atTarget());
    if (creature == SPELL_MAGIC_WOOD) {
      // if magic wood...aec0
      if ((m_selectedCreature >= Arena::WIZARD_INDEX 
            and arena.underTarget() != 0)
          or m_selectedCreature < Arena::WIZARD_INDEX)
      {
        // we are a wizard and the tree is occupied
        // or we are a creature attacking a tree (can attack our own trees)
        // jump to attack b168 (via aeea)
        makeAttack();
        return;
      } 
    } else {
      // not a magic wood
      // get the target square creature
      //aede
      int currentPlayer(arena.currentPlayer());
      if ((creature < SPELL_MAGIC_FIRE) or (creature == SPELL_SHADOW_WOOD))  {
        // a creature or shadow wood  
        if (owner != currentPlayer) {
          // jump b168 attack
          makeAttack();
          return;
        } 
        else if ( (Arena::WIZARD_INDEX + currentPlayer) == m_selectedCreature)
        {
          // aeed
          // creature we are moving to belongs to current player
          // check if we have a wizard selected for movement...
          // we are a wizard... check the target creature
          if (creature < SPELL_HORSE or creature >= SPELL_BAT) {
            // not a mount, return
            return;
          }
          // Cannot ride around on sleeping creatures.
          if (arena.isAsleep(targetIndex))
          {
            return;
          }
          // successful move
        } else {
          return;
        }
      } else {
        // not an animal-type creature or shadow wood (i.e. could be a wizard or a citadel)
        // jump af0d
        if (creature < SPELL_MAGIC_CASTLE or creature >= SPELL_WALL) {
          // not a magic castle/citadel, must be 23 (fire) or wizard
          // jump af2b
          if (creature < Arena::WIZARD_INDEX) //must be fire - can't move here
            return;

          if (m_selectedCreature < Arena::WIZARD_INDEX)
          {
            // not a wizard selected, so check we aren't attacking our owner
            if ( arena.owner(arena.startIndex()) == (creature - Arena::WIZARD_INDEX) )
              return;
          }
          // got to here?, we are a wizard attacking another wizard
          // or a creature attacking an enemy wizard
          //attack - jump b168
          makeAttack();
          return;
        } else {
          // a magic castle/citadel
          if ((Arena::WIZARD_INDEX+currentPlayer) == m_selectedCreature) {
            // wizard selected for movement
            if (owner != currentPlayer) {
              // not our castle so can't go in
              return;
            }
          } else {
            // not a wizard selected, so can't go inside or attack
            return;
          }
          // got to here? we are a wizard and the castle is ours, so move
        }
      }
    }
  }
  //else nothing here, jump to af50
  // af50...
  doSuccessfulMove(distanceMoved);
  
}

// based on af50...
void Movement::doSuccessfulMove(int distanceMoved)
{
  if (m_engagedFlag)
    return;
  
  if (m_selectedCreature == SPELL_SHADOW_WOOD) {
    // shadow wood, don't move!
    // end creature turn? jump b06f
    end();
    return;
  }
    
  SoundEffect::play(SND_WALK);
  // flag that we need to remake the CPU movement table

  WizardCPU * cpu  = static_cast<WizardCPU*>(Wizard::currentPlayer().computer());
  cpu->setMoveTableCreated(0);
  
  // tidy up the start square
  Arena & arena(Arena::instance());
  if (arena.doSuccessfulMove(m_selectedCreature)) {
    // end the wizard's move
    m_wizardMovementAllowance = 0;
    m_movementAllowance = 0;
  }
  // b007
  // do a sound effect...
  // check engaged flag if we haven't moved to a magic wood
  if (m_attacker != 0) {
    end();
    return;
  }
  
  if (arena.at(0, arena.startIndex()) != SPELL_MAGIC_WOOD) {
    // we have moved onto something that isn't a magic wood
    checkEngaged(arena.startIndex());
    if (m_engagedFlag)
      return;
    
    if (m_wizardMovementAllowance != 0) {
      // jump b06f...
      end();
      return;
    } 
    
  }
  // b039
  Misc::waitForLetgo();
  int ma = m_movementAllowance - distanceMoved;
  if (m_isFlying or ma <= 0)  {
    m_movementAllowance = 0;
    // jump b06f
    end();
    return;
  }
  m_movementAllowance = ma;
  // show "movement points left" message
  char str[3];
  Text16::instance().clearMessage();
//  sprintf(str, "%d", (m_movementAllowance+1)>>1);
  Text16::int2a((m_movementAllowance+1)/2, str);
  
  Text16::instance().displayMessage("MOVEMENT POINTS LEFT=",Color(0,30,0)); // green
  Text16::instance().print(str,Text16::MESSAGE_X+21,Text16::MESSAGE_Y, 13);  
  Text16::instance().setColour(13, Color(30,30,0)); // yellow
  //////////   end message /////////////
  
}

static void drawBreathFrame(int x, int y, int frame) {
    
  Arena::instance().setPalette8(x*2,y*2,8);
  Arena::instance().drawGfx8(_binary_fire_anim_raw_start, 
      _binary_fire_anim_map_start, 
      x*2, y*2, frame);
}
// this code was originally at b94f...
// cpu specific stuff moved to computer.c
void Movement::doRangeAttack()
{
  // have a target... attack!
  //char str[30];
  Arena & arena(Arena::instance());
  if (arena.targetIndex() == arena.startIndex()) {
    // can't attack self.
    return;
  }
  
  int distance = Arena::distance(arena.targetIndex(), arena.startIndex());

  if (m_rangeAttack < distance) {
    // print out of range and return
    Text16::instance().clearMessage();
    Text16::instance().displayMessage("OUT OF RANGE",Color(0,30,30)); // light blue
    return;
  }
  // in range, not attacking self...
  if (arena.isBlockedLOS(true)) {
    Misc::delay(4,false);
    return;
  }
  arena.enableCursor(false);
  Misc::delay(4,false);
  
  // if got to here, do the range attack animation...
  Line::Line_t rangeAttackType = Line::ARROW; // range attack type
  if (m_selectedCreature >= SPELL_GREEN_DRAGON 
      and m_selectedCreature <= SPELL_GOLDEN_DRAGON)
  {
    rangeAttackType = Line::DRAGON;
  }
  if (m_selectedCreature == SPELL_MANTICORE) {
    rangeAttackType = Line::SHORT_ARROW;
  }
  // do the line animation, which is the same code as the los check
  SoundEffect::play(SND_BEAM);
  Line::doLine(rangeAttackType);
  // redraw the creatures now
  Misc::delay(4,false);
  
  // do the attack animation 
  void (*animFunc)(int x, int y, int frame);
  if (rangeAttackType == Line::DRAGON) {
    // dragon breath attack
    animFunc = drawBreathFrame;
    SoundEffect::play(SND_FIRE);
  }
  else {
    // bloop attack thing
    animFunc = Graphics::drawSplatFrame;
    SoundEffect::play(SND_SPELLSUCCESS);
  }
  int x, y;
  arena.targetXY(x, y);
  for (int i = 0; i < 8; i++) {
    Misc::delay(4,false);
    animFunc(x-1, y-1, i);
  }
  Misc::delay(4,false);
  
  int creature(arena.atTarget());
  if (not (creature == 0 or arena.isDead(arena.targetIndex()))) 
  {
    if (creature < SPELL_MAGIC_FIRE 
        or creature >= Arena::WIZARD_INDEX
        or creature == SPELL_MAGIC_WOOD 
        or creature == SPELL_SHADOW_WOOD) 
    {
        // make the attack on the creature...
        makeAttack();
    }
  }
  
  arena.setBorderColour(arena.currentPlayer());
  
  m_selectedCreature = 0;
  m_rangeAttack = 0;
  m_attacker = 0;
  if (not Wizard::currentPlayer().isCpu())
    arena.enableCursor();
}


void Movement::checkEngaged(int index)
{
  // check engaged to enemy for the creature at start_index
  
  // first part based on code at be21
  int x,y;
  Arena::getXY(index, x, y);
  m_engagedFlag = 0;
  Arena & arena(Arena::instance());
  int surround_creature = 0;
  y--;        // x  y-1
  arena.ownerAt(x,y,surround_creature);
  x--;        // x-1  y-1
  arena.ownerAt(x,y,surround_creature);
  y++;        // x-1  y
  arena.ownerAt(x,y,surround_creature);
  y++;        // x-1  y+1
  arena.ownerAt(x,y,surround_creature);
  x++;        // x  y+1
  arena.ownerAt(x,y,surround_creature);
  x++;        // x+1  y+1
  arena.ownerAt(x,y,surround_creature);
  y--;        // x+1  y
  arena.ownerAt(x,y,surround_creature);
  y--;        // x+1  y-1
  arena.ownerAt(x,y,surround_creature);
  
  if (surround_creature == 0) {
    return;
  }
  
  // if here, then get the arena 4 value of "index"
  // In ZX Chaos, mounted wizards can never be engaged to enemy
  // change this so they can be, unless the wizard dismounts
  if (Options::instance().option(Options::OLD_BUGS) != 0 and arena.at(4,index) != 0) {
    // the old way, "bugged"
    return;
  }
  else if (arena.at(4,index) != 0 and m_selectedCreature >= Arena::WIZARD_INDEX)
  {
    // the new way, can be engaged - unless the wizard is dismounting
    return;
  }

  // compare maneouvre ratings.
  int our_man = s_spellData[arena.at(0, index)].manvr;
  int r = Misc::rand(10) + 2; 
  
  //  CALL be0a  ; get parameter E=12 of creature D
  //  LD   E,A   ; E = manoeuvre of creature in the square where the cursor is
  //  CALL be94  ; random number 0-9 stored in A
  //  INC  A          
  //  INC  A          
  //  CP   E     ; compare = rand(10) + 2  - manoeuvre
  if ( (r - our_man) >= 0) {
    // engaged
    setEngagedToEnemy();
  }
  
  
}

void Movement::setEngagedToEnemy()
{
  m_engagedFlag = 1;
  m_movementAllowance = 0x1;
  m_isFlying = 0; // if engaged, then we can't fly about
  // print message
  Text16::instance().clearMessage();
  Text16::instance().displayMessage("ENGAGED TO ENEMY", Color(30,30,0)); // yellow
}

// start_index attacks thing at target_index 
// code taken from b168 - either moving to a creature or firing on one
void Movement::makeAttack()
{
  Arena & arena(Arena::instance());
  int startIndex(arena.startIndex());
  int targetIndex(arena.targetIndex());

  m_attacker = arena.at(0, startIndex);
  u8 defender = arena.atTarget();
  s8 attacking_val = 0;
  s8 defending_val = 0;
  u8 attacker_undead = 0;
  u8 defender_undead = 0;
  u8 attacker_modifier = 0;
  u8 defender_modifier = 0;
  
  
  if ( arena.isUndead(startIndex) ) {
    attacker_undead = m_attacker;
  }
  
  
  if (m_attacker >= Arena::WIZARD_INDEX) {
    // wizard attacking
    // remove shadow form
    Wizard & player(Wizard::player(m_attacker-Arena::WIZARD_INDEX));
    player.setHasShadowForm(false);
    attacker_modifier = player.attackModifier();
    // can attack undead with magic knife/sword
    attacker_undead = attacker_modifier;
    if (player.rangedCombat())
      attacker_undead = player.rangedCombat();
    // possible bug here - if has magic bow, can melee attack undead?
  }
  
  // continues at b1f8...
  if ( arena.isUndead(targetIndex)) {
    defender_undead = defender;
  }
  
  if (defender >= Arena::WIZARD_INDEX) {
    // modify defence if defender has shield or armour
    Wizard & player(Wizard::player(m_attacker-Arena::WIZARD_INDEX));
    defender_modifier = player.defenceModifier();
    if (defender_modifier) {
      defender_modifier++;
    }
    // if defender has shadow form, gets bonus 3 defence!
    if (player.hasShadowForm()) {
      defender_modifier += 3;
    }
  }
  // b23d
  if (defender_undead != 0 and attacker_undead == 0) {
    // UNDEAD-CANNOT BE ATTACKED and return 
    Text16::instance().clearMessage();
    Text16::instance().displayMessage("UNDEAD-CANNOT BE ATTACKED", 
                                      Color(0,30,30)); // light blue
    // do a sound fx
    Misc::waitForLetgo();
    Wizard::currentPlayer().waitForKeypress();
    Text16::instance().clearMessage();
    return;
  }
  
  // get attacking val...
  if (m_rangeAttack) {
    if (m_attacker < Arena::WIZARD_INDEX)
      attacking_val = s_spellData[m_attacker].rangedCombat;
    else
      attacking_val = Wizard::player(m_attacker-Arena::WIZARD_INDEX).rangedCombat();
  }
  else {
    if (m_attacker < Arena::WIZARD_INDEX) {
      attacking_val = s_spellData[m_attacker].combat;
      // penalty for blind attackers
      if (arena.isBlind(startIndex)) { 
        attacking_val = 1;
      }
    }
    else {
      attacking_val = Wizard::player(m_attacker-Arena::WIZARD_INDEX).combat();
    }
  }
  
  // bug here - if the wizard has magic knife, it improves his ranged combat!
  attacking_val += attacker_modifier + Misc::rand(10);
  
  // get defending val...
  if (defender < Arena::WIZARD_INDEX) {
    defending_val = s_spellData[defender].defence;
  }
  else {
    defending_val = Wizard::player(defender-Arena::WIZARD_INDEX).defence();
  }
  
  defending_val += defender_modifier + Misc::rand(10);
  
  Misc::waitForLetgo();
  
  if (not m_rangeAttack) {
    // do attack anim...
    // CALL b375
    
    doAttackAnim(targetIndex);
  }
  
  if (defending_val < attacking_val) {
    // attack was a success...
    if (arena.underTarget() == 0) {
      // nothing in arena 4...
      if (defender >= Arena::WIZARD_INDEX) {
        // was a wizard, do wizard death anim...
        Wizard::player(defender -  Arena::WIZARD_INDEX).kill();
      } 
      if (not (defender >= SPELL_VAMPIRE
          or arena.isUndead(targetIndex)
          or arena.isIllusion(targetIndex)))
      {
        defender = 0xFF;
      } // else shouldn't leave anything underneath...

      // jump b310
      removeCreature(defender);
    } else {
      // there was something in the arena 4... b2e0
      arena.killCreature(); // kill the thing in targetIndex
      // jump b06f
      end();
    } 
  } else {
    // attack fails...
    // jump b06f
    end();
  }
}

void draw_fight_frame(int x, int y, int frame) {
  
  Arena::instance().setPalette8(x*2, y*2, 8);
  Arena::instance().drawGfx8(_binary_fight_raw_start, 
      _binary_fight_map_start, x*2, y*2, frame);
  
}

void Movement::doAttackAnim(int index)
{
  Arena::instance().enableCursor(false);
  // do sound fx
  int x, y;
  Arena::getXY(index, x, y);

  SoundEffect::play(SND_ATTACK);
  // the bug here is that in the timer code, I do:
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 4; j++) {
      Misc::delay(2,false);
      draw_fight_frame(x-1, y-1, j);
    }
  }
  
  if (not Wizard::currentPlayer().isCpu())
    Arena::instance().enableCursor();
}


// b310
void Movement::removeCreature(int creature)
{
  // remove the creature in target_index and move there
  m_engagedFlag = 0;
  m_movementAllowance = 0;
  m_wizardMovementAllowance = 0;
  
  Arena & arena(Arena::instance());
  if (creature == 0xFF)
  {
    // leaves corpse
    arena.leaveCorpse();
  } else {
    arena.killUndead();
  }
  if (m_rangeAttack == 0)
    doSuccessfulMove(3);
}
