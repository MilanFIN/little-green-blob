
/*
#include "map00.h"
#include "map01.h"
#include "map02.h"
#include "map03.h"
#include "map04.h"
#include "map05.h"
#include "map06.h"
#include "map07.h"
*/
#include <gbdk/platform.h>

#include "map09.h"



typedef struct MapObj {
	const unsigned char* tilePlane;
	const unsigned char* palettePlane;
	const uint16_t width;
	const uint16_t height;
	const uint8_t bank;
}MapObj;


uint8_t MAPCOUNT = 9;


/*
const MapObj MAPS[1] = {
	
    {Map00PLN0, Map00PLN1, Map00Width, Map00Height, 0},
	{Map01PLN0, Map01PLN1, Map01Width, Map01Height, 0},
	{Map02PLN0, Map02PLN1, Map02Width, Map02Height, 0},
	{Map03PLN0, Map03PLN1, Map03Width, Map03Height, 0},
	{Map04PLN0, Map04PLN1, Map04Width, Map04Height, 0},
	{Map05PLN0, Map05PLN1, Map05Width, Map05Height, 0},
	{Map06PLN0, Map06PLN1, Map06Width, Map06Height, 0},
	{Map07PLN0, Map07PLN1, Map07Width, Map07Height, 0},
	{Map08PLN0, Map08PLN1, Map08Width, Map08Height, 0},
	
	{Map09PLN0, Map09PLN1, Map09Width, Map09Height, 9}
	
};
*/

uint8_t currentMap = 9;


const UWORD primaryBackgroundPalette[] = {
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
	TilesetCGBPal4c3,
	
	TilesetCGBPal5c0,
	TilesetCGBPal5c1,
	TilesetCGBPal5c2,
	TilesetCGBPal5c3,
	
	TilesetCGBPal6c0,
	TilesetCGBPal6c1,
	TilesetCGBPal6c2,
	TilesetCGBPal6c3,
	
	TilesetCGBPal7c0,
	TilesetCGBPal7c1,
	TilesetCGBPal7c2,
	TilesetCGBPal7c3,
	
};
