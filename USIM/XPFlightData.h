#ifndef _XPFLIGHT_DATA_H
#define _XPFLIGHT_DATA_H

#define dwMaxWait		5	// 5 ms	for mutex locking
const LPCTSTR sMutexFileName = "4BD625E8-C16F-4836-9F62-60E151CCE3EC";

typedef struct _XPDATA_MEM
{
	XPLMDataRef	DataRef;	// 4
	UCHAR		Active;		// 1
	UCHAR		Write;		// 1
	double		dDataRead;	// 8 typecast all other types
	double		dDataWrite;	// 8 typecast all other types
	UINT		inOffset;	// 1 offset to read in array
} XPDATA_MEM;

typedef struct _XPCMD_MEM
{
	XPLMDataRef	DataRef;	// 4
	UCHAR		Active;		// 1
	UCHAR		Press;		// 1
	UCHAR		Release;	// 1
} XPCMD_MEM;

class XPFlightData
{

public:
	XPCMD_MEM Cmd[ArraySize(XPCmdArray)];
	XPDATA_MEM Data[ArraySize(XPDataArray)];

protected:
	HANDLE m_hMutex;
};

#endif
