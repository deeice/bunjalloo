#ifndef nds_h_seen
#define nds_h_seen
#if !defined(ARM9) && !defined(ARM7)
// wrapper functions and defines to emulate NDS calls.
typedef unsigned char           u8;
typedef unsigned short int      u16;
typedef unsigned int            u32;
typedef unsigned long long int  u64;

typedef signed char             s8;
typedef signed short int        s16;
typedef signed int              s32;
typedef signed long long int    s64;

typedef volatile u8          vu8;
typedef volatile u16         vu16;
typedef volatile u32         vu32;
typedef volatile u64         vu64;

typedef volatile s8           vs8;
typedef volatile s16          vs16;
typedef volatile s32          vs32;
typedef volatile s64          vs64;
typedef void (* VoidFunctionPointer)(void);

#define BIT(n) (1 << (n))
#define BG_WRAP_ON     (1 << 13)
#define BG_MOSAIC_ON   (BIT(6))
#define BG_256_COLOR   (BIT(7))
#define BG_16_COLOR   (0)
#define BG_SIZE(m)		((m<<14))

#define RGB5(r,g,b)  ((r)|((g)<<5)|((b)<<10))

// Sprite control defines

// Attribute 0 consists of 8 bits of Y plus the following flags:
#define ATTR0_NORMAL			(0<<8)
#define ATTR0_ROTSCALE			(1<<8)
#define ATTR0_DISABLED			(2<<8)
#define ATTR0_ROTSCALE_DOUBLE	(3<<8)

#define ATTR0_TYPE_NORMAL		(0<<10)
#define ATTR0_TYPE_BLENDED		(1<<10)
#define ATTR0_TYPE_WINDOWED		(2<<10)
#define ATTR0_BMP				(3<<10)

#define ATTR0_MOSAIC			(1<<12)

#define ATTR0_COLOR_16		(0<<13) //16 color in tile mode...16 bit in bitmap mode
#define ATTR0_COLOR_256		(1<<13)

#define ATTR0_SQUARE		(0<<14)
#define ATTR0_WIDE			(1<<14)
#define ATTR0_TALL			(2<<14)

#define OBJ_Y(m)			((m)&0x00ff)

// Atribute 1 consists of 9 bits of X plus the following flags:
#define ATTR1_ROTDATA(n)      ((n)<<9)  // note: overlaps with flip flags
#define ATTR1_FLIP_X          (1<<12)
#define ATTR1_FLIP_Y          (1<<13)
#define ATTR1_SIZE_8          (0<<14)
#define ATTR1_SIZE_16         (1<<14)
#define ATTR1_SIZE_32         (2<<14)
#define ATTR1_SIZE_64         (3<<14)

#define OBJ_X(m)			((m)&0x01ff)

// Atribute 2 consists of the following:
#define ATTR2_PRIORITY(n)     ((n)<<10)
#define ATTR2_PALETTE(n)      ((n)<<12)
#define ATTR2_ALPHA(n)		  ((n)<<12)


typedef struct sSpriteEntry {
  unsigned short attribute[3];
  unsigned short filler;
} SpriteEntry, * pSpriteEntry;
typedef struct sSpriteRotation {
  unsigned short filler1[3];
  short hdx;

  unsigned short filler2[3];
  short hdy;

  unsigned short filler3[3];
  short vdx;

  unsigned short filler4[3];
  short vdy;
} SpriteRotation, * pSpriteRotation;
void swiWaitForVBlank();

void dmaCopy(const void * source, void * destination, unsigned int length);

#define POWER_ALL 
#define POWER_ALL_2D
void powerON(void);

// Setup the Main screen for 3D 
#define MODE_0_2D      0x10000
#define MODE_1_2D      0x10001
#define MODE_2_2D      0x10002
#define MODE_3_2D      0x10003
#define MODE_4_2D      0x10004
#define MODE_5_2D      0x10005

#define IRQ_VBLANK 0
void irqInit();
void irqSet(int, int);

#define glViewPort glViewport
#define POLY_ALPHA(x)
#define POLY_CULL_NONE

#define glPolyFmt(n) 

#include <GL/gl.h>
#include <GL/glu.h>
#define glClearColor(r,g,b)  glClearColor(r,g,b,0)
#define glPopMatrix(i)  glPopMatrix()

void swiWaitForVBlank();
void glReset(void);

#define COPY_MODE_HWORD  (0)
#define COPY_MODE_WORD  (1<<26)
#define COPY_MODE_COPY  (0)
#define COPY_MODE_FILL  (1<<24)
void swiCopy(const void * source, void * dest, int flags);

//!	Bit values for the keypad buttons.
typedef enum KEYPAD_BITS {
  KEY_A      = BIT(0),  //!< Keypad A button.
  KEY_B      = BIT(1),  //!< Keypad B button.
  KEY_SELECT = BIT(2),  //!< Keypad SELECT button.
  KEY_START  = BIT(3),  //!< Keypad START button.
  KEY_RIGHT  = BIT(4),  //!< Keypad RIGHT button.
  KEY_LEFT   = BIT(5),  //!< Keypad LEFT button.
  KEY_UP     = BIT(6),  //!< Keypad UP button.
  KEY_DOWN   = BIT(7),  //!< Keypad DOWN button.
  KEY_R      = BIT(8),  //!< Right shoulder button.
  KEY_L      = BIT(9),  //!< Left shoulder button.
  KEY_X      = BIT(10), //!< Keypad X button.
  KEY_Y      = BIT(11), //!< Keypad Y button.
  KEY_TOUCH  = BIT(12), //!< Touchscreen pendown.
  KEY_LID    = BIT(13)  //!< Lid state.
} KEYPAD_BITS;
void scanKeys();
u32 keysDown();
u32 keysHeld();
u32 keysDownRepeat();
void keysSetRepeat( u8 setDelay, u8 setRepeat );
void irqSet(int irq, VoidFunctionPointer fp);

void setGenericSound( u32 rate, u8 vol, u8 pan, u8 format);
void playGenericSound(const void* data, u32 length);

// ARM9, ARM7
#else
#include <nds.h>
#endif
#endif
