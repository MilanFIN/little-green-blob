# Little Green Blob

A gameboy platformer written in C using gbdk-2020.

# Compiling

Modify `GBDK_HOME` variable in the `Makefile` with the path to the gbdk-2020 toolset

After that call `Make -B`. Output will be placed in `build/gb/main.gb`

# Project structure

Most important functions should (hopefully in the future) have a descriptor comments on what their purpose is

`main.c`: Program code that resides in `BANK 0`.

`banked/banked.c`: Banked functions that couldn't fit in the primary memory bank.

`banked/mapholder.c`: a manager for retrieving pointers to map data & their corresponding memory banks.

`maps/`: all map source files

## converter
Contains a python script to convert gameboy map builder exports into versions compatible with this project.

Example call eg: `python3 converter.py -sc map14.c -he map14.h`

Outputs will be placed into equivalent files with .out suffix

