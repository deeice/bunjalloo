#include "Wizard.h"

void Wizard::aiCastCreature() 
{
  // the computer tries to cast a creature..
  // code based on 9984
  
  // get the square closest to the best target...
  int strongest_index(get_strongest_wizard(0xFF));
  
  start_index = wizard_index;
  target_square_found = 0;
  u8 range = 3;
  if (current_spell >= SPELL_GOOEY_BLOB)
    range = 13;
  
  s16 in_range = (s16)create_range_table(strongest_index, range);
  
  LUT_index = in_range*2+1;
  get_furthest_inrange();
  
  if (target_square_found) {
    print_name_spell();
    delay(80);
    spell_animation();
    temp_illusion_flag = 0;
    
    if ( (current_spell < SPELL_GOOEY_BLOB)) {
      // randomly cast illusions, but more chance of trying if the spell is tricky to cast
      if ( (current_spell_chance < 4 && GetRand(10) > 5) || (GetRand(10) > 7) )
        temp_illusion_flag = 1;
    }
    
    set_spell_success();
    
    
    if (temp_success_flag)
      creature_spell_succeeds(target_index);
    print_success_status();
    
    draw_all_creatures();
    delay(10);
  } // else no square found!
  
  temp_cast_amount = 0;
}

// based on code at c78d
// additionally, pass in the attakcer - or 0xff if no attacker ready 
// and check "current spell" value
int Wizard::get_strongest_wizard(int attacker_index) {
  if (players[current_player].timid < 0xA) {
    // if we have less than 10 in this value, 
    // go for the wizard who's creature is closest to us
    create_table_enemies();
    
    // the best value will now be the index of the creature closest to us
    // or the wizard closest to us, as they are treated the same in the create_table_enemies routine
    u8 enemy_creature = prio_table[0];
    
    if (enemy_creature == 0) {
      
      // no dangerous creature found..
      // create the priority table based on "strongest wizard" - one with most/best creatures
      create_table_wizards();
      target_index = prio_table[1];
      
      
      // in the actual game it does a pointless thing here...
      return target_index;
      
    } else {
      // first of all, get the most dangerous creature... regardless of whether we can kill it or not
      // if we can't kill it, later we will attack its owner to do away with it
      order_table(target_count, prio_table);
      
      enemy_creature = prio_table[1];
      
      u8 enemy_wizard = GET_OWNER(arena[3][enemy_creature]); 
      
      u8 i;
      for (i = 0; i < 0x9f; i++) {
        if (arena[0][i] - WIZARD_INDEX == enemy_wizard) {
          enemy_wizard = i;
          break;
        }
        if (arena[4][i] - WIZARD_INDEX == enemy_wizard) {
          enemy_wizard = i;
          break;
        }
      }
      // enemy wizard = index to the enemy wizard attacking us
      // enemy creature = index to the enemy creature attacking us
      // now see which is closer and attack them
      u16 creature_range, wizard_range;
      get_distance(enemy_creature, start_index, &creature_range);
      get_distance(enemy_wizard, start_index, &wizard_range);
      
      /* 
        Magic Fire close to a wizard confuses the enemy creatures
        Try this out, so that fire is not attractive
        Also, undead creatures should not be attractive to living ones
      */
      u8 attacker_undead = 0;
      if (attacker_index == 0xff) {
        // a creature spell to cast... so use the spellid
        attacker_undead = (current_spell >= SPELL_VAMPIRE  &&
                            current_spell <= SPELL_ZOMBIE );
      } else {
        // is a valid square
        // see if the attacking square is undead
        attacker_undead = IS_UNDEAD(arena[3][attacker_index]) ||
                          (arena[0][attacker_index] >= SPELL_VAMPIRE  &&
                            arena[0][attacker_index] <= SPELL_ZOMBIE );
      }
      
      u8 defender_undead = 0;
      
      int tmp_prio_index = 1;
      while (enemy_creature != 0xFF) {
        enemy_creature = prio_table[tmp_prio_index];
        if (arena[0][enemy_creature] != SPELL_MAGIC_FIRE) {
          // not magic fire so check undeadness
          // remember that we are still scared of magic fire and undead creatures
          // just that we are not worried about attacking it
          defender_undead = (arena[0][enemy_creature] >= SPELL_VAMPIRE  &&
                            arena[0][enemy_creature] <= SPELL_ZOMBIE ) || 
                            IS_UNDEAD(arena[3][enemy_creature]);
          if ( !defender_undead || attacker_undead) { 
            
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
    target_index = prio_table[1];
    
    // in the actual game it does a pointless thing here...
    return target_index;
  }
  
}
