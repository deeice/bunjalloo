#include <stdio.h>
#include "WizardCPU.h"
#include "Wizard.h"
#include "Arena.h"
#include "SpellData.h"
#include "Misc.h"
#include "Text16.h"
#include "Casting.h"
#include "Movement.h"
#include "GameState.h"
#include "Casting.h"
#include "magic.h"

// create the priority table - stored at d3f2
// will contain {priority, index} for all living enemy creatures
static unsigned char s_priorityTable[320];

WizardCPU::WizardCPU(Wizard & theWizard) :
  m_tableIndex(0),
  m_wizard(theWizard),
  m_targetCount(0)
{}

void WizardCPU::doAiSpell()
{
  /*
    set spell 0 priorty = 0x0c + GetRand(10) (I imagine this is in case Disbelieve was top priority?)
    order the spell list by priority
    best spell = 0
    while (best spell < spell list length) {
      select the best spell 
      select the best square for this spell
      if no square is good
        best spell ++
      else
        break
    }
    
    if a spell has been chosen...
      cast spell at chosen square
      remove casted spell from list
    
    move on to next player...
  */
  
  if (m_wizard.isDead()) { 
    return;
  }
  
  m_wizard.setCastAmount(0);
  m_wizard.orderSpells();
  int spellCount(m_wizard.spellCount());
  int bestSpell(0);
  while (bestSpell < spellCount) {
    m_wizard.setSelectedSpell(bestSpell);
    // "cast" the spell... each casting routine has the CPU AI code for that spell built into it.
    // if no good square was found, then go to the next spell
    m_targetSquareFound = false;
    int currentSpellId(m_wizard.selectedSpellId());
    if (currentSpellId != 0)
    {
      m_targetSquareFound = true;
      s_spellData[currentSpellId].spellFunction();
    }
    if (m_targetSquareFound) {
      // spell was cast succesfully
      m_wizard.removeSelectedSpell();
      return;
    }
    else {
      bestSpell++;
    }
  }
}
void WizardCPU::aiCast(int castType) 
{
  switch (castType)
  {
    case CREATURE:
      aiCastCreature();
      break;
    case DISBELIEVE:
      aiCastDisbelieve();
      break;
    case TREES:
      aiCastTreesCastles();
      break;
    case WALL:
      aiCastWall();
      break;
    case MAGIC_MISSILE:
      aiCastMagicMissile();
      break;
    case JUSTICE:
      aiCastJustice();
      break;
    case RAISEDEAD:
      aiCastRaiseDead();
      break;
    case SUBVERSION:
      aiCastSubversion();
      break;
    case TURMOIL:
      aiCastTurmoil();
      break;
    default:
      break;
  }
}

void WizardCPU::aiCastCreature() 
{
  // the computer tries to cast a creature..
  // code based on 9984
  
  // get the square closest to the best target...
  int strongestIndex(strongestWizard(0xFF));
  
  /* TODO - this may not be needed
  m_startIndex = m_wizardIndex;
  */
  m_targetSquareFound = false;
  int range = 3;
  int currentSpellId(m_wizard.selectedSpellId());
  if (currentSpellId >= SPELL_GOOEY_BLOB)
    range = 13;
  
  int inRange = createRangeTable(strongestIndex, range);
  
  // have a table, and an index. uses m_targetSquareFound to do stuff
  m_tableIndex = inRange*2+1;
  setFurthestInrange();
  
  if (m_targetSquareFound) {
    m_wizard.printNameSpell();
    Misc::delay(80);
    Casting::spellAnimation();
    m_wizard.setIllusionCast(false);
    
    if (currentSpellId < SPELL_GOOEY_BLOB) {
      // randomly cast illusions, but more chance of trying if the spell is tricky to cast
      int currentSpellChance(s_spellData[currentSpellId].realCastChance());
      if ( (currentSpellChance < 4 and Misc::rand(10) > 5) 
          or (Misc::rand(10) > 7) )
      {
        m_wizard.setIllusionCast(true);
      }
    }
    
    if (Casting::calculateSpellSuccess())
      Arena::instance().creatureSpellSucceeds();
    Casting::printSuccessStatus();
    
    Arena::instance().drawCreatures();
    Misc::delay(10);
  } // else no square found!
  
  m_wizard.setCastAmount(0);
}

