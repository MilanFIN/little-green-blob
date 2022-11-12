#include <gbdk/platform.h>

#ifndef _banked_h
#define _banked_h

/*
extern const unsigned char FLOORTILES[2];
BANKREF_EXTERN(FLOORTILES)

extern const uint8_t FLOORTILECOUNT;
BANKREF_EXTERN(FLOORTILECOUNT)

extern const unsigned char ROOFTILES[1];
BANKREF_EXTERN(ROOFTILES)

extern const uint8_t ROOFTILECOUNT;
BANKREF_EXTERN(ROOFTILECOUNT)

extern const unsigned char OBSTACLETILES[1];
BANKREF_EXTERN(OBSTACLETILES)

extern const uint8_t OBSTACLECOUNT;
BANKREF_EXTERN(OBSTACLETILES)

extern const unsigned char SWITCHBLOCKS[2];
BANKREF_EXTERN(SWITCHBLOCKS)

extern const uint8_t SWITCHTILE;
BANKREF_EXTERN(SWITCHTILE)

extern const unsigned char TIMETRAPBLOCKS[1];
BANKREF_EXTERN(TIMETRAPBLOCKS)

extern const unsigned char TIMEPLATFORMBLOCKS[2];
BANKREF_EXTERN(TIMEPLATFORMBLOCKS)
*/











uint8_t uClamp( uint8_t value, uint8_t min, uint8_t max ) BANKED;
BANKREF_EXTERN(uClamp)

int16_t i16Clamp( int16_t value, int16_t min, int16_t max ) BANKED;
BANKREF_EXTERN(i16Clamp)

uint16_t u16Clamp( uint16_t value, uint16_t min, uint16_t max ) BANKED;
BANKREF_EXTERN(u16Clamp)

int16_t i16abs(int16_t value) BANKED;
BANKREF_EXTERN(i16abs)


void playDeathAnimation(uint16_t x, uint16_t y) BANKED;
BANKREF_EXTERN(playDeathAnimation)


uint8_t checkFinish(uint16_t finishTileIndex, uint16_t mapWidth, uint16_t x, uint16_t y) BANKED;
BANKREF_EXTERN(checkFinish)

#endif