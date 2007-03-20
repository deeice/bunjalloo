#ifndef WizardData_h_seen
#define WizardData_h_seen

typedef struct 
{
  const unsigned short * const gfx;
  const unsigned short * const map;
} WizardData_t;

enum {
   GFX_MAGIC_SHIELD = 8,
   GFX_MAGIC_ARMOUR,
   GFX_MAGIC_SWORD,
   GFX_MAGIC_KNIFE,
   GFX_MAGIC_BOW,
   GFX_MAGIC_WINGS,
};

extern const WizardData_t s_wizardData[];
#endif
