#include <gb/gb.h>  
#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>

#include <gb/cgb.h>

#include "tileset.h"

#include "primaryblocks.h"
#include "secondaryblocks.h"
#include "timetrapsup.h"
#include "timetrapsdown.h"


#include "mapholder.h"
#include "banked.h"


#include "palette.h"

#include "playertiles.h"


#include "projectile.h"

#include "projectileholder.h"

#define MIN(A,B) ((A)<(B)?(A):(B))

//increment this, whenever adding new maps
const uint8_t MAPCOUNT = 20;

//addresses for different map tiles in vram
const unsigned char FLOORTILES[2] = {0x01, 0x06}; 
const uint8_t FLOORTILECOUNT = 2;
const unsigned char ROOFTILES[1] = {0x01}; 
const uint8_t ROOFTILECOUNT = 1;
const unsigned char OBSTACLETILES[1] = {0x07};
const uint8_t OBSTACLECOUNT = 1;
const unsigned char SWITCHBLOCKS[2] = {0x08, 0x09}; 
const uint8_t SWITCHTILE = 0x0a;
const unsigned char TIMETRAPBLOCKS[1] = {0x0b}; 
const unsigned char TIMEPLATFORMBLOCKS[2] = {0x0c, 0x0d}; 
const uint8_t SPAWNPOINTTILE = 0x04;


//pointers to hold map data for a currently loaded map
uint8_t mapBank;
unsigned char* mapTiles;
unsigned char* mapPalette;
uint8_t mapWidth;
uint8_t mapHeight; 

//serial id to keep track of the current map/level
uint8_t currentMap = 0;






//palette data for different sprites
const UWORD playerPalette[] = {
	PlayerTiles0CGBPal0c0,
	PlayerTiles0CGBPal0c1,
	PlayerTiles0CGBPal0c2,
	PlayerTiles0CGBPal0c3,
	PlayerTiles0CGBPal1c0,
	PlayerTiles0CGBPal1c1,
	PlayerTiles0CGBPal1c2,
	PlayerTiles0CGBPal1c3
};



const UWORD projectilePalette[] = {
	ProjectileCGBPal0c0,
	ProjectileCGBPal0c1,
	ProjectileCGBPal0c2,
	ProjectileCGBPal0c3
};


//used to keep track of the original bank whenever switching away from the main one
uint8_t _saved_bank;


//red/blue switch for switchblocks, 0 for red
uint8_t switchState = 0;
//raises/lowers spike traps, 0 for down
uint8_t timeTrapState = 1;
//counter for time traps, triggers when reaching zero
uint8_t timeTrapCounter = 60;
const uint8_t TIMETRAPTIMELIMIT = 60;

//center of screen 
const uint8_t CENTERX = 80;
const uint8_t CENTERY = 72;

uint8_t joydata = 0;
uint8_t previousJoydata = 0;

//timers for switching player palette whenever taking damage
uint8_t playerHurtPaletteTime = 10;//frames
uint8_t playerHurt = 0;
uint8_t playerHurtPaletteCounter = 0;

//player position when scaled down onto draw space, not actual onscreen position, but instead related
//to the background map's top left edge
// x: center of player
//y: bottom edge of player
uint16_t playerX = 80;
uint16_t playerY = 50;

//player position in game logic,
//multiplied by 8 to achieve subpixel movement
int16_t playerXScaled = 80<<3;
int16_t playerYScaled = 50<<3;

//1 when player facing forward
int8_t xDir = 1;

//accels for horizontal movement
const int16_t groundAcceleration = 16;
const int16_t airAcceleration = 4;
//scaled by factor of 2^3 = 8
int16_t xSpeed = 0;
const int16_t maxXSpeed = 64;

//positive when going down
// scaled down by factor of 2^3 = 8, before applying to scaledposition
int16_t ySpeed = 0;
//accel downwards
const int16_t AIRGRAVITY = 8;
//clamping speed downward whenever in water
const int16_t WATERFALLSPEEDLIMIT = 32;
//initial jump power
const int16_t JUMPPOWER = 80;
//how much holding A will affect jump height
const int16_t JUMPCARRY = 16;


//keeping track of jump status with these
uint8_t onGround = 0;
uint8_t inWater = 0;
uint8_t jumping = 0;
//0 when a is pressed
uint8_t jumpReleased = 1;

