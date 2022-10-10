#include <gb/gb.h>  
#include <stdio.h>
#include <gb/cgb.h>

#include "tileset.h"
#include "mapzero.h"

//#include "graphics/playertiles.c"
#include "playertiles.h"

const UWORD backgroundPalette[] = {

	TilesetCGBPal0c0,
	TilesetCGBPal0c1,
	TilesetCGBPal0c2,
	TilesetCGBPal0c3,

	TilesetCGBPal1c0,
	TilesetCGBPal1c1,
	TilesetCGBPal1c2,
	TilesetCGBPal1c3,

	TilesetCGBPal2c0,
	TilesetCGBPal2c1,
	TilesetCGBPal2c2,
	TilesetCGBPal2c3,

	TilesetCGBPal3c0,
	TilesetCGBPal3c1,
	TilesetCGBPal3c2,
	TilesetCGBPal3c3,
};

const UWORD spritePalette[] = {
	PlayerTilesSGBPal1c0,
	PlayerTilesSGBPal1c1,
	PlayerTilesSGBPal1c2,
	PlayerTilesSGBPal1c3
};


uint8_t joydata = 0;

uint8_t playerX = 80;
uint8_t playerY = 80;

int16_t playerXScaled = 80<<3;
int16_t playerYScaled = 80<<3;

int16_t speed = 8;


void movePlayer() {
	if (joydata & J_LEFT) {
		playerXScaled -= speed;
	}
	if (joydata & J_RIGHT) {
		playerXScaled += speed;
	}
	playerX = (uint8_t) (playerXScaled >> 3);
	playerY = (uint8_t) (playerYScaled >> 3);

	move_sprite(0, playerX,playerY);
	move_sprite(1, playerX+8,playerY);

}


void main(){



	//disable_interrupts();

	DISPLAY_ON;
	SPRITES_8x16;


	NR52_REG = 0x80; // sound on 
    NR50_REG = 0x77; // volume
    NR51_REG = 0xFF; // all channels


	//SHOW_WIN;
	SHOW_SPRITES;
	SHOW_BKG;


	set_bkg_palette(0, 4, &backgroundPalette[0]);

	set_bkg_data(0, 4, Tileset); 

	VBK_REG = 1;
	set_bkg_tiles(0,0, 32, 32, MapZeroPLN1);
	VBK_REG = 0;

	set_bkg_tiles(0, 0, 32, 32, MapZeroPLN0);
	move_bkg(0,0);

	set_sprite_palette(0,1,&spritePalette[0]); // loading 1 palette of 4 colors

	set_sprite_data(0, 4, PlayerTiles); //single tile
	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);

	set_sprite_prop(0,0); //loading 0th palette
	set_sprite_prop(1,0); //also 0th


	while(1) {

		joydata = joypad();

		movePlayer();

		wait_vbl_done();

	}
}