// based on code at c78d
// additionally, pass in the attakcer - or 0xff if no attacker ready 
// and check "current spell" value
int WizardCPU::strongestWizard(int attackerIndex) {
  if (m_wizard.timid() < 10) {
    // if we have less than 10 in this value, 
    // go for the wizard who's creature is closest to us
    createTableEnemies();
    
    // the best value will now be the index of the creature closest to us
    // or the wizard closest to us, as they are treated the same in the createTableEnemies routine
    int enemy_creature = s_priorityTable[0];
    
    if (enemy_creature == 0) {
      
      // no dangerous creature found..
      // create the priority table based on "strongest wizard" - one with most/best creatures
      createTableWizards();
      Arena::instance().setTargetIndex(s_priorityTable[1]);
      
      // in the actual game it does a pointless thing here...
      return s_priorityTable[1];
      
    } else {
      // first of all, get the most dangerous creature... regardless of whether we can kill it or not
      // if we can't kill it, later we will attack its owner to do away with it
      Misc::orderTable(m_targetCount, s_priorityTable);
      
      enemy_creature = s_priorityTable[1];
      
      int enemy_wizard = Arena::instance().owner(enemy_creature);
      
      enemy_wizard = Arena::instance().wizardIndex(enemy_wizard);

      // enemy wizard = index to the enemy wizard attacking us
      // enemy creature = index to the enemy creature attacking us
      // now see which is closer and attack them
      int creature_range = Arena::distance(enemy_creature, Arena::instance().startIndex());
      int wizard_range   = Arena::distance(enemy_wizard, Arena::instance().startIndex());
      
      /* 
        Magic Fire close to a wizard confuses the enemy creatures
        Try this out, so that fire is not attractive
        Also, undead creatures should not be attractive to living ones
      */
      bool attacker_undead(false);
      if (attackerIndex == 0xff) {
        // a creature spell to cast... so use the spellid
        int current_spell = m_wizard.selectedSpellId();
        attacker_undead = (current_spell >= SPELL_VAMPIRE and current_spell <= SPELL_ZOMBIE );
      } else {
        // is a valid square
        // see if the attacking square is undead
        
        attacker_undead = Arena::instance().isUndead(attackerIndex);
      }
      
      u8 defender_undead = 0;
      
      int tmp_prio_index(1);
      while (enemy_creature != 0xFF) {
        enemy_creature = s_priorityTable[tmp_prio_index];
        if (Arena::instance().at(0,enemy_creature) != SPELL_MAGIC_FIRE) {
          // not magic fire so check undeadness
          // remember that we are still scared of magic fire and undead creatures
          // just that we are not worried about attacking it
          defender_undead = Arena::instance().isUndead(enemy_creature);
          if ( not defender_undead or attacker_undead) { 
            break;
          } 
        }
        
        tmp_prio_index += 2;
      }
      
      if (enemy_creature == 0xFF) {
        return enemy_wizard;
      }
      
      if (creature_range < wizard_range) {
        return enemy_creature;
      }
      return enemy_wizard;
    }
    
  } else {
    // create the priority table based on "strongest wizard" - one with most/best creatures
    createTableWizards();
    Arena::instance().setTargetIndex(s_priorityTable[1]);
    
    // in the actual game it does a pointless thing here...
    return s_priorityTable[1];
  }
  
}

// based on code at cdaa
void WizardCPU::resetPriorityTable() {
  for (int i = 0, index = 0; i < 0x9E; i++, index += 2) 
  {
    s_priorityTable[index]   = 0x00;
    s_priorityTable[index+1] = 0xFF;
  }
}


// based on code at c955 
// returns squares in range...
//s16 create_range_table(u8 target, u8 range) 

int WizardCPU::createRangeTable(int target, int range)
{
  
  // fill the priority table with all squares in range 
  // distance is from target index
  resetPriorityTable();
  
  int pi(0);
  int range_count(1);
  for (int i = 0; i < 0x9f; i++) {
    int tmprange = Arena::distance(i, Arena::instance().startIndex());
    if (range >= tmprange) {
      // square is in range
      tmprange = Arena::distance(i, target);
      s_priorityTable[pi] = tmprange;
      pi++;
      s_priorityTable[pi] = i;
      pi++;
      range_count++;
    }
  }
  
  Misc::orderTable(range_count, s_priorityTable);
  return range_count;
}

// c9dc
void WizardCPU::setFurthestInrange() 
{
  // get the furthest square away still in range...
  do {
    Arena::instance().setTargetIndex(s_priorityTable[m_tableIndex]);
    
    Misc::delay(5);
    // check xpos < 10  - code at c63d
    int x,y;
    Arena::instance().targetXY(x, y);
    if (x < 0x10) {
      // in range
      // los check, no messages
      if (not Arena::instance().isBlockedLOS(false)) {
        
        if (Arena::instance().atTarget() == 0) {
          // nothing here
          m_targetSquareFound = true;
        } 
        else if (Arena::instance().isDead(Arena::instance().targetIndex()))
        {
          // is the thing here dead?
          m_targetSquareFound = true;
        }
      }
    }
    if (m_tableIndex - 2 < 0)
      m_tableIndex = 0;
    else
      m_tableIndex -=2;
  } while (m_tableIndex > 0 and (m_targetSquareFound == false));
  
}

// based on code at c7bc
void WizardCPU::createTableEnemies() 
{
  // start_index and wizard_index are in Arena...
  Arena::instance().setStartIndex(Arena::instance().wizardIndex());
  // target_index is also in Arena :-(
  Arena::instance().setTargetIndex(0); // the current "target" square
  m_targetCount = 0;  // the number of targets
  resetPriorityTable();
  
  u16 index = 0; // index to prio table
  for (int i = 0; i < 0x9f; i++) {
    int valid = Arena::instance().containsEnemy(Arena::instance().targetIndex());
    if (valid > 0) {
      m_targetCount++;
      int range = Arena::distance(Arena::instance().targetIndex(), 
          Arena::instance().startIndex());
      range = range >> 1;
      valid += 0x14;
      valid -= range;
      s_priorityTable[index] = valid;
      index++;
      s_priorityTable[index] = Arena::instance().targetIndex();
      index++;
    }
    int targetIndex(Arena::instance().targetIndex());
    Arena::instance().setTargetIndex(targetIndex+1);
  }
}

void WizardCPU::createTableWizards() 
{
  // code is from c825 
  resetPriorityTable();
  //  char str[20];
  
  m_tableIndex = 0;
  for (int i = 0; i < 0x9f; i++) {
    Arena::instance().setTargetIndex(i);
    int wizardId = Arena::instance().wizardId(i);
    if (wizardId != -1) {
      createEnemyTableEntry(wizardId);
    }
  }
  
  Misc::orderTable(7, s_priorityTable);
  
}

