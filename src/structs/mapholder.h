#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>

/*
#include "map00.h"
#include "map01.h"
#include "map02.h"
#include "map03.h"
#include "map04.h"
#include "map05.h"
*/
#include "map09.h"
#include "map08.h"
#include "map07.h"
//#include "map06.h"



/*
typedef struct MapPointer {
	FAR_PTR tilePtr;
	FAR_PTR palettePtr;
	FAR_PTR widthPtr;
	FAR_PTR heightPtr;
	uint8_t bank;
};
*/

//const uint8_t MAPCOUNT = 9;

FAR_PTR tilePtr;
FAR_PTR palettePtr;
FAR_PTR widthPtr;
FAR_PTR heightPtr;


uint8_t mapBank;
unsigned char* mapTiles;//FAR_CALL(tilePtr,uint16_t (*)(void));
unsigned char* mapPalette; //FAR_CALL(palettePtr,uint16_t (*)(void));
uint8_t mapWidth; //FAR_CALL(widthPtr,uint8_t (*)(void));
uint8_t mapHeight; //FAR_CALL(widthPtr,uint8_t (*)(void));


//struct MapPointer MAPPOINTERS[3];
//uint8_t currentMap = 0;


void initMapPointers(uint8_t  mapNumber) {
	//currentMap = mapNumber;

	//tähän case default rakenne, niin on nätimpi :3



	switch(mapNumber) {
		case 0:
			tilePtr = to_far_ptr(getMap09TilePlane, BANK(getMap09TilePlane));
			palettePtr = to_far_ptr(getMap09PalettePlane, BANK(getMap09PalettePlane));
			widthPtr = to_far_ptr(getMap09Width, BANK(getMap09Width));
			heightPtr = to_far_ptr(getMap09Height, BANK(getMap09Height));
			mapBank = BANK(Map09PLN0);
			
			mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
			mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
			mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
			mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
			break;
		case 1:
			tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
			palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
			widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
			heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
			mapBank = BANK(Map08PLN0);
			
			mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
			mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
			mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
			mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
			break;
		
		case 2:
			tilePtr = to_far_ptr(getMap07TilePlane, BANK(getMap07TilePlane));
			palettePtr = to_far_ptr(getMap07PalettePlane, BANK(getMap07PalettePlane));
			widthPtr = to_far_ptr(getMap07Width, BANK(getMap07Width));
			heightPtr = to_far_ptr(getMap07Height, BANK(getMap07Height));
			mapBank = BANK(Map07PLN0);
			
			mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
			mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
			mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
			mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
			break;
		/*
		case 3:
			tilePtr = to_far_ptr(getMap06TilePlane, BANK(getMap06TilePlane));
			palettePtr = to_far_ptr(getMap06PalettePlane, BANK(getMap06PalettePlane));
			widthPtr = to_far_ptr(getMap06Width, BANK(getMap06Width));
			heightPtr = to_far_ptr(getMap06Height, BANK(getMap06Height));
			mapBank = BANK(Map06PLN0);
			
			mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
			mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
			mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
			mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
			break;
		*/
		default:
			tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
			palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
			widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
			heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
			mapBank = BANK(Map08PLN0);
			
			mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
			mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
			mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
			mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
	}

	/*
	if (mapNumber == 0) {
		FAR_PTR tilePtr = to_far_ptr(getMap09TilePlane, BANK(getMap09TilePlane));
		FAR_PTR palettePtr = to_far_ptr(getMap09PalettePlane, BANK(getMap09PalettePlane));
		FAR_PTR widthPtr = to_far_ptr(getMap09Width, BANK(getMap09Width));
		FAR_PTR heightPtr = to_far_ptr(getMap09Height, BANK(getMap09Height));
		mapBank = BANK(Map09PLN0);
		
		mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
		mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
		mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
		mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));

		return;
	}
	
	if (mapNumber == 1) {
		FAR_PTR tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
		FAR_PTR palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
		FAR_PTR widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
		FAR_PTR heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
		mapBank = BANK(Map08PLN0);
		
		mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
		mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
		mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
		mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
		return;
	}
	if (mapNumber == 2) {
		FAR_PTR tilePtr = to_far_ptr(getMap07TilePlane, BANK(getMap07TilePlane));
		FAR_PTR palettePtr = to_far_ptr(getMap07PalettePlane, BANK(getMap07PalettePlane));
		FAR_PTR widthPtr = to_far_ptr(getMap07Width, BANK(getMap07Width));
		FAR_PTR heightPtr = to_far_ptr(getMap07Height, BANK(getMap07Height));
		mapBank = BANK(Map07PLN0);
		
		mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
		mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
		mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
		mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
		return;
	}
	
	if (mapNumber == 3) {
		FAR_PTR tilePtr = to_far_ptr(getMap06TilePlane, BANK(getMap06TilePlane));
		FAR_PTR palettePtr = to_far_ptr(getMap06PalettePlane, BANK(getMap06PalettePlane));
		FAR_PTR widthPtr = to_far_ptr(getMap06Width, BANK(getMap06Width));
		FAR_PTR heightPtr = to_far_ptr(getMap06Height, BANK(getMap06Height));
		mapBank = BANK(Map06PLN0);
		
		mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
		mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
		mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
		mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));
		return;
	}
	*/

	//		struct MapPointer test;

	/*
	//map09
	test.tilePtr = to_far_ptr(getMap09TilePlane, BANK(getMap09TilePlane));
	test.palettePtr = to_far_ptr(getMap09PalettePlane, BANK(getMap09PalettePlane));
	test.widthPtr = to_far_ptr(getMap09Width, BANK(getMap09Width));
	test.heightPtr = to_far_ptr(getMap09Height, BANK(getMap09Height));
	test.bank = BANK(Map09PLN0);
	
	MAPPOINTERS[0] = test;
	
	
	//map08
	test.tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
	test.palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
	test.widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
	test.heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
	test.bank = BANK(Map08PLN0);

	MAPPOINTERS[1] = test;
*/
	
	//map07
	//test.tilePtr = to_far_ptr(getMap07TilePlane, BANK(getMap07TilePlane));
	//test.palettePtr = to_far_ptr(getMap07PalettePlane, BANK(getMap07PalettePlane));
	//test.widthPtr = to_far_ptr(getMap07Width, BANK(getMap07Width));
	//test.heightPtr = to_far_ptr(getMap07Height, BANK(getMap07Height));
	//test.bank = BANK(Map07PLN0);

	//MAPPOINTERS[2] = test;

	//mapTiles = FAR_CALL(MAPPOINTERS[currentMap].tilePtr,uint16_t (*)(void));
	//mapPalette = FAR_CALL(MAPPOINTERS[currentMap].palettePtr,uint16_t (*)(void));; 
	//mapWidth = FAR_CALL(MAPPOINTERS[currentMap].widthPtr,uint8_t (*)(void));
	//mapHeight = FAR_CALL(MAPPOINTERS[currentMap].heightPtr,uint8_t (*)(void));
	//mapBank = MAPPOINTERS[currentMap].bank;
}





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
