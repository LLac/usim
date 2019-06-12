#ifndef _F4FLIGHT_DATA_H
#define _F4FLIGHT_DATA_H

//To retain compatibility with previous versions, in case this definitions
//change, always increase version number and add new fields to the end of
//the existing structure.
#define RWRINFO_SIZE 512
#define CALLSIGN_LEN 12
#define MAX_CALLSIGNS 32

class F4FlightData
{
public:
    enum LightBits
    {
         MasterCaution = 0x1,

         // Brow Lights
         TF        = 0x2,
         OBS       = 0x4,
         ALT       = 0x8,
		 WOW       = 0x10,  // MD -- NB: this is not a lamp bit!
         ENG_FIRE  = 0x20,
		 CONFIG	   = 0x40,
         HYD       = 0x80,
         OIL       = 0x100,
         DUAL      = 0x200,
         CAN       = 0x400,
         T_L_CFG   = 0x800,

         // AOA Indexers
         AOAAbove  = 0x1000,
         AOAOn     = 0x2000,
         AOABelow  = 0x4000,

         // Refuel/NWS
         RefuelRDY = 0x8000,
         RefuelAR  = 0x10000,
         RefuelDSC = 0x20000,

         // Caution Lights
         FltControlSys = 0x40000,
         LEFlaps       = 0x80000,
         EngineFault   = 0x100000,
         Overheat      = 0x200000,
         FuelLow       = 0x400000,
         Avionics      = 0x800000,
         RadarAlt      = 0x1000000,
         IFF           = 0x2000000,
         ECM           = 0x4000000,
         Hook          = 0x8000000,
         NWSFail       = 0x10000000,
         CabinPress    = 0x20000000,
 		 AutoPilotOn   = 0x40000000,  // MD -- 20031125: is AP on??  NB: This is not a lamp bit!
		 TFR_STBY      = 0x80000000,

		 // MD 20031016: Use this with the MAL/IND light code in cautions.cpp
		 // please update this is you add/change bits!
		 AllLampBitsOn     = 0xBFFFFFEF
    };

	enum LightBits2
    {
		// Threat Warning Prime
		HandOff      =       0x1,
		Launch       =	      0x2,
		PriMode      =       0x4,
		Naval        =       0x8,
		Unk          =      0x10,
		TgtSep       =      0x20,

		// EWS
		Go	= 0x40,	    // On and operating normally
		NoGo    = 0x80,     // On but malfunction present
		Degr    = 0x100,    // Status message: AUTO DEGR
		Rdy     = 0x200,    // Status message: DISPENSE RDY
		ChaffLo = 0x400,    // Bingo chaff quantity reached
		FlareLo = 0x800,    // Bingo flare quantity reached

		// Aux Threat Warning
		AuxSrch      =    0x1000,
		AuxAct       =    0x2000,
		AuxLow       =    0x4000,
		AuxPwr       =    0x8000,

		// ECM
		EcmPwr       =   0x10000,
		EcmFail      =   0x20000,

		// Caution Lights
		FwdFuelLow   =   0x40000,
		AftFuelLow   =   0x80000,

		//Engine lights
		EPUOn		  =	  0x100000,
		JFSOn		  =	  0x200000,

		// Caution panel
		SEC		  =	  0x400000,
		OXY_LOW	  =   0x800000,
		PROBEHEAT	  =   0x1000000,
		SEAT_ARM	  =   0x2000000,
		BUC		  =   0x4000000,
		FUEL_OIL_HOT =   0x8000000,
		ANTI_SKID	  =   0x10000000,
		TFR_ENGAGED  =   0x20000000,
		GEARHANDLE	  =	  0x40000000,
		ENGINE       =   0x80000000,  // lower half of glareshield ENG FIRE/ENGINE lamp

		// MD 20031016: Use this with the MAL/IND light code in cautions.cpp
		// please update this is you add/change bits!
		AllLampBits2On     = 0xFFFFFEFF
	};

	enum LightBits3
	{
		// Elec panel
		FlcsPmg		=	0x1,
		MainGen		=	0x2,
		StbyGen		=	0x4,
		EpuGen		=	0x8,
		EpuPmg		=	0x10,
		ToFlcs		=	0x20,
		FlcsRly		=	0x40,
		BatFail		=	0x80,

		// EPU panel
		Hydrazine	=	0x100,
		Air			=	0x200,

