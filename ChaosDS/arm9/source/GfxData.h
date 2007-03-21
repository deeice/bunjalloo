#ifndef GfxData_h_seen
#define GfxData_h_seen
typedef struct {
  const u16 * const data; //!< Pointer to the data.
  u32 size;               //!< Number of half words in the data.
} GfxData;


extern const GfxData s_paletteData[10];
#endif
