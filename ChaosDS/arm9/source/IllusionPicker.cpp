#include "libnds.h"
#include "ndspp.h"
#include "IllusionPicker.h"
#include "Text16.h"
#include "Wizard.h"
#include "SoundEffect.h"
#include "GameState.h"
#include "GameMenu.h"
#include "HotSpot.h"

using nds::Rectangle;

static const int YES_POS_X(17);
static const int YES_POS_Y(10);

IllusionPicker::IllusionPicker():SpellSelect(false) 
{
  Rectangle yesRect = { YES_POS_X*8, YES_POS_Y*8, 6*8, 16};
  m_hotspots.push_back(new HotSpot(yesRect, yesCb, this));
  Rectangle noRect = { (YES_POS_X*8)+yesRect.w, YES_POS_Y*8, 6*8, 16};
  m_hotspots.push_back(new HotSpot(noRect, noCb, this));
}

void IllusionPicker::yesCb(void * arg)
{
  IllusionPicker * self = (IllusionPicker*)arg;
  Wizard & player(Wizard::currentPlayer());
  player.setIllusionCast(true);
  self->b();
}

void IllusionPicker::noCb(void * arg)
{
  IllusionPicker * self = (IllusionPicker*)arg;
  Wizard & player(Wizard::currentPlayer());
  player.setIllusionCast(false);
  self->b();
}

void IllusionPicker::show()
{
  // ask if they want an illusion and wait for response
  Text16 & text16(Text16::instance());
  text16.setColour(14, 0);  
  text16.setColour(12, nds::Color(31,31,31));
  text16.print("ILLUSION?", 18, 8, 12);  
  text16.print("(A=YES B=NO)", YES_POS_X, YES_POS_Y, 12);

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
  if (keysSlow & KEY_TOUCH)
  {
    handleTouch();
  }
  if (choiceMade)
  {
    SoundEffect::play(SND_CHOSEN);
    b();
  }
}
