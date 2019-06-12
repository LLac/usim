#ifndef BIUSB_H
#define BIUSB_H

//Application global constants
#define MANUF_NAME				"Beta Innovations Inc."

// Accepted Vendor IDs
#define VENDOR_ID_1				0x6666
#define VENDOR_ID_2				0x12DA

// HID Class Devices
#define PID_0X64B				0x0300 // 0x64B
#define PID_GAMMARAY			0x0500 // GammaRay-256
#define PID_GAMMARAY64			0x0700 // GammaRay-64
#define PID_ELECTRONFLUX		0x0800 // ElectronFlux Core
#define PID_GAMMARAY_V2			0x0510 // GammaRay-256 V2 Core
#define PID_GAMMARAY_V3			0x0520 // GammaRay-256 V3 Core
#define PID_GAMMATRON			0x0710 // GammaTron Core	
#define PID_NITRO_SLG			0x1500 // Nitro-SLG Core
#define PID_NITRO_XG			0x2000 // Nitro-XG Core
#define PID_DRO					0x9600 // DRO Core

#define PIDN_0X64B				"0X64B USB MODULE"
#define PIDN_GAMMARAY			"GammaRay-256 USB MODULE"
#define PIDN_GAMMARAY64			"GammaRay-64 USB MODULE"
#define PIDN_ELECTRONFLUX		"ElectronFlux"
#define PIDN_GAMMARAY_V2		"GammaRay V2"
#define PIDN_GAMMARAY_V3		"GammaRay V3"
#define PIDN_GAMMATRON			"GammaTron"
#define PIDN_NITRO_SLG			"Nitro-SLG"
#define PIDN_NITRO_XG			"Nitro-XG"
#define PIDN_DRO				"DRO"

// Standard Joystick Class Devices
#define PID_4X24BH				0x0100 // 4x24bhat
#define PID_6X16B				0x0200 // 6x16b
#define PID_6X13B				0x0250 // 6x13b
#define PID_5X18BH				0x0400 // 5x18bhat
#define PID_PLASMA				0x0600 // PLASMA single device
#define PID_PLASMA_HOTAS		0x0601 // PLASMA HOTAS single device
#define PID_PLASMA_DUAL			0x0602 // PLASMA dual device
#define PID_PLASMA_HOTAS_DUAL	0x0603 // PLASMA HOTAS dual device
#define PID_PLASMA_LITE			0x0900 // PLASMA-LITE
#define PID_PLASMA_LITE_V2		0x0910 // PLASMA-Lite v2
#define PID_PLASMA_MM2			0x0920 // PLASMA-MM2
#define PID_FUSION				0x1600 // FUSION

#define PIDN_4X24BH				"4X24BH"
#define PIDN_6X16B				"6X16B"
#define PIDN_6X13B				"6X13B"
#define PIDN_5X18BH				"5X18BH"
#define PIDN_PLASMA				"Plasma - USB Adapter"
#define PIDN_PLASMA_HOTAS		"Plasma HOTAS - USB Adapter"
#define PIDN_PLASMA_DUAL		"Plasma - Dual USB Adapter"
#define PIDN_PLASMA_HOTAS_DUAL	"Plasma HOTAS - Dual USB Adapter"
#define PIDN_PLASMA_LITE		"Plasma-Lite"
#define PIDN_PLASMA_LITE_V2		"Plasma-Lite"
#define PIDN_PLASMA_MM2			"Plasma-MM2"
#define PIDN_FUSION				"Fusion"

// Configuration devices
#define PIDC_PLASMA_LITE_V2		0x0910 // PLASMA-LITE V2 CONFIG HID
#define PIDC_GAMMARAY_V2		0x0510 // GammaRay V2 CONFIG HID
#define PIDC_GAMMARAY_V3		0x0520 // GammaRay V3 CONFIG HID
#define PIDC_ELECTRONFLUX		0x0800 // ElectronFlux CONFIG HID
#define PIDC_GAMMATRON			0x0710 // GammaTron CONFIG HID
#define PIDC_PLASMA_MM2			0x0920 // PLASMA-MM2 CONFIG HID
#define PIDC_NITRO_SLG			0x1500 // NITRO-SLG CONFIG HID
#define PIDC_FUSION				0x1600 // FUSION CONFIG HID
#define PIDC_NITRO_XG			0x2000 // NITRO-XG CONFIG HID
#define PIDC_DRO				0x9600 // DRO CONFIG HID

#define PIDNC_PLASMA_LITE_V2	"Plasma-Lite CFG"
#define PIDNC_GAMMARAY_V2		"GammaRay V2 CFG"
#define PIDNC_GAMMARAY_V3		"GammaRay V3 CFG"
#define PIDNC_ELECTRONFLUX		"ElectronFlux CFG"
#define PIDNC_GAMMATRON			"GammaTron CFG"
#define PIDNC_PLASMA_MM2		"Plasma-MM2 CFG"
#define PIDNC_NITRO_SLG			"NITRO-SLG CFG"
#define PIDNC_NITRO_XG			"NITRO-XG CFG"
#define PIDNC_FUSION			"FUSION CFG"
#define PIDNC_DRO				"DRO CFG"

