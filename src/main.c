#include <gb/gb.h>  
#include <stdio.h>
#include <gb/cgb.h>

#include "tileset.h"
#include "mapzero.h"

//#include "playertiles.h"
//#include "playerjumptiles.h"
//#include "test.h"

#include "playertiles0.h"
#include "playertiles1.h"
#include "playertiles2.h"
#include "playertiles3.h"
#include "playertiles4.h"
#include "playertiles5.h"
#include "playertiles6.h"
#include "playertiles7.h"

#define MIN(A,B) ((A)<(B)?(A):(B))


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
	PlayerTiles0SGBPal1c0,
	PlayerTiles0CGBPal0c1,
	PlayerTiles0CGBPal0c2,
	PlayerTiles0CGBPal0c3
};

const unsigned char* playerTilesets[] = {
	PlayerTiles0,
	PlayerTiles1,
	PlayerTiles2,
	PlayerTiles3,
	PlayerTiles4,
	PlayerTiles5,
	PlayerTiles6,
	PlayerTiles7

};


const uint8_t CENTERX = 80;
const uint8_t CENTERY = 72;

uint8_t joydata = 0;

uint16_t playerX = 80;
uint16_t playerY = 50;

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

uint8_t previousTile = 0;

uint8_t old_map_pos_x = 255;
uint8_t old_map_pos_y = 255;
uint16_t oldXDiff = 0;
uint16_t oldYDiff = 0;


//returns true if there is no collision in a point
inline uint8_t checkCollision(uint16_t x, uint16_t y) {
	uint16_t ind = MapZeroWidth*((y)>>3) + ((x)>>3);
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
	playerX = playerXScaled >> 3;


	if (ySpeed >= 0) {
		//player bottom
		uint16_t newPlayerY = (playerYScaled + ySpeed) >> 3;

		if (checkCollision(playerX, newPlayerY) && checkCollision(playerX+ sideEdge+1, newPlayerY) && checkCollision(playerX-sideEdge-1, newPlayerY)) {
			ySpeed += 1;
			onGround = 0;
			set_sprite_tile(0, 12);
			set_sprite_tile(1,  14);

		}
		else {
			onGround = 1;
			jumping = 0;
			ySpeed = 0;
		}
	}

	if (ySpeed < 0) {
		//player top
		uint16_t newPlayerY = ((playerYScaled + ySpeed) >> 3) - hatHeight;

		if (!checkCollision(playerX, newPlayerY) || !checkCollision(playerX + sideEdge + 1, newPlayerY) || !checkCollision(playerX - sideEdge - 1, newPlayerY)) {
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
			set_sprite_tile(0, 8);
			set_sprite_tile(1,  10);
		}
		else if (jumping < 12 && !onGround ) {
			jumping += 1;
			ySpeed -= 2;
		}
	} else {
		jumpReleased = 1;
	}




	playerYScaled += ySpeed;
	playerY = playerYScaled >> 3;

	if (hp > 800) {
		hp = 800;
	}


	uint16_t currentTile = (maxHp - hp) / 100;
	if (currentTile != previousTile) {
		set_sprite_data(0, 16, playerTilesets[currentTile]);
		previousTile = currentTile;
	}
	//printf("%d\n", (playerFrame << 1));

	
	if (ySpeed == 0) {
		set_sprite_tile(0,  (playerFrame << 2));
		set_sprite_tile(1,  2+ (playerFrame << 2));
	}
	

}


void updateEntityPositions() {

	int16_t xDiff = playerX - CENTERX;
	int16_t yDiff = playerY - CENTERY - 32;

	if (xDiff < 0) {
		xDiff = 0;
	}
	if (xDiff >= 200) { //map width - 160 = (95)
		xDiff = 200;
	}
	if (yDiff < 0) {
		yDiff = 0;
	}
	if (yDiff >= 95) {
		yDiff = 95;
	}


	move_sprite(0, playerX - xDiff, playerY - yDiff);
	move_sprite(1, playerX - xDiff+8, playerY - yDiff);



    uint8_t map_pos_x = (uint8_t)(xDiff >> 3u);
	uint8_t map_pos_y = (uint8_t)(yDiff >> 3u);
	
	//x direction
	if (map_pos_x != old_map_pos_x) {
		if (xDiff < oldXDiff) {
			VBK_REG = 1;
			set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, MapZeroHeight - map_pos_y), MapZeroPLN1, MapZeroWidth);     
			VBK_REG = 0;
			set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, MapZeroHeight - map_pos_y), MapZeroPLN0, MapZeroWidth);     
		} else {
			if ((MapZeroWidth - 20u) > map_pos_x) {
				VBK_REG = 1;
				set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, MapZeroHeight - map_pos_y), MapZeroPLN1, MapZeroWidth);   
				VBK_REG = 0;
				set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, MapZeroHeight - map_pos_y), MapZeroPLN0, MapZeroWidth);   
			}   
		}
		old_map_pos_x = map_pos_x;
		oldXDiff = xDiff;
    }

	//y direction

	if (map_pos_y != old_map_pos_y) { 
		if (yDiff < oldYDiff) {
			VBK_REG = 1;
			set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, MapZeroWidth-map_pos_x), 1, MapZeroPLN1, MapZeroWidth);
			VBK_REG = 0;
			set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, MapZeroWidth-map_pos_x), 1, MapZeroPLN0, MapZeroWidth);

		} 
		else {
			if ((MapZeroHeight - 18u) > map_pos_y) {
				VBK_REG = 1;
				set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, MapZeroWidth-map_pos_x), 1, MapZeroPLN1, MapZeroWidth); 
				VBK_REG = 0;
				set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, MapZeroWidth-map_pos_x), 1, MapZeroPLN0, MapZeroWidth); 
			}   
		}
		old_map_pos_y = map_pos_y;
		oldYDiff = yDiff;

    }

	

    SCX_REG = xDiff; SCY_REG = yDiff; 


}

void moveToLevelStart() {
	uint16_t x = 0;
	uint16_t y = 0;
	uint8_t shouldBreak = 0;
	for (y = 0; y < MapZeroHeight; y++) {
		if (shouldBreak) {
			break;
		}
		for (x = 0; x < MapZeroWidth; x++) {
			uint16_t ind = MapZeroWidth*y + x;
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
	uint16_t ind = MapZeroWidth*((playerY - 8)>>3) + ((playerX)>>3);

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
	//set_bkg_tiles(0,0, 32, 32, MapZeroPLN1);
	set_bkg_submap(0, 0, 20, 18, MapZeroPLN1, MapZeroWidth);
	VBK_REG = 0;
	//set_bkg_tiles(0, 0, 32, 32, MapZeroPLN0);
	//move_bkg(0,0);


	set_bkg_submap(0, 0, 20, 18, MapZeroPLN0, MapZeroWidth);


	set_sprite_palette(0, 1, &spritePalette[0]); // loading 1 palette of 4 colors

	set_sprite_data(0, 16, playerTilesets[0]);
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