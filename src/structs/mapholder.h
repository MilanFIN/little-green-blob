

#include "mapzero.h"



typedef struct MapObj {
	const unsigned char* tilePlane;
	const unsigned char* palettePlane;
	const uint16_t width;
	const uint16_t height;
	uint8_t title;
}MapObj;

typedef struct Test {
    const unsigned char * tilePlane;
    uint8_t j;
}Test;

uint8_t MAPCOUNT = 1;

const MapObj MAPS[1] = {
    {MapZeroPLN0, MapZeroPLN1, MapZeroWidth, MapZeroHeight, 0}
};

uint8_t currentMap = 0;


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
};
