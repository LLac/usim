#ifndef _LFS_DATA_H
#define _LFS_DATA_H

// DL_x - bits for OutGaugePack DashLights and ShowLights

#define DL_SHIFT		0x0001	// bit 0	- shift light
#define DL_FULLBEAM		0x0002	// bit 1	- full beam
#define DL_HANDBRAKE	0x0004	// bit 2	- handbrake
#define DL_PITSPEED		0x0008	// bit 3	- pit speed limiter
#define DL_TC			0x0010	// bit 4	- TC active or switched off
#define DL_SIGNAL_L		0x0020	// bit 5	- left turn signal
#define DL_SIGNAL_R		0x0040	// bit 6	- right turn signal
#define DL_SIGNAL_ANY	0x0080	// bit 7	- shared turn signal
#define DL_OILWARN		0x0100	// bit 8	- oil pressure warning
#define DL_BATTERY		0x0200	// bit 9	- battery warning
#define DL_ABS			0x0400	// bit 10	- ABS active or switched off

struct _LFSDATA_LIST
{
	LPTSTR		VarTokenName;
	int			VarType;
	BOOL		Writable;
	LPTSTR		Units;
	LPTSTR		Description;
};

#define LFSBITS		0x0100

_BITS_LIST const LFSBitsArray[] = { 
	{"",							0,				0},
	{"LFS/shift_light",					DL_SHIFT,		LFSBITS},	// bit 0
	{"LFS/full_beam",					DL_FULLBEAM,	LFSBITS},	// bit 1
	{"LFS/handbrake",					DL_HANDBRAKE,	LFSBITS},	// bit 2
	{"LFS/pit_speed_limiter",			DL_PITSPEED,	LFSBITS},	// bit 3
	{"LFS/TC_active_or_switched_off",	DL_TC,			LFSBITS},	// bit 4
	{"LFS/left_turn_signal",			DL_SIGNAL_L,	LFSBITS},	// bit 5
	{"LFS/right_turn_signal",			DL_SIGNAL_R,	LFSBITS},	// bit 6
	{"LFS/shared_turn_signal",			DL_SIGNAL_ANY,	LFSBITS},	// bit 7
	{"LFS/oil_pressure_warning",		DL_OILWARN,		LFSBITS},	// bit 8
	{"LFS/battery_warning",				DL_BATTERY,		LFSBITS},	// bit 9
	{"LFS/ABS_active_or_switched_off",	DL_ABS,			LFSBITS}	// bit 10
};

_LFSDATA_LIST const LFSDataArray[] = { 
	{"",						xplmType_Int,		0,		"",			""},
	{"LFS/Time",				xplmType_Int,		0,		"ms",		"time in milliseconds"},
	{"LFS/Car",					xplmType_Char,		0,		"ms",		"Car name"},
	{"LFS/Flags",				xplmType_IntHex,	0,		"",			""},
	{"LFS/Gear",				xplmType_Byte,		0,		"",			"Reverse:0, Neutral:1, First:2"},
	{"LFS/Speed",				xplmType_Float,		0,		"m/s",		"speed in meter / sec"},
	{"LFS/RPM",					xplmType_Float,		0,		"rpm",		"RPM"},
	{"LFS/Turbo",				xplmType_Float,		0,		"bar",		"Turbo"},
	{"LFS/EngTemp",				xplmType_Float,		0,		"deg C",	"Engine Temp"},
	{"LFS/Fuel",				xplmType_Float,		0,		"",			"0 to 1"},
	{"LFS/OilPressure",			xplmType_Float,		0,		"bar",		"Oil Pressure"},
	{"LFS/OilTemp",				xplmType_Float,		0,		"deg C",	"Oil Temp"},
	{"LFS/DashLights",			xplmType_IntHex,	0,		"",			"Dash lights available"},
	{"LFS/ShowLights",			xplmType_IntHex,	0,		"",			"Dash lights currently switched on"},
	{"LFS/Throttle",			xplmType_Float,		0,		"",			"0 to 1"},
	{"LFS/Brake",				xplmType_Float,		0,		"",			"0 to 1"},
	{"LFS/Clutch",				xplmType_Float,		0,		"",			"0 to 1"},
	{"LFS/Display1",			xplmType_Char,		0,		"",			"Usually Fuel"},
	{"LFS/Display2",			xplmType_Char,		0,		"",			"Usually Settings"}
};

#endif