//nonzero value means, that player should fall through floor only tiles (aka single directional platforms)
// used for dropping down platforms
uint8_t dropDownFrames = 0;
const uint8_t DROPDOWNDURATION = 20;

//will be set to the tile index that has the finish
// initialized during level load
uint16_t finishTileIndex = 0;

//health
int16_t hp = 800;
const int16_t maxHp = 800;

//frame counter for player idle animation
uint8_t playerFrame = 0;
uint8_t frameCounter = 20;
const uint8_t ANIMFRAMETIME = 20;

//keeps track of last player "size" tile from previous frame
uint8_t previousTile = 0;

//map position related stuff, a bit complex due to using submap
uint8_t old_map_pos_x = 255;
uint8_t old_map_pos_y = 255;
int16_t xDiff = 0;
int16_t yDiff = 0;

int16_t oldXDiff = 0;
int16_t oldYDiff = 0;

int16_t onScreenX = 0;
int16_t onScreenY = 0;

int16_t oldOnScreenX = 0;


//how many times fall damage is bit shifted right before applying
// (larger number for less dmg)
const uint8_t FALLDAMAGESCALE = 1;

//keeps track of the height of player in pixels
uint16_t hatHeight;
//keeps track of width of player from center in both directions
uint16_t sideEdge;
//1 if a map has traps that switch based on time
uint8_t hasTraps = 0;

//true if a+down was just pressed
uint8_t justDroppedDown = 0;

//shifts time based traps (spikes) up & down
void updateTraps() {
	if (timeTrapCounter == 0) {
		if (hasTraps) {
			playSound(7);

		}
		if (timeTrapState) {
			timeTrapState = 0;
			VBK_REG=0;
			set_bkg_data(TIMETRAPBLOCKS[0], 3, TimeTrapsDown); 
		}
		else {
			timeTrapState = 1;
			VBK_REG=0;
			set_bkg_data(TIMETRAPBLOCKS[0], 3, TimeTrapsUp); 
		}
		timeTrapCounter = TIMETRAPTIMELIMIT;
	}
}

//deactivates a projectile blob and moves it away from viewport
void removeProjectile(uint8_t i) {
	projectiles[i].active = 0;
	move_sprite(projectiles[i].tile, 200, 200);
}

//switches blue&red blocks on/off. One is always a platform, and the other is not
void switchBlocks() {

	playSound(6);
	if (switchState) {
		switchState = 0;
		VBK_REG=0;
		set_bkg_data(SWITCHBLOCKS[0], 3, PrimaryBlocks); 

	}
	else {
		switchState = 1;
		VBK_REG=0;
		set_bkg_data(SWITCHBLOCKS[0], 3, SecondaryBlocks); 
	}
}

//checks for collision with floors in a specific point (onscreen x & y)
//presumes that the ROM BANK has been set to the one stored in mapBank before function call
//returns: 1 if there is no collision with floortiles or switch controlled & timed blocks in a point
//			0 if there is a collision
inline uint8_t checkFloorCollision(uint16_t x, uint16_t y) {
	uint16_t ind = mapWidth*((y)>>3) + ((x)>>3);
	uint8_t floor = 0;
	
	for (uint8_t i=0; i < FLOORTILECOUNT; i++) {
		if (mapTiles[ind] == FLOORTILES[i]) {
			floor = 1;
		} 
	}
	if (floor) {
		return 0;
	}

	else {
		if (mapTiles[ind] == SWITCHBLOCKS[0] && switchState == 0) {
			return 0;
		}
		if (mapTiles[ind] == SWITCHBLOCKS[1] && switchState == 1) {
			return 0;
		}
		if (mapTiles[ind] == TIMEPLATFORMBLOCKS[0] && timeTrapState == 0) {
			return 0;
		}		
		if (mapTiles[ind] == TIMEPLATFORMBLOCKS[1] && timeTrapState == 1) {
			return 0;
		}
		return 1;
	}
}


