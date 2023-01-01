# Little Green Blob

A gameboy platformer written in C using gbdk-2020.

## Compiling

Modify `GBDK_HOME` variable in the `Makefile` with the path to the gbdk-2020 toolset

After that run `Make -B`. Output will be placed in `build/gb/main.gb`

## Project structure

Most important functions should (hopefully in the future) have a descriptor comments on what their purpose is

`src/main.c`: Program code that resides in `BANK 0`.

`src/banked/banked.c`: Banked functions that couldn't fit in the primary memory bank.

`src/banked/mapholder.c`: a manager for retrieving pointers to map data & their corresponding memory banks.

`src/maps/`: all map (as in levels) source files

## adding new maps

* The score saving system currently has enough room for total of 100 maps. Beyond that some modifications have to be made to the memory management of the save.

* See GBMB project files for reference or base for modifications: `src/map_project_files/`

* Export the new map with GBMB. Name your map uniquely, eg. Map20 (names Map00 to Map19 are already in use)
Name the export files similarly to those in `src/map_project_files/`

* Run the converter described below to convert the exported source files to be compatible with the project

* Place the map source files into `src/maps/`

* Modify each function in `src/banked/mapholder.c` to include an entry for the new map. This is how the game loads the maps.

* Increment `MAPCOUNT` in `src/main.c`

* Compile the project. Now the new map should have an entry in the level selection menu.


## converter
Contains a python script to convert gameboy map builder exports into versions compatible with this project.

Example call eg: `python3 converter.py -sc map14.c -he map14.h`

Outputs will be placed into equivalent files with .out suffix

