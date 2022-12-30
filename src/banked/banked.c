#include <gbdk/platform.h>
#include <gb/gb.h>
#include <gbdk/font.h>
#include <text.h>
#include <font.h>
#include <splashlogo.h>

#pragma bank 255


const uint8_t SPLASHDOWNFRAMETIME = 1;
const uint8_t SPLASHFRAMECOUNT = 8;

uint8_t splashFrame = 0;
uint8_t splashFrameCounter = 0;
uint8_t splashActive = 0;

const uint8_t BLEEDFRAMETIME = 3;
const uint8_t BLEEDFRAMECOUNT = 8;

uint8_t bleedFrame = 0;
uint8_t bleedFrameCounter = 0;
uint8_t bleedActive = 0;

int8_t bleedDir = 0;


char *saveInitialized = (char *)0xa000; //1 byte * 2
uint8_t *saveCompletePointer = (int *)0xa002; //1 bytes * 20
int16_t *saveScoresPointer = (int *)0xa016; //4 bytes * 20

uint8_t mapCompletedSaveValues[20];
int16_t mapScoreSaveValues[20];




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


BANKREF(playSound)
void playSound(uint8_t id) BANKED
{
	if (id == 0) { //splash confirm
		NR10_REG = 0x74;
		NR11_REG = 0x82;
		NR12_REG = 0x83;
		NR13_REG = 0xc1;
		NR14_REG = 0x86;
	}
	else if (id == 1)  {//menu left/right
		NR10_REG = 0x00;
		NR11_REG = 0x80;
		NR12_REG = 0x42;
		NR13_REG = 0xa3;//0x11;
		NR14_REG = 0x85;
	}
	else if (id == 2) {//menu confirm
		NR41_REG = 0x3f;
		NR42_REG = 0xf1;
		NR43_REG = 0x80;  
		NR44_REG = 0xa0;  
	}
	else if (id == 3) {//jump
		NR10_REG = 0x57;
		NR11_REG = 0xbf;
		NR12_REG = 0x45;//57
		NR13_REG = 0x40;
		NR14_REG = 0x86;
	}
	else if (id == 4) {//fire
		NR21_REG = 0x81;
		NR22_REG = 0x42;
		NR23_REG = 0x9f;
		NR24_REG = 0x86;
	}
	else if (id == 5) { //landing
		NR10_REG = 0x1b;
		NR11_REG = 0xbf;
		NR12_REG = 0x43;//57
		NR13_REG = 0xe0;//73
		NR14_REG = 0x86;
	}
	else if (id == 6) { //switch
		NR41_REG = 0x0a;
		NR42_REG = 0x71;
		NR43_REG = 0x36;  
		NR44_REG = 0xc0;  
	}
	else if (id == 7) { //timed blocks
		NR41_REG = 0x0a;
		NR42_REG = 0x51;
		NR43_REG = 0x37;  
		NR44_REG = 0xc0;  
	}
	else if (id == 8) {//finish
		NR10_REG = 0x46;
		NR11_REG = 0x83;
		NR12_REG = 0x60;
		NR13_REG = 0xd6;
		NR14_REG = 0x86;
	}
	else if (id == 9) {//restart/dead
		NR10_REG = 0x7d;
		NR11_REG = 0x80;
		NR12_REG = 0x77;
		NR13_REG = 0x0e;
		NR14_REG = 0x86;
	}

}


