#ifndef _SIMBIN_DATA_H
#define _SIMBIN_DATA_H

struct _SIMBINDATA_LIST
{
	LPTSTR		VarTokenName;
	UINT		VarType;
	BOOL		Writable;
	LPTSTR		Units;
	LPTSTR		Description;
};

_SIMBINDATA_LIST const SimBinDataArray[] = { 
	{"",										xplmType_Int,		0,		"",			""},
	{"SimBin/common/simrunning",				xplmType_Int,		0,		"",			""},
	{"SimBin/driver/Lap_Time",					xplmType_Float,		0,		"sec",		""},
	{"SimBin/driver/Pos_Total",					xplmType_Int,		0,		"",			""},	
	{"SimBin/driver/Pos",						xplmType_Int,		0,		"",			""},
	{"SimBin/engine/Engine_RPM",				xplmType_Float,		0,		"rpm",		"RPM/9.548"},
	{"SimBin/engine/Oil_Pressure",				xplmType_Float,		0,		"kPa",		""},
	{"SimBin/engine/Oil_Temp",					xplmType_Float,		0,		"deg C",	""},
	{"SimBin/engine/Water_Temp",				xplmType_Float,		0,		"deg C",	""},
	{"SimBin/vehicle/Fuel",						xplmType_Float,		0,		"",			""},
	{"SimBin/vehicle/Gear",						xplmType_Int,		0,		"",			""},
	{"SimBin/vehicle/Speed",					xplmType_Float,		0,		"m/s",		"m/s*3.6=KPH, m/s*2.237=MPH"},
	{"SimBin/vehicle/wheels/FL_Wheel_L_Temp",	xplmType_Float,		0,		"deg C",	"Front Left Wheel Left temp"},
	{"SimBin/vehicle/wheels/FL_Wheel_C_Temp",	xplmType_Float,		0,		"deg C",	"Front Left Wheel Center temp"},
	{"SimBin/vehicle/wheels/FL_Wheel_R_Temp",	xplmType_Float,		0,		"deg C",	"Front Left Wheel Right temp"},
	{"SimBin/vehicle/wheels/FR_Wheel_L_Temp",	xplmType_Float,		0,		"deg C",	"Front Right Wheel Left temp"},
	{"SimBin/vehicle/wheels/FR_Wheel_C_Temp",	xplmType_Float,		0,		"deg C",	"Front Right Wheel Center temp"},
	{"SimBin/vehicle/wheels/FR_Wheel_R_Temp",	xplmType_Float,		0,		"deg C",	"Front Right Wheel Right temp"},
	{"SimBin/vehicle/wheels/RL_Wheel_L_Temp",	xplmType_Float,		0,		"deg C",	"Rear Left Wheel Left temp"},
	{"SimBin/vehicle/wheels/RL_Wheel_C_Temp",	xplmType_Float,		0,		"deg C",	"Rear Left Wheel Center temp"},
	{"SimBin/vehicle/wheels/RL_Wheel_R_Temp",	xplmType_Float,		0,		"deg C",	"Rear Left Wheel Right temp"},
	{"SimBin/vehicle/wheels/RR_Wheel_L_Temp",	xplmType_Float,		0,		"deg C",	"Rear Right Wheel Left temp"},
	{"SimBin/vehicle/wheels/RR_Wheel_C_Temp",	xplmType_Float,		0,		"deg C",	"Rear Right Wheel Center temp"},
	{"SimBin/vehicle/wheels/RR_Wheel_R_Temp",	xplmType_Float,		0,		"deg C",	"Rear Right Wheel Right temp"}
};

#endif