//checks for collision with roof tiles in a specific point (onscreen x & y)
//presumes that the ROM BANK has been set to the one stored in mapBank before function call
//returns: 1 if there is no roof tiles, 0 if there is a collision
inline uint8_t checkRoofCollision(uint16_t x, uint16_t y) {
	
	uint16_t ind = mapWidth*((y)>>3) + ((x)>>3);
	uint8_t roof = 0;
	for (uint8_t i=0; i < ROOFTILECOUNT; i++) {
		if (mapTiles[ind] == ROOFTILES[i]) {
			roof = 1;
		} 
	}
	if (roof) {
		return 0;
	}
	
	else {
		if (mapTiles[ind] == SWITCHBLOCKS[0] && switchState == 0) {
			return 0;
		}
		if (mapTiles[ind] == SWITCHBLOCKS[1] && switchState == 1) {
			return 0;
		}
		if (mapTiles[ind] == TIMEPLATFORMBLOCKS[0] && timeTrapState == 0) {
			return 0;
		}		
		if (mapTiles[ind] == TIMEPLATFORMBLOCKS[1] && timeTrapState == 1) {
			return 0;
		}
		return 1;
	}
}

//checks if point overlaps any active traps or other obstacles
//returns 1, if there is no overlap, 0 otherwise
//presumes that the ROM BANK has been set to the one stored in mapBank before function call
inline uint8_t checkObstacleCollision(uint16_t x, uint16_t y) {
	uint16_t ind = mapWidth*((y)>>3) + ((x)>>3);
	uint8_t dead = 0;
	for (uint8_t i=0; i < OBSTACLECOUNT; i++) {
		if (mapTiles[ind] == OBSTACLETILES[i]) {
			dead = 1;
		} 
	}
	if (dead) {
		return 0;
	}
	else {
		if (timeTrapState) { //traps are in up position
			if (mapTiles[ind] == TIMETRAPBLOCKS[0]) {
				return 0;
			}
		}
	}
	return 1 ;
}

//checks if player collides with any traps or obtacles
//if so, sets hp to 0
//presumes that the ROM BANK has been set to the one stored in mapBank before function call
void checkTrapCollision() {
	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	if (!checkObstacleCollision(playerX - sideEdge +1, playerY - (hatHeight >> 1) +1)
	|| !checkObstacleCollision(playerX - sideEdge +1, playerY-1) 
	|| !checkObstacleCollision(playerX + sideEdge -1, playerY - (hatHeight >> 1)+1)
	|| !checkObstacleCollision(playerX + sideEdge +1, playerY-1) )
	{
		hp = 0;
	}
	SWITCH_ROM(_saved_bank);
}



//moves all active projectiles horizontally & vertically
//stops them if they hit the floor & removes them if they are too far from the player
void moveProjectiles() {

	for (uint8_t i = 0; i < PROJECTILECOUNT ; i++) {

		if (projectiles[i].active) {
			if (projectiles[i].xSpeed > 0) {
				projectiles[i].scaledX += projectiles[i].xSpeed >> 3;

			}
			if (projectiles[i].xSpeed < 0) {
				projectiles[i].scaledX += -((-projectiles[i].xSpeed) >> 3);
			}

			
			if (!checkFloorCollision(projectiles[i].x, projectiles[i].y+3)) {
				projectiles[i].ySpeed = 0;
			}
			else {
				projectiles[i].ySpeed += AIRGRAVITY ;
			}
			


			projectiles[i].scaledY += projectiles[i].ySpeed >> 3;

			projectiles[i].x = projectiles[i].scaledX >> 3;
			projectiles[i].y = projectiles[i].scaledY >> 3;

			projectiles[i].framesSinceActivation++;

		
			
			if  (i16abs(playerX - projectiles[i].x) > 200 || i16abs(playerY - projectiles[i].y) > 100) {
				removeProjectile(i);
			}
			


		}
		
	}
}

