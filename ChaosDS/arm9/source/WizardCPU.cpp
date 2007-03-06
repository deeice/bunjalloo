#include "WizardCPU.h"
#include "Wizard.h"
#include "Arena.h"
#include "SpellData.h"
#include "Misc.h"
#include "Casting.h"

// create the priority table - stored at d3f2
// will contain {priority, index} for all living enemy creatures
static unsigned char s_priorityTable[320];

WizardCPU::WizardCPU(Wizard & theWizard) :
  m_tableIndex(0), m_wizard(theWizard), m_targetCount(0)
{}

void WizardCPU::doAISpell()
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
  unsigned char * spells(m_wizard.spellArray());
  int spellCount(m_wizard.spellCount());
  spells[0] = 12 + Misc::getRand(10);
  Misc::orderTable(spellCount, spells);

  int bestSpell(0);
  while (bestSpell < spellCount) {
    m_wizard.setSelectedSpell(bestSpell);
    // "cast" the spell... each casting routine has the CPU AI code for that spell built into it.
    // if no good square was found, then go to the next spell
    m_targetSquareFound = false;
    int currentSpellId(m_wizard.getSelectedSpellId());
    if (currentSpellId != 0) {
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

void WizardCPU::aiCastCreature() 
{
  // the computer tries to cast a creature..
  // code based on 9984
  
  // get the square closest to the best target...
  int strongestIndex(getStrongestWizard(0xFF));
  
  /* TODO - this may not be needed
  m_startIndex = m_wizardIndex;
  */
  m_targetSquareFound = false;
  int range = 3;
  int currentSpellId(m_wizard.getSelectedSpellId());
  if (currentSpellId >= SPELL_GOOEY_BLOB)
    range = 13;
  
  int inRange = createRangeTable(strongestIndex, range);
  
  // how to do this?
  // have a table, and an index. uses m_targetSquareFound to do stuff
  // also needs access to other Wizard and Arena innards :(
  // Maybe create a class (AIHandler?) that is inside the Wizard?
  m_tableIndex = inRange*2+1;
  getFurthestInrange();
  
  if (m_targetSquareFound) {
    m_wizard.printNameSpell();
    Misc::delay(80);
    Casting::spellAnimation();
    m_wizard.setIllusion(false);
    
    if (currentSpellId < SPELL_GOOEY_BLOB) {
      // randomly cast illusions, but more chance of trying if the spell is tricky to cast
      int currentSpellChance(s_spellData[currentSpellId].getCastChance());
      if ( (currentSpellChance < 4 and Misc::getRand(10) > 5) 
          or (Misc::getRand(10) > 7) )
      {
        m_wizard.setIllusion(true);
      }
    }
    
    if (Casting::setSpellSuccess())
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
int WizardCPU::getStrongestWizard(int attackerIndex) {
  if (m_wizard.timid() < 10) {
    // if we have less than 10 in this value, 
    // go for the wizard who's creature is closest to us
    createTableEnemies();
    
    // the best value will now be the index of the creature closest to us
    // or the wizard closest to us, as they are treated the same in the create_table_enemies routine
    int enemy_creature = s_priorityTable[0];
    
    if (enemy_creature == 0) {
      
      // no dangerous creature found..
      // create the priority table based on "strongest wizard" - one with most/best creatures
      create_table_wizards();
      Arena::instance().setTargetIndex(s_priorityTable[1]);
      
      // in the actual game it does a pointless thing here...
      return s_priorityTable[1];
      
    } else {
      // first of all, get the most dangerous creature... regardless of whether we can kill it or not
      // if we can't kill it, later we will attack its owner to do away with it
      Misc::orderTable(m_targetCount, s_priorityTable);
      
      enemy_creature = s_priorityTable[1];
      
      int enemy_wizard = Arena::instance().getOwner(enemy_creature);
      // GET_OWNER(arena[3][enemy_creature]); 
      
      enemy_wizard = Arena::instance().getWizardIndex(enemy_wizard);

      // enemy wizard = index to the enemy wizard attacking us
      // enemy creature = index to the enemy creature attacking us
      // now see which is closer and attack them
      int creature_range = Arena::getDistance(enemy_creature, Arena::instance().startIndex());
      int wizard_range   = Arena::getDistance(enemy_wizard, Arena::instance().startIndex());
      
      /* 
        Magic Fire close to a wizard confuses the enemy creatures
        Try this out, so that fire is not attractive
        Also, undead creatures should not be attractive to living ones
      */
      bool attacker_undead(false);
      if (attackerIndex == 0xff) {
        // a creature spell to cast... so use the spellid
        int current_spell = m_wizard.getSelectedSpellId();
        attacker_undead = (current_spell >= SPELL_VAMPIRE and current_spell <= SPELL_ZOMBIE );
      } else {
        // is a valid square
        // see if the attacking square is undead
        attacker_undead = IS_UNDEAD(arena[3][attackerIndex]) 
          or (arena[0][attackerIndex] >= SPELL_VAMPIRE  and arena[0][attackerIndex] <= SPELL_ZOMBIE );
      }
      
      u8 defender_undead = 0;
      
      int tmp_prio_index = 1;
      while (enemy_creature != 0xFF) {
        enemy_creature = s_priorityTable[tmp_prio_index];
        if (arena[0][enemy_creature] != SPELL_MAGIC_FIRE) {
          // not magic fire so check undeadness
          // remember that we are still scared of magic fire and undead creatures
          // just that we are not worried about attacking it
          defender_undead = (arena[0][enemy_creature] >= SPELL_VAMPIRE  and
                            arena[0][enemy_creature] <= SPELL_ZOMBIE ) or 
                            IS_UNDEAD(arena[3][enemy_creature]);
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
    create_table_wizards();
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
    int tmprange = Arena::getDistance(i, Arena::instance().startIndex());
    if (range >= tmprange) {
      // square is in range
      tmprange = Arena::getDistance(i, target);
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
void WizardCPU::getFurthestInrange() 
{
  // get the furthest square away still in range...
  do {
    Arena::instance().setTargetIndex(s_priorityTable[m_tableIndex]);
    
    Misc::delay(5);
    // check xpos < 10  - code at c63d
    int x,y;
    Arena::getXY(Arena::instance().targetIndex(), x, y);
    if (x < 0x10) {
      // in range
      // isBlockedLOS()
      if (!Arena::instance().isBlockedLOS()) {
        
        if (arena[0][Arena::instance().targetIndex()] == 0) {
          // nothing here
          m_targetSquareFound = true;
        } else if (arena[2][Arena::instance().targetIndex()] == 4){
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
    int valid = contains_enemy(Arena::instance().targetIndex());
    if (valid > 0) {
      m_targetCount++;
      int range = Arena::getDistance(Arena::instance().targetIndex(), 
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

void create_table_wizards(void) {
  // code is from c825 
  reset_priority_table();
//  char str[20];
  
  prio_table_index = 0;
  u8 i;
  target_index = 0;
  for (i = 0; i < 0x9f; i++) {
    if (arena[0][target_index] >= WIZARD_INDEX) {
      create_enemy_table_entry(arena[0][i]-WIZARD_INDEX);
    }
    if (arena[4][target_index] >= WIZARD_INDEX) {
      create_enemy_table_entry(arena[4][i]-WIZARD_INDEX);
    }
    target_index++;

  }
  
  order_table(7, prio_table);
  
  
}

// create an enemy table entry for this wizard
// code at c859
void create_enemy_table_entry(u8 wizardid) {
  if (wizardid == current_player) 
    return;
  
  prio_table[prio_table_index+1] = target_index;
  u8 i;
  for (i = 0; i < 0x9f; i++) {
    if (arena[0][i] > 1 && arena[0][i] < 0x26) {
      if (GET_OWNER(arena[3][i]) != wizardid) {
        prio_table[prio_table_index] = (attack_pref[arena[0][i]]>>2) + prio_table[prio_table_index] 
                                      + priority_offset;
        
      }
    }
    
  }
  prio_table_index +=2;
    
}

// code based on c8c7
// get the priorities of squares surrounding "index" based on distance from "strongest"
void get_surrounding_square_prios(u8 index, u8 strongest) {
  
  reset_priority_table();
  u8 i;
  u8 surround_index = 0;
  u16 range;
  u8 lookat_i;
  LUT_index = 0;
  for (i = 0; i < 8; i++) {
    lookat_i = apply_position_modifier(index, surround_index);
    surround_index++;
    if (lookat_i == 0) {
      // out of bounds
      continue;
    }
    lookat_i--;
    
    get_distance(strongest, lookat_i, &range);
    // add a check in here to make sure we don't
    // tread on a magic wood. This table is ordered largest
    // first. But the potential squares are read starting 
    // at the end (LUT_index, then LUT_index-2, etc)
    if (arena[0][lookat_i] == SPELL_MAGIC_WOOD)
      range += 1;  // "increase" the distance so it isn't as appealing
                   // careful not to increase too much, may go backwards!
    
    prio_table[LUT_index++] = range;
    prio_table[LUT_index++] = i;
    
  }
  order_table(7, prio_table);
  LUT_index = 0xF;
  
}

