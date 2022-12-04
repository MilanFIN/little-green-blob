#include <gbdk/platform.h>
#include <gb/gb.h>
#include <gbdk/font.h>
#include <text.h>
#include <font.h>

#pragma bank 255


const uint8_t SPLASHDOWNFRAMETIME = 3;
const uint8_t SPLASHFRAMECOUNT = 8;

uint8_t splashFrame = 0;
uint8_t splashFrameCounter = 0;
uint8_t splashActive = 0;

//font_t min_font;

/*

BANKREF(SWITCHBLOCKS)
const unsigned char SWITCHBLOCKS[2] = {0x08, 0x09}; 
BANKREF(SWITCHTILE)
const uint8_t SWITCHTILE = 0x0a;
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


BANKREF(initSplashDownSprite)
void initSplashDownSprite() BANKED
{

	const UWORD playerPalette[] = {
		32767,
		992,
		744,
		5344
	};

	const UWORD splashPalette[] = {
		32767,
		21140,
		10570,
		5285
	};
	set_sprite_palette(5,1, &playerPalette[0]);

	const unsigned char Splashdown[] =
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x08,0x08,0x7C,0x7C,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x04,0x04,0x0E,0x0E,0x3E,0x3E,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x02,0x02,0x07,0x07,0x0F,0x0F,0x1E,0x1E,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,
		0x07,0x07,0x07,0x07,0x0F,0x0F,0x0F,0x0F,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x02,0x02,0x07,0x07,
		0x07,0x07,0x07,0x07,0x06,0x06,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x02,0x02,0x07,0x07,
		0x07,0x07,0x02,0x02,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x02,0x02,0x07,0x07,
		0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x06,0x06,0x04,0x04,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
	set_sprite_data(0x20, 16, Splashdown);

	//sprite 20 & 21 for splash damage
	set_sprite_tile(20, 0x20);
	set_sprite_tile(21, 0x20);

	set_sprite_prop(20,5);  //(5th) palette
	set_sprite_prop(21,5 | S_FLIPX);  //(5th) palette & flipped
	move_sprite(20, 200, 200);
	move_sprite(21, 200, 200);


}

void spawnSplash(uint16_t x, uint16_t y) BANKED
{

	splashFrame = 0;
	splashFrameCounter = 0;
	splashActive = 1;
	set_sprite_tile(20, 0x20 + (splashFrame<<1));
	set_sprite_tile(21, 0x20 + (splashFrame<<1));


	//20: +8 for zero position going to right
	//21 0 for zero position going to left
	move_sprite(20, x+12, y);
	move_sprite(21, x-4, y);

}

BANKREF(scrollSplash)
void scrollSplash(int16_t xchange, int16_t ychange) BANKED
{
	scroll_sprite(20, (uint8_t)xchange, (uint8_t)ychange);
	scroll_sprite(21, (uint8_t)xchange, (uint8_t)ychange);

}

BANKREF(loopSplash)
void loopSplash() BANKED
{
	if (!splashActive) {
		return;
	}



	set_sprite_tile(20, 0x20 + (splashFrame<<1));
	set_sprite_tile(21, 0x20 + (splashFrame<<1));

	splashFrameCounter++;
	if (splashFrameCounter > SPLASHDOWNFRAMETIME) {
		splashFrameCounter = 0;
		splashFrame++;
		if (splashFrame >= SPLASHFRAMECOUNT) {
			splashFrame = 0;
			move_sprite(20, 200,200);
			move_sprite(21, 200,200);
			splashActive = 0;
		}
	}
}


BANKREF(initFont)
void initFont() BANKED
{
	set_bkg_palette(0, 1, &fontPalette[0]);

	
	set_bkg_data(0x30, 37, FontTiles);
	

	wait_vbl_done();

}




BANKREF(showStartMenu)
void showStartMenu() BANKED
{
	initFont();
	
	move_bkg(4,0);
	VBK_REG=1;
	set_bkg_tiles(5,8, 11, 1, PressStartTextPLN1);
	VBK_REG=0;
	set_bkg_tiles(5,8, 11, 1, PressStartTextPLN0);

	waitpad(J_START | J_A);
	waitpadup();
	wait_vbl_done();

}

void clearScreen() 
{
	for (uint8_t i=0; i < 32; ++i) {
		VBK_REG=1;
		set_bkg_tiles(0,i,20,1,emptyRow);
		VBK_REG=0;
		set_bkg_tiles(0,i,20,1,emptyRow);

	}
	for (uint8_t j=0; j<30; ++j) {
		move_sprite(j, 200, 200);
	}

}

BANKREF(levelSelectionMenu)
uint8_t levelSelectionMenu(uint8_t mapcount) BANKED
{
	initFont();
	clearScreen();

	move_bkg(0,0);
	uint8_t joydata = 0;


	uint8_t map = 0;

	while (1) {
		for (int8_t i = -2; i < 3; i++) {
			int8_t optionIndex = i+map+1;
			int8_t position = i+2;
			if (optionIndex >= 1 && optionIndex <= mapcount) {
				uint8_t firstTile = optionIndex / 10;
				VBK_REG=1;
				set_bkg_tiles(3+3*position, 8, 1, 1, levelsPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position, 8, 1, 1, levelsPLN0+firstTile);
				uint8_t secondTile = optionIndex % 10;
				VBK_REG=1;
				set_bkg_tiles(3+3*position+1, 8, 1, 1, levelsPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position+1, 8, 1, 1, levelsPLN0+secondTile);
			}
			else {
				//clear this, as it should have no value
				VBK_REG=1;
				set_bkg_tiles(3+3*position, 8, 1, 1, levelsPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position, 8, 1, 1, levelsPLN0+10);
				VBK_REG=1;
				set_bkg_tiles(3+3*position+1, 8, 1, 1, levelsPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position+1, 8, 1, 1, levelsPLN0+10);

			}
		}
		waitpad(J_RIGHT | J_LEFT | J_A | J_START);
		joydata = joypad();

		waitpadup();
		if (joydata & J_RIGHT) {
			if (map < mapcount - 1) {
				map++;
			}
		}
		if (joydata & J_LEFT) {
			if (map >  1) {
				map--;
			}
		}
		if (joydata & J_A || joydata & J_START) {
			break;
		}
	}

	return map;
} 