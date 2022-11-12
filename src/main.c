#include <gb/gb.h>  
#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>

#include <stdio.h>
#include <gb/cgb.h>

#include "tileset.h"

#include "primaryblocks.h"
#include "secondaryblocks.h"
#include "timetrapsup.h"
#include "timetrapsdown.h"


#include "mapholder.h"

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


//TODO: 
// paletin vaihto kun ottaa vahinkoa, ja ajoittain kun esim hp <200
// aaltoefekti kun ylilatautunut hp? skaala hp:n perusteella, lähestyy hiljalleen maxhp
// alas + a pudottautuu?


unsigned char FLOORTILES[2] = {0x01, 0x06}; 
uint8_t FLOORTILECOUNT = 2;
unsigned char ROOFTILES[1] = {0x01}; 
uint8_t ROOFTILECOUNT = 1;
unsigned char OBSTACLETILES[1] = {0x07};
uint8_t OBSTACLECOUNT = 1;
unsigned char SWITCHBLOCKS[2] = {0x08, 0x09}; 
uint8_t SWITCHTILE = 0x0a;
unsigned char TIMETRAPBLOCKS[1] = {0x0b}; 
unsigned char TIMEPLATFORMBLOCKS[2] = {0x0c, 0x0d}; 






UWORD playerPalette[] = {
	PlayerTiles0SGBPal0c0,
	PlayerTiles0CGBPal0c1,
	PlayerTiles0CGBPal0c2,
	PlayerTiles0CGBPal0c3,
	PlayerTiles0SGBPal1c0,
	PlayerTiles0CGBPal1c1,
	PlayerTiles0CGBPal1c2,
	PlayerTiles0CGBPal1c3

};

UWORD projectilePalette[] = {
	ProjectileCGBPal0c0,
	ProjectileCGBPal0c1,
	ProjectileCGBPal0c2,
	ProjectileCGBPal0c3
};

unsigned char* playerTilesets[] = {
	PlayerTiles0,
	PlayerTiles1,
	PlayerTiles2,
	PlayerTiles3,
	PlayerTiles4,
	PlayerTiles5,
	PlayerTiles6,
	PlayerTiles7
};


uint8_t _saved_bank;


//red/blue switch for switchblocks, 0 for red
uint8_t switchState = 0;
//raises/lowers spike traps, 0 for down
uint8_t timeTrapState = 1;

uint8_t timeTrapCounter = 60;
uint8_t TIMETRAPTIMELIMIT = 60;


uint8_t CENTERX = 80;
uint8_t CENTERY = 72;

uint8_t joydata = 0;
uint8_t previousJoydata = 0;


uint8_t playerHurtPaletteTime = 10;//frames
uint8_t playerHurt = 0;
uint8_t playerHurtPaletteCounter = 0;

uint16_t playerX = 80;
uint16_t playerY = 50;

int16_t playerXScaled = 80<<3;
int16_t playerYScaled = 50<<3;

//where player is facing toward
int8_t xDir = 1;


int16_t groundAcceleration = 16;
int16_t airAcceleration = 4;
//scaled by factor of 2^3 = 8
int16_t xSpeed = 0;
int16_t maxXSpeed = 64;

//positive when going down
// scaled by factor of 2^3 = 8
int16_t ySpeed = 0;
int16_t AIRGRAVITY = 8;
int16_t WATERFALLSPEEDLIMIT = 32;
//initial jump power
int16_t JUMPPOWER = 80;
//how much holding A will affect jump height
int16_t JUMPCARRY = 16;



uint8_t onGround = 0;
uint8_t inWater = 0;
uint8_t jumping = 0;
uint8_t jumpReleased = 1;

//nonzero value means, that player should fall through floor only tiles (aka single directional platforms)
uint8_t dropDownFrames = 0;
uint8_t DROPDOWNDURATION = 20;

//will be set to the tile index that has the finish
// initialized during level load
uint16_t finishTileIndex = 0;

int16_t hp = 800;
int16_t maxHp = 800;

uint8_t playerFrame = 0;
uint8_t frameCounter = 20;
uint8_t ANIMFRAMETIME = 20;

uint8_t previousTile = 0;

uint8_t old_map_pos_x = 255;
uint8_t old_map_pos_y = 255;
uint16_t oldXDiff = 0;
uint16_t oldYDiff = 0;
uint16_t onScreenX = 0;
uint16_t onScreenY = 0;


//how many times fall damage is bit shifted right before applying
// (larger number for less dmg)
uint8_t FALLDAMAGESCALE = 1;


