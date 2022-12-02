
#ifndef __map18_h_INCLUDE
#define __map18_h_INCLUDE
const unsigned char* getMap18TilePlane() BANKED;
BANKREF_EXTERN(getMap18TilePlane)
const unsigned char* getMap18PalettePlane() BANKED;
BANKREF_EXTERN(getMap18PalettePlane)

extern const uint8_t getMap18Width() BANKED;
BANKREF_EXTERN(getMap18Width)
extern const uint8_t getMap18Height() BANKED;
BANKREF_EXTERN(getMap18Height)



extern const uint8_t Map18Width;
BANKREF_EXTERN(Map18Width)
extern const uint8_t Map18Height;
BANKREF_EXTERN(Map18Height)

extern const unsigned char Map18PLN0[];
BANKREF_EXTERN(Map18PLN0)
extern const unsigned char Map18PLN1[];
BANKREF_EXTERN(Map18PLN1)

#endif
