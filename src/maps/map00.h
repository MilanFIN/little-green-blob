/*

 MAP00.H

 Map Include File.

 Info:
   Section       : 
   Bank          : 0
   Map size      : 30 x 20
   Tile set      : Z:\home\milan\Documents\gameboy\splat\src\maps\tileset.gbr
   Plane count   : 2 planes (16 bits)
   Plane order   : Planes are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/
#ifndef __map00_h_INCLUDE
#define __map00_h_INCLUDE



const unsigned char* getMap00TilePlane() BANKED;
BANKREF_EXTERN(getMap00TilePlane)
const unsigned char* getMap00PalettePlane() BANKED;
BANKREF_EXTERN(getMap00PalettePlane)

extern const uint8_t getMap00Width() BANKED;
BANKREF_EXTERN(getMap00Width)
extern const uint8_t getMap00Height() BANKED;
BANKREF_EXTERN(getMap00Height)



extern const uint8_t Map00Width;
BANKREF_EXTERN(Map00Width)
extern const uint8_t Map00Height;
BANKREF_EXTERN(Map00Height)

extern const unsigned char Map00PLN0[];
BANKREF_EXTERN(Map00PLN0)
extern const unsigned char Map00PLN1[];
BANKREF_EXTERN(Map00PLN1)
#endif

/* End of MAP00.H */