// create an enemy table entry for this wizard
// code at c859
void WizardCPU::createEnemyTableEntry(int wizardid) 
{
  if (wizardid == m_wizard.id()) 
    return;
  
  s_priorityTable[m_tableIndex+1] = Arena::instance().targetIndex();
  for (int i = 0; i < 0x9f; i++) {
    // Check that this is right..
    int creature(Arena::instance().at(0,i));
    if (creature >= SPELL_KING_COBRA and creature < SPELL_MAGIC_CASTLE) 
    {
      if (Arena::instance().owner(i) != wizardid) {
        s_priorityTable[m_tableIndex] += (Arena::instance().attackPref(creature) / 4) + 
          m_wizard.priorityOffset();
      }
    }
    
  }
  m_tableIndex +=2;
    
}

// code based on c8c7
// get the priorities of squares surrounding "index" based on distance from "strongest"
void WizardCPU::getSurroundingSquarePrios(int index, int strongest)
{
  resetPriorityTable();
  int surroundIndex = 0;
  m_tableIndex = 0;
  for (int i = 0; i < 8; i++) {
    int lookat_i = Arena::applyPositionModifier(index, surroundIndex);
    surroundIndex++;
    if (lookat_i == 0) {
      // out of bounds
      continue;
    }
    lookat_i--;
    
    int range = Arena::distance(strongest, lookat_i);
    // add a check in here to make sure we don't
    // tread on a magic wood. This table is ordered largest
    // first. But the potential squares are read starting 
    // at the end (m_tableIndex, then m_tableIndex-2, etc)
    if (Arena::instance().at(0,lookat_i) == SPELL_MAGIC_WOOD) {
      range += 1;  // "increase" the distance so it isn't as appealing
                   // careful not to increase too much, may go backwards!
    }
    s_priorityTable[m_tableIndex++] = range;
    s_priorityTable[m_tableIndex++] = i;
    
  }
  Misc::orderTable(7, s_priorityTable);
  m_tableIndex = 0xF;
  
}

void WizardCPU::doAiMovement()
{
  /*
   if wiz has creatures to do his bidding...

     get current position, set up relative
     movement advantages for squares we can go to

     if we are in a wood or castle, stay there  
     else ...
     select the wizard
     do the movement
   
   end if
   
   for all squares on the board...
    if we have a creature here
    then try selecting it
      if we selected it successfully (it hadn't moved already)
        then do the movement for this creature
   end for

   so if the wizard is creatureless, he himself acts like a normal creature
   */
  
  // the computer movement stuff...
  if (not m_wizard.timid()) 
  {
    //  ca92...
    setupMove();
    // if we are in a safe place, don't move
    int arena0 = Arena::instance().at(0, Arena::instance().startIndex());
    if (   arena0 == SPELL_MAGIC_WOOD 
        or arena0 == SPELL_MAGIC_CASTLE 
        or arena0 == SPELL_DARK_CITADEL)
    { 
      // set has moved...
      m_hasMoved = true;
      Arena::instance().setHasMoved(Arena::instance().startIndex());
    }
    else {
      // move wizard...
      // select the wizard...
      m_hasMoved = false;
      // and we have the move table in memory..
      m_moveTableCreated = true;
      
      Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
      movement->execute(); // select the wizard...
      Misc::delay(30);
      // now try moving it to the target square...
      doThisMovement();
      
      Misc::delay(10);
    }
  }
  // here? move creatures
  // we have already dealt with the wizard...
  m_hasMoved = true;
  Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
  for (int i = 0; i < Arena::ARENA_SIZE; i++) {
    Arena::instance().setTargetIndex(i);
    Arena::instance().setStartIndex(i);
    // select the creature...
    movement->execute();
    if (movement->selectedCreature() != 0) {
      Misc::delay(30);
      doThisMovement();
    }
    
  }
  Misc::delay(20);
}

void WizardCPU::doThisMovement()
{
  // this is effectively the first half of the movement polling
  // the code in Chaos starts at ae50 and is only called if a 
  // creature is successfully selected...
  Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
  
  while (movement->selectedCreature() != 0) {
    
    if (movement->isFlying()) {
      // jump b0a8
      
      doFlyingMove();
      // what if it becomes engaged?
      // in chaos code, jumps to ae7a
      if (movement->rangeAttack()!= 0) {
        goto range_attacking;
      }
      
    }
    else {
      // walking creature...
      if (not m_hasMoved) {
        // ae67 - wizard has not moved yet...
        if (not m_moveTableCreated) {
          // need to create the wizard movement table
          setupMove();
        }
      } else {
        // wizard has moved already.. 
        // ae70
        if (not m_moveTableCreated) {
          // need to create the creature movement table
          setupCreatureMove();
        }
      }  
      
      // get the first best square to move to...
      while(s_priorityTable[m_tableIndex] == 0xFF) {
        if (m_tableIndex > 2) {
          m_tableIndex -= 2;
        } else {
          movement->cancel();
          // if we have range attack after our movement, make sure it is handled properly
          // best solution is a goto here... otherwise I'd have a load of confusing ifs
          if (movement->rangeAttack() != 0) {
            goto range_attacking;
          }
          return;
        }
      }
      
      int target_index = Arena::applyPositionModifier(Arena::instance().startIndex(),
          s_priorityTable[m_tableIndex]);
      target_index--;
      Arena::instance().setTargetIndex(target_index);

      Text16::instance().clearMessage();
      movement->execute(); 
      
      Misc::delay(4);
      
      if (movement->rangeAttack() != 0) {
        // could have used break instead of goto
        // but goto makes the intention clearer
        goto range_attacking;
      }
      
      // get the next square to move to
      // note that movement_a() sets "move_table_created" to 0, 
      // so after a move, we need to remake the table.
      if (m_tableIndex > 2) {
        m_tableIndex -= 2;
      } else {
	// no good square to go to..
        movement->cancel();
        
        if (movement->rangeAttack() != 0) {
          // again, could have used break instead of goto
          // but goto makes the intention clearer
          goto range_attacking;
        }
        return;
      }

    }
    
  }
  
// GOTO label - for range attacks jump here
range_attacking:
  while (movement->rangeAttack() != 0)
  {  
    // range attack... this is identical for flying or not, wiz or not...
    // get the best value (call cccb)
    int ra = getBestRangeattack();
    if (ra == 0x4b) {
      // the best square is crap
      movement->cancel();
      return;
    } else {
      // the target square is good
      movement->execute();
    }
    
  }
  Misc::delay(20);
}


