#include <nds.h>
#include "ndspp.h"
#include "VictoryScreen.h"
#include "Arena.h"
#include "Text16.h"
#include "Misc.h"
#include "Graphics.h"
using namespace nds;

VictoryScreen::VictoryScreen(Victory_t winOrDraw):
  m_winOrDraw(winOrDraw),m_highlightItem(0)
{}

void VictoryScreen::show()
{
  if (m_winOrDraw == WIN)
  {
    win();
  }
  else
  {
    draw();
  }
}

void VictoryScreen::draw()
{
#if 0
  // "THE CONTEST IS DRAWN BETWEEN"
  // then wipe all the flags and stuff and return to splash screen
  // clear arena screen...
  fade_down();
  remove_cursor();
  clear_arena();
  clear_bg();
  clear_palettes();
  
  draw_decor_border(15, Color(0,0,31),Color(0,31,31)); 
  
  text16.setColour(10, Color(30,31,0)); // yellow
  text16.print("THE CONTEST IS DRAWN BETWEEN", 1,1, 10);  
  u8 y = 3, i;
  for (i = 0; i < playercount; i++) {
    if (!IS_WIZARD_DEAD(players[i].modifier_flag)) {
      text16.print(players[i].name, 9,y, 13);  
      y+=2;
    }
  }
  fade_up();
  end_game();
#endif
}

void VictoryScreen::win()
{
#if 0
  // <player>" IS THE WINNER" 
  // then wipe all the flags and stuff and return to splash screen
  fade_down();
  u8 winner = 0, i;
  for (i = 0; i < playercount; i++) {
    if (!IS_WIZARD_DEAD(players[i].modifier_flag)) {
      winner = i;
      break;
    }
  }
  // clear arena screen...
  remove_cursor();
  clear_arena();
  clear_bg();
  clear_palettes();
  
  draw_decor_border(15, Color(0,0,31),Color(0,31,31)); 
  
  //char str[30];
  text16.setColour(10, chaos_cols[2]); 
  text16.setColour(12, chaos_cols[4]); 
  text16.setColour(13, chaos_cols[8]); 
  
  text16.print("THE WINNER IS:", 8,2, 10);  
  text16.print("++++++++++++++++", 7,6, 12);  
  text16.print("+", 7,8, 12);  text16.print("+", 22,8, 12);  
  text16.print("+", 7,10, 12);  text16.print("+", 22,10, 12);  
  text16.print(players[winner].name, 10,10, 13);  
  text16.print("+", 7,12, 12);  text16.print("+", 22,12, 12);  
  text16.print("++++++++++++++++", 7,14, 12);  
  fade_up();
  end_game();
#endif
  
}

void VictoryScreen::animate()
{
}
void VictoryScreen::handleKeys()
{
}
#if 0
void end_game(void) {
  players[0].plyr_type = PLYR_HUMAN;
  current_player = 0; // otherwise cpu player might just immediately "press" a key!
  wait_for_letgo();
  u8 keypressed = 0;
  u8 i = 0, j = 4, k = 8;
  /* Place this in handleKeys() */
  while (!keypressed) {
    Misc::delay(4,false);
    text16.setColour(10, chaos_cols[i]); 
    text16.setColour(12, chaos_cols[j]); 
    text16.setColour(13, chaos_cols[k]); 
    
    BGPaletteMem[16*15+1] = chaos_cols[j];
    BGPaletteMem[16*15+2] = chaos_cols[i];
    
    i++; j++; k++;
    if (i > 8) i=0;
    if (j > 8) j=0;
    if (k > 8) k=0;
    
    wait_vsync_int();
    UpdateKeys();
    if (KeyPressed (KEY_A)){
      keypressed = 1;
    }
  }
  fade_down();
  clear_game_border();
  wait_for_letgo();
  reset_arena_tables();
  reset_players();
  world_chaos = 0;
  current_player = 0;
  dead_wizards = 0;
  playercount = 0;
  load_all_palettes();
  show_splash();
  move_screen_to(0);
  fade_up();
}
#endif

// Only CPU left, ask if we should carry on...
void VictoryScreen::endGameOption()
{
  Video::instance().fade();
  // clear arena screen...
  Arena & arena(Arena::instance());
  Text16 & text16(Text16::instance());
  arena.enableCursor(false);
  arena.clear();
  text16.clear();
  Graphics::instance().clearPalettes();
  arena.decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 
  
  //char str[30];
  text16.setColour(9, Color(31,31,0));
  
  text16.print("CONTINUE THE BATTLE?", 5,4, 9);  
  text16.print("YES", 10,8, 7);  
  text16.print("NO",  16,8, 8);  
  
  text16.print("PRESS A", 11,16,  9);  
  
  text16.setColour(7, Color(31,31,0)); 
  text16.setColour(8, Color(12,12,0)); 
  
  bool decided(false);
  int highlightItem = 0;
  Video::instance().fade(false);
  while (not decided) {
    Misc::delay(2,false);
    scanKeys();
    u16 keysSlow = keysDownRepeat();
    
    if (keysSlow & KEY_LEFT) {
      highlightItem = 0;
    }
    if (keysSlow & KEY_RIGHT) {
      highlightItem = 1;
    }
    if (keysSlow & KEY_A) {
      decided = true;
    }
    
    if (keysSlow & KEY_B) {
      decided = true;
      highlightItem = 0;
    }
    
    if (highlightItem  == 0 ) {
      text16.setColour(7, Color(31,31,0)); 
      text16.setColour(8, Color(12,12,0)); 
    } else {
      text16.setColour(8, Color(31,31,0)); 
      text16.setColour(7, Color(12,12,0)); 
    }
  }
  arena.setCurrentPlayer(0);
  if (highlightItem) {
    arena.setCurrentPlayer(9);
  }
}
