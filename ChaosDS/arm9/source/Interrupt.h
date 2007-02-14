#ifndef Interrupt_h_seen
#define Interrupt_h_seen
namespace Interrupt
{
  //! handle the vblank interrupt.
  void processVblank();

  void enable();
  void disable();
};
#endif