// taken from b0a8
void WizardCPU::doFlyingMove()
{
  //char str[30];
  int currentIndex = Arena::instance().startIndex(); // save the current square, just in case
  int strongestIndex;
  Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
  /* bug fix 26/12/04 
   * If wiz is flying and engaged to undead, causes problems
   */
  if (movement->isEngaged()) {
    movement->setMovementAllowance(3);
  }
  if (not m_flyingTargetFound) {
    
    if (not m_hasMoved) {
      // ae67 - wizard has not moved yet...
      if (not m_moveTableCreated) {
        // need to create the wizard movement table
        setupMove();
      }
      // code here really follows after cc31
      // gets the "worst" square for the wizard (i.e. least dangerous)
      while(s_priorityTable[m_tableIndex] == 0xFF) {
        if (m_tableIndex > 2) {
          m_tableIndex -= 2;
        } else {
          movement->cancel();
          return;
        }
        
      }
      strongestIndex = s_priorityTable[m_tableIndex];
    } else {
      // wizard has moved already.. 
      // need to create the creature movement table
      resetPriorityTable();
      strongestIndex = strongestWizard(currentIndex);
      
    }
    Arena::instance().setStartIndex(currentIndex);
    // for creatures, si will be the one closest to the "best" enemy
    // for wizards, will be the safest square in range
    int inRange = createRangeTable( strongestIndex, movement->movementAllowance());
    m_tableIndex = inRange*2 + 1;
    m_flyingTargetFound = true;
    
  }
  
  int bi = getBestIndex();
  
  if (bi == 0x53) {
    Arena::instance().setStartIndex(currentIndex);
    // found one! target_index contains the target square...
    
    m_flyingTargetFound = true;
    Text16::instance().clearMessage();
    movement->execute();
    Misc::delay(30);
  } else {
    // ran out...
    movement->cancel();
    return;
  }
 
}

// get the best square for range attack
int WizardCPU::getBestRangeattack()
{
  Arena & arena(Arena::instance());
  // cccb
  while (s_priorityTable[m_tableIndex] != 0xFF) {
    // check the value at this index...
    int creature = arena.at(0,s_priorityTable[m_tableIndex]);
    if (creature >= Arena::WIZARD_INDEX 
        or (creature == SPELL_MAGIC_WOOD and arena.at(4,s_priorityTable[m_tableIndex]) != 0)
        or creature < SPELL_MAGIC_FIRE 
        or creature == SPELL_SHADOW_WOOD )
    {
      // wizard, or wizard in wood, shadow wood or an actual creature
      // anything >= magic fire will be ignored
      
      // ccfd
      // check if we are trying to attack an undead creature...
      // original chaos didn't check the arena[3] value for raise deaded creatures
      // wizards can always attack undead with their magic bow range attacks
      Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
      bool attacker_undead = arena.isUndead(arena.startIndex()) 
        or (movement->selectedCreature() >= Arena::WIZARD_INDEX);
      bool defender_undead = arena.isUndead(s_priorityTable[m_tableIndex]);
      if ( (not defender_undead) or attacker_undead)
      {
        arena.setTargetIndex(s_priorityTable[m_tableIndex]);
        m_tableIndex++;
        m_tableIndex++;
        return 0x53;
      } // else, trying to range attack undead with a living creature, so get next value... 
    } 
    m_tableIndex++;
    m_tableIndex++;
  }
   
  return 0x4b;
}

// called at the start of lightning casting amongst other stuff...
// creates the priority table based on all enemy creatures and wizards
// wizards are rated higher than creatures and table is ordered and 
// the first value in the table is pointed to by value in cd86
void WizardCPU::createAllEnemiesTable()
{
  // based on cc56
  m_wizard.setPriorityOffset(0x3c);
  // create the priority table based on "strongest wizard"
  createTableWizards();
  m_wizard.setPriorityOffset(0x20);
  m_tableIndex = 14;
  int tableSize = 9;
  // now create the rest of the table, for the enemy creatures
  for (int i = 0; i < Arena::ARENA_SIZE ; i++) {
    // call c67a
    int tmp(Arena::instance().containsEnemy(i));
    
    if (tmp != 0) {
      tableSize++;
      s_priorityTable[m_tableIndex++] = tmp;
      s_priorityTable[m_tableIndex++] = i;
    }
  }
  Misc::orderTable(tableSize,s_priorityTable);
  m_tableIndex = 1;
  m_wizard.setPriorityOffset(0);
}

void WizardCPU::aiCastDisbelieve() {
  int currentIndex = Arena::instance().startIndex();
  createAllEnemiesTable();
  Arena::instance().setStartIndex(currentIndex);

  while (s_priorityTable[m_tableIndex] != 0xFF) {
    int target_index = s_priorityTable[m_tableIndex];
    

    if (Arena::instance().at(0,target_index) < SPELL_GOOEY_BLOB 
        and not Arena::instance().hasDisbelieveCast(target_index)  ) {
      // is a creature and has not had disbeleive cast on it yet...
      m_targetSquareFound = true;
      m_wizard.printNameSpell();
      Arena::instance().setTargetIndex(target_index);
      Misc::delay(80);
      return;
    }
    
    m_tableIndex += 2;
  }
  // got to here? must have run out of decent targets
  Arena::instance().setTargetIndex(currentIndex);
  m_targetSquareFound = false;
  
}