uint16_t hatHeight;
uint16_t sideEdge;



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




/*

//returns true if there is no collision in a point
inline uint8_t checkFloorCollision(uint16_t x, uint16_t y) {
	uint16_t ind = mapWidth*((y)>>3) + ((x)>>3);
	uint8_t floor = 0;
	
	SWITCH_ROM(mapBank);
	for (uint8_t i=0; i < FLOORTILECOUNT; i++) {
		if (MAPS[currentMap].tilePlane[ind] == FLOORTILES[i]) {
			floor = 1;
		} 
	}
	if (floor) {
		return 0;
	}

	else {
		if (MAPS[currentMap].tilePlane[ind] == SWITCHBLOCKS[0] && switchState == 0) {
			return 0;
		}
		if (MAPS[currentMap].tilePlane[ind] == SWITCHBLOCKS[1] && switchState == 1) {
			return 0;
		}
		if (MAPS[currentMap].tilePlane[ind] == TIMEPLATFORMBLOCKS[0] && timeTrapState == 0) {
			return 0;
		}		
		if (MAPS[currentMap].tilePlane[ind] == TIMEPLATFORMBLOCKS[1] && timeTrapState == 1) {
			return 0;
		}
		return 1;
	}
}


inline uint8_t checkRoofCollision(uint16_t x, uint16_t y) {
	uint16_t ind = MAPS[currentMap].width*((y)>>3) + ((x)>>3);
	uint8_t roof = 0;
	for (uint8_t i=0; i < ROOFTILECOUNT; i++) {
		if (MAPS[currentMap].tilePlane[ind] == ROOFTILES[i]) {
			roof = 1;
		} 
	}
	if (roof) {
		return 0;
	}
	
	else {
		return 1;
	}
}


//returns 1, if player should stay alive
inline uint8_t checkObstacleCollision(uint16_t x, uint16_t y) {
	uint16_t ind = MAPS[currentMap].width*((y)>>3) + ((x)>>3);
	uint8_t dead = 0;
	for (uint8_t i=0; i < OBSTACLECOUNT; i++) {
		if (MAPS[currentMap].tilePlane[ind] == OBSTACLETILES[i]) {
			dead = 1;
		} 
	}
	if (dead) {
		return 0;
	}
	else {
		if (timeTrapState) { //traps are in up position
			if (MAPS[currentMap].tilePlane[ind] == TIMETRAPBLOCKS[0]) {
				return 0;
			}
		}
	}
	return 1 ;
}

inline void addGravity() {
	uint16_t ind = MAPS[currentMap].width*((playerY - 8)>>3) + ((playerX)>>3);
	ySpeed += AIRGRAVITY;

	
	if (MAPS[currentMap].tilePlane[ind] == 0x02 || MAPS[currentMap].tilePlane[ind] == 0x03 ) {
		inWater = 1;
		if (ySpeed > WATERFALLSPEEDLIMIT) {
			ySpeed = WATERFALLSPEEDLIMIT;
		}
	}
	else {
		inWater = 0;
	}
}


void putPlayerOnGround(uint16_t sideEdge) {
	uint16_t yDistance = 0;
	uint16_t newPlayerY = playerY;
	while (yDistance < 16) {
		
        if (!checkFloorCollision(playerX, newPlayerY) || 
			!checkFloorCollision(playerX+ sideEdge -1, newPlayerY) ||
			!checkFloorCollision(playerX-sideEdge + 1, newPlayerY))
		{	
			break;
		}
		newPlayerY++;
		yDistance++;
	}

	playerY = newPlayerY;
	playerYScaled = playerY << 3;

}



void moveHorizontal() {
	hatHeight = (hp >> 6) + 3;
	sideEdge = (hp >> 7) + 2;

	int16_t acceleration = (onGround) ? groundAcceleration : airAcceleration;

	if (joydata & J_LEFT) {
			xDir = -1;
			xSpeed -= acceleration;
	}
	else if (joydata & J_RIGHT) {
			xDir = 1;
			xSpeed += acceleration;
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


}

void moveVertical() {


	if (ySpeed >= 0) {
		//player bottom
		uint16_t newPlayerY = (playerYScaled + (ySpeed>>3)) >> 3;

		//FALLING DOWN CHECKS
		//first check if player shold drop through a one way platform (a+down)
		if (dropDownFrames > 0) {
			if (checkRoofCollision(playerX, newPlayerY) && checkRoofCollision(playerX+ sideEdge -1, newPlayerY) && checkRoofCollision(playerX-sideEdge + 1, newPlayerY)) { 
				addGravity();
				onGround = 0;
				dropDownFrames--;
				set_sprite_tile(0, 12);
				set_sprite_tile(1,  14);

			}
			else {
				onGround = 1;
			}
		}
		//regular gravity check
		else {
			if (checkFloorCollision(playerX, newPlayerY) && checkFloorCollision(playerX+ sideEdge -1, newPlayerY) && checkFloorCollision(playerX-sideEdge + 1, newPlayerY)) {
				addGravity();
				onGround = 0;
				set_sprite_tile(0, 12);
				set_sprite_tile(1,  14);
			}
			//on solid ground
			else {
				onGround = 1;
			}
		}
		if (onGround) {
			if (ySpeed != 0) {
				//when hitting ground, player might be a couple pixels off the ground
				putPlayerOnGround(sideEdge);
				playerHurt = 1;
				playerHurtPaletteTime = uClamp(ySpeed >> 4, 1, 10);
				hp -= ySpeed >> FALLDAMAGESCALE;
			}
			jumping = 0;
			ySpeed = 0;

		}

	}

	if (ySpeed < 0) {
		//player top if movement goes through this frame
		uint16_t newPlayerY = ((playerYScaled + (ySpeed>>3)) >> 3) - hatHeight;

		if (!checkRoofCollision(playerX, newPlayerY) || !checkRoofCollision(playerX + sideEdge - 1, newPlayerY) || !checkRoofCollision(playerX - sideEdge + 1, newPlayerY)) {
			ySpeed = 0;
		}
		addGravity();
	}

	if (joydata & J_A && !(joydata & J_DOWN)) {
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

	if (onGround && xSpeed != 0) {
		hp -= 1;
	}

	if (inWater) {
		hp += 10;

	}

	hp = i16Clamp(hp, 0, maxHp);


	playerYScaled += ySpeed >> 3;
	playerY = playerYScaled >> 3;




	//tile as in vram tiles for player tileset
	uint16_t currentTile = (maxHp - hp) / 100;
	currentTile = u16Clamp(currentTile,0, 7);
	if (currentTile != previousTile) {
		set_sprite_data(0, 16, playerTilesets[currentTile]);
		previousTile = currentTile;
	}

	
	if (ySpeed == 0) {
		set_sprite_tile(0,  (playerFrame << 2));
		set_sprite_tile(1,  2+ (playerFrame << 2));
	}

	

}

void checkTrapCollision() {
	if (!checkObstacleCollision(playerX - sideEdge +1, playerY - (hatHeight >> 1) +1)
	|| !checkObstacleCollision(playerX - sideEdge +1, playerY-1) 
	|| !checkObstacleCollision(playerX + sideEdge -1, playerY - (hatHeight >> 1)+1)
	|| !checkObstacleCollision(playerX + sideEdge +1, playerY-1) )
	{
		hp = 0;
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

	if (playerHurt) {
		if (playerHurtPaletteCounter == 0) {
			set_sprite_palette(0,1, &playerPalette[4]);
		}
		playerHurtPaletteCounter++;
		if (playerHurtPaletteCounter == playerHurtPaletteTime) {
			playerHurt = 0;
			set_sprite_palette(0,1, &playerPalette[0]);
			playerHurtPaletteCounter = 0;

		}
	}
}

//1 if finished, 0 otherwise
uint8_t checkFinish() {
	uint16_t ind = MAPS[currentMap].width*((playerY - 8)>>3) + ((playerX)>>3);

	if (ind == finishTileIndex) {
		return 1;
	}
	else {
		return 0;
	}
}

void playDeathAnimation() {
	set_sprite_palette(0, 1, &playerPalette[0]); 
	set_sprite_tile(0,  (playerFrame << 2));
	set_sprite_tile(1,  2+ (playerFrame << 2));

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

	if (framesSinceLastFire < fireDelay) {
		return;
	}
	if (projectiles[oldestProjectile].active) {
		return;
	}


	hp -= 10;
	framesSinceLastFire = 0;


	uint16_t ind = MAPS[currentMap].width*((playerY - 3)>>3) + ((playerX)>>3);

	if (MAPS[currentMap].tilePlane[ind] == SWITCHTILE) {
		switchBlocks();
		return;
	}

	
	projectiles[oldestProjectile].active = 1;
	projectiles[oldestProjectile].x = playerX + 4;
	projectiles[oldestProjectile].y = playerY - 8;
	projectiles[oldestProjectile].scaledX = (playerX + 4)<<3;
	projectiles[oldestProjectile].scaledY = (playerY -8)<<3;

	projectiles[oldestProjectile].framesSinceActivation = 0;
	

	if (xDir > 0) {
		projectiles[oldestProjectile].xSpeed = PROJECTILEINITXSPEED + xSpeed;
	}
	if (xDir < 0) {
		projectiles[oldestProjectile].xSpeed = -PROJECTILEINITXSPEED + xSpeed;
	}
	projectiles[oldestProjectile].ySpeed = -PROJECTILEINITYSPEED + ySpeed;

	oldestProjectile++;
	if (oldestProjectile >= PROJECTILECOUNT) {
		oldestProjectile = 0;
	}
}

static inline void removeProjectile(uint8_t i) {
	projectiles[i].active = 0;
	move_sprite(projectiles[i].tile, 200, 200);
}

void moveProjectiles() {

	for (uint8_t i = 0; i < PROJECTILECOUNT ; i++) {

		if (projectiles[i].active) {
			if (projectiles[i].xSpeed > 0) {
				projectiles[i].scaledX += projectiles[i].xSpeed >> 3;

			}
			if (projectiles[i].xSpeed < 0) {
				projectiles[i].scaledX += -((-projectiles[i].xSpeed) >> 3);
			}

			projectiles[i].ySpeed += AIRGRAVITY ;

			projectiles[i].scaledY += projectiles[i].ySpeed >> 3;

			projectiles[i].x = projectiles[i].scaledX >> 3;
			projectiles[i].y = projectiles[i].scaledY >> 3;

			projectiles[i].framesSinceActivation++;

		
			
			if  (i16abs(playerX - projectiles[i].x) > 1000 || i16abs(playerY - projectiles[i].y) > 1000) {
				removeProjectile(i);
			}
			


		}
		
	}
}

void switchBlocks() {


	if (switchState) {
		switchState = 0;
		set_bkg_data(SWITCHBLOCKS[0], 3, PrimaryBlocks); 

	}
	else {
		switchState = 1;
		set_bkg_data(SWITCHBLOCKS[0], 3, SecondaryBlocks); 
	}
}

void checkProjectileCollisions() {
	for (uint8_t i = 0; i < PROJECTILECOUNT ; i++) {

		if (projectiles[i].active) {
			if (!checkFloorCollision(projectiles[i].x, projectiles[i].y+3)) {
				projectiles[i].ySpeed = - (PROJECTILEINITYSPEED >> 1);
			}
			if (!checkFloorCollision(projectiles[i].x-3, projectiles[i].y-4) || !checkFloorCollision(projectiles[i].x+3, projectiles[i].y-4)) {
				removeProjectile(i);
				continue;

			}


			uint16_t ind = MAPS[currentMap].width*((projectiles[i].y-3)>>3) + (((projectiles[i].x-3)>>3));
			uint16_t ind2 = MAPS[currentMap].width*((projectiles[i].y-3)>>3) + (((projectiles[i].x+3)>>3));

			if (MAPS[currentMap].tilePlane[ind] == SWITCHTILE || 
				MAPS[currentMap].tilePlane[ind2] == SWITCHTILE) {
				
				if (projectiles[i].framesSinceActivation > PROJECTILEACTIVATIONCOOLDOWN) {
					switchBlocks();

					projectiles[i].framesSinceActivation = 0;
				}
			} 

		}
	}
}

void updateTraps() {
	if (timeTrapCounter == 0) {
		if (timeTrapState) {
			timeTrapState = 0;
			set_bkg_data(TIMETRAPBLOCKS[0], 3, TimeTrapsDown); 
		}
		else {
			timeTrapState = 1;
			set_bkg_data(TIMETRAPBLOCKS[0], 3, TimeTrapsUp); 
		}
		timeTrapCounter = TIMETRAPTIMELIMIT;
	}



}

*/

