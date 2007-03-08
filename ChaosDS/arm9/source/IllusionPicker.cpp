#include <nds.h>
#include "ndspp.h"
#include "IllusionPicker.h"
#include "Text16.h"
#include "Wizard.h"
#include "SoundEffect.h"
#include "GameState.h"
#include "GameMenu.h"

void IllusionPicker::show()
{
  // ask if they want an illusion and wait for response
  Text16 & text16(Text16::instance());
  text16.setColour(14, 0);  
  text16.setColour(12, nds::Color(31,31,31));
  text16.print("ILLUSION?", 18, 8, 12);  
  text16.print("(A=YES B=NO)", 17,10, 12);

}

void IllusionPicker::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  bool choiceMade(false);

  if (keysSlow & KEY_A) {
    Wizard & player(Wizard::currentPlayer());
    player.setIllusionCast(true);
    choiceMade = true;
  } 
  else if (keysSlow & KEY_B) {
    choiceMade = true;
  } 
  if (choiceMade)
  {
    SoundEffect::play(SND_CHOSEN);
    b();
  }
}