bool WizardCPU::hasTargetSquare() const
{
  return m_targetSquareFound;
}
void WizardCPU::setHasTargetSquare(bool has)
{
  m_targetSquareFound = has;
}


// defines for the special moves the wizard can make, with wings or on a mount
#define SPECIAL_MOVE_MOUNT 1
#define SPECIAL_MOVE_WINGS 2

// ca92.
void WizardCPU::setupMove() {
  resetPriorityTable();
  // get the arena square with the wizard in it...
  int currentPlayer(Arena::instance().currentPlayer());
  int tmpWiz(Arena::instance().wizardIndex(currentPlayer));
  Arena::instance().setTargetIndex(tmpWiz);
  int targetIndex(tmpWiz);
  int targetCreature(Arena::instance().at(0,targetIndex));
  // check the value in arena 0 at the wizard's location
  if ( targetCreature >= SPELL_PEGASUS 
      and targetCreature <= SPELL_MANTICORE) 
  {
    // wizard on a flying mount...
    // cade...
    // call cbb9
    flyingMove(SPECIAL_MOVE_MOUNT);
    m_hasMoved = true;
    return;
  } else {
    Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
    bool tmp_is_flying(false);
    if (movement)
    {
      tmp_is_flying = movement->isFlying();
    }

    // not on a flying mount (may be on a regular one though)
    if (targetCreature >= Arena::WIZARD_INDEX 
	and Wizard::player(currentPlayer).hasMagicWings() 
      /* bug fix 26/12/04 
       * If wiz is flying and engaged to undead, causes problems
       */
      and tmp_is_flying)
    {
      // has magic wings and is in the open
      // cafb....
      // call cbc7
      flyingMove(SPECIAL_MOVE_WINGS);
      m_hasMoved = true;
      return;
    } else {
      // cb06...
      int surroundIndex = 0;
      m_tableIndex = 0;
      int targetIndex(Arena::instance().targetIndex());
      for (int i = 0; i < 8; i++) {
        int lookat_i = Arena::applyPositionModifier(targetIndex, surroundIndex);
        surroundIndex++;
        if (lookat_i == 0) {
          // out of bounds
          continue;
        }
        lookat_i--;
        int creatureLookedAt(Arena::instance().at(0, lookat_i));
        if (creatureLookedAt >= SPELL_MAGIC_CASTLE 
	    and creatureLookedAt <= SPELL_DARK_CITADEL) 
	{
          s_priorityTable[m_tableIndex++] = 0;
          s_priorityTable[m_tableIndex++] = i;
          m_targetSquareFound = true;
        } else
        if (creatureLookedAt == SPELL_MAGIC_WOOD) {
          s_priorityTable[m_tableIndex++] = 1;
          s_priorityTable[m_tableIndex++] = i;
          m_targetSquareFound = true;
        } else 
        if (creatureLookedAt >= SPELL_HORSE 
            and creatureLookedAt <= SPELL_MANTICORE
            and Arena::instance().owner(lookat_i) == currentPlayer 
            and Arena::instance().at(4,lookat_i) == 0) // is not mounted...
        {
          // is a mount and is ours and no one is on it 
          s_priorityTable[m_tableIndex++] = 2;
          s_priorityTable[m_tableIndex++] = i;
          m_targetSquareFound = true;
        } else {
          // is none of the above
          s_priorityTable[m_tableIndex++] = getPriorityVal(lookat_i)+3;
          s_priorityTable[m_tableIndex++] = i;
        }
          
      }
      // cb8b
      Misc::orderTable(7, s_priorityTable);
      Arena::instance().setStartIndex(tmpWiz);
      Arena::instance().setTargetIndex(tmpWiz);
      m_tableIndex = 0xF;
    } // end if has magic wings/out in open
  
  } // end if wizard on mount
  // cba2
  m_moveTableCreated = true;
}


// used for wizards on flying mounts or with wings
void WizardCPU::flyingMove(int type)
{
  Movement * movement(static_cast<Movement*>(GameState::instance().currentScreen()));
  // based on code at cbb9/cbc7
  // default to wings
  int movementAllowance = 13;
  if (type == SPECIAL_MOVE_MOUNT) {
    movementAllowance = 1 + 
      (s_spellData[Arena::instance().atTarget()].movement*2); 
  }
  movement->setMovementAllowance(movementAllowance);
  
  resetPriorityTable();
  
  //cbcf
  m_targetCount = 1;
  m_tableIndex = 0;
  int tmpWiz(Arena::instance().targetIndex());
  // loop over arena to get priority of all squares in range
  for (int i = 0; i < Arena::ARENA_SIZE; i++) {
    int x, y;
    Arena::getXY(i, x, y);
    if (x < 16) {
      // in bounds
      // get the distance from wizard start square to this square (i)
      int dist = Arena::distance(tmpWiz, i);
      if (movementAllowance >= dist) {
        // is in range of the wiz start square...
        // find the distance/danger of all enemies relative to this square  (call cd92)
        // store as priority // code to here is at cc0d
        s_priorityTable[m_tableIndex++] = getPriorityVal(i);
        s_priorityTable[m_tableIndex++] = i;
        m_targetCount++;
      }
    }  
  }
  // cc31
  Misc::orderTable(m_targetCount, s_priorityTable);
  Arena::instance().setStartIndex(tmpWiz);
  m_tableIndex--;
}



