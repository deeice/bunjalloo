#include <nds.h>
#include <dswifi7.h>
#include "Wifi7.h"

using namespace nds;
static void wait_for_fifo()
{
  while(REG_IPC_FIFO_CR&IPC_FIFO_RECV_EMPTY) {
    swiWaitForVBlank();
  }
}

// callback to allow wifi library to notify arm9
static void arm7_synctoarm9() { // send fifo message
  REG_IPC_FIFO_TX = 0x87654321;
}
// interrupt handler to allow incoming notifications from arm9
static void arm7_fifo() { // check incoming fifo messages
  u32 msg = REG_IPC_FIFO_RX;
  if(msg==0x87654321) 
    Wifi_Sync();
  else if (msg == 0xDEADFEED)
  {
    // shut down the power to the wifi signal
    Wifi_Deinit();
    irqDisable(IRQ_WIFI);
    irqDisable(IRQ_FIFO_NOT_EMPTY);
    REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR; 
  }
}

Wifi7::Wifi7():m_enabled(false)
{
  REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR; 
}

void Wifi7::mainLoop()
{
  while (1)  {
    if (m_enabled)
      swiWaitForVBlank();
    else
      initialise();
  }
}

void Wifi7::initialise() 
{
  // set up wifi interrupt
  irqSet(IRQ_WIFI, Wifi_Interrupt);
  irqEnable(IRQ_WIFI);

  {
    // sync with arm9 and init wifi
    u32 fifo_temp;
    // wait for magic number
    while(1) {
      wait_for_fifo();
      fifo_temp=REG_IPC_FIFO_RX;
      if(fifo_temp==0x12345678) {
        break;
      }
    }
    // wait for arm9 Wifi_Init return value
    wait_for_fifo();

    fifo_temp=REG_IPC_FIFO_RX; 
    // give arm9's Wifi_Init value to arm 7 Wifi_Init
    Wifi_Init(fifo_temp);

    irqSet(IRQ_FIFO_NOT_EMPTY,arm7_fifo); // set up fifo irq
    irqEnable(IRQ_FIFO_NOT_EMPTY);
    REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ;

    // allow wifi lib to notify arm9
    Wifi_SetSyncHandler(arm7_synctoarm9); 
    // arm7 wifi init complete
    m_enabled = true;
  }
}

