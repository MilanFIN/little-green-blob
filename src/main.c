#include <gb/gb.h>  
#include <stdio.h>

#include "maps/tileset.c"
#include "maps/0.c"

void main(){



	//disable_interrupts();



	NR52_REG = 0x80; // sound on 
    NR50_REG = 0x77; // volume
    NR51_REG = 0xFF; // all channels


	//SHOW_WIN;
	SHOW_SPRITES;
	SHOW_BKG;

	while(1) {

		set_bkg_data(0, 3, Tileset); 

		set_bkg_tiles(0, 0, 32, 32, MapZero);
		move_bkg(0,0);

		wait_vbl_done();

	}
}