// Flash Loader devices
#define PIDF_PLASMA_LITE_V2		0x1000 // PLASMA-LITE V2 Flash Loader
#define PIDNF_PLASMA_LITE_V2	"Plasma-Lite"
#define PIDF_GAMMARAY_V2		0x1100 // GammaRay V2 Flash Loader
#define PIDNF_GAMMARAY_V2		"GammaRay V2"
#define PIDF_GAMMARAY_V3		0x1110 // GammaRay V3 Flash Loader
#define PIDNF_GAMMARAY_V3		"GammaRay V3"
#define PIDF_ELECTRONFLUX		0x1200 // ElectronFlux Flash Loader
#define PIDNF_ELECTRONFLUX		"ElectronFlux"
#define PIDF_GAMMATRON			0x1300 // GammaTron Flash Loader
#define PIDNF_GAMMATRON			"GammaTron"
#define PIDF_PLASMA_MM2			0x1010 // PLASMA-MM2 Flash Loader
#define PIDNF_PLASMA_MM2		"Plasma-MM2"
#define PIDF_NITRO_SLG			0x1400 // NITRO-SLG Flash Loader
#define PIDNF_NITRO_SLG			"NITRO-SLG"
#define PIDF_NITRO_XG			0x1800 // NITRO-XG Flash Loader
#define PIDNF_NITRO_XG			"NITRO-XG"
#define PIDF_FUSION				0x1700 // FUSION Flash Loader
#define PIDNF_FUSION			"FUSION"
#define PIDF_DRO				0x9700 // DRO Flash Loader
#define PIDNF_DRO				"DRO"

#define MAX_DEVICES			128
#define MAX_PORTS			25	// PORT Y added.
#define MAX_INPUTS			512
#define MAX_OUTPUTS			MAX_PORTS*15
#define MAX_ANALOG			8
#define MAX_HATS			4
#define MAX_ROTARY			200
#define MAX_PWM				512
#define MAX_LCDS			200
#define MAX_GLCDS			MAX_PORTS
#define MAX_MUXDISPLAYS		MAX_PORTS*56
#define MAX_ALPHANUMERICS	100
#define	MAX_RWR				1
#define	MAX_RWR_CONTACTS	40
#define MAX_SPI				MAX_PORTS*10
#define MAX_LATCHED			MAX_PORTS*64
#define MAX_DOTMATRIX		MAX_PORTS*4
#define MAX_STEPPER			MAX_PORTS*8

#define MAX_CHAR				256
#define MAX_RETRY				6
#define MAX_INPUT_REPORT_BYTES	65
#define MAX_OUTPUT_REPORT_BYTES	65
#define MAX_TIMEOUT_MSEC		1000
#define	DEV_FAILED				0x0001
#define	DEV_INPUT				0x0002
#define	DEV_WAIT				0x0003
#define	DEV_TIMEOUT				0x0004
#define	DEV_NODE_FAILED			0x0005
#define	DEV_OUTPUT				0x0006
#define DT_STANDARD				0x00
#define DT_FLASH				0x01
#define DT_DEVICES				0x02
#define DT_REPORT				0x03
#define DT_HID					0x04
#define DT_INPUT				0x05
#define DT_ALL					0xFF

// LCD Commands
#define LCD_CLEAR		0x00
#define LCD_PAGE1		0x01
#define LCD_PAGE2		0x02
#define LCD_TEST		0x03
#define LCD_DEFCHAR		0x04

// GLCD Commands
#define GLCD_CLEAR		0x00
#define GLCD_TEST		0x01
#define GLCD_DISPLAY	0x02
#define GLCD_BGCOLOR	0x03
#define GLCD_POWERUP	0x04
#define GLCD_SHUTDOWN	0x05

// RWR Page defines
#define RWRPAGE_NORMAL	0x00
#define RWRPAGE_TEST	0x01
#define RWRPAGE_BLANK	0x02

// Port Y Index
#define PORT_Y	24

#ifdef HID_NONE
	#define HID_API
#else
#ifdef HID_EXPORTS
	#define HID_API __declspec(dllexport)
#else
#ifdef LIB_IMPORTS
	#pragma message("automatic link to biusb.lib")
	#pragma comment(lib, "biusb.lib")
	#define HID_API
#else
	#pragma message("automatic link to biusb.lib")
	#pragma comment(lib, "biusb.lib")
	#define HID_API __declspec(dllimport)
#endif
#endif
#endif

//Application global structure

