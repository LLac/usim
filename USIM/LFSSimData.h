#ifndef _LFSSIM_DATA_H
#define _LFSSIM_DATA_H

// OutGauge - EXTERNAL DASHBOARD SUPPORT
// ========

// The user's car in multiplayer or the viewed car in single player or
// single player replay can output information to a dashboard system
// while viewed from an internal view.

// This can be controlled by 5 lines in the cfg.txt file :

// OutGauge Mode 0        :0-off 1-driving 2-driving+replay
// OutGauge Delay 1       :minimum delay between packets (100ths of a sec)
// OutGauge IP 0.0.0.0    :IP address to send the UDP packet
// OutGauge Port 0        :IP port
// OutGauge ID 0          :if not zero, adds an identifier to the packet

// Each update sends the following UDP packet :

typedef unsigned char byte;
typedef unsigned short word;

struct OutGaugePack
{
	unsigned int	Time;		// time in milliseconds (to check order)

	char		Car[4];			// Car name
	word		Flags;			// Info (see OG_x below)
	byte		Gear;			// Reverse:0, Neutral:1, First:2...
	byte		SpareB;
	float		Speed;			// M/S
	float		RPM;			// RPM
	float		Turbo;			// BAR
	float		EngTemp;		// C
	float		Fuel;			// 0 to 1
	float		OilPressure;	// BAR
	float		OilTemp;		// C
	unsigned int	DashLights;		// Dash lights available (see DL_x below)
	unsigned int	ShowLights;		// Dash lights currently switched on
	float		Throttle;		// 0 to 1
	float		Brake;			// 0 to 1
	float		Clutch;			// 0 to 1
	char		Display1[16];	// Usually Fuel
	char		Display2[16];	// Usually Settings

	int			ID;				// optional - only if OutGauge ID is specified
};

// OG_x - bits for OutGaugePack Flags

#define OG_TURBO		8192	// show turbo gauge
#define OG_KM			16384	// if not set - user prefers MILES
#define OG_BAR			32768	// if not set - user prefers PSI

/*
// DL_x - bits for OutGaugePack DashLights and ShowLights

#define DL_SHIFT		0x0001	// bit 0	- shift light
#define DL_FULLBEAM		0x0002	// bit 1	- full beam
#define DL_HANDBRAKE	0x0004	// bit 2	- handbrake
#define DL_PITSPEED		0x0008	// bit 3	- pit speed limiter
#define DL_TC			0x0010	// bit 4	- TC active or switched off
#define DL_SIGNAL_L		0x0020	// bit 5	- left turn signal
#define DL_SIGNAL_R		0x0040	// bit 6	- right turn signal
#define DL_SIGNAL_ANY	0x0080	// bit 7	- shared turn signal
#define DL_OILWARN		0x0100	// bit 8	- oil pressure warning
#define DL_BATTERY		0x0200	// bit 9	- battery warning
#define DL_ABS			0x0400	// bit 10	- ABS active or switched off
*/

class LFSSimData
{
public:
    OutGaugePack	Data;
};


#endif