//initiates a new projectile if the oldest known projectile is not active
void fire() {

	if (framesSinceLastFire < fireDelay) {
		return;
	}
	if (projectiles[oldestProjectile].active) {
		return;
	}

	playSound(4);
	hp -= 10;
	framesSinceLastFire = 0;


	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	uint16_t ind = mapWidth*((playerY - 3)>>3) + ((playerX)>>3);

	if (mapTiles[ind] == SWITCHTILE) {
		switchBlocks();
		SWITCH_ROM(_saved_bank);

		return;
	}

	SWITCH_ROM(_saved_bank);
	
	projectiles[oldestProjectile].active = 1;
	projectiles[oldestProjectile].x = playerX + 4;
	projectiles[oldestProjectile].y = playerY - 8;
	projectiles[oldestProjectile].scaledX = (playerX + 4)<<3;
	projectiles[oldestProjectile].scaledY = (playerY -8)<<3;

	projectiles[oldestProjectile].framesSinceActivation = PROJECTILEACTIVATIONCOOLDOWN;
	

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


//loops through the 2 player idle animation frames
//and animates the palette change for when taking damage
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

//checks if a projectile collides with the following and performs actions
// switches: triggers switch blocks to flip
// vertical wall: delete projectile
void checkProjectileCollisions() {
	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	for (uint8_t i = 0; i < PROJECTILECOUNT ; i++) {

		if (projectiles[i].active) {

			if (!checkFloorCollision(projectiles[i].x-3, projectiles[i].y-4) //-4 
				|| !checkFloorCollision(projectiles[i].x+3, projectiles[i].y -4)) 
			{
				removeProjectile(i);
				continue;

			}


			uint16_t ind = mapWidth*((projectiles[i].y-3)>>3) + (((projectiles[i].x-3)>>3));
			uint16_t ind2 = mapWidth*((projectiles[i].y-3)>>3) + (((projectiles[i].x+3)>>3));

			if (mapTiles[ind] == SWITCHTILE || 
				mapTiles[ind2] == SWITCHTILE) {
				
				if (projectiles[i].framesSinceActivation > PROJECTILEACTIVATIONCOOLDOWN) {
					switchBlocks();

					projectiles[i].framesSinceActivation = 0;
				}
			} 

		}
	}
	SWITCH_ROM(_saved_bank);

}

//presumes that ROM_BANK is set to mapBank before a call
//puts a player that is slightly off floor level onto ground
//iterates downward from the player's feet and checks when floor is reached
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





//presumes that ROM_BANK is set to mapBank before a call
//moves player downward in accelerating manner.
//speed down is clamped when overlapping with water tiles
inline void addGravity() {
	uint16_t ind = mapWidth*((playerY - 2)>>3) + ((playerX)>>3);
	ySpeed += AIRGRAVITY;
	if (ySpeed > 256) {
		ySpeed = 256;
	}

	
	if (mapTiles[ind] == 0x02 || mapTiles[ind] == 0x03 ) {
		if (!inWater) {
			playSound(10);
		}
		inWater = 1;
		if (ySpeed > WATERFALLSPEEDLIMIT) {
			ySpeed = WATERFALLSPEEDLIMIT;
		}
	}
	else {
		inWater = 0;
	}
}

//moves player vertically
void moveVertical() {
	_saved_bank = _current_bank;

	SWITCH_ROM(mapBank);

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

				if (justDroppedDown) {
					playSound(5);
					justDroppedDown = 0;
				}

			}
			else {
				onGround = 1;
				justDroppedDown = 0;
				dropDownFrames = 0;

			}
		}
		else {
			//checking if player should fall (not on a floor)
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
			//checking if the player has just "hit" the floor
			if (ySpeed != 0) {
				//when hitting ground, player might be a couple pixels off, so move them firmly on the ground
				playSound(5);
				putPlayerOnGround(sideEdge);
				playerHurt = 1;
				playerHurtPaletteTime = uClamp(ySpeed >> 4, 1, 10);
				hp -= ySpeed >> FALLDAMAGESCALE;
				spawnSplash(onScreenX, onScreenY, sideEdge);
			}
			jumping = 0;
			ySpeed = 0;

		}

	}

	//check if moving upwards
	if (ySpeed < 0) {
		//player top if movement goes through this frame
		uint16_t newPlayerY = ((playerYScaled + (ySpeed>>3)) >> 3) - hatHeight;
		//checking if the player has hit a roof
		if (!checkRoofCollision(playerX, newPlayerY) || !checkRoofCollision(playerX + sideEdge - 1, newPlayerY) || !checkRoofCollision(playerX - sideEdge + 1, newPlayerY)) {
			ySpeed = 0;
		}
		//accelerate down due to gravity
		addGravity();
	}

	if (joydata & J_A && !(joydata & J_DOWN)) {
		//detect situation where player wants to initiate a jump
		if (onGround && !jumping && jumpReleased) {
			ySpeed = -JUMPPOWER;
			jumping = 1;
			onGround = 0;
			jumpReleased = 0;
			set_sprite_tile(0, 8);
			set_sprite_tile(1,  10);
			playSound(3);
		}
		//detect when A button is being held down, so jump higher
		else if (jumping < 12 && !onGround && ySpeed < 0) {
			jumping += 1;
			ySpeed -= JUMPCARRY;
		}
	} else {
		jumpReleased = 1;
	}



	//being in water adds hp
	if (inWater) {
		hp += 10;

	}

	//moving horizontally bleeds hp, this should be somewhere else, but it's tied to movement
	if (onGround && xSpeed != 0) {
		hp -= 1;
	}
	hp = i16Clamp(hp, 0, maxHp);


	playerYScaled += ySpeed >> 3;
	playerY = playerYScaled >> 3;


	SWITCH_ROM(_saved_bank);


	//tile as in vram tiles for player tileset
	uint16_t currentTile = (maxHp - hp) / 100;
	currentTile = u16Clamp(currentTile,0, 7);
	if (currentTile != previousTile) {
		_saved_bank = _current_bank;
		SWITCH_ROM(BANK(playerTilesets));
		set_sprite_data(0, 16, playerTilesets[currentTile]);
		SWITCH_ROM(_saved_bank);

		spawnBleed(onScreenX, onScreenY, xDir);

		previousTile = currentTile;
	}

	
	if (ySpeed == 0) {
		set_sprite_tile(0,  (playerFrame << 2));
		set_sprite_tile(1,  2+ (playerFrame << 2));
	}

	

}

