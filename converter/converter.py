#!/usr/bin/python

import sys
import argparse
import re

parser = argparse.ArgumentParser()
parser.add_argument('--source', "-sc", type=str )
parser.add_argument('--header', "-he", type=str )

args = parser.parse_args()
args = vars(args)

headerTemplate = """
#ifndef ___MAPLOWERCASE__h_INCLUDE
#define ___MAPLOWERCASE__h_INCLUDE
const unsigned char* get_MAPNAME_TilePlane() BANKED;
BANKREF_EXTERN(get_MAPNAME_TilePlane)
const unsigned char* get_MAPNAME_PalettePlane() BANKED;
BANKREF_EXTERN(get_MAPNAME_PalettePlane)

extern const uint8_t get_MAPNAME_Width() BANKED;
BANKREF_EXTERN(get_MAPNAME_Width)
extern const uint8_t get_MAPNAME_Height() BANKED;
BANKREF_EXTERN(get_MAPNAME_Height)



extern const uint8_t _MAPNAME_Width;
BANKREF_EXTERN(_MAPNAME_Width)
extern const uint8_t _MAPNAME_Height;
BANKREF_EXTERN(_MAPNAME_Height)

extern const unsigned char _MAPNAME_PLN0[];
BANKREF_EXTERN(_MAPNAME_PLN0)
extern const unsigned char _MAPNAME_PLN1[];
BANKREF_EXTERN(_MAPNAME_PLN1)

#endif
"""


def convert(header, source):

	mapname = ""


	with open(header) as f:
		lines = f.readlines()
		for line in lines:
			if ("Width" in line):
				mapname = re.search('#define (.*)Width', line).group(1)
	headerOutput = headerTemplate.replace("_MAPNAME_", mapname).replace("_MAPLOWERCASE_", mapname.lower())
	with open(header+".out", "w") as output:
		output.write(headerOutput)



if (len(args.keys()) == 2):
	sourcefile = args["source"]
	headerfile = args["header"]


	convert(headerfile, sourcefile)