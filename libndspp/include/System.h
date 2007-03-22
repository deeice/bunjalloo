#ifndef System_h_seen
#define System_h_seen

namespace nds
{
  class System
  {

    public:
      /**
       * When the lid is closed, this function sets up a lid open interrupt,
       * powers down and sleeps. When the lid is opened again, the power is
       * restored and the program continues.
       * <p>
       * On the arm7 all interrupts are turned off except IRQ_LID. On the arm9
       * no changes are made to the IRQ flags. Since the arm7 has deactivated
       * VBLANK, the arm9 sleeps until the the arm7 turns IRQ_VBLANK on again -
       * this occurrs when the arm7 is woken by the LID being opened.
       */
      static void checkSleep();

  };
}
#endif
