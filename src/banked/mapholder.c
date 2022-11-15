#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>

#include "map11.h"
#include "map10.h"
#include "map09.h"
#include "map08.h"
#include "map07.h"
#include "map06.h"
#include "map05.h"
#include "map04.h"
#include "map03.h"
#include "map02.h"
#include "map01.h"
#include "map00.h"


#pragma bank 255




BANKREF(getMapBank)
uint8_t getMapBank(uint8_t  mapNumber) BANKED {
	uint8_t mapBank;

	switch(mapNumber) {
		case 0:
			mapBank = BANK(Map00PLN0);
			break;
		case 1:
			mapBank = BANK(Map01PLN0);
			break;
		
		case 2:
			mapBank = BANK(Map02PLN0);
			break;
		
		case 3:
			mapBank = BANK(Map03PLN0);
			break;
		case 4:
			mapBank = BANK(Map04PLN0);
			break;		
		case 5: 
			mapBank = BANK(Map05PLN0);
			break;
		case 6:
			mapBank = BANK(Map06PLN0);
			break;
		case 7:
			mapBank = BANK(Map07PLN0);
			break;
		case 8:
			mapBank = BANK(Map08PLN0);
			break;
		case 9:
			mapBank = BANK(Map09PLN0);
			break;
		case 10:
			mapBank = BANK(Map10PLN0);
			break;
		case 11:
			mapBank = BANK(Map11PLN0);
			break;		
		default:
			mapBank = BANK(Map00PLN0);	
	}

	return mapBank;
}


BANKREF(getMapWidth)
uint8_t getMapWidth(uint8_t  mapNumber) BANKED {
	
	FAR_PTR widthPtr;

	switch(mapNumber) {
		case 0:
			widthPtr = to_far_ptr(getMap00Width, BANK(getMap00Width));
			break;
		case 1:
			widthPtr = to_far_ptr(getMap01Width, BANK(getMap01Width));
			break;
		
		case 2:
			widthPtr = to_far_ptr(getMap02Width, BANK(getMap02Width));
			break;
		case 3:
			widthPtr = to_far_ptr(getMap03Width, BANK(getMap03Width));
			break;
		case 4:
			widthPtr = to_far_ptr(getMap04Width, BANK(getMap04Width));
			break;		
		case 5: 
			widthPtr = to_far_ptr(getMap05Width, BANK(getMap05Width));
			break;
		case 6:
			widthPtr = to_far_ptr(getMap06Width, BANK(getMap06Width));
			break;
		case 7:
			widthPtr = to_far_ptr(getMap07Width, BANK(getMap07Width));
			break;
		case 8:
			widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
			break;
		case 9:
			widthPtr = to_far_ptr(getMap09Width, BANK(getMap09Width));
			break;
		case 10:
			widthPtr = to_far_ptr(getMap10Width, BANK(getMap10Width));
			break;
		case 11:
			widthPtr = to_far_ptr(getMap11Width, BANK(getMap11Width));
			break;		
		default:
			widthPtr = to_far_ptr(getMap00Width, BANK(getMap00Width));			
	}

	uint8_t mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));

	return mapWidth;
}

BANKREF(getMapHeight)
uint8_t getMapHeight(uint8_t  mapNumber) BANKED {
	
	FAR_PTR heightPtr;

	switch(mapNumber) {
		case 0:
			heightPtr = to_far_ptr(getMap00Height, BANK(getMap00Height));
			break;
		case 1:
			heightPtr = to_far_ptr(getMap01Height, BANK(getMap01Height));
			break;
		case 2:
			heightPtr = to_far_ptr(getMap02Height, BANK(getMap02Height));
			break;
		
		case 3:
			heightPtr = to_far_ptr(getMap03Height, BANK(getMap03Height));
			break;
		case 4:
			heightPtr = to_far_ptr(getMap04Height, BANK(getMap04Height));
			break;		
		case 5: 
			heightPtr = to_far_ptr(getMap05Height, BANK(getMap05Height));
			break;
		case 6:
			heightPtr = to_far_ptr(getMap06Height, BANK(getMap06Height));
			break;
		case 7:
			heightPtr = to_far_ptr(getMap07Height, BANK(getMap07Height));
			break;
		case 8:
			heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
			break;
		case 9:
			heightPtr = to_far_ptr(getMap09Height, BANK(getMap09Height));
			break;
		case 10:
			heightPtr = to_far_ptr(getMap10Height, BANK(getMap10Height));
			break;
		case 11:
			heightPtr = to_far_ptr(getMap11Height, BANK(getMap11Height));
			break;		
		default:
			heightPtr = to_far_ptr(getMap00Height, BANK(getMap00Height));
	}

	uint8_t mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));

	return mapHeight;
}