BANKREF(playDeathAnimation)
void playDeathAnimation(uint16_t x, uint16_t y, int8_t xDir) BANKED 
{
	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);

	if (xDir >= 0) {
		set_sprite_prop(0,0); 
		set_sprite_prop(1,0); 
	}
	else {
		set_sprite_prop(0,S_FLIPX); 
		set_sprite_prop(1,S_FLIPX); 
	}

	for(uint8_t i = 0; i < 5;i++) {
		move_sprite(0, 200, 200);
		move_sprite(1, 200, 200);
		wait_vbl_done();
		delay(100);

		if (xDir >= 0) {
			move_sprite(0, x, y);
			move_sprite(1, x + 8, y);
		}
		else {
			move_sprite(0, x+8, y);
			move_sprite(1, x, y);
		}

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


BANKREF(clearSave)
void clearSave() BANKED
{
	ENABLE_RAM_MBC1; // Enable RAM

	saveInitialized[0] = 'f';
	saveInitialized[1] = 'f';

	DISABLE_RAM_MBC1; // Disable RAM


}


BANKREF(loadSave)
void loadSave() BANKED
{
	ENABLE_RAM_MBC1; // Enable RAM

	if (saveInitialized[0] != 'o' || saveInitialized[1] != 'k') {

		for (uint8_t i = 0; i < 20; i++) {
			saveCompletePointer[i] = 0;
		}
		for (uint8_t i = 0; i < 20; i++) {
			saveScoresPointer[i] = 0;
		}
		
		saveInitialized[0] = 'o';
		saveInitialized[1] = 'k';

	} 

	for (uint8_t i = 0; i < 20; i++) {
		if (saveCompletePointer[i] == 1) {
			mapCompletedSaveValues[i] = 1;
		}
		else {
			mapCompletedSaveValues[i] = 0;
		}
		mapScoreSaveValues[i] = saveScoresPointer[i];
		
	}

	DISABLE_RAM_MBC1; // Disable RAM

}


BANKREF(initBleedSprite)
void initBleedSprite() BANKED
{
	const unsigned char BleedTiles[] =
	{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x02,0x02,0x01,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,
	0x0E,0x0E,0x04,0x04,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0x08,0x18,0x18,0x18,0x18,0x08,0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x38,0x38,0x18,0x18,
	0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,
	0x28,0x28,0x38,0x38,0x10,0x10,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x20,0x20,0x30,0x30,0x10,0x10,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};

	const UWORD playerPalette[] = {
		32767,
		992,
		744,
		5344
	};

	set_sprite_data(0x30, 10, BleedTiles);
	set_sprite_palette(6,1, &playerPalette[0]);

	set_sprite_tile(23, 0x30);

	set_sprite_prop(23,5);  //(5th) palette //| S_FLIPX
	move_sprite(23, 200, 200);


}

BANKREF_EXTERN(spawnBleed)
void spawnBleed(uint16_t x, uint16_t y, int8_t direction) BANKED
{
	return;
	//TODO:
	if (direction < 0) {
		move_sprite(23, x, y);
		set_sprite_prop(23,5);
	}
	if (direction > 0) {
		set_sprite_prop(23,5 | S_FLIPX);
		move_sprite(23, x+8, y);
	}
	bleedDir = direction;
	bleedActive = 1;
}

BANKREF_EXTERN(scrollBleed)
void scrollBleed(int16_t x, int16_t y) BANKED
{
	if (bleedActive) {
		//scroll_sprite(23, (uint8_t)xchange, (uint8_t)ychange);
		if (bleedDir < 0) {
			move_sprite(23, x, y);
		}
		if (bleedDir > 0) {
			move_sprite(23, x+8, y);
		}
	}
}

BANKREF(loopBleed)
void loopBleed() BANKED
{
	if (!bleedActive) {
		return;
	}

	set_sprite_tile(23, 0x30 + (bleedFrame<<1));

	bleedFrameCounter++;
	if (bleedFrameCounter > BLEEDFRAMETIME) {
		bleedFrameCounter = 0;
		bleedFrame++;
		if (bleedFrame >= BLEEDFRAMECOUNT) {
			bleedFrame = 0;
			move_sprite(23, 200,200);
			bleedActive = 0;
		}
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
  0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xE0,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x1C,0x1C,0x78,0x78,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x0E,0x0E,0x1C,0x1C,0x38,0x38,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x07,0x07,0x0F,0x0F,0x0E,0x0E,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x07,0x07,0x0F,0x0F,0x06,0x06,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x07,0x07,0x07,0x07,0x02,0x02,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x02,0x02,0x07,0x07,0x02,0x02,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x03,0x03,0x02,0x02,0x00,0x00
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

void spawnSplash(uint16_t x, uint16_t y, uint16_t sideEdge) BANKED
{

	sideEdge -= 2;
	splashFrame = 0;
	splashFrameCounter = 0;
	splashActive = 1;
	set_sprite_tile(20, 0x20 + (splashFrame<<1));
	set_sprite_tile(21, 0x20 + (splashFrame<<1));


	//20: +8 for zero position going to right
	//21 0 for zero position going to left
	move_sprite(20, x+8+sideEdge, y+2);
	move_sprite(21, x-sideEdge, y+2);

}

BANKREF(scrollSplash)
void scrollSplash(int16_t xchange, int16_t ychange) BANKED
{
	if (splashActive) {
		scroll_sprite(20, (uint8_t)xchange, (uint8_t)ychange);
		scroll_sprite(21, (uint8_t)xchange, (uint8_t)ychange);
	}
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
	set_bkg_palette(0, 6, &fontPalette[0]);

	
	set_bkg_data(0x30, 39, FontTiles);
	

	wait_vbl_done();

}




BANKREF(showStartMenu)
void showStartMenu() BANKED
{
	initFont();
	clearScreen();

	move_win(4, 104);
	VBK_REG=1;
	set_win_tiles(5, 0, 11, 1, PressStartTextPLN1);
	VBK_REG=0;
	set_win_tiles(5, 0, 11, 1, PressStartTextPLN0);


	const UWORD splashBkgPalette[] = {
		32767, 6117, 744, 0//5344
	};

	set_bkg_palette(7, 1, &splashBkgPalette[0]);

	uint8_t splashPLN1[128];
	for (uint8_t j = 0; j < 128; ++j) {
		splashPLN1[j] = 0x07;
	}
	VBK_REG=1;
	set_bkg_tiles(3,2, 16, 8, splashPLN1);


	VBK_REG=0;
	move_bkg(8,0);
	unsigned char offset_splash_map[128];
	for (uint8_t i = 0; i < 128; ++i) {
		offset_splash_map[i] = splash_map[i] + 0x60;
	}
	set_bkg_data(0x60, 64, splash_data); //0x25
	set_bkg_tiles(3,2, 16, 8, offset_splash_map);


	SHOW_WIN;

	waitpad(J_START | J_A);
	waitpadup();
	playSound(0);
	wait_vbl_done();
	HIDE_WIN;

}

void clearScreen() 
{
	for (uint8_t i=0; i < 32; ++i) {
		VBK_REG=1;
		set_bkg_tiles(0,i,32,1,emptyRowPLN1);
		set_win_tiles(0,i,32,1,emptyRowPLN1);
		VBK_REG=0;
		set_bkg_tiles(0,i,32,1,emptyRowPLN0);
		set_win_tiles(0,i,32,1,emptyRowPLN0);

	}
	for (uint8_t j=0; j<30; ++j) {
		move_sprite(j, 200, 200);
	}

}

BANKREF(initLevelMenu)
void initLevelMenu() BANKED
{
	
	initFont();
	clearScreen();

	move_bkg(0,4);

	loadSave();

	VBK_REG=1;
	set_bkg_tiles(4, 4, 12, 1, SelectLevelTextPLN1);
	VBK_REG=0;
	set_bkg_tiles(4, 4, 12, 1, SelectLevelTextPLN0);

	VBK_REG=1;
	set_bkg_tiles(7, 11, 6, 1, PlayATextPLN1);
	VBK_REG=0;
	set_bkg_tiles(7, 11, 6, 1, PlayATextPLN0);

	VBK_REG=1;
	set_bkg_tiles(9, 9, 2, 1, SelectionLinePLN1);
	VBK_REG=0;
	set_bkg_tiles(9, 9, 2, 1, SelectionLinePLN0);

	VBK_REG=1;
	set_bkg_tiles(3, 17, 17, 1, SelectEraseSavePLN1);
	VBK_REG=0;
	set_bkg_tiles(3, 17, 17, 1, SelectEraseSavePLN0);


}

BANKREF(eraseSaveMenu)
uint8_t eraseSaveMenu() BANKED
{
	uint8_t joydata = 0;
	initFont();
	clearScreen();

	move_bkg(0,4);

	VBK_REG=1;
	set_bkg_tiles(5, 7, 10, 1, EraseSavePLN1);
	VBK_REG=0;
	set_bkg_tiles(5, 7, 10, 1, EraseSavePLN0);

	VBK_REG=1;
	set_bkg_tiles(7, 10, 5, 1, AYesPLN1);
	VBK_REG=0;
	set_bkg_tiles(7, 10, 5, 1, AYesPLN0);

	VBK_REG=1;
	set_bkg_tiles(7, 11, 4, 1, AYesPLN1);
	VBK_REG=0;
	set_bkg_tiles(7, 11, 4, 1, BNoPLN0);

	uint8_t reset = 0;
	while (1) {
		waitpad(J_A | J_B);
		uint8_t joydata = joypad();
		waitpadup();

		if (joydata & J_A) {
			//clear memory
			clearSave();
			reset = 1;
			playSound(0);

			break;
		}
		if (joydata & J_B) {
			playSound(9);

			break;
		}
	}
	return reset;
	
}

BANKREF(levelSelectionMenu)
uint8_t levelSelectionMenu(uint8_t mapcount, uint8_t map) BANKED
{

	uint8_t joydata = 0;
	
	initLevelMenu();


	while (1) {
		for (int8_t i = -2; i < 3; i++) {
			int8_t optionIndex = i+map+1;
			int8_t position = i+2;

			if (i == 0) { //middle item, the one to be chosen
				if (mapCompletedSaveValues[optionIndex-1] == 1) {
					uint8_t scoreFirstDigit = (mapScoreSaveValues[optionIndex-1] /100)% 10;
					uint8_t scoreSecondDigit = (mapScoreSaveValues[optionIndex-1] / 10) % 10;
					uint8_t scoreThirdDigit = mapScoreSaveValues[optionIndex-1] % 10;

					//set points
					VBK_REG=1;
					set_bkg_tiles(7, 7, 1, 1, levelsPLN1+2);
					set_bkg_tiles(8, 7, 1, 1, levelsPLN1+2);
					set_bkg_tiles(9, 7, 1, 1, levelsPLN1+2);
					VBK_REG=0;
					set_bkg_tiles(7, 7, 1, 1, levelsPLN0+scoreFirstDigit);
					set_bkg_tiles(8, 7, 1, 1, levelsPLN0+scoreSecondDigit);
					set_bkg_tiles(9, 7, 1, 1, levelsPLN0+scoreThirdDigit);

					VBK_REG=1;
					set_bkg_tiles(10, 7, 3, 1, ptsPLN1);
					VBK_REG=0;
					set_bkg_tiles(10, 7, 3, 1, ptsPLN0);
				}
				else {
					VBK_REG=1;
					set_bkg_tiles(0, 7, 32, 1, emptyRowPLN1);
					VBK_REG=0;
					set_bkg_tiles(0, 7, 32, 1, emptyRowPLN0);
				}
			}


			//set options & completed mark if necessary
			if (optionIndex >= 1 && optionIndex <= mapcount) {

				int8_t numberPaletteOffset = (i >= 0 ? i : -i);

				uint8_t firstTile = optionIndex / 10;
				VBK_REG=1;
				set_bkg_tiles(3+3*position, 9, 1, 1, levelsPLN1+numberPaletteOffset);
				VBK_REG=0;
				set_bkg_tiles(3+3*position, 9, 1, 1, levelsPLN0+firstTile);
				uint8_t secondTile = optionIndex % 10;
				VBK_REG=1;
				set_bkg_tiles(3+3*position+1, 9, 1, 1, levelsPLN1+numberPaletteOffset);
				VBK_REG=0;
				set_bkg_tiles(3+3*position+1, 9, 1, 1, levelsPLN0+secondTile);

				VBK_REG=1;
				set_bkg_tiles(3+3*position+2, 9, 1, 1, LevelCompletedCheckMarkPLN1+numberPaletteOffset);
				VBK_REG=0;
				set_bkg_tiles(3+3*position+2, 9, 1, 1, LevelCompletedCheckMarkPLN0 + mapCompletedSaveValues[optionIndex-1]);


			}
			else {
				//clear this, as it should have no value
				VBK_REG=1;
				set_bkg_tiles(3+3*position, 9, 1, 1, emptyRowPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position, 9, 1, 1, emptyRowPLN0);
				VBK_REG=1;
				set_bkg_tiles(3+3*position+1, 9, 1, 1, emptyRowPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position+1, 9, 1, 1, emptyRowPLN0);
				VBK_REG=1;
				set_bkg_tiles(3+3*position+2, 9, 1, 1, emptyRowPLN1);
				VBK_REG=0;
				set_bkg_tiles(3+3*position+2, 9, 1, 1, emptyRowPLN0);

			}
		}
		waitpad(J_RIGHT | J_LEFT | J_A | J_START | J_SELECT);
		joydata = joypad();

		waitpadup();
		if (joydata & J_RIGHT) {
			playSound(1);
			if (map < mapcount - 1) {
				map++;
			}
		}
		if (joydata & J_LEFT) {
			playSound(1);
			if (map >  0) {
				map--;
			}
		}
		if (joydata & J_A || joydata & J_START) {
			playSound(0);
			break;
		}
		if (joydata & J_SELECT) {
			playSound(0);
			uint8_t reset = eraseSaveMenu();
			if (reset) {
				map = 0;
			}
			initLevelMenu();
		}
		wait_vbl_done();
	}

	clearScreen();
	wait_vbl_done();

	return map;
} 


BANKREF(mapCompleted)
void mapCompleted(uint8_t map, int16_t score) BANKED
{
	//TODO: SHOW SAVING SCREEN HERE
	ENABLE_RAM_MBC1; 

	if (mapCompletedSaveValues[map] != 1) {
		saveCompletePointer[map] = 1;			
	}
	if (saveScoresPointer[map] < score) {
		saveScoresPointer[map] = score;

	}

	DISABLE_RAM_MBC1;
	mapCompletedSaveValues[map] = 1;
	mapScoreSaveValues[map] = score;


}

