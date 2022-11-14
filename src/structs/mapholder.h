#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>


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




FAR_PTR tilePtr;
FAR_PTR palettePtr;
FAR_PTR widthPtr;
FAR_PTR heightPtr;


uint8_t mapBank;
unsigned char* mapTiles;//FAR_CALL(tilePtr,uint16_t (*)(void));
unsigned char* mapPalette; //FAR_CALL(palettePtr,uint16_t (*)(void));
uint8_t mapWidth; //FAR_CALL(widthPtr,uint8_t (*)(void));
uint8_t mapHeight; //FAR_CALL(widthPtr,uint8_t (*)(void));


uint8_t currentMap = 10;
const uint8_t MAPCOUNT = 11;


void initMapPointers(uint8_t  mapNumber) {

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