BANKREF(getMapTiles)
uint16_t getMapTiles(uint8_t mapNumber) BANKED {
	FAR_PTR tilePtr;
	switch(mapNumber) {
		case 0:
			tilePtr = to_far_ptr(getMap00TilePlane, BANK(getMap00TilePlane));
			break;
		case 1:
			tilePtr = to_far_ptr(getMap01TilePlane, BANK(getMap01TilePlane));
			break;		
		case 2:
			tilePtr = to_far_ptr(getMap02TilePlane, BANK(getMap02TilePlane));
			break;
		
		case 3:
			tilePtr = to_far_ptr(getMap03TilePlane, BANK(getMap03TilePlane));
			break;
		case 4:
			tilePtr = to_far_ptr(getMap04TilePlane, BANK(getMap04TilePlane));
			break;		
		case 5: 
			tilePtr = to_far_ptr(getMap05TilePlane, BANK(getMap05TilePlane));
			break;
		case 6:
			tilePtr = to_far_ptr(getMap06TilePlane, BANK(getMap06TilePlane));
			break;
		case 7:
			tilePtr = to_far_ptr(getMap07TilePlane, BANK(getMap07TilePlane));
			break;
		case 8:
			tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
			break;
		case 9:
			tilePtr = to_far_ptr(getMap09TilePlane, BANK(getMap09TilePlane));
			break;
		case 10:
			tilePtr = to_far_ptr(getMap10TilePlane, BANK(getMap10TilePlane));
			break;
		case 11:
			tilePtr = to_far_ptr(getMap11TilePlane, BANK(getMap11TilePlane));
			break;
		default:
			tilePtr = to_far_ptr(getMap00TilePlane, BANK(getMap00TilePlane));			
	}

	uint16_t mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
	return mapTiles;
}


BANKREF(getMapPalette)
uint16_t getMapPalette(uint8_t mapNumber) BANKED {
	FAR_PTR palettePtr;
	switch(mapNumber) {
		case 0:
			palettePtr = to_far_ptr(getMap00PalettePlane, BANK(getMap00PalettePlane));
			break;
		case 1:
			palettePtr = to_far_ptr(getMap01PalettePlane, BANK(getMap01PalettePlane));
			break;
		case 2:
			palettePtr = to_far_ptr(getMap02PalettePlane, BANK(getMap02PalettePlane));
			break;
		case 3:
			palettePtr = to_far_ptr(getMap03PalettePlane, BANK(getMap03PalettePlane));
			break;
		case 4:
			palettePtr = to_far_ptr(getMap04PalettePlane, BANK(getMap04PalettePlane));
			break;		
		case 5: 
			palettePtr = to_far_ptr(getMap05PalettePlane, BANK(getMap05PalettePlane));
			break;
		case 6:
			palettePtr = to_far_ptr(getMap06PalettePlane, BANK(getMap06PalettePlane));
			break;
		case 7:
			palettePtr = to_far_ptr(getMap07PalettePlane, BANK(getMap07PalettePlane));
			break;
		case 8:
			palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
			break;
		case 9:
			palettePtr = to_far_ptr(getMap09PalettePlane, BANK(getMap09PalettePlane));
			break;
		case 10:
			palettePtr = to_far_ptr(getMap10PalettePlane, BANK(getMap10PalettePlane));
			break;
		case 11:
			palettePtr = to_far_ptr(getMap11PalettePlane, BANK(getMap11PalettePlane));
			break;	
		default:
			palettePtr = to_far_ptr(getMap00PalettePlane, BANK(getMap00PalettePlane));			
	}

	uint16_t mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));
	return mapPalette;

}

