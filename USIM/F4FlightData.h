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
		 OXY_BROW  = 0x4,   // repurposed for eyebrow OXY LOW (was OBS, unused)
		 EQUIP_HOT = 0x8,   // Caution light; repurposed for cooling fault (was: not used)
		 ONGROUND  = 0x10,  // True if on ground: this is not a lamp bit!
         ENG_FIRE  = 0x20,
		 CONFIG	   = 0x40,
         HYD       = 0x80,
         Flcs_ABCD = 0x100, // TEST panel FLCS channel lamps; repurposed, was OIL (see HYD; that lamp is not split face)
         FLCS      = 0x200, // Right eyebrow; was called DUAL which matches block 25, 30/32 and older 40/42
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
		AllLampBits2On = 0xFFFFF03F,
		AllLampBits2OnExceptCarapace = AllLampBits2On ^ HandOff ^ Launch ^ PriMode ^ Naval ^ Unk ^ TgtSep ^ AuxSrch ^ AuxAct ^ AuxLow ^ AuxPwr
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
        Elec_Fault = 0x400,
        Lef_Fault  = 0x800,

		OnGround	  = 0x1000,   // weight-on-wheels
        FlcsBitRun    = 0x2000,   // FLT CONTROL panel RUN light (used to be Multi-engine fire light)
        FlcsBitFail   = 0x4000,   // FLT CONTROL panel FAIL light (used to be Lock light Cue; non-F-16)
        DbuWarn       = 0x8000,   // Right eyebrow DBU ON cell; was Shoot light cue; non-F16
        NoseGearDown  = 0x10000,  // Landing gear panel; on means down and locked
        LeftGearDown  = 0x20000,  // Landing gear panel; on means down and locked
        RightGearDown = 0x40000,  // Landing gear panel; on means down and locked
		ParkBrakeOn   = 0x100000, // Parking brake engaged; NOTE: not a lamp bit
        Power_Off     = 0x200000, // Set if there is no electrical power.  NB: not a lamp bit

		// Caution panel
		cadc	= 0x400000,
		// Left Aux console
		SpeedBrake = 0x800000,  // True if speed brake is in anything other than stowed position

        // Threat Warning Prime - additional bits
		SysTest  = 0x1000000,

		// Master Caution WILL come up (actual lightBit has 3sec delay like in RL),
		// usable for cockpit builders with RL equipment which has a delay on its own.
		// Will be set to false again as soon as the MasterCaution bit is set.
		MCAnnounced = 0x2000000,

		//MLGWOW is only for AFM , it means WOW switches on MLG are triggered => FLCS switches to WOWPitchRockGain
		MLGWOW = 0x4000000,
		NLGWOW = 0x8000000,

		ATF_Not_Engaged = 0x10000000,
		
		// Caution panel
		Inlet_Icing = 0x20000000,
		// Free bits in LightBits3		
		//0x20000000,
		//0x40000000,
		//0x80000000,

		// Used with the MAL/IND light code to light up "everything"
        // please update this if you add/change bits!
		AllLampBits3On = 0x3147EFFF,
		AllLampBits3OnExceptCarapace = AllLampBits3On ^ SysTest
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
	//float nozzlePos2;   // MOVED TO FlightData2! Ownship engine nozzle2 percent open (0-100) 
	float internalFuel; // Ownship internal fuel (Lbs)
	float externalFuel; // Ownship external fuel (Lbs)
	float fuelFlow;     // Ownship fuel flow (Lbs/Hour)
	float rpm;          // Ownship engine rpm (Percent 0-103)
	//float rpm2;         // MOVED TO FlightData2! Ownship engine rpm2 (Percent 0-103)
	float ftit;         // Ownship Forward Turbine Inlet Temp (Degrees C)
	//float ftit2;        // MOVED TO FlightData2! Ownship Forward Turbine Inlet Temp2 (Degrees C)
	float gearPos;      // Ownship Gear position 0 = up, 1 = down;
	float speedBrake;   // Ownship speed brake position 0 = closed, 1 = 60 Degrees open
	float epuFuel;      // Ownship EPU fuel (Percent 0-100)
	float oilPressure;  // Ownship Oil Pressure (Percent 0-100)
	//float oilPressure2; // MOVED TO FlightData2! Ownship Oil Pressure2 (Percent 0-100)
    unsigned int   lightBits;    // Cockpit Indicator Lights, one bit per bulb. See enum

	// These are inputs. Use them carefully
	float headPitch;    // Head pitch offset from design eye (radians)
	float headRoll;     // Head roll offset from design eye (radians)
	float headYaw;      // Head yaw offset from design eye (radians)

	// new lights
    unsigned int   lightBits2;   // Cockpit Indicator Lights, one bit per bulb. See enum
    unsigned int   lightBits3;   // Cockpit Indicator Lights, one bit per bulb. See enum

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
    unsigned int hsiBits;      // HSI flags

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
	char PRKMag;			// Parking Brake magnetic switch
	char O1[5];				// OSRAM Display O1
	char O2[5];				// OSRAM Display O2
	char O3Chaff[5];		// OSRAM Display O3
	char O4Flare[5];		// OSRAM Display O4
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
		BOXY_BROW     = 0x100,	// defined in LightBits  - monitor fault during Obogs
		BEPUOn        = 0x200,	// defined in LightBits3 - abnormal EPU operation
		JFSOn_Slow   = 0x400,	// defined in LightBits3 - slow blinking: non-critical failure
		JFSOn_Fast   = 0x800,	// defined in LightBits3 - fast blinking: critical failure
	};

	// CMDS mode state
	enum CmdsModes : int
	{
		CmdsOFF  = 0,
		CmdsSTBY = 1,
		CmdsMAN  = 2,
		CmdsSEMI = 3,
		CmdsAUTO = 4,
		CmdsBYP  = 5,
	};

	// HSI/eHSI mode state
	enum NavModes : unsigned char
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

	// RTT area indices
	enum RTT_areas
	{
		RTT_HUD       = 0,
		RTT_PFL       = 1,
		RTT_DED       = 2,
		RTT_RWR       = 3,
		RTT_MFDLEFT   = 4,
		RTT_MFDRIGHT  = 5,
		RTT_HMS       = 6,
		RTT_noOfAreas = 7,
	};

	// instrument backlight brightness
	enum InstrLight : unsigned char
	{
		INSTR_LIGHT_OFF = 0,
		INSTR_LIGHT_DIM = 1,
		INSTR_LIGHT_BRT = 2,
	};

	// Bitching Betty VMS sounds playing
	enum BettyBits : unsigned int
	{
		Betty_Allwords       = 0x00001,
		Betty_Pullup         = 0x00002,
		Betty_Altitude       = 0x00004,
		Betty_Warning        = 0x00008,
		Betty_Jammer         = 0x00010,
		Betty_Counter        = 0x00020,
		Betty_ChaffFlare     = 0x00040,
		Betty_ChaffFlare_Low = 0x00080,
		Betty_ChaffFlare_Out = 0x00100,
		Betty_Lock           = 0x00200,
		Betty_Caution        = 0x00400,
		Betty_Bingo          = 0x00800,
		Betty_Data           = 0x01000,
		Betty_IFF            = 0x02000,
		Betty_Lowspeed       = 0x04000,
		Betty_Beeps          = 0x08000,
		Betty_AOA            = 0x10000,
		Betty_MaxG           = 0x20000,
	};

	// various flags - chances are that by now, we'll add new flags rarely and sparsely, so having a single "bulk pool" seems reasonable, size-wise
	enum MiscBits : unsigned int
	{
		RALT_Valid			= 0x01, // indicates weather the RALT reading is valid/reliable
		Flcs_Flcc_A			= 0x02,
		Flcs_Flcc_B			= 0x04,
		Flcs_Flcc_C			= 0x08,
		Flcs_Flcc_D			= 0x10,
		SolenoidStatus		= 0x20, //0 not powered or failed or WOW  , 1 is working OK

		AllLampBitsFlccOn	= 0x1e,
	};

	// VERSION 1
	float nozzlePos2;       // Ownship engine nozzle2 percent open (0-100)
	float rpm2;             // Ownship engine rpm2 (Percent 0-103)
	float ftit2;            // Ownship Forward Turbine Inlet Temp2 (Degrees C)
	float oilPressure2;     // Ownship Oil Pressure2 (Percent 0-100)
	NavModes navMode;       // (unsigned char) current mode selected for HSI/eHSI, see NavModes enum for details
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
	CmdsModes cmdsMode;	// (int) Ownship CMDS mode state, see CmdsModes enum for details
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
	// VERSION 10
	float bumpIntensity; // Intensity of a "bump" while taxiing/rolling, 0..1

	// VERSION 11
	float latitude;      // Ownship latitude in degrees (as known by avionics)
	float longitude;     // Ownship longitude in degrees (as known by avionics)

	// VERSION 12
	unsigned short RTT_size[2];                 // RTT overall width and height
	unsigned short RTT_area[RTT_noOfAreas][4];  // For each area: left/top/right/bottom

	// VERSION 13
	char iffBackupMode1Digit1;                     // IFF panel backup Mode1 digit 1
	char iffBackupMode1Digit2;                     // IFF panel backup Mode1 digit 2
	char iffBackupMode3ADigit1;                    // IFF panel backup Mode3A digit 1
	char iffBackupMode3ADigit2;                    // IFF panel backup Mode3A digit 2

	// VERSION 14
	InstrLight instrLight;  // (unsigned char) current instrument backlight brightness setting, see InstrLight enum for details

	// VERSION 15
	unsigned int bettyBits;      // see BettyBits enum for details
	unsigned int miscBits;       // see MiscBits enum for details
	float RALT;                  // radar altitude (only valid/ reliable if MiscBit "RALT_Valid" is set)
	float bingoFuel;             // bingo fuel level
	float caraAlow;              // cara alow setting
	float bullseyeX;             // bullseye X in sim coordinates (same as ownship, i.e. North (Ft))
	float bullseyeY;             // bullseye Y in sim coordinates (same as ownship, i.e. East (Ft))
	int BMSVersionMajor;         // E.g.  4.
	int BMSVersionMinor;         //         34.
	int BMSVersionMicro;         //            1
	int BMSBuildNumber;          //              build 20050
	unsigned int StringAreaSize; // the overall size of the StringData/FalconSharedMemoryAreaString area
	unsigned int StringAreaTime; // last time the StringData/FalconSharedMemoryAreaString area has been changed - you only need to re-read the string shared mem if this changes
	unsigned int DrawingAreaSize;// the overall size of the DrawingData/FalconSharedMemoryAreaDrawing area
	
	// VERSION 16
	float turnRate;              // actual turn rate (no delay or dampening) in degrees/second
};

#endif