void updateEntityPositions(uint8_t force) {

	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	int16_t xDiff = playerX - CENTERX;
	int16_t yDiff = playerY - CENTERY - 32;

	if (xDiff < 0) {
		xDiff = 0;
	}
	if (xDiff > (mapWidth << 3) - 160) { //map width - 160 = (95)
		xDiff = (mapWidth << 3) - 160;
	}
	if (yDiff < 0) {
		yDiff = 0;
	}
	if (yDiff > (mapHeight << 3) - 144) { 
		yDiff = (mapHeight << 3) - 144;
	}
	//player positions
	onScreenX = playerX - xDiff;
	onScreenY = playerY - yDiff;
	move_sprite(0, onScreenX, onScreenY);
	move_sprite(1, onScreenX + 8, onScreenY);


	



    uint8_t map_pos_x = (uint8_t)(xDiff >> 3u);
	uint8_t map_pos_y = (uint8_t)(yDiff >> 3u);
	
	//x direction
	if (map_pos_x != old_map_pos_x || force) {
		if (xDiff < oldXDiff) {
			VBK_REG = 1;
			set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, mapHeight - map_pos_y), mapPalette, mapWidth);     
			VBK_REG = 0;
			set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, mapHeight - map_pos_y),mapTiles, mapWidth);     
		} else {
			if ((mapWidth - 20u) > map_pos_x) {
				VBK_REG = 1;
				set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, mapHeight - map_pos_y), mapPalette, mapWidth);   
				VBK_REG = 0;
				set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, mapHeight - map_pos_y), mapTiles, mapWidth);   
			}   
		}
		old_map_pos_x = map_pos_x;
		oldXDiff = xDiff;
    }

	//y direction
	if (map_pos_y != old_map_pos_y || force) { 
		if (yDiff < oldYDiff) {
			VBK_REG = 1;
			set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, mapWidth-map_pos_x), 1, mapPalette, mapWidth);
			VBK_REG = 0;
			set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, mapWidth-map_pos_x), 1, mapTiles, mapWidth);

		} 
		else {
			if ((mapHeight - 18u) > map_pos_y) {
				VBK_REG = 1;
				set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, mapWidth-map_pos_x), 1, mapPalette, mapWidth); 
				VBK_REG = 0;
				set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, mapWidth-map_pos_x), 1, mapTiles, mapWidth); 
			}   
		}
		old_map_pos_y = map_pos_y;
		oldYDiff = yDiff;

    }

	

    SCX_REG = xDiff; SCY_REG = yDiff; 


	SWITCH_ROM(_saved_bank);


	for (uint8_t i = 0; i < PROJECTILECOUNT ; i++) {
	if (projectiles[i].active) {
		//projectile positions
		uint16_t projectileScreenX = projectiles[i].x - xDiff;
		uint16_t projectileScreenY = projectiles[i].y - yDiff;
		//position probably overflows at values > 256
		if (projectileScreenX > 0 && projectileScreenX < 200 && projectileScreenY > 0 && projectileScreenY < 200) {
			move_sprite(projectiles[i].tile, projectileScreenX+4, projectileScreenY);
		}
		else {
			move_sprite(projectiles[i].tile, 220, 220);
		}
	}
}
	
}
//shuffling to initialize bkg submap correctly
void shufflePlayer(uint16_t x, uint16_t y) {

	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);

	//DISPLAY_OFF;

	playerX = 0;
	playerY = 0;
	while (playerX < mapWidth*8 || playerY < mapHeight*8 ) {
		if (playerX + 8 < mapWidth*8) {
			playerX += 1; //8
		}
		if (playerX< mapWidth*8) {
			playerX += 1;
		}
		if (playerY + 8 < mapHeight*8) {
			playerY += 1; //8
		}
		if (playerY < mapHeight*8) {
			playerY += 1;
		}
		updateEntityPositions(1);

	}
	
	while (playerY > y || playerX > x) {

		if (playerY - 8 > y) {
			playerY -= 8; //8
		}
		else if (playerY > y) {
			playerY--;
		}
		if (playerX - 8 > x) {
			playerX -= 8; //8
		}
		else if (playerX > x) {
			playerX--;
		}
		updateEntityPositions(1);

	}
	
	playerX = x;
	playerY = y;
		
	DISPLAY_ON;
	
	SWITCH_ROM(_saved_bank);


}