typedef struct
{
    HANDLE		DeviceHandle;
	USHORT		NumberInputIndices;
	USHORT		NumberOutputIndices;
	USHORT		NumberPortIndices;
	USHORT		NumberAnalogIndices;
	USHORT		NumberHATIndices;
	USHORT		NumberRotaryIndices;
	USHORT		NumberCRTRWRIndices;
	USHORT		NumberPWMIndices;
	USHORT		NumberLCDIndices;
	USHORT		NumberGLCDIndices;
	USHORT		NumberMuxDisplayIndices;
	USHORT		NumberAlphanumericIndices;
	USHORT		NumberSPIIndices;
	USHORT		NumberLatchedIndices;
	USHORT		NumberDotMatrixIndices;
	USHORT		NumberStepperIndices;
    USHORT		VendorID;
    USHORT		ProductID;
    USHORT		VersionNumber;
	USHORT		FlashVersion;
	CHAR		ProgramFlag;
	CHAR		DevicePath[MAX_CHAR];
	ULONG		PathLength;
	CHAR		DeviceName[MAX_CHAR];
	ULONG		DeviceNameLength;
	CHAR		ManufName[MAX_CHAR];
	ULONG		ManufNameLength;
	CHAR		SerialNum[4];
	ULONG		SerialNumLength;
	CHAR		ConfigFlag;
	USHORT		DevIndex;
	USHORT		InputReportByteLength;
	USHORT		OutputReportByteLength;
	USHORT		Usage;
	USHORT		UsagePage;
} DeviceParam, *pDeviceParam;

typedef struct
{
	CHAR	InputActive[MAX_INPUTS];
	CHAR	OutputActive[MAX_OUTPUTS];
	CHAR	PortActive[MAX_PORTS];
	CHAR	PortIOMode[MAX_PORTS];
	CHAR	PortMode[MAX_PORTS];
	CHAR	OutputMode[MAX_PORTS];
	CHAR	AnalogActive[MAX_ANALOG];
	CHAR	HATActive[MAX_HATS];
	CHAR	RotaryActive[MAX_ROTARY];
	CHAR	CRTRWRActive[MAX_RWR];
	CHAR	PWMActive[MAX_PWM];
	CHAR	LCDActive[MAX_LCDS];
	CHAR	GLCDActive[MAX_GLCDS];
	CHAR	MuxDisplayActive[MAX_MUXDISPLAYS];
	CHAR	AlphanumericActive[MAX_ALPHANUMERICS];
	CHAR	SPIActive[MAX_SPI];
	CHAR	LatchedActive[MAX_LATCHED];
	CHAR	DotMatrixActive[MAX_DOTMATRIX];
	CHAR	StepperActive[MAX_STEPPER];
} DeviceStatus, *pDeviceStatus;

//Application export prototypes

HID_API BOOL APIENTRY DetectDevice(UINT *, DeviceParam *, UINT, UINT);
HID_API void APIENTRY RetrieveStatus(DeviceParam *, DeviceStatus *);
HID_API BOOL APIENTRY CloseDevices(UINT, DeviceParam *);
HID_API LONG APIENTRY ReadData(DeviceParam *, char *);
HID_API LONG APIENTRY WriteData(DeviceParam *, char *);
HID_API LONG APIENTRY ReadInputData(DeviceParam *, char *, UINT);
HID_API LONG APIENTRY ReadInputs(DeviceParam *, char *);
HID_API LONG APIENTRY ResetRotary(DeviceParam *iDeviceList);
HID_API LONG APIENTRY ReadRotary(DeviceParam *iDeviceList, char *iDataBuffer);
HID_API LONG APIENTRY ClearLCD(DeviceParam *, UCHAR);
HID_API LONG APIENTRY WriteLCD(DeviceParam *, UCHAR, UCHAR, char *, UCHAR);
HID_API LONG APIENTRY CmdLCD(DeviceParam *, UCHAR, UCHAR, char *, UCHAR);
HID_API LONG APIENTRY WriteDirectOutput(DeviceParam *, UCHAR, DIRECT_OUTPUT *);
HID_API LONG APIENTRY WriteMuxOutput(DeviceParam *, UCHAR, DIRECT_OUTPUT *);
HID_API LONG APIENTRY WriteDisplayOutput(DeviceParam *, UCHAR, char *, UCHAR);
HID_API LONG APIENTRY Write7Segment(DeviceParam *, UCHAR *);
HID_API LONG APIENTRY WriteSPIDAC(DeviceParam *, UCHAR, DAC_OUTPUTS *, UCHAR);
HID_API LONG APIENTRY CmdGLCD(DeviceParam *, UCHAR, UCHAR, UCHAR);
HID_API LONG APIENTRY WriteGLCD(DeviceParam *, UCHAR, UCHAR, UCHAR, UCHAR, char *);
HID_API LONG APIENTRY WriteLatchedOutput(DeviceParam *, UCHAR, UCHAR *, UCHAR);
HID_API LONG APIENTRY WriteCRTRWR(DeviceParam *, UCHAR, UCHAR, UCHAR, CRTRWR_OUTPUT *);
HID_API LONG APIENTRY WriteDotMatrix(DeviceParam *, UCHAR, char *, UCHAR);
HID_API LONG APIENTRY WriteFUSION(DeviceParam *, FUSION_OUTPUT *, UCHAR);
HID_API LONG APIENTRY WriteStepper(DeviceParam *, UCHAR, long *);

#endif
