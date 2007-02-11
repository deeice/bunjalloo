#include <nds.h>
#include <dswifi9.h>
#include "Wifi9.h"
#define         VCOUNT          (*((u16 volatile *) 0x04000006))

// wifi timer function, to update internals of sgIP
static void Timer_50ms(void) {
  Wifi_Timer(50);
}

// notification function to send fifo message to arm7
static void arm9_synctoarm7() {
  // send fifo message
  REG_IPC_FIFO_TX=0x87654321;
}

// interrupt handler to receive fifo messages from arm7
static void arm9_fifo() {
  // check incoming fifo messages
  u32 value = REG_IPC_FIFO_RX;
  if(value == 0x87654321) Wifi_Sync();
}

using namespace nds;

Wifi9::Wifi9():m_connected(false)
{
  initialise();
}
Wifi9 & Wifi9::instance()
{
  static Wifi9 s_wifi9;
  return s_wifi9;
}

bool Wifi9::connected() const
{
  return m_connected;
}

void Wifi9::initialise()
{ 
  // send fifo message to initialize the arm7 wifi
  // enable & clear FIFO
  REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;

  u32 Wifi_pass= Wifi_Init(WIFIINIT_OPTION_USELED);
  REG_IPC_FIFO_TX=0x12345678;
  REG_IPC_FIFO_TX=Wifi_pass;

  // disable timer3
  *((volatile u16 *)0x0400010E) = 0;

  irqInit();
  // setup timer IRQ
  irqSet(IRQ_TIMER3, Timer_50ms);
  irqEnable(IRQ_TIMER3);

  // setup fifo IRQ
  irqSet(IRQ_FIFO_NOT_EMPTY, arm9_fifo);
  irqEnable((IRQ_MASK)(IRQ_FIFO_NOT_EMPTY|IRQ_VBLANK|IRQ_TIMER3));

  // enable FIFO IRQ
  REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ;

  // tell wifi lib to use our handler to notify arm7
  Wifi_SetSyncHandler(arm9_synctoarm7); 

  // set timer3
  // 6553.1 * 256 cycles = ~50ms;
  *((volatile u16 *)0x0400010C) = -6553;
  // enable, irq, 1/256 clock
  *((volatile u16 *)0x0400010E) = 0x00C2; 

  // wait for arm7 to be initted successfully
  while(Wifi_CheckInit()==0) {
    // wait for vblank
    while(VCOUNT>192); 
    while(VCOUNT<192);
  }
  // wifi init complete - wifi lib can now be used!
}

bool Wifi9::connect()
{ 
  // if connected, you can use the berkley sockets interface to connect to the internet
  if (m_connected)
    return true;

  // simple WFC connect: 
  int i; 
  // request connect - uses the stored data to connect
  m_connected = false;
  Wifi_AutoConnect();
  while(1) { 
    // check status 
    i=Wifi_AssocStatus(); 
    if(i==ASSOCSTATUS_ASSOCIATED) { 
      m_connected = true;
      return true;
    } 
    if(i==ASSOCSTATUS_CANNOTCONNECT) { 
      return false;
    } 
  } 
} 

void Wifi9::disconnect()
{
  REG_IPC_FIFO_TX=0xDEADFEED;
  m_connected = false;
  Wifi_DisableWifi();
}