/*
BANKREF(initMapPointers)
void initMapPointers(uint8_t  mapNumber) BANKED {

	FAR_PTR tilePtr;
	FAR_PTR palettePtr;
	FAR_PTR widthPtr;
	FAR_PTR heightPtr;
	switch(mapNumber) {
		case 0:
			tilePtr = to_far_ptr(getMap00TilePlane, BANK(getMap00TilePlane));
			palettePtr = to_far_ptr(getMap00PalettePlane, BANK(getMap00PalettePlane));
			widthPtr = to_far_ptr(getMap00Width, BANK(getMap00Width));
			heightPtr = to_far_ptr(getMap00Height, BANK(getMap00Height));
			mapBank = BANK(Map00PLN0);
			break;
		case 1:
			tilePtr = to_far_ptr(getMap01TilePlane, BANK(getMap01TilePlane));
			palettePtr = to_far_ptr(getMap01PalettePlane, BANK(getMap01PalettePlane));
			widthPtr = to_far_ptr(getMap01Width, BANK(getMap01Width));
			heightPtr = to_far_ptr(getMap01Height, BANK(getMap01Height));
			mapBank = BANK(Map01PLN0);
			break;
		
		case 2:
			tilePtr = to_far_ptr(getMap02TilePlane, BANK(getMap02TilePlane));
			palettePtr = to_far_ptr(getMap02PalettePlane, BANK(getMap02PalettePlane));
			widthPtr = to_far_ptr(getMap02Width, BANK(getMap02Width));
			heightPtr = to_far_ptr(getMap02Height, BANK(getMap02Height));
			mapBank = BANK(Map02PLN0);

			break;
		
		case 3:
			tilePtr = to_far_ptr(getMap03TilePlane, BANK(getMap03TilePlane));
			palettePtr = to_far_ptr(getMap03PalettePlane, BANK(getMap03PalettePlane));
			widthPtr = to_far_ptr(getMap03Width, BANK(getMap03Width));
			heightPtr = to_far_ptr(getMap03Height, BANK(getMap03Height));
			mapBank = BANK(Map03PLN0);
			
			break;
		case 4:
			tilePtr = to_far_ptr(getMap04TilePlane, BANK(getMap04TilePlane));
			palettePtr = to_far_ptr(getMap04PalettePlane, BANK(getMap04PalettePlane));
			widthPtr = to_far_ptr(getMap04Width, BANK(getMap04Width));
			heightPtr = to_far_ptr(getMap04Height, BANK(getMap04Height));
			mapBank = BANK(Map04PLN0);

			break;		
		case 5: 
			tilePtr = to_far_ptr(getMap05TilePlane, BANK(getMap05TilePlane));
			palettePtr = to_far_ptr(getMap05PalettePlane, BANK(getMap05PalettePlane));
			widthPtr = to_far_ptr(getMap05Width, BANK(getMap05Width));
			heightPtr = to_far_ptr(getMap05Height, BANK(getMap05Height));
			mapBank = BANK(Map05PLN0);
			break;

		case 6:
			tilePtr = to_far_ptr(getMap06TilePlane, BANK(getMap06TilePlane));
			palettePtr = to_far_ptr(getMap06PalettePlane, BANK(getMap06PalettePlane));
			widthPtr = to_far_ptr(getMap06Width, BANK(getMap06Width));
			heightPtr = to_far_ptr(getMap06Height, BANK(getMap06Height));
			mapBank = BANK(Map06PLN0);
			break;
		case 7:
			tilePtr = to_far_ptr(getMap07TilePlane, BANK(getMap07TilePlane));
			palettePtr = to_far_ptr(getMap07PalettePlane, BANK(getMap07PalettePlane));
			widthPtr = to_far_ptr(getMap07Width, BANK(getMap07Width));
			heightPtr = to_far_ptr(getMap07Height, BANK(getMap07Height));
			mapBank = BANK(Map07PLN0);
			break;
		case 8:
			tilePtr = to_far_ptr(getMap08TilePlane, BANK(getMap08TilePlane));
			palettePtr = to_far_ptr(getMap08PalettePlane, BANK(getMap08PalettePlane));
			widthPtr = to_far_ptr(getMap08Width, BANK(getMap08Width));
			heightPtr = to_far_ptr(getMap08Height, BANK(getMap08Height));
			mapBank = BANK(Map08PLN0);
			break;
		case 9:
			tilePtr = to_far_ptr(getMap09TilePlane, BANK(getMap09TilePlane));
			palettePtr = to_far_ptr(getMap09PalettePlane, BANK(getMap09PalettePlane));
			widthPtr = to_far_ptr(getMap09Width, BANK(getMap09Width));
			heightPtr = to_far_ptr(getMap09Height, BANK(getMap09Height));
			mapBank = BANK(Map09PLN0);
			break;
		case 10:
			tilePtr = to_far_ptr(getMap10TilePlane, BANK(getMap10TilePlane));
			palettePtr = to_far_ptr(getMap10PalettePlane, BANK(getMap10PalettePlane));
			widthPtr = to_far_ptr(getMap10Width, BANK(getMap10Width));
			heightPtr = to_far_ptr(getMap10Height, BANK(getMap10Height));
			mapBank = BANK(Map10PLN0);
			break;		
		default:
			tilePtr = to_far_ptr(getMap00TilePlane, BANK(getMap00TilePlane));
			palettePtr = to_far_ptr(getMap00PalettePlane, BANK(getMap00PalettePlane));
			widthPtr = to_far_ptr(getMap00Width, BANK(getMap00Width));
			heightPtr = to_far_ptr(getMap00Height, BANK(getMap00Height));
			mapBank = BANK(Map00PLN0);
			
	}

	mapTiles = FAR_CALL(tilePtr,uint16_t (*)(void));
	mapPalette = FAR_CALL(palettePtr,uint16_t (*)(void));; 
	mapWidth = FAR_CALL(widthPtr,uint8_t (*)(void));
	mapHeight = FAR_CALL(heightPtr,uint8_t (*)(void));

}

*/