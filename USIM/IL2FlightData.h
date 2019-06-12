#ifndef _IL2FLIGHT_DATA_H
#define _IL2FLIGHT_DATA_H

typedef struct _IL2DATA_MEM
{
	UINT		DataRef;
	UCHAR		Active;
	UCHAR		Write;
	double		dDataRead;	// typecast all other types
	double		dDataWrite;	// typecast all other types
} IL2DATA_MEM;

typedef struct _IL2CMD_MEM
{
	UINT		DataRef;
	UCHAR		Active;
	UCHAR		Press;
	UCHAR		Release;
} IL2CMD_MEM;

class IL2FlightData
{

public:
	IL2DATA_MEM Data[ArraySize(IL2DataArray)];
	IL2CMD_MEM Cmd[ArraySize(IL2CmdArray)];
};

#endif
