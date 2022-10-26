

typedef struct ProjectileObj {
	uint8_t active;
	int16_t x;
	int16_t y;
	int16_t scaledX;
	int16_t scaledY;
	int16_t xSpeed;
	int16_t ySpeed;
	uint8_t tile;
};


/*struct ProjectileObj projectile = {
	0,0,0,0,0, 2
};
*/

struct ProjectileObj projectiles[3];

const uint8_t PROJECTILECOUNT = 3;

const uint16_t fireDelay = 20; //frames

const int16_t PROJECTILEINITXSPEED= 100;

const int16_t PROJECTILEINITYSPEED= 75;


uint8_t oldestProjectile = 0;

uint16_t framesSinceLastFire = 1000;

