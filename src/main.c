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

	TilesetCGBPal4c0,
	TilesetCGBPal4c1,
	TilesetCGBPal4c2,
	TilesetCGBPal4c3
};

const UWORD spritePalette[] = {
	PlayerTilesSGBPal1c0,
	PlayerTilesSGBPal1c1,
	PlayerTilesSGBPal1c2,
	PlayerTilesSGBPal1c3
};


const uint8_t CENTERX = 80;
const uint8_t CENTERY = 72;

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

//will be set to the tile index that has the finish
// initialized during level load
uint16_t finishTileIndex = 0;

uint16_t hp = 800;
uint16_t maxHp = 800;

uint8_t playerFrame = 0;
uint8_t frameCounter = 20;
const uint8_t ANIMFRAMETIME = 20;


//returns true if there is no collision in a point
inline uint8_t checkCollision(uint8_t x, uint8_t y) {
	uint16_t ind = 32*((y)>>3) + ((x)>>3);
	return (MapZeroPLN0[ind] != 0x01) ;

}


void movePlayer() {
	uint16_t hatHeight = (hp >> 6) + 3;
	uint16_t sideEdge = (hp >> 7) + 2;

	if (joydata & J_LEFT) {

		if (checkCollision(playerX - sideEdge, playerY - (hatHeight >> 1)) && checkCollision(playerX - sideEdge, playerY-1) && checkCollision(playerX - sideEdge, playerY - hatHeight)) {
			playerXScaled -= movementSpeed;
			hp -= 2;
		}
	}
	if (joydata & J_RIGHT) {
		if (checkCollision(playerX + sideEdge, playerY - (hatHeight >> 1)) && checkCollision(playerX + sideEdge, playerY-1) && checkCollision(playerX + sideEdge, playerY - hatHeight)) {
			playerXScaled += movementSpeed;
			hp -= 2;
		}
	}
	playerX = (uint8_t) (playerXScaled >> 3);


	if (ySpeed >= 0) {
		//player bottom
		uint8_t newPlayerY = (playerYScaled + ySpeed) >> 3;

		if (checkCollision(playerX, newPlayerY) && checkCollision(playerX+ sideEdge+1, newPlayerY) && checkCollision(playerX-sideEdge+1, newPlayerY)) {
			ySpeed += 1;
			onGround = 0;
		}
		else {
			onGround = 1;
			jumping = 0;
			ySpeed = 0;
		}
	}

	if (ySpeed < 0) {
		//player top
		uint8_t newPlayerY = ((playerYScaled + ySpeed) >> 3) - hatHeight;

		if (!checkCollision(playerX, newPlayerY) || !checkCollision(playerX + sideEdge + 1, newPlayerY) || !checkCollision(playerX - sideEdge + 1, newPlayerY)) {
			ySpeed = 0;
		}
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

	if (hp > 800) {
		hp = 800;
	}


	uint16_t currentTile = (maxHp - hp) / 100;


	set_sprite_tile(0, currentTile*4 + (playerFrame << 5));
	set_sprite_tile(1, currentTile*4+2 + (playerFrame << 5));




}


void updateEntityPositions() {

	int16_t xDiff = playerX - CENTERX;
	int16_t yDiff = playerY - CENTERY - 32;

	if (xDiff < 0) {
		xDiff = 0;
	}
	if (xDiff >= 95) { //map width - 160
		xDiff = 95;
	}
	if (yDiff < 0) {
		yDiff = 0;
	}
	if (yDiff >= 95) {
		yDiff = 95;
	}


	move_sprite(0, playerX - xDiff, playerY - yDiff);
	move_sprite(1, playerX - xDiff+8, playerY - yDiff);

	move_bkg(xDiff, yDiff);

}

void moveToLevelStart() {
	uint16_t x = 0;
	uint16_t y = 0;
	uint8_t shouldBreak = 0;
	for (y = 0; y < 32; y++) {
		if (shouldBreak) {
			break;
		}
		for (x = 0; x < 32; x++) {
			uint16_t ind = 32*y + x;
			if (MapZeroPLN0[ind] == 0x04) {
				shouldBreak = 1;
				break;
			}
		}
	}
	x = x*8 + 4;
	y *= 8;
	playerXScaled = x << 3;
	playerYScaled = y << 3;
	playerX = x;
	playerY = y;

	shouldBreak = 0;
	for (uint16_t j = 0; j < 32; j++) {
		if (shouldBreak) {
			break;
		}
		for (uint16_t i = 0; i < 32; i++) {
			uint16_t ind = 32*j + i;
			if (MapZeroPLN0[ind] == 0x05) {
				finishTileIndex = ind;
				shouldBreak = 1;
				break;
			}
		}
	}

}

void animatePlayer() {

	frameCounter -= 1;
	if (frameCounter == 0) {
		playerFrame += 1;
		if (playerFrame == 2) {
			playerFrame = 0;
		}
		frameCounter = ANIMFRAMETIME;
	}
}

//1 if finished, 0 otherwise
uint8_t checkFinish() {
	uint16_t ind = 32*((playerY - 8)>>3) + ((playerX)>>3);

	if (ind == finishTileIndex) {
		return 1;
	}
	else {
		return 0;
	}
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


	set_bkg_palette(0, 5, &backgroundPalette[0]);

	set_bkg_data(0, 6, Tileset); 

	VBK_REG = 1;
	set_bkg_tiles(0,0, 32, 32, MapZeroPLN1);
	VBK_REG = 0;

	set_bkg_tiles(0, 0, 32, 32, MapZeroPLN0);
	move_bkg(0,0);

	set_sprite_palette(0,1,&spritePalette[0]); // loading 1 palette of 4 colors

	set_sprite_data(0, 64, PlayerTiles);
	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);

	set_sprite_prop(0,0); //loading 0th palette
	set_sprite_prop(1,0); //also 0th

	while(1) {

		moveToLevelStart();

		while(1) {

			joydata = joypad();

			movePlayer();

			animatePlayer();

			updateEntityPositions();

			if (checkFinish()) {
				break;
			}

			wait_vbl_done();

		}

	}
	
}