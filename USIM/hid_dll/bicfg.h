#ifndef BICFG_H
#define BICFG_H

/** D E F I N E S ************************************************************/

#define EEEF_NUM_OUTPUTS_PER_PORT	15
#define NUM_MUX_PER_PORT			56
#define NUM_LATCHED_PER_PORT		64
#define NUM_LATCHED_PER_XG			16
#define NUM_DMX_PER_PORT			4
#define MAX_RWR_SYMBOLS				40
#define EF_BYTES_PER_RWR			4

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

#endif