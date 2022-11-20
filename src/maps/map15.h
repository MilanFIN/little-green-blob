
#ifndef __map15_h_INCLUDE
#define __map15_h_INCLUDE
const unsigned char* getMap15TilePlane() BANKED;
BANKREF_EXTERN(getMap15TilePlane)
const unsigned char* getMap15PalettePlane() BANKED;
BANKREF_EXTERN(getMap15PalettePlane)

extern const uint8_t getMap15Width() BANKED;
BANKREF_EXTERN(getMap15Width)
extern const uint8_t getMap15Height() BANKED;
BANKREF_EXTERN(getMap15Height)



extern const uint8_t Map15Width;
BANKREF_EXTERN(Map15Width)
extern const uint8_t Map15Height;
BANKREF_EXTERN(Map15Height)

extern const unsigned char Map15PLN0[];
BANKREF_EXTERN(Map15PLN0)
extern const unsigned char Map15PLN1[];
BANKREF_EXTERN(Map15PLN1)

#endif
