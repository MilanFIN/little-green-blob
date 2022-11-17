/*

 MAP13.H

 Map Include File.

 Info:
   Section       : 
   Bank          : 0
   Map size      : 25 x 45
   Tile set      : tileset.gbr
   Plane count   : 2 planes (16 bits)
   Plane order   : Planes are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/
#ifndef __map13_h_INCLUDE
#define __map13_h_INCLUDE

const unsigned char* getMap13TilePlane() BANKED;
BANKREF_EXTERN(getMap13TilePlane)
const unsigned char* getMap13PalettePlane() BANKED;
BANKREF_EXTERN(getMap13PalettePlane)

extern const uint8_t getMap13Width() BANKED;
BANKREF_EXTERN(getMap13Width)
extern const uint8_t getMap13Height() BANKED;
BANKREF_EXTERN(getMap13Height)



extern const uint8_t Map13Width;
BANKREF_EXTERN(Map13Width)
extern const uint8_t Map13Height;
BANKREF_EXTERN(Map13Height)

extern const unsigned char Map13PLN0[];
BANKREF_EXTERN(Map13PLN0)
extern const unsigned char Map13PLN1[];
BANKREF_EXTERN(Map13PLN1)

#endif

/* End of MAP13.H */
