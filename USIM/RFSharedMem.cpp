// RFSharedMem.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "RFSharedMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRFSharedMem

CRFSharedMem::CRFSharedMem()
{
	MapVars();	// default for pull down list if not connected
}

CRFSharedMem::~CRFSharedMem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRFSharedMem message handlers

void CRFSharedMem::StopSharedMem()
{
//	theApp.StopAllThreads();

	if (m_SimDataMap != NULL) {
		CloseHandle(m_SimDataMap);
		m_SimDataMap = NULL;
		theApp.m_pRFSimData = NULL;
	}

	memset(&theApp.m_RFSimData, 0, sizeof(theApp.m_RFSimData));
	Sleep(100);	// wait for outputs to update
//	theApp.StartAllThreads();
}

BOOL CRFSharedMem::StartSharedMem (UINT nIndex)
{
	m_SimDataMap = NULL;
	m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "rFactorMemoryArea");

	if (m_SimDataMap != NULL) {	// check if we have a valid mapping
		theApp.m_pRFSimData = NULL;

		theApp.m_pRFSimData = (RFSimData *)MapViewOfFile(m_SimDataMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (theApp.m_pRFSimData != NULL) {	// check to see if we have a valid view
			// Copy the the data from the shared memory file into our local stucture.
			// This is typicaly done in a timer loop, but you might want to optimize
			// doing some kind of notification upon data changed.
			return true;
		} else {
			// do exception handling here
			StopSharedMem();
		}
	}

	return false;
}

void CRFSharedMem::MapVars()
{
	int i = 0;

	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mExpansion[0];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mCurrentET;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mEndET;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mGamePhase;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mLapDist;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mMaxLaps;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mNumRedLights;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mNumVehicles;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mSectorFlag[0];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mSectorFlag[1];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mSectorFlag[2];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mSession;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mStartLight;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mTrackName;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mYellowFlagState;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mAmbientTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mDarkCloud;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mRaining;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_ScoringInfoV2.mTrackTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mDeltaTime;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLapNumber;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLapStartET;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mScheduledStops;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mEngineMaxRPM;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mEngineOilTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mEngineRPM;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mEngineWaterTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mOverheating;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mClutchRPM;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mDetached;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mFuel;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mGear;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLastImpactET;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLocalAccel.x;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLocalAccel.y;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLocalAccel.y;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLocalVel.x;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLocalVel.y;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mLocalVel.z;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mSteeringArmForce;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mUnfilteredThrottle;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mUnfilteredBrake;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mUnfilteredClutch;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[0].mBrakeTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[0].mPressure;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[0].mTemperature[0];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[0].mTemperature[1];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[0].mTemperature[2];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[1].mBrakeTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[1].mPressure;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[1].mTemperature[0];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[1].mTemperature[1];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[1].mTemperature[2];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[2].mBrakeTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[2].mPressure;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[2].mTemperature[0];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[2].mTemperature[1];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[2].mTemperature[2];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[3].mBrakeTemp;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[3].mPressure;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[3].mTemperature[0];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[3].mTemperature[1];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mWheel[3].mTemperature[2];
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mTrackName;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_TelemInfoV2.mVehicleName;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLapsBehindLeader;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLapsBehindNext;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLapStartET;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mTimeBehindLeader;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mTimeBehindNext;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mBestLapTime;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mBestSector1;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mBestSector2;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mControl;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mCurSector1;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mCurSector2;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mDriverName;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mFinishStatus;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mInPits;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mIsPlayer;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLapDist;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLastLapTime;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLastSector1;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mLastSector2;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mNumPenalties;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mNumPitstops;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mPathLateral;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mPlace;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mSector;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mTotalLaps;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mTrackEdge;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mVehicleClass;
	theApp.m_pRFVarToken[i++] = (double *)&theApp.m_RFSimData.m_VehicleScoringInfoV2.mVehicleName;
}

void CRFSharedMem::UpdateSharedMem()
{
	static unsigned int msec_900 = 0;

	if (theApp.m_pRFSimData != NULL) {
		memcpy (&theApp.m_RFSimData.m_TelemInfoV2, &theApp.m_pRFSimData->m_TelemInfoV2, sizeof(TelemInfoV2));
		if (msec_900++ == 90) { // Update these every 900 ms. This sub is called every 10ms in OutputThread
			msec_900 = 0;
			memcpy (&theApp.m_RFSimData.m_ScoringInfoV2, &theApp.m_pRFSimData->m_ScoringInfoV2, sizeof(ScoringInfoV2));
			memcpy (&theApp.m_RFSimData.m_VehicleScoringInfoV2, &theApp.m_pRFSimData->m_VehicleScoringInfoV2, sizeof(VehicleScoringInfoV2));
		}
	}
}
