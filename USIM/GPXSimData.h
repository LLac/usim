#ifndef __GPXSIMDATA_H__
#define __GPXSIMDATA_H__

#define GPXCINFO_NAME       "GPxCInfo"
#define GPXCINFO_MUTEX_NAME GPXCINFO_NAME "_Mutex"

// TFS_GPxData struct
#define IVIBE_NAME			"ivibe_GPx" //memory mapped file
#define IVIBE_MUTEX_NAME IVIBE_NAME	"_Mutex" //Mutex

#define GPX_CIF_PITLIGHT    0x01 // set if pit light is on
#define GPX_CIF_REDLED      0x02 // set if red led is on (failure/warning)
#define GPX_CIF_YELLOWLED   0x04 // set if yellow led is on (yellow flag)
#define GPX_CIF_BLUELED     0x08 // set if blue led is on (blue flag)

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
struct GPxCInfoData {
	BYTE flags;             // see GPX_CIF_* defines above
	BYTE lap;               // lap number
	BYTE position;          // race position
	WORD fuel;              // fuel laps (value = fuel laps * 10)
	BYTE temp;              // water temperature
	int  speed;             // rounded speed in either kph or mph
	DWORD splittime;        // last split or lap time
	char gear;              // gear
	WORD revs;              // rpm
	DWORD time;             // session time in millis
	DWORD timeFront;        // difference in millis to driver in front (or 0xc0000000 if n/a)
	DWORD timeBehind;       // difference in millis to driver behind (or 0xc0000000 if n/a)
	char driverFront[24];   // initial and last name of driver in front, if available
	char driverBehind[24];  // initial and last name of driver behind, if available

	float spare;		    // 0
	char flag_string[40];
};
#pragma pack(pop)	/* restore original alignment from stack */

// Note: X-axis is left to right, Y-axis is bottom to top, and Z-axis is close to far

#define GPX_FLAG_INCOCKPIT              0x00000001      // false if user is navigating in the menu's (all other data is not updated then)
#define GPX_FLAG_REPLAY                 0x00000002      // true if currently replay mode is active
#define GPX_FLAG_DRIVERINCAR			0x00000100		// true if the driver is actually in the car
#define GPX_FLAG_ENGINERUNNING			0x00000200		// true if the engine is running

#define GPX_CAMF_CHASEFRONT             0x10
#define GPX_CAMF_CHASE                  0x20
#define GPX_CAMF_ONBOARD                0x40
#define GPX_CAMF_EXTERNAL               0x80
#define GPX_CAM_COCKPIT                 0
#define GPX_CAM_CHASEBEHIND             (CAMF_EXTERNAL | CAMF_CHASE)
#define GPX_CAM_CHASEFRONT              (CAMF_EXTERNAL | CAMF_CHASE | CAMF_CHASEFRONT)
#define GPX_CAM_ONBOARD                 (CAMF_EXTERNAL | CAMF_ONBOARD)

#define GPX_CAR_NUM_PARTS                       23
#define GPX_CAR_PARTF_COCKPIT           0x00000001      // including driver helmet
#define GPX_CAR_PARTF_NOSE              0x00000002      // excluding front wing
#define GPX_CAR_PARTF_RF_WING           0x00000004      // right horizontal part
#define GPX_CAR_PARTF_RF_SIDEWING       0x00000008      // right vertical part
#define GPX_CAR_PARTF_LF_WING           0x00000010      // left horizontal part
#define GPX_CAR_PARTF_LF_SIDEWING       0x00000020      // left vertical part
#define GPX_CAR_PARTF_RF_SUSP           0x00000040
#define GPX_CAR_PARTF_RF_WHEEL          0x00000080
#define GPX_CAR_PARTF_LF_SUSP           0x00000100
#define GPX_CAR_PARTF_LF_WHEEL          0x00000200
#define GPX_CAR_PARTF_R_BODY            0x00000400      // right sidepot
#define GPX_CAR_PARTF_L_BODY            0x00000800      // left sidepot
#define GPX_CAR_PARTF_ENGINE            0x00001000      // behind driver
#define GPX_CAR_PARTF_RR_SUSP           0x00002000
#define GPX_CAR_PARTF_RR_WHEEL          0x00004000
#define GPX_CAR_PARTF_LR_SUSP           0x00008000
#define GPX_CAR_PARTF_LR_WHEEL          0x00010000
#define GPX_CAR_PARTF_MR_WING           0x00020000      // mid horizontal part
#define GPX_CAR_PARTF_RR_WING           0x00040000      // right vertical part
#define GPX_CAR_PARTF_LR_WING           0x00080000      // left vertical part
#define GPX_CAR_PARTF_TR_WING           0x00100000      // top horizontal part
#define GPX_CAR_PARTF_R_SMALLWING       0x00200000      // before right sidepot
#define GPX_CAR_PARTF_L_SMALLWING       0x00400000      // before left sidepot

#define GPX_SURFACE_TYPE_TRACK                  0
#define GPX_SURFACE_TYPE_KERBLOW                1
#define GPX_SURFACE_TYPE_KERBHIGH               2
#define GPX_SURFACE_TYPE_GRASS                  3
#define GPX_SURFACE_TYPE_GRIND                  4
#define GPX_SURFACE_TYPE_KERBGRASS              5       // a high kerb with grass on it

struct TFS_GPxData
{
	// Global data
	unsigned int dwFlags;           // see GPX_FLAG_* defines
	unsigned int dwGameTime;        // time in ms; start time undefined; runs slower than real-time when PO > 100%
	unsigned char bCameraView;      // see GPX_CAM* defines

	// Data of selected car
	unsigned int dwDamageState;     // see GPX_CAR_PARTF_* defines; any set bit means that part is removed from the car
	unsigned char bSurfaceType[4];  // see GPX_SURFACE_TYPE_* defines; surface type for each wheel: LR, RR, LF, RF
	unsigned char bBumpiness;       // indicates bumps in the track; values are generally less than 5
	double dblSpeed;                // in m/s, negative when driving going backwards
	double dblSpeedX;               // in m/s
	double dblSpeedZ;               // in m/s
	double dblWheelSpeed[4];        // in m/s, for each wheel: LR, RR, LF, RF
	double dblPositionX;            // in m
	double dblPositionY;            // in m
	double dblPositionZ;            // in m
	double dblAngleX;               // in rad
	double dblAngleY;               // in rad
	double dblAngleZ;               // in rad
	double dblAngleSpeedY;          // in rad, angle around Y-axis of speed vector (derived value)
	double dblLongAcc;              // in m/s^2
	double dblLatAcc;               // in m/s^2
	double dblRideHeight[4];        // in m, for each wheel: LR, RR, LF, RF
	double dblCompression[4];       // in m, for each wheel: LR, RR, LF, RF
	char bGear;                     // -1, 0, 1, 2, 3, 4, 5, 6 (and 7 for GP4)
	int nRevs;                      // in rpm
	int nTemperature;               // in degrees Celsius
	double dblSteer;                // in rad
	double dblThrottle;             // 0.0 to 1.0
	double dblBrake;                // 0.0 to 1.0

	float spare;		    // 0
};

class GPXSimData
{
public:
    GPxCInfoData	m_GPxCInfoData;
};

class GPXTFSSimData
{
public:
	TFS_GPxData		m_TPS_GPxData;
};

#endif
