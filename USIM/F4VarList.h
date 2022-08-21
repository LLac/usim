// F4VarList.h: interface for the CF4VarList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_F4VARLIST_H__CA36354A_1443_4BC7_ACB7_90FF6AE36F97__INCLUDED_)
#define AFX_F4VARLIST_H__CA36354A_1443_4BC7_ACB7_90FF6AE36F97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _BITS_LIST
{
	LPTSTR		TokenString;
	int			BitMask;
	UINT		BitSimType;
};

#define LIGHTBITS_1		0x0001
#define LIGHTBITS_2		0x0002
#define LIGHTBITS_3		0x0004
#define HSIBITS			0x0008
#define ALLLIGHTBITS	0x000F
#define USERLIGHTBITS_1	0x0010
#define USERLIGHTBITS_2	0x0020
#define USERLIGHTBITS_3	0x0040
#define USERHSIBITS		0x0080
#define ALTBITS			0x0100
#define POWERBITS		0x0200
#define BLINKBITS		0x0400
#define TACANBITS		0x0800
#define MISCBITS		0x1000
#define ALLUSERBITS		0x0FFF

_BITS_LIST const F4BitsArray[] = { 
	{"",													0,								0},
//
	{"Falcon_4/LightBits_1/User_Defined_LightBits_1",		0,								USERLIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/Master_Caution",	F4FlightData::MasterCaution,	LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/TF",					F4FlightData::TF,				LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/OXY_BROW",			F4FlightData::OXY_BROW,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/EQUIP_HOT",			F4FlightData::EQUIP_HOT,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/ONGROUND",		 F4FlightData::ONGROUND,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/ENG_FIRE",			F4FlightData::ENG_FIRE,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/CONFIG",				F4FlightData::CONFIG,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/HYD",				F4FlightData::HYD,				LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/Flcs_ABCD",			F4FlightData::Flcs_ABCD,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/FLCS",				F4FlightData::FLCS,				LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/CAN",				F4FlightData::CAN,				LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Brow_Lights/T_L_CFG",			F4FlightData::T_L_CFG,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/AOA_Indexers/AOAAbove",			F4FlightData::AOAAbove,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/AOA_Indexers/AOAOn",				F4FlightData::AOAOn,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/AOA_Indexers/AOABelow",			F4FlightData::AOABelow,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Refuel-NWS/RefuelRDY",			F4FlightData::RefuelRDY,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Refuel-NWS/RefuelAR",			F4FlightData::RefuelAR,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Refuel-NWS/RefuelDSC",			F4FlightData::RefuelDSC,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/FltControlSys",	F4FlightData::FltControlSys,	LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/LEFlaps",			F4FlightData::LEFlaps,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/EngineFault",		F4FlightData::EngineFault,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/Overheat", 		F4FlightData::Overheat,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/FuelLow", 		F4FlightData::FuelLow,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/Avionics", 		F4FlightData::Avionics,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/RadarAlt", 		F4FlightData::RadarAlt,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/IFF", 			F4FlightData::IFF,				LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/ECM", 			F4FlightData::ECM,				LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/NWSFail", 		F4FlightData::NWSFail,			LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/CabinPress", 		F4FlightData::CabinPress,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/AutoPilotOn",		F4FlightData::AutoPilotOn,		LIGHTBITS_1},
	{"Falcon_4/LightBits_1/Caution_Lights/TFR_STBY",		F4FlightData::TFR_STBY,			LIGHTBITS_1},
//
	{"Falcon_4/LightBits_2/User_Defined_LightBits_2",		0,								USERLIGHTBITS_2},
	{"Falcon_4/LightBits_2/Threat_Warning_Prime/HandOff",	F4FlightData::HandOff,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Threat_Warning_Prime/Launch",	F4FlightData::Launch,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Threat_Warning_Prime/PriMode",	F4FlightData::PriMode,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Threat_Warning_Prime/Naval",		F4FlightData::Naval,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Threat_Warning_Prime/Unk",		F4FlightData::Unk,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Threat_Warning_Prime/TgtSep",	F4FlightData::TgtSep,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/EWS/OF:Go",						F4FlightData::Go,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/EWS/OF:NoGo",					F4FlightData::NoGo,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/EWS/OF:Degr",					F4FlightData::Degr,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/EWS/OF:Rdy",						F4FlightData::Rdy,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/EWS/OF:ChaffLo",					F4FlightData::ChaffLo,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/EWS/OF:FlareLo",					F4FlightData::FlareLo,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Aux_Threat_Warning/AuxSrch",		F4FlightData::AuxSrch,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Aux_Threat_Warning/AuxAct",		F4FlightData::AuxAct,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Aux_Threat_Warning/AuxLow",		F4FlightData::AuxLow,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Aux_Threat_Warning/AuxPwr",		F4FlightData::AuxPwr,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/ECM/EcmPwr",						F4FlightData::EcmPwr,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/ECM/EcmFail",					F4FlightData::EcmFail,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Caution_Lights/FwdFuelLow",		F4FlightData::FwdFuelLow,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Caution_Lights/AftFuelLow",		F4FlightData::AftFuelLow,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/EPUOn",			F4FlightData::EPUOn,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/JFSOn",			F4FlightData::JFSOn,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/SEC",				F4FlightData::SEC,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/OXY_LOW",			F4FlightData::OXY_LOW,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/PROBEHEAT",		F4FlightData::PROBEHEAT,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/SEAT_ARM",			F4FlightData::SEAT_ARM,			LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/BUC",				F4FlightData::BUC,				LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/FUEL_OIL_HOT",		F4FlightData::FUEL_OIL_HOT,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/ANTI_SKID",		F4FlightData::ANTI_SKID,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/TFR_ENGAGED",		F4FlightData::TFR_ENGAGED,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/GEARHANDLE",		F4FlightData::GEARHANDLE,		LIGHTBITS_2},
	{"Falcon_4/LightBits_2/Engine_lights/ENGINE",			F4FlightData::ENGINE,			LIGHTBITS_2},
//
	{"Falcon_4/LightBits_3/User_Defined_LightBits_3",		0,								USERLIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/FlcsPmg",				F4FlightData::FlcsPmg,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/MainGen",				F4FlightData::MainGen,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/StbyGen",				F4FlightData::StbyGen,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/EpuGen",				F4FlightData::EpuGen,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/EpuPmg",				F4FlightData::EpuPmg,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/ToFlcs",				F4FlightData::ToFlcs,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/FlcsRly",				F4FlightData::FlcsRly,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/BatFail",				F4FlightData::BatFail,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/Hydrazine",			F4FlightData::Hydrazine,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/Air",					F4FlightData::Air,				LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/Elec_Fault",			F4FlightData::Elec_Fault,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/Lef_Fault",			F4FlightData::Lef_Fault,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/OnGround",			F4FlightData::OnGround,			LIGHTBITS_3},

	{"Falcon_4/LightBits_3/Electrical/FlcsBitRun",			F4FlightData::FlcsBitRun,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/FlcsBitFail",			F4FlightData::FlcsBitFail,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/FlcsBitFail",			F4FlightData::FlcsBitFail,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/NoseGearDown",		F4FlightData::NoseGearDown,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/LeftGearDown",		F4FlightData::LeftGearDown,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/RightGearDown",		F4FlightData::RightGearDown,	LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/ParkBrakeOn",			F4FlightData::ParkBrakeOn,		LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Electrical/Power_Off",			F4FlightData::Power_Off,		LIGHTBITS_3},

	{"Falcon_4/LightBits_3/Caution Panel/cadc",				F4FlightData::cadc,				LIGHTBITS_3},
	{"Falcon_4/LightBits_3/Caution Panel/Inlet_Icing",		F4FlightData::Inlet_Icing,		LIGHTBITS_3},

	{"Falcon_4/LightBits_3/Left Aux Console/SpeedBrake",	F4FlightData::SpeedBrake,		LIGHTBITS_3},

	{"Falcon_4/LightBits_3/Threat Warning Prime/SysTest",	F4FlightData::SysTest,			LIGHTBITS_3},

	{"Falcon_4/LightBits_3/Master Caution/MCAnnounced",		F4FlightData::MCAnnounced,		LIGHTBITS_3},

	{"Falcon_4/LightBits_3/MLG/MLGWOW",						F4FlightData::MLGWOW,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/MLG/NLGWOW",						F4FlightData::NLGWOW,			LIGHTBITS_3},
	{"Falcon_4/LightBits_3/MLG/ATF_Not_Engaged",			F4FlightData::ATF_Not_Engaged,	LIGHTBITS_3},
//
	{"Falcon_4/HSI_Bits/HSI/User_Defined_HSIBits",			0,								USERHSIBITS},
	{"Falcon_4/HSI_Bits/HSI/To_True",						F4FlightData::ToTrue,			HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/Ils_Warning",					F4FlightData::IlsWarning,		HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/Course_Warning",				F4FlightData::CourseWarning,	HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/Init",							F4FlightData::Init,				HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/Total_Flags",					F4FlightData::TotalFlags,		HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/ADI_OFF",						F4FlightData::ADI_OFF,			HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/ADI_AUX",						F4FlightData::ADI_AUX,			HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/ADI_GS",						F4FlightData::ADI_GS,			HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/ADI_LOC",						F4FlightData::ADI_LOC,			HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/HSI_OFF",						F4FlightData::HSI_OFF,			HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/BUP_ADI_OFF",					F4FlightData::BUP_ADI_OFF,		HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/VVI",							F4FlightData::VVI,				HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/AOA",							F4FlightData::AOA,				HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/AVTR",							F4FlightData::AVTR,				HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/OuterMarker",					F4FlightData::OuterMarker,		HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/MiddleMarker",					F4FlightData::MiddleMarker,		HSIBITS},
	{"Falcon_4/HSI_Bits/HSI/FromTrue",						F4FlightData::FromTrue,			HSIBITS},
// NEW SECTION SM 2
	{ "Falcon_4/TACAN_Bits/Band",							F4FlightData::band,				TACANBITS },
	{ "Falcon_4/TACAN_Bits/Mode",							F4FlightData::mode,				TACANBITS },

	{ "Falcon_4/Alt_Bits/Cal_Type",							F4FlightData::CalType,			ALTBITS },
	{ "Falcon_4/Alt_Bits/Pneu_Flag",						F4FlightData::PneuFlag,			ALTBITS },

	{"Falcon_4/Power_Bits/Bus_Power_Battery",				F4FlightData::BusPowerBattery,		POWERBITS},
	{"Falcon_4/Power_Bits/Bus_Power_Emergency",				F4FlightData::BusPowerEmergency,	POWERBITS},
	{"Falcon_4/Power_Bits/Bus_Power_Essential",				F4FlightData::BusPowerEssential,	POWERBITS},
	{"Falcon_4/Power_Bits/Bus_Power_Non_Essential",			F4FlightData::BusPowerNonEssential,	POWERBITS},
	{"Falcon_4/Power_Bits/Main_Generator",					F4FlightData::MainGenerator,		POWERBITS},
	{"Falcon_4/Power_Bits/Standby_Generator",				F4FlightData::StandbyGenerator,		POWERBITS},
	{"Falcon_4/Power_Bits/Jet_Fuel_Starter",				F4FlightData::JetFuelStarter,		POWERBITS},

	{"Falcon_4/Blink_Bits/Outer_Marker",					F4FlightData::BOuterMarker,		BLINKBITS},
	{"Falcon_4/Blink_Bits/Middle_Marker",					F4FlightData::BMiddleMarker,	BLINKBITS},
	{"Falcon_4/Blink_Bits/PROBE_HEAT",						F4FlightData::BPROBEHEAT,		BLINKBITS},
	{"Falcon_4/Blink_Bits/Aux_Srch",						F4FlightData::BAuxSrch,			BLINKBITS},
	{"Falcon_4/Blink_Bits/Launch",							F4FlightData::BLaunch,			BLINKBITS},
	{"Falcon_4/Blink_Bits/Pri_Mode",						F4FlightData::BPriMode,			BLINKBITS},
	{"Falcon_4/Blink_Bits/Unk",								F4FlightData::BUnk,				BLINKBITS},
	{"Falcon_4/Blink_Bits/Elec_Fault",						F4FlightData::BElec_Fault,		BLINKBITS},
	{"Falcon_4/Blink_Bits/OXY_BROW",						F4FlightData::BOXY_BROW,		BLINKBITS},
	{"Falcon_4/Blink_Bits/EPU_On",							F4FlightData::BEPUOn,			BLINKBITS},
	{"Falcon_4/Blink_Bits/JFS_On_Slow",						F4FlightData::JFSOn_Slow,		BLINKBITS},
	{"Falcon_4/Blink_Bits/JFS_On_Fast",						F4FlightData::JFSOn_Fast,		BLINKBITS},

	{"Falcon_4/Misc_Bits/RALT_Valid",						F4FlightData::RALT_Valid,		MISCBITS},
	{"Falcon_4/Misc_Bits/Flcs_Flcc_A",						F4FlightData::Flcs_Flcc_A,		MISCBITS},
	{"Falcon_4/Misc_Bits/Flcs_Flcc_B",						F4FlightData::Flcs_Flcc_B,		MISCBITS},
	{"Falcon_4/Misc_Bits/Flcs_Flcc_C",						F4FlightData::Flcs_Flcc_C,		MISCBITS},
	{"Falcon_4/Misc_Bits/Flcs_Flcc_D",						F4FlightData::Flcs_Flcc_D,		MISCBITS},
	{"Falcon_4/Misc_Bits/Solenoid_Status",					F4FlightData::SolenoidStatus,	MISCBITS}
};

struct _F4DATA_MEM
{
	LPTSTR		Name;
	CHAR		VarType;
	LPTSTR		VarTokenName;
	LPTSTR		Units;
	LPTSTR		Description;
};

_F4DATA_MEM const F4DataArray[] = {
	{"NONE",						xplmType_Int,	"",													"",			""},
	{"x",							xplmType_Float, "Falcon_4/flightmodel/x",							"ft",		"Ownship North (Ft)"},
	{"y",							xplmType_Float, "Falcon_4/flightmodel/y",							"ft",		"Ownship East (Ft)"},
	{"z",							xplmType_Float, "Falcon_4/flightmodel/z",							"ft",		"Ownship Down (Ft)"},
	{"xDot",						xplmType_Float, "Falcon_4/flightmodel/xDot",						"ft/sec",	"Ownship North Rate (ft/sec)"},
	{"yDot",						xplmType_Float, "Falcon_4/flightmodel/yDot",						"ft/sec",	"Ownship East Rate (ft/sec)"},
	{"zDot",						xplmType_Float, "Falcon_4/flightmodel/zDot",						"ft/sec",	"Ownship Down Rate (ft/sec)"},
	{"AOA",							xplmType_Float, "Falcon_4/flightmodel/AOA",							"deg",		"Ownship AOA (Degrees)"},
	{"Beta",						xplmType_Float, "Falcon_4/flightmodel/Beta",						"deg",		"Ownship Beta (Degrees)"},
	{"Gamma",						xplmType_Float, "Falcon_4/flightmodel/Gamma",						"radians",	"Ownship Gamma (Radians)"},
	{"Pitch",						xplmType_Float, "Falcon_4/flightmodel/Pitch",						"radians",	"Ownship Pitch (Radians)"},
	{"Roll",						xplmType_Float, "Falcon_4/flightmodel/Roll",						"radians",	"Ownship Roll (Radians)"},
	{"Yaw",							xplmType_Float, "Falcon_4/flightmodel/Yaw",							"radians",	"Ownship Yaw (Radians)"},
	{"Mach Number",					xplmType_Float, "Falcon_4/flightmodel/Mach_Number",					"mach",		"Ownship Mach number"},
	{"Airspeed Indicated",			xplmType_Float, "Falcon_4/flightmodel/Airspeed_Indicated",			"knots",	"Ownship Indicated Airspeed (Knots)"},
	{"Airspeed True",				xplmType_Float, "Falcon_4/flightmodel/Airspeed_True",				"ft/sec",	"Ownship True Airspeed (Ft/Sec)"},
	{"Normal Gs",					xplmType_Float, "Falcon_4/flightmodel/Normal_Gs",					"Gs",		"Ownship Normal Gs"},
	{"Wind Offset",					xplmType_Float, "Falcon_4/flightmodel/Wind_Offset",					"radians",	"Wind delta to FPM (Radians)"},
	{"AAUZ",						xplmType_Float,	"Falcon_4/flightmodel/AAUZ",						"",			"Ownship barometric altitude given by AAU"},
	{"Alt Cal Reading",				xplmType_Int,	"Falcon_4/flightmodel/Alt_Cal_Reading",				"",			"Barometric altitude calibration (depends on CalType)"},

	{"Nozzle Position",				xplmType_Float, "Falcon_4/aircraft/Nozzle_Position",				"%",		"Ownship engine nozzle percent open (0-100)"},
	{"Nozzle Position 2",			xplmType_Float, "Falcon_4/aircraft/Nozzle_Position_2",				"%",		"Ownship engine nozzle 2 percent open (0-100) multi-engine"},
	{"LOX",							xplmType_Float, "Falcon_4/aircraft/LOX",							"l",		"Liquid Oxygen (Liters 0-5)"},
	{"Fuel Internal",				xplmType_Float, "Falcon_4/aircraft/Fuel_Internal",					"lbs",		"Ownship internal fuel (Lbs)"},
	{"Fuel External",				xplmType_Float, "Falcon_4/aircraft/Fuel_External",					"lbs",		"Ownship external fuel (Lbs)"},
	{"Fuel Flow",					xplmType_Float, "Falcon_4/aircraft/Fuel_Flow",						"lbs/hr",	"Ownship fuel flow (Lbs/Hour)"},
	{"Fuel Flow 2",					xplmType_Float, "Falcon_4/aircraft/Fuel_Flow_2",					"lbs/hr",	"Ownship fuel flow (Lbs/Hour)"},
	{"Engine RPM",					xplmType_Float, "Falcon_4/aircraft/Engine_RPM",						"%",		"wnship engine rpm (Percent 0-103)"},
	{"Engine RPM 2",				xplmType_Float, "Falcon_4/aircraft/Engine_RPM_2",					"%",		"wnship engine rpm2 (Percent 0-103) multi-engine"},
	{"Fwd Turbine Inlet Temp",		xplmType_Float, "Falcon_4/aircraft/Fwd_Turbine_Inlet_Temp",			"deg C",	"Ownship Forward Turbine Inlet Temp (Degrees C)"},
	{"Fwd Turbine Inlet Temp 2",	xplmType_Float, "Falcon_4/aircraft/Fwd_Turbine_Inlet_Temp_2",		"deg C",	"Ownship Forward Turbine Inlet Temp 2 (Degrees C) multi-engine"},
	{"Gear Position",				xplmType_Float, "Falcon_4/aircraft/Gear_Position",					"0-1",		"Ownship Gear position 0 = up, 1 = down"},
	{"Speed Brake",					xplmType_Float, "Falcon_4/aircraft/Speed_Brake",					"0-1",		"Ownship speed brake position 0 = closed, 1 = 60 Degrees open"},
	{"EPU Fuel",					xplmType_Float, "Falcon_4/aircraft/EPU_Fuel",						"%",		"Ownship EPU fuel (Percent 0-100)"},
	{"Oil Pressure",				xplmType_Float, "Falcon_4/aircraft/Oil_Pressure",					"%",		"Ownship Oil Pressure (Percent 0-100)"},
	{"Oil Pressure 2",				xplmType_Float, "Falcon_4/aircraft/Oil_Pressure_2",					"%",		"Ownship Oil Pressure2 (Percent 0-100) multi-engine"},
	{"Fuel - FWD",					xplmType_Float,	"Falcon_4/aircraft/Fuel-FWD",						"",			"Fuel - FWD"},
	{"Fuel - AFT",					xplmType_Float,	"Falcon_4/aircraft/Fuel-AFT",						"",			"Fuel - AFT"},
	{"Fuel - Total",				xplmType_Float,	"Falcon_4/aircraft/Fuel-Total",						"",			"Fuel - Total"},
	{"Cabin Alt",					xplmType_Float,	"Falcon_4/aircraft/Cabin_ALT",						"ft",		"Ownship Cabin Pressure (0-50000) FT"},
	{"Hyd Pressure A",				xplmType_Float,	"Falcon_4/aircraft/Hyd_Pressure_A",					"psi",		"Ownship HYD Pressure (0-4000) PSI"},
	{"Hyd Pressure B",				xplmType_Float,	"Falcon_4/aircraft/Hyd_Pressure_B",					"psi",		"Ownship HYD Pressure (0-4000) PSI"},
	{"Lef Pos",						xplmType_Float, "Falcon_4/aircraft/Lef_Pos",						"",			"Ownship LEF position"},
	{"Tef Pos",						xplmType_Float, "Falcon_4/aircraft/Tef_Pos",						"",			"Ownship TEF position"},
	{"VTOL Pos",					xplmType_Float, "Falcon_4/aircraft/VTOL_Pos",						"",			"Ownship VTOL exhaust angle"},

	{"Trim Pitch",					xplmType_Float, "Falcon_4/controls/Trim_Pitch",						"-0.5/+0.5","Value of trim in pitch axis, -0.5 to +0.5"},
	{"Trim Roll",					xplmType_Float, "Falcon_4/controls/Trim_Roll",						"-0.5/+0.5","Value of trim in roll axis, -0.5 to +0.5"},
	{"Trim Yaw",					xplmType_Float, "Falcon_4/controls/Trim_Yaw",						"-0.5/+0.5","Value of trim in yaw axis, -0.5 to +0.5"},

	{"Count Chaff",					xplmType_Float, "Falcon_4/cockpit/misc/Count_Chaff",				"",			"Number of Chaff remaining"},
	{"Count Flare",					xplmType_Float, "Falcon_4/cockpit/misc/Count_Flare",				"",			"Number of Flare remaining"},
	{"Gear Nose Pos",				xplmType_Float, "Falcon_4/cockpit/misc/Gear_Nose_Pos",				"",			"Position of the nose landinggear"},
	{"Gear Left Pos",				xplmType_Float, "Falcon_4/cockpit/misc/Gear_Left_Pos",				"",			"Position of the left landinggear"},
	{"Gear Right Pos",				xplmType_Float, "Falcon_4/cockpit/misc/Gear_Right_Pos",				"",			"Position of the right landinggear"},
	{"Cmds Mode",					xplmType_Int,	"Falcon_4/cockpit/misc/Cmds_Mode",					"",			"Ownship CMDS mode stat"},

	{"ADI - ILS Hor Pos",			xplmType_Float, "Falcon_4/cockpit/ADI/ADI-ILS_Hor_Pos",				"",			"ADI - ILS Hor Pos"},
	{"ADI - ILS Ver Pos",			xplmType_Float, "Falcon_4/cockpit/ADI/ADI-ILS_Ver_Pos",				"",			"ADI - ILS Ver Pos"},
	
	{"HSI - STA CRS State",			xplmType_Int,	"Falcon_4/cockpit/HSI/HSI-STA_CRS_State",			"",			"HSI - STA CRS State"},
	{"HSI - STA HDG State",			xplmType_Int,	"Falcon_4/cockpit/HSI/HSI-STA_HDG_State",			"",			"HSI - STA HDG State"},
	{"HSI - STA Total States",		xplmType_Int,	"Falcon_4/cockpit/HSI/HSI-STA_Total_States",		"",			"HSI - STA Total States"},
	{"HSI - Course Deviation",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Course_Deviation",		"",			"HSI - Course Deviation"},
	{"HSI - Desired Course",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Desired_Course",			"",			"HSI - Desired Course"},
	{"HSI - Distance To Beacon",	xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Distance_To_Beacon",		"",			"HSI - Distance To Beacon"},
	{"HSI - Bearing To Beacon",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Bearing_To_Beacon",		"",			"HSI - Bearing To Beacon"},
	{"HSI - Current Heading",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Current_Heading",			"",			"HSI - Current Heading"},
	{"HSI - Desired Heading",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Desired_Heading",			"",			"HSI - Desired Heading"},
	{"HSI - Deviation Limit",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Deviation_Limit",			"",			"HSI - Deviation Limit"},
	{"HSI - Half Deviation Limit",	xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Half_Deviation_Limit",	"",			"HSI - Half Deviation Limit"},
	{"HSI - Localizer Course",		xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Localizer_Course",		"",			"HSI - Localizer Course"},
	{"HSI - Airbase X",				xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Airbase_X",				"",			"HSI - Airbase X"},
	{"HSI - Airbase Y",				xplmType_Float, "Falcon_4/cockpit/HSI/HSI-Airbase_Y",				"",			"HSI - Airbase Y"},
	{"HSI - Total Values",			xplmType_Float,	"Falcon_4/cockpit/HSI/HSI-Total_Values",			"",			"HSI - Total Values"},
	{"HSI - Flags Bits",			xplmType_IntHex,"Falcon_4/cockpit/HSI/HSI-Flags_Bits",				"",			"HSI - Flags Bits"},
	{"HSI - Nav Mode",				xplmType_Byte,	"Falcon_4/cockpit/HSI/Nav_Mode",					"",			"Current mode selected for HSI"},
	
	{"Tacan Channel UFC",			xplmType_Int,	"Falcon_4/cockpit/radios/Tacan_Channel_UFC",		"",			"Tacan Channel UFC"},
	{"Tacan Channel AUX",			xplmType_Int,	"Falcon_4/cockpit/radios/Tacan_Channel_AUX",		"",			"Tacan Channel AUX"},
	{"Tacan Info 1",				xplmType_Byte,	"Falcon_4/cockpit/radios/Tacan_Info_1",				"",			"Tacan band/mode settings for UFC and AUX COMM"},
	{"Tacan Info 2",				xplmType_Byte,	"Falcon_4/cockpit/radios/Tacan_Info_2",				"",			"Tacan band/mode settings for UFC and AUX COMM"},
	{"Bup UHF Preset",				xplmType_Int,	"Falcon_4/cockpit/radios/Bup_UHF_Preset",			"",			"BUP UHF channel preset"},
	{"Bup UHF Frequency",			xplmType_Int,	"Falcon_4/cockpit/radios/Bup_UHF_Freq",				"",			"BUP UHF channel frequency"},
	
	{"Main Power",					xplmType_Int,	"Falcon_4/cockpit/electrical/Main_Power",			"",			"Main Power"},
	{"Light Bits 1",				xplmType_IntHex,"Falcon_4/cockpit/electrical/Light_Bits_1",			"",			"Panel light bits"},
	{"Light Bits 2",				xplmType_IntHex,"Falcon_4/cockpit/electrical/Light_Bits_2",			"",			"Caution / Warning light bits"},
	{"Light Bits 3",				xplmType_IntHex,"Falcon_4/cockpit/electrical/Light_Bits_3",			"",			"Electrical / EPU / Caution panel bits"},
	{"Parking Brake",				xplmType_Byte,	"Falcon_4/cockpit/electrical/Parking_Brake",		"",			"Parking Brake magnetic switch"},
	{"Alt Bits",					xplmType_IntHex,"Falcon_4/cockpit/electrical/Alt_Bits",				"",			"Various altimeter bits"},
	{"Power Bits",					xplmType_IntHex,"Falcon_4/cockpit/electrical/Power_Bits",			"",			"Various Power bits"},
	{"Blink Bits",					xplmType_IntHex,"Falcon_4/cockpit/electrical/Blink_Bits",			"",			"Cockpit indicator lights blink status"},
	{"Misc Bits",					xplmType_IntHex,"Falcon_4/cockpit/electrical/Misc_Bits",			"",			"Various status flags"},
	
	{"DED Line 1",					xplmType_Char,	"Falcon_4/cockpit/displays/DED_Line_1",				"",			"DED Line 1"},
	{"DED Line 2",					xplmType_Char,	"Falcon_4/cockpit/displays/DED_Line_2",				"",			"DED Line 2"},
	{"DED Line 3",					xplmType_Char,	"Falcon_4/cockpit/displays/DED_Line_3",				"",			"DED Line 3"},
	{"DED Line 4",					xplmType_Char,	"Falcon_4/cockpit/displays/DED_Line_4",				"",			"DED Line 4"},
	{"DED Line 5",					xplmType_Char,	"Falcon_4/cockpit/displays/DED_Line_5",				"",			"DED Line 5"},
	{"PFL Line 1",					xplmType_Char,	"Falcon_4/cockpit/displays/PFL_Line_1",				"",			"PFL Line 1"},
	{"PFL Line 2",					xplmType_Char,	"Falcon_4/cockpit/displays/PFL_Line_2",				"",			"PFL Line 2"},
	{"PFL Line 3",					xplmType_Char,	"Falcon_4/cockpit/displays/PFL_Line_3",				"",			"PFL Line 3"},
	{"PFL Line 4",					xplmType_Char,	"Falcon_4/cockpit/displays/PFL_Line_4",				"",			"PFL Line 4"},
	{"PFL Line 5",					xplmType_Char,	"Falcon_4/cockpit/displays/PFL_Line_5",				"",			"PFL Line 5"},
	{"OSRAM Display O1",			xplmType_Char,	"Falcon_4/cockpit/displays/OSRAM_Display_O1",		"",			"OSRAM Display O1"},
	{"OSRAM Display O2",			xplmType_Char,	"Falcon_4/cockpit/displays/OSRAM_Display_O2",		"",			"OSRAM Display O2"},
	{"OSRAM Display O3 - Chaff",	xplmType_Char,	"Falcon_4/cockpit/displays/OSRAM_Display_O3-Chaff",	"",			"OSRAM Display O3 - Chaff"},
	{"OSRAM Display O4 - Flare",	xplmType_Char,	"Falcon_4/cockpit/displays/OSRAM_Display_O4-Flare",	"",			"OSRAM Display O4 - Flare"},

	{"Current Time",				xplmType_Int,	"Falcon_4/simulation/Current_Time",					"sec",		"Ownship ACD index number, i.e. which aircraft type are we flying."},
	{"Vehicle ACD",					xplmType_Int,	"Falcon_4/simulation/Vehicle_ACD",					"",			"Cockpit indicator lights blink status"}
};

#endif // !defined(AFX_F4VARLIST_H__CA36354A_1443_4BC7_ACB7_90FF6AE36F97__INCLUDED_)
