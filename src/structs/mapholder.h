#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>

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
#include "map09.h"




typedef struct MapPointer {
	FAR_PTR tilePtr;
	FAR_PTR palettePtr;
	FAR_PTR widthPtr;
	FAR_PTR heightPtr;
	uint8_t bank;
};

uint8_t MAPCOUNT = 9;

uint8_t mapBank = 0;
unsigned char* mapTiles;//FAR_CALL(tilePtr,uint16_t (*)(void));
unsigned char* mapPalette; //FAR_CALL(palettePtr,uint16_t (*)(void));
uint8_t mapWidth; //FAR_CALL(widthPtr,uint8_t (*)(void));
uint8_t mapHeight; //FAR_CALL(widthPtr,uint8_t (*)(void));


struct MapPointer MAPPOINTERS[1];
uint8_t currentMap;


void initMapPointers() {
	
	struct MapPointer test = {
		to_far_ptr(getMap09TilePlane, BANK(getMap09TilePlane)),
		to_far_ptr(getMap09PalettePlane, BANK(getMap09PalettePlane)),
		to_far_ptr(getMap09Width, BANK(getMap09Width)),
		to_far_ptr(getMap09Height, BANK(getMap09Height)),
		BANK(Map09PLN0)

	};
	MAPPOINTERS[0] = test;



	mapTiles = FAR_CALL(MAPPOINTERS[currentMap].tilePtr,uint16_t (*)(void));
	mapPalette = FAR_CALL(MAPPOINTERS[currentMap].palettePtr,uint16_t (*)(void));; //FAR_CALL(palettePtr,uint16_t (*)(void));
	mapWidth = FAR_CALL(MAPPOINTERS[currentMap].widthPtr,uint8_t (*)(void));
	mapHeight = FAR_CALL(MAPPOINTERS[currentMap].heightPtr,uint8_t (*)(void));
	mapBank = MAPPOINTERS[currentMap].bank;

}

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