//moves player horizontally
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


	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
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
	SWITCH_ROM(_saved_bank);


	playerX = playerXScaled >> 3;
}

//figures out where the player & background map should be put on the screen
//playerx & y mean how far the player should be from the top left corner of map
//map position is calculated accordingly to scroll smoothly, but not showing outside of the map in viewport
void updateEntityPositions(uint8_t force) {

	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);
	xDiff = playerX - CENTERX;
	yDiff = playerY - CENTERY - 32;

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
	if (xDir >= 0) {
		set_sprite_prop(0,0); 
		set_sprite_prop(1,0); 

		move_sprite(0, onScreenX, onScreenY);
		move_sprite(1, onScreenX + 8, onScreenY);
	}
	else {
		set_sprite_prop(0,S_FLIPX); 
		set_sprite_prop(1,S_FLIPX); 
		move_sprite(0, onScreenX+8, onScreenY);
		move_sprite(1, onScreenX, onScreenY);

	}




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
		//oldXDiff = xDiff;
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

    }
	
	
	scrollSplash(oldXDiff - xDiff, oldYDiff - yDiff);
	

	oldXDiff = xDiff;
	oldYDiff = yDiff;


    SCX_REG = xDiff; SCY_REG = yDiff; 


	SWITCH_ROM(_saved_bank);



}

//moves projectiles to correct position on screen, based on their position in relation to the background map
//updateentitypositions must be called before this to update xdiff & ydiff
void updateProjectilePositions() {
	for (uint8_t i = 0; i < PROJECTILECOUNT ; i++) {
		if (projectiles[i].active) {
			//projectile positions converted to screen space from game locations
			uint16_t projectileScreenX = projectiles[i].x - xDiff;
			uint16_t projectileScreenY = projectiles[i].y - yDiff;
			//position probably overflows at values > 256
			if (projectileScreenX > 0 && projectileScreenX < 200 && projectileScreenY > 0 && projectileScreenY < 200) {
				move_sprite(projectiles[i].tile, projectileScreenX+4, projectileScreenY+4);
			}
			else {
				move_sprite(projectiles[i].tile, 220, 220);
			}
		}
	}
	
}

