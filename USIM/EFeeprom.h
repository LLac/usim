#ifndef EFNODE_H
#define EFNODE_H

/** D E F I N E S ************************************************************/
#define EEEF_PORT_INDEX			6
#define EEEF_PORT_BYTES			4
#define EEEF_OUTPUT1_INDEX		(EEEF_PORT_INDEX + EEEF_PORT_BYTES)
#define EEEF_OUTPUT1_BYTES		15
#define EEEF_OUTPUT2_INDEX		(EEEF_OUTPUT1_INDEX + EEEF_OUTPUT1_BYTES)
#define EEEF_OUTPUT2_BYTES		15
#define EEEF_GLCD1_INDEX		(EEEF_OUTPUT2_INDEX + EEEF_OUTPUT2_BYTES)
#define EEEF_GLCD1_BYTES		1
#define EEEF_GLCD2_INDEX		(EEEF_GLCD1_INDEX + EEEF_GLCD1_BYTES)
#define EEEF_GLCD2_BYTES		1
#define EEEF_SPI1_INDEX			(EEEF_GLCD2_INDEX + EEEF_GLCD2_BYTES)
#define EEEF_SPI1_BYTES			10
#define EEEF_SPI2_INDEX			(EEEF_SPI1_INDEX + EEEF_SPI1_BYTES)
#define EEEF_SPI2_BYTES			10
#define EEEF_STEPPER1_INDEX		(EEEF_SPI2_INDEX + EEEF_SPI2_BYTES)
#define EEEF_STEPPER1_BYTES		48
#define EEEF_STEPPER2_INDEX		(EEEF_STEPPER1_INDEX + EEEF_STEPPER1_BYTES)
#define EEEF_STEPPER2_BYTES		48
#define EEEE_DOTMATRIX1_INDEX	(EEEE_STEPPER2_INDEX + EEEE_STEPPER2_BYTES)
#define EEEE_DOTMATRIX1_BYTES	4
#define EEEE_DOTMATRIX2_INDEX	(EEEE_DOTMATRIX1_INDEX + EEEE_DOTMATRIX1_BYTES)
#define EEEE_DOTMATRIX2_BYTES	4

#define EEEF_BYTES_PER_PORT			2
#define EEEF_BYTES_PER_OUTPUT		1
#define EEEF_BYTES_PER_GLCD			1
#define EEEF_BYTES_PER_DMX			1
#define EEEF_BYTES_PER_SPI			1
#define EEEF_BYTES_PER_RWR			4
#define EEEF_BYTES_PER_STEPPER		6

#define EEEF_NUM_PORTS				8
#define EEEF_NUM_OUTPUTS			30
#define EEEF_NUM_OUTPUTS_PER_PORT	15
#define EEEF_NUM_GLCDS				2
#define EEEF_NUM_GLCDS_PER_PORT		1
#define EEEF_NUM_SPI				20
#define EEEF_NUM_SPI_PER_PORT		10
#define EEEF_NUM_DMX				8
#define EEEF_NUM_DMX_PER_PORT		4
#define EEEF_NUM_STEPPER			16
#define EEEF_NUM_STEPPER_PER_PORT	8
#define EEEF_BYTES_STEPPER_PER_PORT	48

#define NUM_MUX_PER_PORT		56
#define NUM_RWR_PER_PORT		2
#define NUM_DAC_PER_PORT		10
#define NUM_SPI_PER_PORT		10
#define NUM_LATCHED_PER_PORT	64
#define NUM_DOTMATRIX_PER_PORT	4
#define NUM_STEPPER_PER_PORT	8

#define MODE_MIXED			-1
#define MODE_DIRECT			0
#define MODE_MUX			1
#define MODE_RWR			2
#define MODE_GLCD			3
#define MODE_DAC			4
#define MODE_LATCHED		5
#define MODE_SPI			6
#define MODE_DOTMATRIX		7
#define MODE_STEPPER		8
#define MODE_CLCD			9

#define IO_OUTPUT			0
#define IO_INPUT			1

/** C O N F I G  V A R I A B L E S *******************************************/

typedef struct _EF_STRUCT_PORT
{
	// byte 1
	unsigned active:1;
	unsigned iomode:1;
	unsigned mode:4;
	unsigned :2;
	// byte 2
	unsigned level:5;
	unsigned :3;
} EF_PORT_CONFIG;

typedef struct _STRUCT_OUTPUT
{
	// byte 1
	unsigned active:1;
	unsigned input_flag:1;
	unsigned level:5;
	unsigned :1;
} OUTPUT_CONFIG;

typedef struct _STRUCT_GLCD
{
	// byte 1
	unsigned char type;
} GLCD_CONFIG;

typedef struct _STRUCT_DMX
{
	// byte 1
	unsigned active:1;
	unsigned type:7;
} DMX_CONFIG;

typedef struct _STRUCT_SPI
{
	unsigned active:1;
	unsigned nBits:4;
	unsigned nDamp:2;
	unsigned :1;
} SPI_CONFIG;

typedef struct _STRUCT_STEPPER
{
	// byte 1
	unsigned active:1;
	unsigned vref:4;
	unsigned ms:2;
	unsigned loop_flag:1;
	// byte 2
	unsigned AVmode:2;
	unsigned pps:4;
	unsigned home_flag:1;
	unsigned ends_flag:1;
	// byte 3-6
	short home_revs;
	USHORT steps_per_rev;

} STEPPER_CONFIG;

typedef struct _STRUCT_DIRECT_OUTPUT
{
	unsigned char state:1;
	unsigned char level:7;
} DIRECT_OUTPUT;

typedef struct _STRUCT_DISPLAY_OUTPUT
{
	unsigned disp1:8;
	unsigned disp2:8;
	unsigned disp3:8;
	unsigned disp4:8;
	unsigned disp5:8;
	unsigned disp6:8;
	unsigned disp7:8;
	unsigned level:7;
	unsigned :1;
} DISPLAY_OUTPUT;

typedef struct _STRUCT_CRTRWR_OUTPUT
{
	// 4 BYTES
	unsigned symbol:8;
	unsigned xPos:8;
	unsigned yPos:8;
	unsigned missileActivity:1;	// draw circle flash
	unsigned missileLaunch:1;	// draw circle solid
	unsigned newguy:1;
	unsigned selected:1;		// draw diamond if 1
	unsigned :4;	
} CRTRWR_OUTPUT;

//#pragma pack(push)  /* push current alignment to stack */
//#pragma pack(1)     /* set alignment to 1 byte boundary */

typedef union MY_DWORD
{
	long _dword;
    struct
    {
        WORD word0;
        WORD word1;
    };
    struct
    {
        BYTE v[4];
    };
} MYDWORD;

//#pragma pack(pop)   /* restore original alignment from stack */

#endif
