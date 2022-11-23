
#ifndef __map16_h_INCLUDE
#define __map16_h_INCLUDE
const unsigned char* getMap16TilePlane() BANKED;
BANKREF_EXTERN(getMap16TilePlane)
const unsigned char* getMap16PalettePlane() BANKED;
BANKREF_EXTERN(getMap16PalettePlane)

extern const uint8_t getMap16Width() BANKED;
BANKREF_EXTERN(getMap16Width)
extern const uint8_t getMap16Height() BANKED;
BANKREF_EXTERN(getMap16Height)



extern const uint8_t Map16Width;
BANKREF_EXTERN(Map16Width)
extern const uint8_t Map16Height;
BANKREF_EXTERN(Map16Height)

extern const unsigned char Map16PLN0[];
BANKREF_EXTERN(Map16PLN0)
extern const unsigned char Map16PLN1[];
BANKREF_EXTERN(Map16PLN1)

#endif