		// Caution panel
		Elec_Fault	=	0x400,
		Lef_Fault	=	0x800,
		OnGround	=	0x1000,
		Power_Off =   0x1000,   // Set if there is no electrical power
		Eng2_Fire	=	0x2000,//TJL 01/24/04 Multi-engine
		Lock		=	0x4000,//TJL 01/24/04 Lock Light Cue
		Shoot		=	0x8000,//TJL 01/24/04 Shoot light cue
		NoseGearDown  =   0x10000,  // MD -- 20040301: on means down and locked
		LeftGearDown  =   0x20000,  // MD -- 20040301: on means down and locked
		RightGearDown =   0x40000,  // MD -- 20040301: on means down and locked
		ParkBrakeOn   =	0x100000, // Parking brake engaged; NOTE: not a lamp bit
		PowerOffFlag     = 0x200000, // Set if there is no electrical power.  NB: not a lamp bit

		// Caution panel
		cadc	= 0x400000,

		// MD 20031016: Use this with the MAL/IND light code in cautions.cpp
		// please update this is you add/change bits!
		AllLampBits3On     = 0x0047EFFF
	};

	enum HsiBits
	{
		ToTrue        = 0x01,	// HSI_FLAG_TO_TRUE
		IlsWarning    = 0x02,	// HSI_FLAG_ILS_WARN
		CourseWarning = 0x04,	// HSI_FLAG_CRS_WARN
		Init          = 0x08,	// HSI_FLAG_INIT
		TotalFlags    = 0x10,	// HSI_FLAG_TOTAL_FLAGS ???
		ADI_OFF		  = 0x20,	// ADI OFF Flag
		ADI_AUX		  = 0x40,	// ADI AUX Flag
		ADI_GS		  = 0x80,	// ADI GS FLAG
		ADI_LOC		  = 0x100,	// ADI LOC FLAG
		HSI_OFF		  = 0x200,	// HSI OFF Flag
		BUP_ADI_OFF	  = 0x400,	// Backup ADI Off Flag
		VVI			  = 0x800,	// VVI OFF Flag
		AOA			  = 0x1000,	// AOA OFF Flag
		AVTR		  = 0x2000,	// AVTR Light
		OuterMarker   = 0x4000,  // MARKER beacon light for outer marker
		MiddleMarker  = 0x8000,  // MARKER beacon light for middle marker
		FromTrue      = 0x10000, // HSI_FLAG_TO_TRUE == 2, FROM

		 // MD 20031016: Use this with the MAL/IND light code in cautions.cpp
		 // please update this is you add/change bits!
		AllLampHsiBitsOn = 0xE000
	};

	float NONE;         // NULL
	// These are outputs from the sim
	float x;            // Ownship North (Ft)
	float y;            // Ownship East (Ft)
	float z;            // Ownship Down (Ft)
	float xDot;         // Ownship North Rate (ft/sec)
	float yDot;         // Ownship East Rate (ft/sec)
	float zDot;         // Ownship Down Rate (ft/sec)
	float alpha;        // Ownship AOA (Degrees)
	float beta;         // Ownship Beta (Degrees)
	float gamma;        // Ownship Gamma (Radians)
	float pitch;        // Ownship Pitch (Radians)
	float roll;         // Ownship Pitch (Radians)
	float yaw;          // Ownship Pitch (Radians)
	float mach;         // Ownship Mach number
	float kias;         // Ownship Indicated Airspeed (Knots)
	float vt;           // Ownship True Airspeed (Ft/Sec)
	float gs;           // Ownship Normal Gs
	float windOffset;   // Wind delta to FPM (Radians)
	float nozzlePos;    // Ownship engine nozzle percent open (0-100)
	float nozzlePos2;	// Ownship engine nozzle2 percent open (0-100) //TJL 01/14/04 Multi-engine
	float internalFuel; // Ownship internal fuel (Lbs)
	float externalFuel; // Ownship external fuel (Lbs)
	float fuelFlow;     // Ownship fuel flow (Lbs/Hour)
	float rpm;          // Ownship engine rpm (Percent 0-103)
	float rpm2;         // Ownship engine rpm2 (Percent 0-103) //TJL 01/14/04 multi-engine
	float ftit;         // Ownship Forward Turbine Inlet Temp (Degrees C)
	float ftit2;        // Ownship Forward Turbine Inlet Temp2 (Degrees C)//TJL 01/14/04 multi-engine
	float gearPos;      // Ownship Gear position 0 = up, 1 = down;
	float speedBrake;   // Ownship speed brake position 0 = closed, 1 = 60 Degrees open
	float epuFuel;      // Ownship EPU fuel (Percent 0-100)
	float oilPressure;  // Ownship Oil Pressure (Percent 0-100)
	float oilPressure2; // Ownship Oil Pressure2 (Percent 0-100)//TJL 01/14/04 Multi-engine
	int   lightBits;    // Cockpit Indicator Lights, one bit per bulb. See enum

	// These are inputs. Use them carefully
	float headPitch;    // Head pitch offset from design eye (radians)
	float headRoll;     // Head roll offset from design eye (radians)
	float headYaw;      // Head yaw offset from design eye (radians)

