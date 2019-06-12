#ifndef BIAPI_H
#define BIAPI_H

/** D E F I N E S ************************************************************/

#define EEEF_NUM_OUTPUTS_PER_PORT	15
#define NUM_MUX_PER_PORT			56
#define MAX_RWR_SYMBOLS				40
#define EF_BYTES_PER_RWR			4

#define RWRSYM_TEST			0xFF

// HID Devices
#define PID_USIMV1			0x1900	// USIM Core
#define PIDN_USIMV1			"USIM V1"

// Config Devices
#define PIDC_USIMV1			0x1900	// USIM CONFIG HID
#define PIDNC_USIMV1		"USIM V1 CFG"

// Flash Devices
#define PIDF_USIMV1			0x2100 // USIM Flash Loader
#define PIDNF_USIMV1		"USIM V1"

/** S T R U C T U R E S ******************************************************/

typedef struct _STRUCT_DIRECT_OUTPUT
{
	unsigned char state:1;
	unsigned char level:7;
} DIRECT_OUTPUT;

typedef struct _STRUCT_DAC_OUTPUTS
{
	unsigned long DAC_1;
	unsigned long DAC_2;
	unsigned long DAC_3;
	unsigned long DAC_4;
	unsigned long DAC_5;
	unsigned long DAC_6;
	unsigned long DAC_7;
	unsigned long DAC_8;
	unsigned long DAC_9;
	unsigned long DAC_10;
} DAC_OUTPUTS;

typedef struct _STRUCT_STEPPER_OUTPUTS
{
	long STEP_1;
	long STEP_2;
	long STEP_3;
	long STEP_4;
	long STEP_5;
	long STEP_6;
	long STEP_7;
	long STEP_8;
} STEPPER_OUTPUTS;

typedef struct _STRUCT_CRTRWR_OUTPUT
{
	// 4 BYTES
	unsigned char symbol:8;
	unsigned char xPos:8;
	unsigned char yPos:8;
	unsigned char missileActivity:1;	// draw circle flash
	unsigned char missileLaunch:1;	// draw circle solid
	unsigned char newguy:1;
	unsigned char selected:1;		// draw diamond if 1
	unsigned char :4;	
} CRTRWR_OUTPUT;

typedef struct _STRUCT_FUSION
{
	// bytes 52
	char text_a[7];
	char text_b[7];
	char gear;
	unsigned char lbg8;
	unsigned char shiftlight;
	unsigned char level;
} FUSION_OUTPUT;

#endif
