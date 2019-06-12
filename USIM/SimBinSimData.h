#ifndef _SIMBINSIM_DATA_H
#define _SIMBINSIM_DATA_H

struct WheelStruct
{
  float LTemp;	// Left temp (deg C)
  float CTemp;	// Left temp (deg C)
  float RTemp;	// Left temp (deg C)
};

class SimBinSimData
{

public:
	long	Padding1[6];
	float	mEngineRPM;		// RPM/9.548
	float	Padding2[2];
	float	mFuel;			// (?)
	float	Padding3;
	float	mWaterTemp;		// (deg C)
	float	mOilTemp;		// (deg C)
	float	mOilPressure;	// (KPA)
	float	mSpeed;			// (M/S) *3.6=KPH, *2.237=MPH
	long	Padding4;
	long	simrunning;
	long	Padding5[2];
	float	mLapTime;		// (sec)
	long	mPos;
	long	mPosTotal;
	long	mGear;
	WheelStruct	mFLWheelTemp;	// Front Left temp (deg C)
	WheelStruct	mFRWheelTemp;	// Front Right temp (deg C)
	WheelStruct	mRLWheelTemp;	// Rear Left temp (deg C)
	WheelStruct	mRRWheelTemp;	// Rear Right temp (deg C
};

#endif
