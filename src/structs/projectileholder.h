

typedef struct ProjectileObj {
	uint8_t active;
	int16_t x;
	int16_t y;
	int16_t scaledX;
	int16_t scaledY;
	int16_t xSpeed;
	int16_t ySpeed;
	uint8_t tile;
	uint16_t framesSinceActivation;
};


/*struct ProjectileObj projectile = {
	0,0,0,0,0, 2
};
*/

struct ProjectileObj projectiles[1];

const uint8_t PROJECTILECOUNT = 1;

const uint16_t fireDelay = 20; //frames

const int16_t PROJECTILEINITXSPEED= 50; //100

const int16_t PROJECTILEINITYSPEED= 75;

const uint16_t PROJECTILEACTIVATIONCOOLDOWN = 30;


uint8_t oldestProjectile = 0;

uint16_t framesSinceLastFire = 1000;

