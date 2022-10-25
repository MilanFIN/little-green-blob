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
#include "projectile.h"

#include "projectileholder.h"

#define MIN(A,B) ((A)<(B)?(A):(B))


//TODO: korjaa kartan init siirto kun pääsee maaliin
//logo kikkailua tms

//nopeuden skaalaus kusee 


const unsigned char FLOORTILES[2] = {0x01, 0x06}; 
const uint8_t FLOORTILECOUNT = 2;
const unsigned char ROOFTILES[1] = {0x01}; 
const uint8_t ROOFTILECOUNT = 1;
const unsigned char OBSTACLETILES[1] = {0x07};
const uint8_t OBSTACLECOUNT = 1;



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

const UWORD projectilePalette[] = {
	ProjectileCGBPal0c0,
	ProjectileCGBPal0c1,
	ProjectileCGBPal0c2,
	ProjectileCGBPal0c3
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
uint8_t previousJoydata = 0;


uint16_t playerX = 80;
uint16_t playerY = 50;

int16_t playerXScaled = 80<<3;
int16_t playerYScaled = 50<<3;

//where player is facing toward
int8_t xDir = 1;


const int16_t groundAcceleration = 16;
const int16_t airAcceleration = 4;
//scaled by factor of 2^3 = 8
int16_t xSpeed = 0;
const int16_t maxXSpeed = 64;

//positive when going down
// scaled by factor of 2^3 = 8
int16_t ySpeed = 0;
const int16_t AIRGRAVITY = 8;
const int16_t WATERFALLSPEEDLIMIT = 32;
//initial jump power
const int16_t JUMPPOWER = 80;
//how much holding A will affect jump height
int16_t JUMPCARRY = 16;



uint8_t onGround = 0;
uint8_t jumping = 0;
uint8_t jumpReleased = 1;

//will be set to the tile index that has the finish
// initialized during level load
uint16_t finishTileIndex = 0;

int16_t hp = 800;
int16_t maxHp = 800;

uint8_t playerFrame = 0;
uint8_t frameCounter = 20;
const uint8_t ANIMFRAMETIME = 20;

uint8_t previousTile = 0;

uint8_t old_map_pos_x = 255;
uint8_t old_map_pos_y = 255;
uint16_t oldXDiff = 0;
uint16_t oldYDiff = 0;
uint16_t onScreenX = 0;
uint16_t onScreenY = 0;

struct ProjectileObj projectile = {
	0,0,0,0,0, 2
};


inline uint8_t uClamp( uint8_t value, uint8_t min, uint8_t max )
{
    return (value < min) ? min : (value > max) ? max : value;
}

inline int16_t i16Clamp( int16_t value, int16_t min, int16_t max )
{
    return (value < min) ? min : (value > max) ? max : value;
}

inline uint16_t u16Clamp( uint16_t value, uint16_t min, uint16_t max )
{
    return (value < min) ? min : (value > max) ? max : value;
}

inline int16_t i16abs(int16_t value) {
	if (value >= 0) return value;
	else return - value;
}


//returns true if there is no collision in a point
inline uint8_t checkFloorCollision(uint16_t x, uint16_t y) {
	uint16_t ind = MapZeroWidth*((y)>>3) + ((x)>>3);
	uint8_t floor = 0;
	for (uint8_t i=0; i < FLOORTILECOUNT; i++) {
		if (MapZeroPLN0[ind] == FLOORTILES[i]) {
			floor = 1;
		} 
	}
	return !floor ;
}
inline uint8_t checkRoofCollision(uint16_t x, uint16_t y) {
	uint16_t ind = MapZeroWidth*((y)>>3) + ((x)>>3);
	uint8_t roof = 0;
	for (uint8_t i=0; i < ROOFTILECOUNT; i++) {
		if (MapZeroPLN0[ind] == ROOFTILES[i]) {
			roof = 1;
		} 
	}
	return !roof ;
}
//returns 1, if player should stay alive
inline uint8_t checkObstacleCollision(uint16_t x, uint16_t y) {
	uint16_t ind = MapZeroWidth*((y)>>3) + ((x)>>3);
	uint8_t dead = 0;
	for (uint8_t i=0; i < OBSTACLECOUNT; i++) {
		if (MapZeroPLN0[ind] == OBSTACLETILES[i]) {
			dead = 1;
		} 
	}
	return !dead ;
}

inline void addGravity() {
	uint16_t ind = MapZeroWidth*((playerY - 8)>>3) + ((playerX)>>3);
	ySpeed += AIRGRAVITY;

	
	if (MapZeroPLN0[ind] == 0x02 || MapZeroPLN0[ind] == 0x03 ) {
		//ySpeed = i16Clamp(ySpeed, -WATERFALLSPEEDLIMIT, WATERFALLSPEEDLIMIT);
		if (ySpeed > WATERFALLSPEEDLIMIT) {
			ySpeed = WATERFALLSPEEDLIMIT;
		}
	}
}



void movePlayer() {
	uint16_t hatHeight = (hp >> 6) + 3;
	uint16_t sideEdge = (hp >> 7) + 2;

	int16_t acceleration = (onGround) ? groundAcceleration : airAcceleration;

	if (joydata & J_LEFT) {
			xDir = -1;
			xSpeed -= acceleration;
			hp -= 2;
	}
	else if (joydata & J_RIGHT) {
			xDir = 1;
			xSpeed += acceleration;
			hp -= 2;
	}
	else {
		if (xSpeed > 0) {
			if (xSpeed < acceleration) {
				xSpeed = 0;
			}
			else {
				xSpeed -= acceleration;
			}
		}
		if (xSpeed < 0) {
			if (xSpeed > acceleration) {
				xSpeed = 0;
			}
			else {
				xSpeed += acceleration;
			}
		}
	}
	xSpeed = i16Clamp(xSpeed, -maxXSpeed, maxXSpeed);


	if (xSpeed < 0) {
		if (checkRoofCollision(playerX - sideEdge, playerY - (hatHeight >> 1)) && checkRoofCollision(playerX - sideEdge, playerY-1) && checkRoofCollision(playerX - sideEdge, playerY - hatHeight)) {
			playerXScaled += -((-xSpeed) >> 3);
		}
		else {
			xSpeed = 0;
		}
	}
	if (xSpeed > 0) {
		if (checkRoofCollision(playerX + sideEdge, playerY - (hatHeight >> 1)) && checkRoofCollision(playerX + sideEdge, playerY-1) && checkRoofCollision(playerX + sideEdge, playerY - hatHeight)) {
			playerXScaled += xSpeed >> 3;
		}
		else {
			xSpeed = 0;
		}
	}



	playerX = playerXScaled >> 3;


	if (ySpeed >= 0) {
		//player bottom
		uint16_t newPlayerY = (playerYScaled + (ySpeed>>3)) >> 3;

		if (checkFloorCollision(playerX, newPlayerY) && checkFloorCollision(playerX+ sideEdge -1, newPlayerY) && checkFloorCollision(playerX-sideEdge + 1, newPlayerY)) {
			addGravity();
			
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
		uint16_t newPlayerY = ((playerYScaled + (ySpeed>>3)) >> 3) - hatHeight;

		if (!checkRoofCollision(playerX, newPlayerY) || !checkRoofCollision(playerX + sideEdge - 1, newPlayerY) || !checkRoofCollision(playerX - sideEdge + 1, newPlayerY)) {
			ySpeed = 0;
		}
		addGravity();
	}

	if (joydata & J_A) {
		if (onGround && !jumping && jumpReleased) {
			ySpeed = -JUMPPOWER;
			jumping = 1;
			onGround = 0;
			jumpReleased = 0;
			set_sprite_tile(0, 8);
			set_sprite_tile(1,  10);
		}
		else if (jumping < 12 && !onGround ) {
			jumping += 1;
			ySpeed -= JUMPCARRY;
		}
	} else {
		jumpReleased = 1;
	}




	playerYScaled += ySpeed >> 3;
	playerY = playerYScaled >> 3;



	uint16_t currentTile = (maxHp - hp) / 100;
	currentTile = u16Clamp(currentTile,0, 7);
	if (currentTile != previousTile) {
		set_sprite_data(0, 16, playerTilesets[currentTile]);
		previousTile = currentTile;
	}
	//printf("%d\n", (playerFrame << 1));

	
	if (ySpeed == 0) {
		set_sprite_tile(0,  (playerFrame << 2));
		set_sprite_tile(1,  2+ (playerFrame << 2));
	}

	if (!checkObstacleCollision(playerX - sideEdge, playerY - (hatHeight >> 1))
		|| !checkObstacleCollision(playerX - sideEdge, playerY-1) 
		|| !checkObstacleCollision(playerX + sideEdge, playerY - (hatHeight >> 1))
		|| !checkObstacleCollision(playerX + sideEdge, playerY-1) )
	{
		hp = 0;
	}
	

}


void updateEntityPositions() {

	int16_t xDiff = playerX - CENTERX;
	int16_t yDiff = playerY - CENTERY - 32;

	if (xDiff < 0) {
		xDiff = 0;
	}
	//printf("%d\n", (MapZeroWidth << 3) - 160);
	if (xDiff > (MapZeroWidth << 3) - 160) { //map width - 160 = (95)
		xDiff = (MapZeroWidth << 3) - 160;
	}
	if (yDiff < 0) {
		yDiff = 0;
	}
	if (yDiff > (MapZeroHeight << 3) - 144) { 
		yDiff = (MapZeroHeight << 3) - 144;
	}
	//player positions
	onScreenX = playerX - xDiff;
	onScreenY = playerY - yDiff;
	move_sprite(0, onScreenX, onScreenY);
	move_sprite(1, onScreenX + 8, onScreenY);


	if (projectile.active) {
		//projectile positions
		uint16_t projectileScreenX = projectile.x - xDiff;
		uint16_t projectileScreenY = projectile.y - yDiff;
		move_sprite(2, projectileScreenX, projectileScreenY +4);
	}





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

void startLevel() {

	hp = maxHp;

	uint16_t x = 0;
	uint16_t y = 0;

	VBK_REG = 1;
	set_bkg_submap(0, 0, 20, 18, MapZeroPLN1, MapZeroWidth);
	VBK_REG = 0;
	set_bkg_submap(0, 0, 20, 18, MapZeroPLN0, MapZeroWidth);



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
	for (uint16_t j = 0; j < MapZeroHeight; j++) {
		if (shouldBreak) {
			break;
		}
		for (uint16_t i = 0; i < MapZeroWidth; i++) {
			uint16_t ind = MapZeroHeight*j + i;
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

void playDeathAnimation() {
	for(uint8_t i = 0; i < 5;i++) {
		move_sprite(0, 200, 200);
		move_sprite(1, 200, 200);
		wait_vbl_done();
		delay(100);
		move_sprite(0, onScreenX, onScreenY);
		move_sprite(1, onScreenX+8, onScreenY);
		wait_vbl_done();
		delay(100);
	}
}

void fire() {
	projectile.active = 1;
	projectile.x = playerX + 4;
	projectile.y = playerY - 8;
	projectile.scaledX = (playerX + 4)<<3;
	projectile.scaledY = (playerY -8)<<3;

	//TODO: korjaa nopeuden skaalaus
	if (xDir > 0) {
		projectile.xSpeed = 20 + xSpeed;
	}
	if (xDir < 0) {
		projectile.xSpeed = -20 + xSpeed;
	}
	projectile.ySpeed = -80 + ySpeed;
}

void moveProjectiles() {
	projectile.scaledX += projectile.xSpeed;

	projectile.ySpeed += AIRGRAVITY ;

	projectile.scaledY += projectile.ySpeed >> 3;

	projectile.x = projectile.scaledX >> 3;
	projectile.y = projectile.scaledY >> 3;

	
	if  (i16abs(playerX - projectile.x) > 200 || i16abs(playerY - projectile.y) > 200) {
		projectile.active = 0;
		move_sprite(projectile.tile, 200, 200);
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

	set_bkg_data(0, 8, Tileset); 

	VBK_REG = 1;
	//set_bkg_tiles(0,0, 32, 32, MapZeroPLN1);
	set_bkg_submap(0, 0, 20, 18, MapZeroPLN1, MapZeroWidth);
	VBK_REG = 0;
	//set_bkg_tiles(0, 0, 32, 32, MapZeroPLN0);
	//move_bkg(0,0);


	set_bkg_submap(0, 0, 20, 18, MapZeroPLN0, MapZeroWidth);


	set_sprite_palette(0, 1, &spritePalette[0]); // loading 1 palette of 4 colors

	set_sprite_data(0, 16, playerTilesets[0]);


	set_sprite_data(16, 2, Projectile);
	set_sprite_palette(1, 1, &projectilePalette[0]);


	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);

	set_sprite_prop(0,0); //loading 0th palette
	set_sprite_prop(1,0); //also 0th

	set_sprite_tile(2, 0x10);
	set_sprite_prop(2,1); //also 0th
	move_sprite(2, 100, 100);

	while(1) {

		startLevel();

		while(1) {

			previousJoydata = joydata;
			joydata = joypad();

			movePlayer();

			animatePlayer();

			if (joydata & J_B && !(previousJoydata & J_B)) {
				fire();
			}

			moveProjectiles();

			updateEntityPositions();

			if (checkFinish()) {
				break;
			}

			if (hp <= 0) {
				playDeathAnimation();
				break;
			}

			wait_vbl_done();
			

		}

	}
	
}