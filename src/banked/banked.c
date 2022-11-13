#include <gbdk/platform.h>
#include <gb/gb.h>

#pragma bank 255

/*
BANKREF(FLOORTILES)
const unsigned char FLOORTILES[2] = {0x01, 0x06}; 
BANKREF(FLOORTILECOUNT)
const uint8_t FLOORTILECOUNT = 2;
BANKREF(ROOFTILES)
const unsigned char ROOFTILES[1] = {0x01}; 
BANKREF(ROOFTILECOUNT)
const uint8_t ROOFTILECOUNT = 1;
BANKREF(OBSTACLETILES)
const unsigned char OBSTACLETILES[1] = {0x07};
BANKREF(OBSTACLECOUNT)
const uint8_t OBSTACLECOUNT = 1;
BANKREF(SWITCHBLOCKS)
const unsigned char SWITCHBLOCKS[2] = {0x08, 0x09}; 
BANKREF(SWITCHTILE)
const uint8_t SWITCHTILE = 0x0a;
BANKREF(TIMETRAPBLOCKS)
const unsigned char TIMETRAPBLOCKS[1] = {0x0b}; 
BANKREF(TIMEPLATFORMBLOCKS)
const unsigned char TIMEPLATFORMBLOCKS[2] = {0x0c, 0x0d}; 
*/






BANKREF(uClamp)
uint8_t uClamp( uint8_t value, uint8_t min, uint8_t max ) BANKED
{
    return (value < min) ? min : (value > max) ? max : value;
}

BANKREF(i16Clamp)
int16_t i16Clamp( int16_t value, int16_t min, int16_t max ) BANKED
{
    return (value < min) ? min : (value > max) ? max : value;
}

BANKREF(u16Clamp)
uint16_t u16Clamp( uint16_t value, uint16_t min, uint16_t max ) BANKED
{
    return (value < min) ? min : (value > max) ? max : value;
}

BANKREF(i16abs)
int16_t i16abs(int16_t value) BANKED
{
	if (value >= 0) return value;
	else return - value;
}


BANKREF(playDeathAnimation)
void playDeathAnimation(uint16_t x, uint16_t y) BANKED 
{
	set_sprite_tile(0,  (0));
	set_sprite_tile(1,  2);

	for(uint8_t i = 0; i < 5;i++) {
		move_sprite(0, 200, 200);
		move_sprite(1, 200, 200);
		wait_vbl_done();
		delay(100);
		move_sprite(0, x, y);
		move_sprite(1, x+8, y);
		wait_vbl_done();
		delay(100);
	}
}

//1 if finished, 0 otherwise
BANKREF(checkFinish)
uint8_t checkFinish(uint16_t finishTileIndex, uint16_t mapWidth, uint16_t x, uint16_t y) BANKED
{
	uint16_t ind = mapWidth*((y - 8)>>3) + ((x)>>3);

	if (ind == finishTileIndex) {
		return 1;
	}
	else {
		return 0;
	}
}