//set_bkg submap draws garbage tiles on the screen, if the background moves significantly at once(>8px/frame)
//this can happen when a new level is loaded and the player teleports to the location where the level should start
//this can be avoided if the map is scrolled smoothly, as new tiles appear correctly as long as the single jump per frame is not too large
//to achieve this, this function does the following:
// initialize player at top left corner of the map
// move player smoothly to bottom right and back to top left
// move player smoothly to the point x&y
void shufflePlayer(uint16_t x, uint16_t y) {

	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);


	playerX = 0;
	playerY = 0;
	while (playerX < mapWidth*8 || playerY < mapHeight*8 ) {
		if (playerX + 8 < mapWidth*8) {
			playerX += 8; //8
		}
		else if (playerX < mapWidth*8) {
			playerX += 1;
		}
		if (playerY + 8 < mapHeight*8) {
			playerY += 8; //8
		}
		else if (playerY < mapHeight*8) {
			playerY += 1;
		}
		updateEntityPositions(1);
	}

	while (playerY > 0 || playerX > 0) {
		if (playerY > 8) {
			playerY -= 8;
		}
		else if (playerY > 0) {
			playerY--;
		}
		if (playerX > 8) {
			playerX -= 8;
		}
		else if (playerX > 0) {
			playerX--;
		}
		updateEntityPositions(1);
	}	
	while (playerY < y || playerX < x) {
		if (playerY + 8 < y) {
			playerY += 8; //8
		}
		else if (playerY < y) {
			playerY++;
		}
		if (playerX + 8 < x) {
			playerX += 8; //8
		}
		else if (playerX < x) {
			playerX++;
		}
		updateEntityPositions(1);
	}	
	playerX = x;
	playerY = y;
		
	
	SWITCH_ROM(_saved_bank);


}


//initializes the projectile array, see structs/projectileholder.h for projectile related definitions
void initProjectiles() {
	for (uint8_t i = 0; i < PROJECTILECOUNT; ++i) {
		projectiles[i].active = 0;	
		projectiles[i].x = 200;
		projectiles[i].y = 200;
		projectiles[i].scaledX = 200 << 3;
		projectiles[i].scaledY = 200 << 3;
		projectiles[i].tile = 10 + i;
		projectiles[i].framesSinceActivation = PROJECTILEACTIVATIONCOOLDOWN;


		set_sprite_tile(projectiles[i].tile, 0x10);
		set_sprite_prop(projectiles[i].tile,1); //1 (2nd) palette
		move_sprite(projectiles[i].tile, 200, 200);

	}
}


//loads the level, which id is stored in currentMap
//initializes sprite locations, palettes etc.
void startLevel()  {

	HIDE_BKG;
	HIDE_SPRITES;
	DISPLAY_OFF;

	//get map data
	mapBank = getMapBank(currentMap);
	mapWidth = getMapWidth(currentMap);
	mapHeight = getMapHeight(currentMap);

	mapTiles = getMapTiles(currentMap);
	mapPalette = getMapPalette(currentMap);


	//init sprites from second bank
	initSplashDownSprite();


	//red/blue switch for switchblocks, 0 for red
	switchState = 0;
	//raises/lowers spike traps, 0 for down
	timeTrapState = 1;


	xSpeed = 0;
	ySpeed = 0;

	justDroppedDown = 0;

	initProjectiles();
	hp = maxHp;

	uint16_t x = 0; // MAPS[currentMap].width*8
	uint16_t y = 0;//MAPS[currentMap].height*8;

	set_bkg_palette(0, 8, &primaryBackgroundPalette[0]);
	
	//set map tileset data to vram
	_saved_bank = _current_bank;
	SWITCH_ROM(BANK(Tileset));
	VBK_REG=0;
	set_bkg_data(0, 14, Tileset); 
	SWITCH_ROM(_current_bank);

	set_bkg_data(SWITCHBLOCKS[0], 3, PrimaryBlocks); 

	//load background map to top left corner
	SWITCH_ROM(mapBank);
	VBK_REG = 1;
	set_bkg_submap(0, 0, 20, 18, mapPalette, mapWidth);
	VBK_REG = 0;
	set_bkg_submap(0, 0, 20, 18, mapTiles, mapWidth);

	//finds, which tile the player should spawn on the map
	uint8_t shouldBreak = 0;
	for (y = 0; y < mapHeight; y++) {
		if (shouldBreak) {
			break;
		}
		for (x = 0; x < mapWidth; x++) {
			uint16_t ind = mapWidth*y + x;
			if (mapTiles[ind] == SPAWNPOINTTILE) {
				shouldBreak = 1;
				break;
			}
		}
	}
	//these are locations of player in relation to top left corner of the level
	x = x*8 + 4;
	y *= 8;
	//these are the same coordinates game logic locations (8x more values to have subpixel movenet)
	playerXScaled = x << 3;
	playerYScaled = y << 3;


	//set_bkg_submap will mess up the screen if values are too large, so move the player there smoothly
	SWITCH_ROM(_saved_bank);
	shufflePlayer(x, y);

	_saved_bank = _current_bank;
	SWITCH_ROM(mapBank);

	//figuring out where the finishing tile is and if there are any timed traps/platforms on the level
	hasTraps = 0;
	for (uint16_t j = 0; j < mapHeight; j++) {
		for (uint16_t i = 0; i < mapWidth; i++) {
			uint16_t ind = mapWidth*j + i;
			if (mapTiles[ind] == 0x05) {
				finishTileIndex = ind;
			}
			if (mapTiles[ind] == TIMETRAPBLOCKS[0] 
				||mapTiles[ind] == TIMEPLATFORMBLOCKS[0]
				|| mapTiles[ind] == TIMEPLATFORMBLOCKS[1] )
				{
					hasTraps = 1;
				}
		}
	}
	SWITCH_ROM(_saved_bank);

	//finally setting everything where they belong
	updateEntityPositions(1);
	
	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;


}



