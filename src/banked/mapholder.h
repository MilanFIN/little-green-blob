#include <gbdk/platform.h>


#ifndef _mapholder_h
#define _mapholder_h



uint8_t getMapWidth(uint8_t  mapNumber) BANKED;
BANKREF_EXTERN(getMapWidth)

uint8_t getMapHeight(uint8_t  mapNumber) BANKED;
BANKREF_EXTERN(getMapHeight)

uint8_t getMapBank(uint8_t  mapNumber) BANKED;
BANKREF_EXTERN(getMapBank)

uint16_t getMapTiles(uint8_t mapNumber) BANKED;
BANKREF_EXTERN(getMapTiles)

uint16_t getMapPalette(uint8_t mapNumber) BANKED;
BANKREF_EXTERN(getMapPalette)
#endif