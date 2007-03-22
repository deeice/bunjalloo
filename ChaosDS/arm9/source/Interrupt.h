#ifndef Interrupt_h_seen
#define Interrupt_h_seen
namespace Interrupt
{
  //! handle the vblank interrupt.
  void processVblank();

  //! Enable the interrupt - really only enables the animation handling.
  void enable();
  //! Disable the interrupt - really only disables the animation handling.
  void disable();
};
#endif