// taken from cd92 - gets the "danger" of the square at i
// this is calculated based on enemy creature distance and how dangerous that creature is
int WizardCPU::getPriorityVal(int index)
{
  int total = 0;
  Arena::instance().setStartIndex(index);
  for (int i = 0; i < Arena::ARENA_SIZE; i++)
  {
    // gets the priority val for this square from start_index
    total += Arena::instance().containsEnemy(i);
  }
  return total;
}

void WizardCPU::endMove()
{
  // whatever happens, the wizard has definitely moved...
  m_hasMoved = true;
  // and we definitely need to remake the movement table
  m_moveTableCreated = 0;
}


// check if the cpu could dismount (traditionally, the cpu never dismounts)
// return true to dismount
// return false to stay mounted
bool WizardCPU::dismount() const
{
  Arena & arena(Arena::instance());
  int targetIndex(arena.targetIndex());
  int creature(arena.atTarget());
  // which type of mount are we on?
  if (creature >= SPELL_PEGASUS 
      and creature <= SPELL_MANTICORE )
  {
    // too complex to work out...
    return 0;
  } 

  // "type" records type of thing that attracts us
  int type(0xff);
  int surroundIndex = 0;
  for (int i = 0; i < 8; i++) {
    int lookat_i = Arena::applyPositionModifier(targetIndex, surroundIndex);
    surroundIndex++;
    if (lookat_i == 0) {
      // out of bounds
      continue;
    }
    lookat_i--;

    // rules for leaving our mount are consistent with the
    // movement patterns when not mounted, i.e. prefer
    // to move to a castle or tree before getting on a horse.
    int creature(arena.at(0,lookat_i));
    if (creature >= SPELL_MAGIC_CASTLE 
	and creature <= SPELL_DARK_CITADEL
//	&& (players[current_player].timid == 0)  // is this right?
	) 
    {
      type = 0;
    } 
    else if (creature == SPELL_MAGIC_WOOD) 
    {
      if (type > 1) {  
	type = 1;
      }
    } 
    else if ( creature >= SPELL_PEGASUS
          and creature <= SPELL_MANTICORE 
          and arena.owner(lookat_i) == arena.currentPlayer()
          and arena.at(4, lookat_i) == 0 )
    {
      /* Next to us stands a winged mount, we are on a walking mount so swap.
       * Really we should change to any better mount, but that is quite a
       * complex operation - the cpu move table would need resetting and extra
       * checks put in all over the place. Not really worth it.
       */ 
      if (type > 2) {
	type = 2;
      }
    }
  }
  return type != 0xff;
}

void WizardCPU::setMoveTableCreated(bool created)
{
  m_moveTableCreated = created;
}

// code based on that at c8c7...
void WizardCPU::setupCreatureMove()
{
  resetPriorityTable();
  u8 currentIndex = Arena::instance().targetIndex(); // save the current square, just in case
  u16 strongestIndex;
  strongestIndex = strongestWizard(currentIndex);
  getSurroundingSquarePrios(currentIndex, strongestIndex);
  
  Arena::instance().setStartIndex(currentIndex);
  Arena::instance().setTargetIndex(currentIndex);
  
  m_moveTableCreated = 1;
}

// based on code around c9dc
int WizardCPU::getBestIndex()
{
  // get the best index of the prio table.. will be the one 
  // get the furthest square away still in range...
  do {
    if (m_tableIndex - 1 <= 0) 
      return 0x4b;
    if (s_priorityTable[m_tableIndex] == 0xFF) {
      m_tableIndex -= 2;
    } else {
      Arena::instance().setTargetIndex(s_priorityTable[m_tableIndex]);
      // check xpos < 10
      
      int x,y;
      Arena::getXY(s_priorityTable[m_tableIndex], x, y);
      m_tableIndex -= 2;
      if (x < 16) {
        // in range
        return 0x53;
      }
    }
  } while (m_tableIndex > 0);
  return 0x4b;
}

void WizardCPU::aiCastTreesCastles()
{
  Arena & arena(Arena::instance());
  int currentLocation = arena.wizardIndex();
  int tmpCastRange(0xD);
  
  Casting::calculateSpellSuccess();
  m_tableIndex = createRangeTable(currentLocation, tmpCastRange);
  m_tableIndex = m_tableIndex*2+1;
  // loop over all spells...
  for (int i = 0; i < m_wizard.castAmount(); i++) {
    // FIXME: no need for the flag, all can be done with breaks
    bool got_square(false);
    while (not got_square)
    {
      int flag = getBestIndex();
      if (flag == 0x4b) {
        // no more squares to cast to
        i = m_wizard.castAmount();
        break;
      }
      int x, y;
      arena.targetXY(x, y);
      if (x == 16) {
        continue;
      }
      if (arena.atTarget() != 0) {
        continue;
      }
      
      
      if (arena.isTreeAdjacent(m_wizard.selectedSpellId())) {
        continue;
      }
      
      if (arena.isWallAdjacent(m_wizard.selectedSpellId(), m_wizard.id())) {
        continue;
      }
      
      if (arena.isBlockedLOS(false)) {
        continue;
      }
      
      // do spell cast anim, etc
      Casting::spellAnimation();
      // check success...
      if (Casting::spellSuccess()) {
        arena.creatureSpellSucceeds();
        Misc::delay(10);
      } else {
        i = m_wizard.castAmount();
      }
      got_square = true;
    }
    
  }
  // assume for cpu that the spell was worth casting 
  // so cpu doesn't semi check for "line of sight" stuff when seeing if spell is worth casting
  m_targetSquareFound = true;
  m_wizard.setCastAmount(0);
  Casting::printSuccessStatus();
}