void main(){



	_saved_bank = _current_bank;

	//disable_interrupts();

	DISPLAY_ON;
	SPRITES_8x16;


	NR52_REG = 0x80; // sound on 
    NR50_REG = 0x77; // volume
    NR51_REG = 0xFF; // all channels


	//SHOW_WIN;
	SHOW_SPRITES;
	SHOW_BKG;


	//splash screen
	showStartMenu();





	while(1) {
		//ask player to select a map
		currentMap = levelSelectionMenu(MAPCOUNT, currentMap);


		set_sprite_palette(0, 1, &playerPalette[0]); // loading 1 palette of 4 colors


		//load player sprite data
		SWITCH_ROM(BANK(playerTilesets));
		set_sprite_data(0, 16, playerTilesets[0]);

		SWITCH_ROM(_saved_bank);


		//projectile sprite data
		set_sprite_data(16, 2, Projectile);
		set_sprite_palette(1, 1, &projectilePalette[0]);

		set_sprite_tile(0, 0);
		set_sprite_tile(1, 2);

		set_sprite_prop(0,0); //setting 0th palette
		set_sprite_prop(1,0); //also 0th



		//loading the level that is now set in currentMap
		startLevel();

		while(1) {

			
			previousJoydata = joydata;
			joydata = joypad();

			//game lifecycle
			moveHorizontal();
			moveVertical();
			updateEntityPositions(0);
			updateProjectilePositions();
			animatePlayer();

			
			checkTrapCollision();
			moveProjectiles();


			//most button inputs
			if (joydata & J_B && !(previousJoydata & J_B)) {
				fire();
			}
			
			//select restarts level
			if (!(joydata & J_SELECT) && (previousJoydata & J_SELECT)) {
				playSound(9);
				startLevel();
			}
			//A+down drops down from a non roof tile
			if ((joydata & J_A && joydata & J_DOWN) && !(previousJoydata & J_A && previousJoydata & J_DOWN)) {
				justDroppedDown = 1;
				dropDownFrames = DROPDOWNDURATION;
			}

			if (!(joydata & J_START) && (previousJoydata & J_START)) {
				playSound(0);
				break;
			}
			//more lifecycle
			checkProjectileCollisions();
			updateTraps();
			loopSplash();
			loopBleed();
			scrollBleed(onScreenX, onScreenY);

			framesSinceLastFire++;
			timeTrapCounter--;


			//checking if player is on top of finish flag
			if (checkFinish(finishTileIndex, mapWidth, playerX, playerY)) {
				playSound(8);
				mapCompleted(currentMap, hp);
				currentMap++;
				if (currentMap >= MAPCOUNT) {
					currentMap = 0;
					break;
				}
				startLevel();
			}

			//checking if player is dead, and restarting level
			if (hp <= 0) {
				set_sprite_palette(0, 1, &playerPalette[0]); 
				playSound(9);
				playDeathAnimation(onScreenX, onScreenY, xDir);

				startLevel();
			}
			


			wait_vbl_done();
			


		}
		

	}
	
}