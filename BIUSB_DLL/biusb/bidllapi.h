#ifdef BIAPI_H
#define BIAPI_EXTERN
#else
#define BIAPI_EXTERN extern
#endif

#include <oaidl.h>
#include <setupapi.h>

extern "C"
{
#pragma warning (disable : 4201) // C4201 nonstandard extension used : nameless struct/union
#include "hidsdi.h"
#pragma warning (default : 4201) // C4201 nonstandard extension used : nameless struct/union
}

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _INC_MALLOC
#endif

// HID Devices
#define PID_USIMV1			0x1900	// ElectronFlux Core
#define PIDN_USIMV1			"USIM V1"

// Config Devices
#define PIDC_USIMV1			0x1900	// ElectronFlux CONFIG HID
#define PIDNC_USIMV1		"USIM V1 CFG"

// Flash Devices
#define PIDF_USIMV1			0x2100 // ElectronFlux Flash Loader
#define PIDNF_USIMV1		"USIM V1"

typedef struct
{
	USHORT		InputReportByteLength;
	USHORT		OutputReportByteLength;
	PHIDP_PREPARSED_DATA	PreparsedData;
	USHORT		Usage;
	USHORT		UsagePage;
} DevicePHID, *pDevicePHID;

struct DEVICE_LIST
{
	USHORT		ProductID;					// 1
	USHORT		NumberInputIndices;			// 2
	USHORT		NumberOutputIndices;		// 3
	USHORT		NumberPortIndices;			// 4
	USHORT		NumberAnalogIndices;		// 5
	USHORT		NumberHATIndices;			// 6
	USHORT		NumberRotaryIndices;		// 7
	USHORT		NumberPWMIndices;			// 8
	USHORT		NumberLCDIndices;			// 9
	USHORT		NumberMuxDisplayIndices;	// 10
	USHORT		NumberAlphanumericIndices;	// 11
	USHORT		NumberCRTRWRIndices;		// 12
	USHORT		NumberGLCDIndices;			// 13
	USHORT		NumberSPIIndices;			// 14
	USHORT		NumberLatchedIndices;		// 15
	USHORT		NumberDotMatrixIndices;		// 16
	USHORT		NumberStepperIndices;		// 17
};

DEVICE_LIST const DevArray[] = {
	//    1				2		3		4	5	6	7		8		9		10		11		12		13		14		15		16		17	
	{0,					0,		0,		0,	0,	0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// NULL
	{PID_0X64B,			64,		0,		2,	0,	0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// 0x64B
	{PID_GAMMARAY,		256,	0,		8,	0,	0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// GammaRay-256
	{PID_GAMMARAY64,	64,		0,		2,	0,	0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// GammaRay-64
	//    1				2		3				4			5	6	7		8		9		10					11				12				13				14			15				16					17	
	{PID_ELECTRONFLUX,	0,		MAX_OUTPUTS,	MAX_PORTS,	0,	0,	0,		375,	0,		MAX_MUXDISPLAYS,	MAX_PORTS,		MAX_PORTS,		MAX_PORTS,		MAX_SPI,	MAX_LATCHED,	MAX_DOTMATRIX,		MAX_STEPPER},	// ElectronFlux
//	{PID_ELECTRONFLUX,	0,		360,			24,			0,	0,	0,		360,	0,		1344,				24,				24,				24,				240,		1536,			96,					192},			// ElectronFlux
	{PID_GAMMARAY_V2,	256,	0,		8,	0,	0,	16,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// GammaRay V2
	{PID_GAMMATRON,		192,	0,		4,	0,	0,	32,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// GammaTron
	{PID_PLASMA_LITE_V2,0,		12,		1,	0,	0,	8,		0,		1,		1,		0,		0,		0,		8,		0,		0,		0},		// Plasma-Lite V2
	{PID_NITRO_SLG,		0,		0,		1,	0,	0,	0,		0,		0,		1,		0,		0,		0,		8,		0,		0,		0},		// NITRO-SLG
	{PID_FUSION,		15,		9,		0,	0,	0,	7,		0,		1,		8,		0,		0,		0,		10,		0,		0,		0},		// FUSION
	{PID_GAMMARAY_V3,	512,	0,		8,	0,	0,	16,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// GammaRay V3
	{PID_NITRO_XG,		0,		0,		1,	0,	0,	0,		0,		1,		1,		0,		0,		0,		10,		16,		0,		0},		// NITRO-XG
	{PID_PLASMA_MM2,	64,		0,		1,	0,	0,	8,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0},		// Plasma-MM2
	{PID_DRO,			0,		0,		1,	0,	0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0}		// DRO
};

//Application function prototypes

void GetDeviceCapabilities(HANDLE, DeviceParam *);
BOOL GetDevParam(HIDD_ATTRIBUTES *, DeviceParam *, SP_DEVICE_INTERFACE_DETAIL_DATA *, int);
void GetHIDName(HIDD_ATTRIBUTES *, DeviceParam *);
void StoreInputReport(DeviceParam *, CHAR *, CHAR *, BOOL);