	// new lights
	int   lightBits2;         // Cockpit Indicator Lights, one bit per bulb. See enum
	int   lightBits3;         // Cockpit Indicator Lights, one bit per bulb. See enum

	// chaff/flare
	float ChaffCount;	       // Number of Chaff left
	float FlareCount;         // Number of Flare left

	// landing gear
	float NoseGearPos;        // Position of the nose landinggear
	float LeftGearPos;        // Position of the left landinggear
	float RightGearPos;       // Position of the right landinggear

	// ADI values
	float AdiIlsHorPos;       // Position of horizontal ILS bar
	float AdiIlsVerPos;       // Position of vertical ILS bar

	// HSI states
	int courseState;          // HSI_STA_CRS_STATE
	int headingState;         // HSI_STA_HDG_STATE
	int totalStates;          // HSI_STA_TOTAL_STATES ???

	// HSI values
	float courseDeviation;		// HSI_VAL_CRS_DEVIATION
	float desiredCourse;		// HSI_VAL_DESIRED_CRS
	float distanceToBeacon;		// HSI_VAL_DISTANCE_TO_BEACON
	float bearingToBeacon;		// HSI_VAL_BEARING_TO_BEACON
	float currentHeading;		// HSI_VAL_CURRENT_HEADING
	float desiredHeading;		// HSI_VAL_DESIRED_HEADING
	float deviationLimit;		// HSI_VAL_DEV_LIMIT
	float halfDeviationLimit;	// HSI_VAL_HALF_DEV_LIMIT
	float localizerCourse;		// HSI_VAL_LOCALIZER_CRS
	float airbaseX;				// HSI_VAL_AIRBASE_X
	float airbaseY;				// HSI_VAL_AIRBASE_Y
	float totalValues;			// HSI_VAL_TOTAL_VALUES  ???

	float TrimPitch;		// Value of trim in pitch axis, -0.5 to +0.5
	float TrimRoll;			// Value of trim in roll axis, -0.5 to +0.5
	float TrimYaw;			// Value of trim in yaw axis, -0.5 to +0.5

	// HSI flags
	int hsiBits;              // HSI flags

	//DED Lines
	char DEDLines[5][26];	//25 usable chars
	char Invert[5][26];	//25 usable chars

	//PFL Lines
	char PFLLines[5][26];	 //25 usable chars
	char PFLInvert[5][26]; //25 usable chars

	//TacanChannel
	int UFCTChan, AUXTChan;

	// RWR
	int           RwrObjectCount;
	int           RWRsymbol[40];
	float         bearing[40];
	unsigned long missileActivity[40];
	unsigned long missileLaunch[40];
	unsigned long selected[40];
	float         lethality[40];
	unsigned long newDetection[40];

	//fuel values
	float fwd, aft, total;

	int VersionNum;	//Version of Memarea

	// These are inputs. Use them carefully
	float headX;        // Head X offset from design eye (feet)
	float headY;        // Head Y offset from design eye (feet)
	float headZ;        // Head Z offset from design eye (feet)

	int MainPower;

	// Custom VARs
	float hydA;				// Ownship HYD Pressure (0-4000) PSI
	float hydB;				// Ownship HYD Pressure (0-4000) PSI
	float CabinPressure;	// Ownship Cabin Pressure (0-50000) FT
	char PRKMag;			// Parking Brake magnetic switch
	char O1[5];				// OSRAM Display O1
	char O2[5];				// OSRAM Display O2
	char O3Chaff[5];		// OSRAM Display O3
	char O4Flare[5];		// OSRAM Display O4
	float USIMnozzlePos;    // Ownship engine nozzle percent open (0-100)
	float LOX;				// Ownship Liquid Oxygen liters (0-5)

	// New section SM 2

	// TACAN
	enum TacanSources
	{
		UFC = 0,
		AUX = 1,
		NUMBER_OF_SOURCES = 2,
	};
	enum TacanBits
	{
		band = 0x01,   // true in this bit position if band is X
		mode = 0x02,   // true in this bit position if domain is air to air
	};

	// ALTIMETER
	enum AltBits
	{
		CalType  = 0x01,	// true if calibration in inches of Mercury (Hg), false if in hectoPascal (hPa)
		PneuFlag = 0x02,	// true if PNEU flag is visible
	};

	// POWER
	enum PowerBits
	{
		BusPowerBattery      = 0x01,	// true if at least the battery bus is powered
		BusPowerEmergency    = 0x02,	// true if at least the emergency bus is powered
		BusPowerEssential    = 0x04,	// true if at least the essential bus is powered
		BusPowerNonEssential = 0x08,	// true if at least the non-essential bus is powered
		MainGenerator        = 0x10,	// true if the main generator is online
		StandbyGenerator     = 0x20,	// true if the standby generator is online
		JetFuelStarter       = 0x40,	// true if JFS is running, can be used for magswitch
	};

