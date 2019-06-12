#ifndef _FLIGHT_DATA_H
#define _FLIGHT_DATA_H

class BMSFlightData
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
		 AllLampBits2On     = 0xFFFFF03F
	  };

	  enum LightBits3
	  {
		  FlcsPmg		=	0x1,
		  MainGen		=	0x2,
		  StbyGen		=	0x4,
		  EpuGen		=	0x8,
		  EpuPmg		=	0x10,
		  ToFlcs		=	0x20,
		  FlcsRly		=	0x40,
		  BatFail		=	0x80,
		  Hydrazine		=	0x100,
		  Air			=	0x200,
		  Elec_Fault	=	0x400,
		  Lef_Fault		=	0x800,
		  Power_Off     =   0x1000,   // Set if there is no electrical power
		  Eng2_Fire		=	0x2000,//TJL 01/24/04 Multi-engine
		  Lock			=	0x4000,//TJL 01/24/04 Lock Light Cue
		  Shoot			=	0x8000,//TJL 01/24/04 Shoot light cue
		  NoseGearDown  =   0x10000,  // MD -- 20040301: on means down and locked
		  LeftGearDown  =   0x20000,  // MD -- 20040301: on means down and locked
		  RightGearDown =   0x40000,  // MD -- 20040301: on means down and locked

		 // MD 20031016: Use this with the MAL/IND light code in cautions.cpp
		 // please update this is you add/change bits!
		 AllLampBits3On     = 0x0007EFFF
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
		 // MD 20031016: Use this with the MAL/IND light code in cautions.cpp
		 // please update this is you add/change bits!
		AllLampHsiBitsOn = 0x2000
	  };



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
	  float nozzlePos2;	  // Ownship engine nozzle2 percent open (0-100) //TJL 01/14/04 Multi-engine
      float internalFuel; // Ownship internal fuel (Lbs)
      float externalFuel; // Ownship external fuel (Lbs)
      float fuelFlow;     // Ownship fuel flow (Lbs/Hour)
      float rpm;          // Ownship engine rpm (Percent 0-103)
	  float rpm2;          // Ownship engine rpm2 (Percent 0-103) //TJL 01/14/04 multi-engine
      float ftit;         // Ownship Forward Turbine Inlet Temp (Degrees C)
	  float ftit2;         // Ownship Forward Turbine Inlet Temp2 (Degrees C)//TJL 01/14/04 multi-engine
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
      float ChaffCount;	        // Number of Chaff left
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
	  float	courseDeviation;	// HSI_VAL_CRS_DEVIATION
	  float	desiredCourse;		// HSI_VAL_DESIRED_CRS
	  float distanceToBeacon;   // HSI_VAL_DISTANCE_TO_BEACON
	  float	bearingToBeacon;	// HSI_VAL_BEARING_TO_BEACON
	  float currentHeading;		// HSI_VAL_CURRENT_HEADING
	  float	desiredHeading;		// HSI_VAL_DESIRED_HEADING
	  float deviationLimit;		// HSI_VAL_DEV_LIMIT
	  float halfDeviationLimit; // HSI_VAL_HALF_DEV_LIMIT
	  float localizerCourse;	// HSI_VAL_LOCALIZER_CRS
	  float airbaseX;			// HSI_VAL_AIRBASE_X
	  float airbaseY;			// HSI_VAL_AIRBASE_Y
	  float totalValues;		// HSI_VAL_TOTAL_VALUES  ???

	  float TrimPitch;	// Value of trim in pitch axis, -0.5 to +0.5
	  float TrimRoll;	// Value of trim in roll axis, -0.5 to +0.5
	  float TrimYaw;	// Value of trim in yaw axis, -0.5 to +0.5

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
	  int           RWRsymbol[20];
	  float         bearing[20];
      unsigned long missileActivity[20];
	  unsigned long missileLaunch[20];
      unsigned long selected[20];
      float         lethality[20];

	  //fuel values
	  float fwd, aft, total;

      void SetLightBit (int newBit) {lightBits |= newBit;};
      void ClearLightBit (int newBit) {lightBits &= ~newBit;};
      int  IsSet (int newBit) {return ((lightBits & newBit) ? TRUE : FALSE);};

      void SetLightBit2 (int newBit) {lightBits2 |= newBit;};
      void ClearLightBit2 (int newBit) {lightBits2 &= ~newBit;};
      int  IsSet2 (int newBit) {return ((lightBits2 & newBit) ? TRUE : FALSE);};

	  void SetLightBit3 (int newBit) {lightBits3 |= newBit;};
      void ClearLightBit3 (int newBit) {lightBits3 &= ~newBit;};
      int  IsSet3 (int newBit) {return ((lightBits3 & newBit) ? TRUE : FALSE);};

      void SetHsiBit (int newBit) {hsiBits |= newBit;};
      void ClearHsiBit (int newBit) {hsiBits &= ~newBit;};
      int  IsSetHsi (int newBit) {return ((hsiBits & newBit) ? TRUE : FALSE);};

	  int VersionNum;	//Version of Memarea
};

#endif