void initProjectiles() {
	for (uint8_t i = 0; i < PROJECTILECOUNT; ++i) {
		projectiles[i].active = 0;	
		projectiles[i].x = 200;
		projectiles[i].y = 200;
		projectiles[i].scaledX = 200 << 3;
		projectiles[i].scaledY = 200 << 3;
		projectiles[i].tile = 10 + i;
		projectiles[i].framesSinceActivation = 0;


		set_sprite_tile(projectiles[i].tile, 0x10);
		set_sprite_prop(projectiles[i].tile,1); //1 (2nd) palette
		move_sprite(projectiles[i].tile, 200, 200);

	}
}

void startLevel() {

	//red/blue switch for switchblocks, 0 for red
	uint8_t switchState = 0;
	//raises/lowers spike traps, 0 for down
	uint8_t timeTrapState = 1;


	xSpeed = 0;
	ySpeed = 0;

	initProjectiles();
	hp = maxHp;

	uint16_t x = 0; // MAPS[currentMap].width*8
	uint16_t y = 0;//MAPS[currentMap].height*8;

	set_bkg_palette(0, 8, &primaryBackgroundPalette[0]);
	

	set_bkg_data(0, 14, Tileset); 
	set_bkg_data(SWITCHBLOCKS[0], 3, PrimaryBlocks); 

	
	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	VBK_REG = 1;
	set_bkg_submap(0, 0, 20, 18, mapPalette, mapWidth); //Map09PLN1
	VBK_REG = 0;
	set_bkg_submap(0, 0, 20, 18, mapTiles, mapWidth);


	uint8_t shouldBreak = 0;
	for (y = 0; y < mapHeight; y++) {
		if (shouldBreak) {
			break;
		}
		for (x = 0; x < mapWidth; x++) {
			uint16_t ind = mapWidth*y + x;
			if (mapTiles[ind] == 0x04) {
				shouldBreak = 1;
				break;
			}
		}
	}
	x = x*8 + 4;
	y *= 8;
	playerXScaled = x << 3;
	playerYScaled = y << 3;


	SWITCH_ROM(_saved_bank);
	shufflePlayer(x, y);

	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);

	shouldBreak = 0;
	for (uint16_t j = 0; j < mapHeight; j++) {
		if (shouldBreak) {
			break;
		}
		for (uint16_t i = 0; i < mapWidth; i++) {
			uint16_t ind = mapWidth*j + i;
			if (mapTiles[ind] == 0x05) {
				finishTileIndex = ind;
				shouldBreak = 1;
				break;
			}
		}
	}
	SWITCH_ROM(_saved_bank);

	updateEntityPositions(1);
	

}