void WizardCPU::aiCastWall()
{
  // 9b85
  // CALL c7bc
  m_wizard.setCastAmount(4);
  Arena & arena(Arena::instance());

  int current_index = arena.startIndex();
  createTableEnemies();
  arena.setStartIndex(current_index);
  Misc::orderTable(m_targetCount, s_priorityTable);
  
  m_tableIndex = 1;
  
  while (s_priorityTable[m_tableIndex] != 0xFF) {
    if (arena.at(0,s_priorityTable[m_tableIndex]) >= SPELL_SPECTRE )
      break;
    if (arena.at(0,s_priorityTable[m_tableIndex]) < SPELL_PEGASUS )
      break;
    m_tableIndex += 2;
  }
  
  if (s_priorityTable[m_tableIndex] == 0xFF) {
    // no decent square
    m_targetSquareFound = false;
    m_wizard.setCastAmount(0);
    return;
  }
  
  // got to here? OK, a "good" square was found for wall.
  // calculate spell success and update world chaos ...
  Casting::calculateSpellSuccess();
  m_wizard.printNameSpell();
  Misc::delay(80);
  int inRange = createRangeTable(s_priorityTable[m_tableIndex], 0x9);
  Text16::instance().clearMessage();
  
  m_tableIndex = 2*inRange+1;
  u8 tmpSquareFound = 0;
  m_targetSquareFound = false;
  
  // the global m_targetSquareFound is used as a temporary flag here
  // tmp_square_found indicates if we find any castable-to squares for this spell at all
  while (m_wizard.castAmount()) {
    
    m_targetSquareFound = 0;
    setFurthestInrange();
    // if a suitable square was found, target_sq_found will be 1
    if (not m_targetSquareFound) {
      // we have run out of chackable squares!
      break;
    }
    
    // check that this is a valid square for walls...
    int x, y;
    arena.targetXY(x, y);
    if (x >= 16) {
      continue;
    }
    if (arena.atTarget() != 0){
      continue;
    }
    if (arena.isWallAdjacent(m_wizard.selectedSpellId(), m_wizard.id())) {
      continue;
    }
    if (arena.isBlockedLOS(false)){
      continue;
    }
    
    // we have found a suitable square...
    tmpSquareFound = 1;
    doWallCast();
    
  }
  m_wizard.setCastAmount(0);
  m_targetSquareFound = tmpSquareFound;
}

void WizardCPU::addBestIndex(Arena & arena)
{
  // get the best index for each cast...
  while (s_priorityTable[m_tableIndex] != 0xFF) {
    // check the creature at this index...
    int creature = arena.at(0, s_priorityTable[m_tableIndex]);
    if (creature >= Arena::WIZARD_INDEX 
        or (creature < SPELL_GOOEY_BLOB and creature != 0) )
    {
      // wizard or proper creature
      arena.setTargetIndex(s_priorityTable[m_tableIndex]);
      m_tableIndex++;
      m_tableIndex++;
      break;
    } 
    m_tableIndex++;
    m_tableIndex++;
  }
}

// from 9ef9
void WizardCPU::aiCastJustice()
{
  // print name and spell
  m_targetSquareFound = true;
  m_wizard.printNameSpell();
  Misc::delay(80);
  
  if (not Casting::calculateSpellSuccess()) {
    m_wizard.setCastAmount(0);
    Casting::printSuccessStatus();
    return;
  }
  Arena & arena(Arena::instance());
  u8 tmpStart = arena.wizardIndex();
  while (m_wizard.castAmount() != 0) {
    arena.setStartIndex(tmpStart);
    arena.setTargetIndex(arena.startIndex());
    createAllEnemiesTable();
    m_tableIndex = 1;
    
    addBestIndex(arena);
    if (s_priorityTable[m_tableIndex] == 0xFF) {
      return;
    }
    Misc::delay(60);
    doJusticeCast();
    m_wizard.setCastAmount(m_wizard.castAmount()-1);
  }
  
}

bool WizardCPU::shouldCastSleepBlind()
{
  Arena & arena(Arena::instance());
  int creature(arena.atTarget());
  int currentSpellId = m_wizard.selectedSpellId();
  bool sleepSpell(currentSpellId == SPELL_MAGIC_SLEEP or currentSpellId == SPELL_BLIND);
  if ( sleepSpell and creature == SPELL_SHADOW_WOOD)
  {
    // CPU should avoid showdow wood for sleep/blind
    return false;
  }
  if (currentSpellId == SPELL_BLIND and creature == SPELL_GOOEY_BLOB)
  {
    // Avoid blinding a Blob.
    // "My blob's got no eyes. Or nose."
    // "But how does it smell?"
    // "Terrible!"
    return false;
  }
  if (sleepSpell and ( arena.isAsleep(arena.targetIndex()) or arena.isBlind(arena.targetIndex())))
  {
    // already cast on this creature.
    return false;
  }
  if (creature >= Arena::WIZARD_INDEX 
      and Wizard::player(creature-Arena::WIZARD_INDEX).
                creatureCount(currentSpellId == SPELL_MAGIC_SLEEP) == 0)
  {
    // do not cast on creature-less wizard (no effect)
    return false;
  }
  if (creature >= SPELL_HORSE 
      and creature <= SPELL_MANTICORE
      and arena.at(4,arena.targetIndex()) != 0)
  {
    // mounted creature - so no effect
    return false;
  }
  return true;
}

