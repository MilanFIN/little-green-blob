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

sourceTemplate = """
#include <gbdk/platform.h>
#pragma bank 255


BANKREF(_MAPNAME_Width)
const uint8_t _MAPNAME_Width = _MAPWIDTH_;
BANKREF(_MAPNAME_Height)
const uint8_t _MAPNAME_Height = _MAPHEIGHT_;

BANKREF(_MAPNAME_PLN0)
const unsigned char _MAPNAME_PLN0[] =
_PLN0CONTENT_


BANKREF(_MAPNAME_PLN01)
const unsigned char _MAPNAME_PLN1[] =
_PLN1CONTENT_

BANKREF(get_MAPNAME_TilePlane)
unsigned char* get_MAPNAME_TilePlane() __banked {
    return &_MAPNAME_PLN0;
}
BANKREF(get_MAPNAME_4PalettePlane)
unsigned char* get_MAPNAME_PalettePlane() __banked {
    return &_MAPNAME_PLN1;
}

BANKREF(getM_MAPNAME_Width)
const uint8_t get_MAPNAME_Width() __banked {
    return _MAPNAME_Width;
}
BANKREF(get_MAPNAME_Height)
const uint8_t get_MAPNAME_Height() __banked {
    return _MAPNAME_Height;
}


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


	pln0content = ""
	inPln0 = False
	pln1content = ""
	inPln1 = False
	mapWidth = ""
	mapHeight = ""

	with open(source) as f:
		lines = f.readlines()
		for line in lines:
#define Map14Width 20
			if ("#define "+mapname+"Width " in line):
				mapWidth = line.split(' ')[-1]
			if ("#define "+mapname+"Height " in line):
				mapHeight = line.split(' ')[-1]

			if (("const unsigned char "+mapname+"PLN0[]") in line):
				inPln0 = True
				continue
			if (inPln0):
				pln0content += line
				if ("};" in line):
					inPln0 = False
			if (("const unsigned char "+mapname+"PLN1[]") in line):
				inPln1 = True
				continue
			if (inPln1):
				pln1content += line
				if ("};" in line):
					inPln1 = False

	sourceOutput = sourceTemplate.replace("_MAPNAME_", mapname) \
									.replace("_PLN0CONTENT_", pln0content) \
									.replace("_PLN1CONTENT_", pln1content) \
									.replace("_MAPWIDTH_", mapWidth) \
									.replace("_MAPHEIGHT_", mapHeight)
	print(sourceOutput)



if (len(args.keys()) == 2):
	sourcefile = args["source"]
	headerfile = args["header"]


	convert(headerfile, sourcefile)