void main(){

	_saved_bank = _current_bank;
	initMapPointers();

	//disable_interrupts();

	DISPLAY_ON;
	SPRITES_8x16;


	NR52_REG = 0x80; // sound on 
    NR50_REG = 0x77; // volume
    NR51_REG = 0xFF; // all channels


	//SHOW_WIN;
	SHOW_SPRITES;
	SHOW_BKG;




	set_sprite_palette(0, 1, &playerPalette[0]); // loading 1 palette of 4 colors

	set_sprite_data(0, 16, playerTilesets[0]);


	set_sprite_data(16, 2, Projectile);
	set_sprite_palette(1, 1, &projectilePalette[0]);


	set_sprite_tile(0, 0);
	set_sprite_tile(1, 2);

	set_sprite_prop(0,0); //loading 0th palette
	set_sprite_prop(1,0); //also 0th



	//debug
	set_bkg_palette(0, 8, &primaryBackgroundPalette[0]);
	set_bkg_data(0, 14, Tileset); 
	set_bkg_data(SWITCHBLOCKS[0], 3, PrimaryBlocks); 	


	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	VBK_REG = 1;
	set_bkg_submap(0, 0, 20, 18, mapPalette, mapWidth); //Map09PLN1
	VBK_REG = 0;
	set_bkg_submap(0, 0, 20, 18, mapTiles, mapWidth);
	SWITCH_ROM(_saved_bank);



	while(1) {



		
		startLevel();
		//initProjectiles();

		while(1) {
			wait_vbl_done();

			/*
			previousJoydata = joydata;
			joydata = joypad();

			moveHorizontal();
			moveVertical();
			checkTrapCollision();

			animatePlayer();

			if (joydata & J_B && !(previousJoydata & J_B)) {
				fire();
			}
			if (joydata & J_SELECT && !(previousJoydata & J_SELECT)) {
				startLevel();
			}
			if ((joydata & J_A && joydata & J_DOWN) && !(previousJoydata & J_A && previousJoydata & J_DOWN)) {
				dropDownFrames = DROPDOWNDURATION;
			}


			moveProjectiles();

			checkProjectileCollisions();

			updateEntityPositions(0);

			updateTraps();

			if (checkFinish()) {
				break;
			}

			if (hp <= 0) {
				playDeathAnimation();
				break;
			}

			framesSinceLastFire++;
			timeTrapCounter--;

			wait_vbl_done();
			*/


		}

	}
	
}