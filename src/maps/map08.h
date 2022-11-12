#include <gbdk/platform.h>

/*

 MAP08.H

 Map Include File.

 Info:
   Section       : 
   Bank          : 0
   Map size      : 20 x 40
   Tile set      : Z:\home\milan\Documents\gameboy\splat\src\maps\tileset.gbr
   Plane count   : 2 planes (16 bits)
   Plane order   : Planes are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/
#ifndef __map08_h_INCLUDE
#define __map08_h_INCLUDE



const unsigned char* getMap08TilePlane() BANKED;
BANKREF_EXTERN(getMap08TilePlane)
const unsigned char* getMap08PalettePlane() BANKED;
BANKREF_EXTERN(getMap08PalettePlane)

extern const uint8_t getMap08Width() BANKED;
BANKREF_EXTERN(getMap08Width)
extern const uint8_t getMap08Height() BANKED;
BANKREF_EXTERN(getMap08Height)



extern const uint8_t Map08Width;
BANKREF_EXTERN(Map08Width)
extern const uint8_t Map08Height;
BANKREF_EXTERN(Map08Height)

extern const unsigned char Map08PLN0[];
BANKREF_EXTERN(Map08PLN0)
extern const unsigned char Map08PLN1[];
BANKREF_EXTERN(Map08PLN1)


#endif

/* End of MAP08.H */
