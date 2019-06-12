// Plasma-Lite V2 EEPROM Structure
//
//

#ifndef EEPROM_H
#define EEPROM_H

typedef struct _STRUCT_ANALOG
{
	// byte 1
	unsigned cal:1;
	unsigned spike:1;
	unsigned rev:1;
	unsigned scale:1;
	unsigned filter:4;
	// byte 2
	unsigned res:4;
	unsigned :4;
	// byte 3 & 4
	unsigned but_no:5;
	unsigned inc_on:1;
	unsigned axis_on:1;
	unsigned :1;
	// byte 4
	unsigned axis:8;
} STRUCT_ANALOG;

typedef struct _STRUCT_CALIBRATE
{
	// byte 1
	unsigned on:1;
	unsigned cal:1;
	unsigned :6;
	// byte 2
	unsigned trim_max:8;
	// byte 3
	unsigned center:8;
	// byte 4
	unsigned trim_min:8;
	// byte 5
	unsigned dead_zone:8;
} STRUCT_CALIBRATE;

typedef struct _STRUCT_INCREMENTAL
{
	// byte 1
	unsigned cal:1;
	unsigned axis_on:1;
	unsigned rev:1;
	unsigned :1;
	unsigned res:4;
	// byte 2
	unsigned but_no:5;
	unsigned :3;
	// byte 3
	unsigned axis:8;
} STRUCT_INCREMENTAL;

typedef struct _STRUCT_DIGITAL
{
	// byte 1
	unsigned active:1;
	unsigned master:1;
	unsigned inv:1;
	unsigned :1;
	unsigned mode:4;
	// byte 2
	unsigned pw:8;
	// byte 3
	unsigned rot_type:4;
	unsigned :4;
} STRUCT_DIGITAL;

typedef struct _STRUCT_HAT
{
	// byte 1
	unsigned hat_on:1;
	unsigned mode:3;
	unsigned :4;
	// byte 2
	unsigned mouse_vector:8;
} STRUCT_HAT;

typedef struct _STRUCT_ACE
{
	// byte 1
	unsigned ace_on:1;
	unsigned :7;
	// byte 2
	unsigned ace_type:8;
} STRUCT_ACE;

typedef struct _STRUCT_SHIFTER
{
	// byte 1
	unsigned h_shifter:1;
	unsigned s_shifter:1;
	unsigned max_pos:3;
	unsigned delay:3;
	// byte 2
	unsigned pos1:4;
	unsigned pos2:4;
	// byte 3
	unsigned pos3:4;
	unsigned pos4:4;
	// byte 4
	unsigned pos5:4;
	unsigned pos6:4;
	// byte 5
	unsigned pos7:4;
	unsigned pos8:4;
} STRUCT_SHIFTER;

#define EE_ANALOG_CHANNELS	8
#define EE_ANALOG_BYTES		4
#define EE_DIGITAL_CHANNELS	16
#define EE_DIGITAL_BYTES	3
#define EE_SSI_CHANNELS		3
#define EE_SSI_BYTES		4
#define EE_INC_CHANNELS		1
#define EE_INC_BYTES		3
#define EE_HAT_CHANNELS		2
#define EE_HAT_BYTES		2
#define EE_ACE_CHANNELS		1
#define EE_ACE_BYTES		2
#define EE_HS_BYTES			5

#define ACE_4X		1
#define ACE_AE_SSI	2

#endif