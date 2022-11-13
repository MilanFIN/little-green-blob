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
#include "map06.h"
#include "map05.h"



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
			
			break;
		case 1:
			tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
			palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
			widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
			heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
			mapBank = BANK(Map08PLN0);
			
			break;
		
		case 2:
			tilePtr = to_far_ptr(getMap07TilePlane, BANK(getMap07TilePlane));
			palettePtr = to_far_ptr(getMap07PalettePlane, BANK(getMap07PalettePlane));
			widthPtr = to_far_ptr(getMap07Width, BANK(getMap07Width));
			heightPtr = to_far_ptr(getMap07Height, BANK(getMap07Height));
			mapBank = BANK(Map07PLN0);
			
			break;
		
		case 3:
			tilePtr = to_far_ptr(getMap06TilePlane, BANK(getMap06TilePlane));
			palettePtr = to_far_ptr(getMap06PalettePlane, BANK(getMap06PalettePlane));
			widthPtr = to_far_ptr(getMap06Width, BANK(getMap06Width));
			heightPtr = to_far_ptr(getMap06Height, BANK(getMap06Height));
			mapBank = BANK(Map06PLN0);
			
			break;
		case 4:
			tilePtr = to_far_ptr(getMap05TilePlane, BANK(getMap05TilePlane));
			palettePtr = to_far_ptr(getMap05PalettePlane, BANK(getMap05PalettePlane));
			widthPtr = to_far_ptr(getMap05Width, BANK(getMap05Width));
			heightPtr = to_far_ptr(getMap05Height, BANK(getMap05Height));
			mapBank = BANK(Map05PLN0);
			
			break;		
		default:
			tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
			palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
			widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
			heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
			mapBank = BANK(Map08PLN0);
			
	}

	mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
	mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
	mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
	mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));



}





