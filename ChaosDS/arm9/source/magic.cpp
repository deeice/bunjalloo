void cast_disbelieve()
{}

void cast_creature()
{  
  // if human just do the check on target_index, if CPU find the optimum square to cast to
  // if CPU and no square, set a target_square_found = 0 and return withough doing anything.
#if 0
  // temp_cast_amount = 1;  // set this in Wiz
  if (IS_CPU(current_player)) {
    // do the cpu creature cast ai routine 
    ai_cast_creature();
  } else {
    
    // call code at 9856 - validates player's spell cast
    if (!player_cast_ok()) {
      return;
    }
    
    //set_spell_success();
    spell_animation();
    if (temp_success_flag)
      creature_spell_succeeds(target_index);
    print_success_status();
    temp_cast_amount =0;
    draw_all_creatures();
    
    delay(30);
  }
#endif
}

void cast_chaos_law()
{}
void cast_trees_castles()
{}
void cast_fire_goo()
{}
void cast_wall()
{}
void cast_magic_missile()
{}
void cast_justice()
{}

void cast_raise_dead()
{}
void cast_subversion()
{}
void cast_magic_shield()
{}
void cast_magic_armour()
{}
void cast_magic_sword()
{}
void cast_magic_knife()
{}
void cast_magic_bow()
{}
void cast_magic_wings()
{}
void cast_shadow_form()
{}

void cast_turmoil()
{}

