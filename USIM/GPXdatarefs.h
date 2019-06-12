#ifndef _GPX_DATA_H
#define _GPX_DATA_H

struct _GPXDATA_LIST
{
	LPTSTR		VarTokenName;
	int			VarType;
	BOOL		Writable;
	LPTSTR		Units;
	LPTSTR		Description;
};

#define GPXBITS		0x0200

#define GPX_CIF_PITLIGHT    0x01 // set if pit light is on
#define GPX_CIF_REDLED      0x02 // set if red led is on (failure/warning)
#define GPX_CIF_YELLOWLED   0x04 // set if yellow led is on (yellow flag)
#define GPX_CIF_BLUELED     0x08 // set if blue led is on (blue flag)

_BITS_LIST const GPXBitsArray[] = { 
	{"",							0,					0},
	{"GPX/pit_light",				GPX_CIF_PITLIGHT,	GPXBITS},	// bit 0
	{"GPX/failure_warning",			GPX_CIF_REDLED,		GPXBITS},	// bit 1
	{"GPX/yellow_flag",				GPX_CIF_YELLOWLED,	GPXBITS},	// bit 2
	{"GPX/blue_flag",				GPX_CIF_BLUELED,	GPXBITS}	// bit 3
};

_GPXDATA_LIST const GPXDataArray[] = { 
	{"",						xplmType_Int,		0,		"",			""},
	{"GPx/flag",				xplmType_ByteHex,	0,		"",			"flags"},
	{"GPx/lap",					xplmType_Byte,		0,		"",			"lap number"},
	{"GPx/position",			xplmType_Byte,		0,		"",			"race position"},
	{"GPx/fuel",				xplmType_Word,		0,		"",			"fuel laps (value = fuel laps * 10)"},
	{"GPx/temp",				xplmType_Byte,		0,		"",			"water temperature"},
	{"GPx/speed",				xplmType_Int,		0,		"mph/kph",	"rounded speed in either kph or mph"},
	{"GPx/splittime",			xplmType_DWord,		0,		"",			"last split or lap time"},
	{"GPx/gear",				xplmType_Byte,		0,		"",			"gear"},
	{"GPx/revs",				xplmType_Word,		0,		"rpm",		"rpm"},
	{"GPx/time",				xplmType_DWord,		0,		"ms",		"session time in millis"},
	{"GPx/timeFront",			xplmType_DWord,		0,		"ms",		"difference in millis to driver in front or 0 if n/a"},
	{"GPx/timeBehind",			xplmType_DWord,		0,		"ms",		"difference in millis to driver behind or 0 if n/a"},
	{"GPx/driverFront",			xplmType_Char,		0,		"string",	"initial and last name of driver in front, if available"},
	{"GPx/driverBehind",		xplmType_Char,		0,		"string",	"initial and last name of driver behind, if available"},
	{"GPx/flags_string",		xplmType_Char,		0,		"string",	"flag bits text"}
};

#endif