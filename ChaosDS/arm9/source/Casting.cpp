#include <nds.h>
#include "ndspp.h"
#include "Casting.h"
#include "Movement.h"
#include "GameState.h"
#include "SpellData.h"
#include "SoundEffect.h"
#include "Arena.h"
#include "Wizard.h"
#include "Misc.h"
#include "Text16.h"
#include "Line.h"

using namespace nds;
using Misc::delay;

int Casting::s_worldChaos(0);
bool Casting::s_castSuccess(false);

int Casting::worldChaos() {
  return s_worldChaos;
}
void Casting::show()
{
  Arena & arena(Arena::instance());
  arena.resetAnimFrames();
  arena.display();
  arena.setCurrentPlayer(0);
  arena.setBorderColour(0);
  arena.initialiseCursor(0,0,Arena::CURSOR_NORMAL_GFX);
  Video::instance().fade(false);
  startCastRound();
}
void Casting::animate()
{
  Arena::instance().drawCreatures();
}

void Casting::vblank()
{
  Arena::instance().countdownAnim();
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
    Wizard & player(Wizard::currentPlayer());
    // temp_cast_amount = 0; // moved to Wizard - the amount of goes this spell has
    if (player.isDead()) {
      // player is dead...
      player.updateCreatureCount();
      arena.setCurrentPlayer(currentPlayer+1);
      startCastRound();
    } else {
      // this moves to the player, even if they don't have a spell to cast
      arena.setCurrentPlayerIndex();
      if (player.isCpu()) {
        // cpu spell casting...
        // jump 96f3
        arena.enableCursor(false);
        player.doAiSpell();
        delay(10);
        player.updateCreatureCount();
        arena.setCurrentPlayer(currentPlayer+1);
        startCastRound();
      } else {
        int currentSpellId(player.selectedSpellId());
        if (currentSpellId == 0) {
          player.updateCreatureCount();
          arena.setCurrentPlayer(currentPlayer+1);
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
              delay(20);
              player.setCastAmount(0);
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
    // start movement round..
    Arena::instance().nextRound();
    // here, check that there are enough wizards left to carry on
    
#if 0
    FIXME
    if (dead_wizards == (Arena::instance().players()-1)) {
      // uh oh -  no wizards left, do winner screen
      win_contest();
      return;
    }
#endif
    GameState::instance().setNextScreen(new Movement());
  }
}

void Casting::cancel() {
}

void Casting::execute() 
{
  Arena & arena(Arena::instance());
  Wizard & player(Wizard::currentPlayer());
  int currentSpellId(player.selectedSpellId());
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
    Wizard & player(Wizard::currentPlayer());

    player.updateCreatureCount();
    // set the current spell to 0 if it isn't disblv.
    player.removeSelectedSpell();

    arena.setCurrentPlayer(currentPlayer+1);
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
  delay(30);
}

bool Casting::spellSuccess()
{
  return s_castSuccess;
}

bool Casting::setSpellSuccess() 
{
  s_castSuccess = false;
  Wizard & player(Wizard::currentPlayer());
  if (player.illusionCast()) {
    s_castSuccess = true;
  } else {
    int r = Misc::rand(10);
    int currentSpellId(player.selectedSpellId());
    const SpellData & spellData(s_spellData[currentSpellId]);
    int currentSpellChance(spellData.realCastChance()+1);
    if (r < currentSpellChance) {
      s_castSuccess = true;
      s_worldChaos += spellData.chaosRating;
    } 
  } 
#ifdef DEBUG
  s_castSuccess = true;
#endif
  return s_castSuccess;
}

static const u8 s_spellframetable[0x12] = {
  0, 1, 2, 3,
  0, 1, 2, 3,
  0, 1, 2, 3,
  4, 5, 6, 7,
  8, 8,
};
// do the spell animation between start_index and target_index
void Casting::spellAnimation() 
{
  
  Wizard & player(Wizard::currentPlayer());
  int currentSpellId(player.selectedSpellId());
  int castRange(s_spellData[currentSpellId].castRange);

  delay(4);
  // don't do spell line anim for wizard modifier spells...
  if (castRange != 0 or currentSpellId == SPELL_TURMOIL) 
  {
    SoundEffect::play(SND_BEAM);
    Line::doLine(Line::SPELL);
    // wait for redraw..
    delay(4);
  }
  // draw the spell animation gfx...
  int x, y;
  Arena::instance().targetXY(x, y);
  x--;
  y--;
  SoundEffect::play(SND_SPELLSUCCESS);
  Arena & arena(Arena::instance());
  for (int i = 0; i < 18; i++) {
    swiWaitForVBlank();
    arena.drawSpellcastFrame(x, y, s_spellframetable[i]);
  }
}