	// BLINKING LIGHTS - only indicating *IF* a lamp is blinking, not implementing the actual on/off/blinking pattern logic!
	enum BlinkBits
	{
		// currently working
		BOuterMarker  = 0x01,	// defined in HsiBits    - slow flashing for outer marker
		BMiddleMarker = 0x02,	// defined in HsiBits    - fast flashing for middle marker
		BPROBEHEAT    = 0x04,	// defined in LightBits2 - probeheat system is tested
		BAuxSrch      = 0x08,	// defined in LightBits2 - search function in NOT activated and a search radar is painting ownship
		BLaunch       = 0x10,	// defined in LightBits2 - missile is fired at ownship
		BPriMode      = 0x20,	// defined in LightBits2 - priority mode is enabled but more than 5 threat emitters are detected
		BUnk          = 0x40,	// defined in LightBits2 - unknown is not active but EWS detects unknown radar

		// not working yet, defined for future use
		BElec_Fault   = 0x80,	// defined in LightBits3 - non-resetting fault
		OXY_BROW     = 0x100,	// defined in LightBits  - monitor fault during Obogs
		BEPUOn        = 0x200,	// defined in LightBits3 - abnormal EPU operation
		JFSOn_Slow   = 0x400,	// defined in LightBits3 - slow blinking: non-critical failure
		JFSOn_Fast   = 0x800,	// defined in LightBits3 - fast blinking: critical failure
	};

	// CMDS mode state
	enum CmdsModes
	{
		CmdsOFF  = 0,
		CmdsSTBY = 1,
		CmdsMAN  = 2,
		CmdsSEMI = 3,
		CmdsAUTO = 4,
		CmdsBYP  = 5,
	};

	// HSI/eHSI mode state
	enum NavModes 
	{
		ILS_TACAN   = 0,
		TACAN       = 1, 
		NAV         = 2,
		ILS_NAV     = 3,
	};

	// human pilot state
	enum FlyStates
	{
		IN_UI   = 0, // UI      - in the UI
		LOADING = 1, // UI>3D   - loading the sim data
		WAITING = 2, // UI>3D   - waiting for other players
		FLYING  = 3, // 3D      - flying
		DEAD    = 4, // 3D>Dead - dead, waiting to respawn
		UNKNOWN = 5, // ???
	};


	// VERSION 1
	unsigned char navMode;  // current mode selected for HSI/eHSI, see NavModes enum for details
	float AAUZ;             // Ownship barometric altitude given by AAU (depends on calibration)
	char tacanInfo[NUMBER_OF_SOURCES]; // Tacan band/mode settings for UFC and AUX COMM

	// VERSION 2 / 7
	int AltCalReading;	// barometric altitude calibration (depends on CalType)
	unsigned int altBits;		// various altimeter bits, see AltBits enum for details
	unsigned int powerBits;		// Ownship power bus / generator states, see PowerBits enum for details
	unsigned int blinkBits;		// Cockpit indicator lights blink status, see BlinkBits enum for details
						// NOTE: these bits indicate only *if* a lamp is blinking, in addition to the
						// existing on/off bits. It's up to the external program to implement the
						// *actual* blinking.
	int cmdsMode;		// Ownship CMDS mode state, see CmdsModes enum for details
	int BupUhfPreset;	// BUP UHF channel preset

	// VERSION 3
	int BupUhfFreq;		// BUP UHF channel frequency
	float cabinAlt;		// Ownship cabin altitude
	float hydPressureA;	// Ownship Hydraulic Pressure A
	float hydPressureB;	// Ownship Hydraulic Pressure B
	int currentTime;	// Current time in seconds (max 60 * 60 * 24)
	short vehicleACD;	// Ownship ACD index number, i.e. which aircraft type are we flying.
	int VersionNum2;		// Version of FlightData2 mem area

	// VERSION 4
	float fuelFlow2;    // Ownship fuel flow2 (Lbs/Hour)

	// VERSION 5 / 8
	char RwrInfo[RWRINFO_SIZE]; // New RWR Info
	float lefPos;               // Ownship LEF position
	float tefPos;               // Ownship TEF position

	// VERSION 6
	float vtolPos;      // Ownship VTOL exhaust angle

	// VERSION 9
	char pilotsOnline;                                // Number of pilots in an MP session
	char pilotsCallsign[MAX_CALLSIGNS][CALLSIGN_LEN]; // List of pilots callsign connected to an MP session
	char pilotsStatus[MAX_CALLSIGNS];                 // Status of the MP pilots, see enum FlyStates
};

#endif
