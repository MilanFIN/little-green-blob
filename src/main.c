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
uint8_t playerY = 50;

int16_t playerXScaled = 80<<3;
int16_t playerYScaled = 50<<3;


int16_t movementSpeed = 8;

//positive when going down
int16_t ySpeed = 0;


uint8_t onGround = 0;
uint8_t jumping = 0;
uint8_t jumpReleased = 1;


//returns true if there is no collision in a point
inline uint8_t checkCollision(uint8_t x, uint8_t y) {
	uint16_t ind = 32*((y)>>3) + ((x)>>3);
	return (MapZeroPLN0[ind] != 0x01) ;

}


void movePlayer() {
	if (joydata & J_LEFT) {
		if (checkCollision(playerX - 8, playerY - 8) && checkCollision(playerX - 8, playerY-1) && checkCollision(playerX - 8, playerY - 15)) {
			playerXScaled -= movementSpeed;
		}
	}
	if (joydata & J_RIGHT) {
		if (checkCollision(playerX + 8, playerY - 8) && checkCollision(playerX + 8, playerY-1) && checkCollision(playerX + 8, playerY - 15)) {
			playerXScaled += movementSpeed;
		}
	}
	playerX = (uint8_t) (playerXScaled >> 3);

	if (ySpeed >= 0) {
		
		uint8_t newPlayerY = (playerYScaled + ySpeed) >> 3;
		if (checkCollision(playerX, newPlayerY) && checkCollision(playerX+7, newPlayerY) && checkCollision(playerX-7, newPlayerY)) {
			ySpeed += 1;
		}
		else {
			onGround = 1;
			jumping = 0;
			ySpeed = 0;
		}
	}

	if (ySpeed < 0) {
		ySpeed += 1;
	}

	if (joydata & J_A) {
		if (onGround && !jumping && jumpReleased) {
			ySpeed = -10;
			jumping = 1;
			onGround = 0;
			jumpReleased = 0;
		}
		else if (jumping < 12 && !onGround ) {
			jumping += 1;
			ySpeed -= 2;
		}
	} else {
		jumpReleased = 1;
	}




	playerYScaled += ySpeed;
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