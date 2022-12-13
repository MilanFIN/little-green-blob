#include <gbdk/platform.h>

#ifndef _banked_h
#define _banked_h

/*

extern const uint8_t SWITCHTILE;
BANKREF_EXTERN(SWITCHTILE)

extern const unsigned char TIMETRAPBLOCKS[1];
BANKREF_EXTERN(TIMETRAPBLOCKS)
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


void initBleedSprite() BANKED;
BANKREF_EXTERN(initBleedSprite)

void spawnBleed(uint16_t x, uint16_t y, int8_t direction) BANKED;
BANKREF_EXTERN(spawnBleed)

void scrollBleed(int16_t xchange, int16_t ychange) BANKED;
BANKREF_EXTERN(scrollBleed)

void loopBleed() BANKED;
BANKREF_EXTERN(loopBleed)

void initSplashDownSprite() BANKED;
BANKREF_EXTERN(initSplashDownSprite)


void spawnSplash(uint16_t x, uint16_t y, uint16_t sideEdge) BANKED;
BANKREF_EXTERN(spawnSplash)

void scrollSplash(int16_t xchange, int16_t ychange) BANKED;
BANKREF_EXTERN(scrollSplash)

void loopSplash() BANKED;
BANKREF_EXTERN(loopSplash)

void initFont() BANKED;
BANKREF_EXTERN(initFont)

void showStartMenu() BANKED;
BANKREF_EXTERN(showStartMenu)

uint8_t levelSelectionMenu(uint8_t mapcount, uint8_t map) BANKED;
BANKREF_EXTERN(levelSelectionMenu)

uint8_t mapCompleted(uint8_t map, int16_t score) BANKED;
BANKREF_EXTERN(mapCompleted)

#endif