// used for lightning and magic bolt
// code from 9d8a
void WizardCPU::aiCastMagicMissile()
{

  createAllEnemiesTable();
  Arena & arena(Arena::instance());
  int currentSpellId = m_wizard.selectedSpellId();
  bool sleepSpell(currentSpellId == SPELL_MAGIC_SLEEP or currentSpellId == SPELL_BLIND);
  arena.setTargetIndex(s_priorityTable[m_tableIndex]);
  while (arena.targetIndex() != 0xFF) {
    arena.setTargetIndex(s_priorityTable[m_tableIndex]);
    int creature(arena.atTarget());

    // a bit hacky - set the creature to WALL to skip the check.
    if (sleepSpell and not shouldCastSleepBlind())
    {
      creature = SPELL_WALL;
    }

    if (creature == SPELL_SHADOW_WOOD 
        or creature >= Arena::WIZARD_INDEX
        or creature < SPELL_MAGIC_FIRE )
    {
      // see if in range...
      
      if (arena.isSpellInRange(currentSpellId, false))
      {
        if (not arena.isBlockedLOS(false)) {
          // spell is good!
          m_targetSquareFound = 1;
          m_wizard.setCastAmount(0);
          // cas tthe actual spell...
          m_wizard.printNameSpell();
          Misc::delay(80);
          doMagicMissile();
          return;
        } 
      }
    }
    // got to here? then the spell is not valid... look at the next index
    m_tableIndex += 2;
  }
  
  m_targetSquareFound = false;
  m_wizard.setCastAmount(0);
}

// code based on 853b
void WizardCPU::aiCastSubversion() {
  // ths usual, store start pos, create table of all enemies, retrieve start pos and order table
  Arena & arena(Arena::instance());
  int currentIndex = arena.startIndex();
  createTableEnemies();
  arena.setStartIndex(currentIndex);
  Misc::orderTable(m_targetCount, s_priorityTable);
  
  // get the best target square
  m_tableIndex = 1;
  int tmp_cast_range = s_spellData[m_wizard.selectedSpellId()].castRange;
  m_wizard.setCastAmount(1);
  while (s_priorityTable[m_tableIndex] != 0xFF) {
    int creature = arena.at(0,s_priorityTable[m_tableIndex]);
    if (arena.at(4,s_priorityTable[m_tableIndex]) == 0 
       and creature < SPELL_GOOEY_BLOB 
       and creature != 0) {
      // a valid creature for subversion...
      // compare the start and end locations to see if in spell range
      int si = arena.startIndex();
      int dist = Arena::distance(si, s_priorityTable[m_tableIndex]);
      arena.setStartIndex(s_priorityTable[m_tableIndex]);
      bool losBlocked = arena.isBlockedLOS(false);
      arena.setStartIndex(si);
      if (tmp_cast_range >= dist 
          and not losBlocked 
          and not arena.isIllusion(s_priorityTable[m_tableIndex]) )
      {
        // in rangem, have los and is not an illusion (because some one has cast disbeilve on it)
        arena.setTargetIndex(s_priorityTable[m_tableIndex]);
        m_targetSquareFound = true;
        return;
      }
      
    }
    m_tableIndex += 2;
  }
  
  // no decent square found
  m_targetSquareFound = false;
  m_wizard.setCastAmount(0);
}

void WizardCPU::aiCastRaiseDead() {
  m_targetSquareFound = 0;
  // make the byte pair table...
  resetPriorityTable();
  m_targetCount = 0;
  m_tableIndex = 0;
  Arena & arena(Arena::instance());
  for (int i = 0; i < Arena::ARENA_SIZE; i++) {
    if (arena.isDead(i)) {
      int id = arena.at(0,i);
      // is dead, store the priority for this creature
      s_priorityTable[m_tableIndex++] = Arena::attackPref(id);
      s_priorityTable[m_tableIndex++] = i;
      m_targetCount++;
    }
  }
  Misc::orderTable(m_targetCount, s_priorityTable);
  
  // get the best value and try and cast it...
  m_tableIndex = 1;
  while (m_wizard.castAmount() != 0) {
    addBestIndex(arena);
    if (s_priorityTable[m_tableIndex] == 0xFF) {
      return;
    }
    // ok, got a square to cast to... is it in range, los, etc?
    if (arena.atTarget() == 0 or arena.isDead(arena.targetIndex()))
      continue;
    if (not arena.isSpellInRange(m_wizard.selectedSpellId(), false))
      continue;
    if (arena.isBlockedLOS(false))
      continue;
    
    // got to here? good, print name and spell and do cast
    m_wizard.printNameSpell();
    Misc::delay(80);
    doRaisedeadCast();
    m_wizard.setCastAmount(0);
  }
  m_targetSquareFound = true;
}

// code at 86f9
void WizardCPU::aiCastTurmoil()
{
  
  // CALL c7bc
  int currentIndex = Arena::instance().startIndex();
  createTableEnemies();
  Arena::instance().setStartIndex(currentIndex);
  Misc::orderTable(m_targetCount, s_priorityTable);
  
  // this first part depends not on the index values, like most other spells.
  // it instead depends on the priority value... the thinking behind this is: 
  // if there aren't enough enemy creatures really close, it isn't worth casting turmoil
  m_tableIndex = 0;
  u8 total_danger = 0;
  while (s_priorityTable[m_tableIndex] != 0) {
    
    total_danger += s_priorityTable[m_tableIndex];
    m_tableIndex += 2;
    if (total_danger >= 0x1E) {
      break;
    }
  }
  
  if (s_priorityTable[m_tableIndex] == 0) {
    // no decent square
    m_targetSquareFound = 0;
    m_wizard.setCastAmount(0);
    return;
  }
  
  // the spell is worth casting
  m_targetSquareFound = 1;
  m_wizard.setCastAmount(1);
  doTurmoilCast();
}

