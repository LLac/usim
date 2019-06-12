// SimBinSharedMem.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "SimPathTab.h"
#include "SimBinSharedMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimBinSharedMem

CSimBinSharedMem::CSimBinSharedMem()
{
	MapVars();	// default for pull down list if not connected
}

CSimBinSharedMem::~CSimBinSharedMem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSimBinSharedMem message handlers

void CSimBinSharedMem::StopSharedMem()
{
//	theApp.StopAllThreads();

	if (m_SimDataMap != NULL) {
		CloseHandle(m_SimDataMap);
		m_SimDataMap = NULL;
		theApp.m_pSimBinSimData = NULL;
	}

	memset(&theApp.m_SimBinSimData, 0, sizeof(theApp.m_SimBinSimData));
	Sleep(100);	// wait for outputs to update
//	theApp.StartAllThreads();
}

BOOL CSimBinSharedMem::StartSharedMem (UINT nIndex)
{
	m_SimDataMap = NULL;
	// Figure out which Simbin title is running.
	switch (theApp.SimList[nIndex].SimType) {
		case SIM_GTR:
			m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "$simbin$");
			break;

		case SIM_GTR2:
			m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "$gtr2$");
			break;

		case SIM_GTL:
			m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "$gtlegends$");
			break;

		case SIM_EVO:
		case SIM_RACE:
		case SIM_RACE07:
		case SIM_RACEON:
		case SIM_VOLVO:
			m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "$Race$");
			break;
	}
/*
	if (theApp.SimList[nIndex].SimName == "GTR")
		m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , false, "$simbin$");
	else if (theApp.SimList[nIndex].SimName == "GTR2")
		m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , false, "$gtr2$");
	else if (theApp.SimList[nIndex].SimName == "GTR EVO")
		m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , false, "$Race$");
	else if (theApp.SimList[nIndex].SimName == "GTL")
		m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , false, "$gtlegends$");
	else if (theApp.SimList[nIndex].SimName == "RACE")
		m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , false, "$Race$");
	else if (theApp.SimList[nIndex].SimName == "RACE07")
		m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , false, "$Race$");
*/

	if (m_SimDataMap != NULL) {	// check if we have a valid mapping
		theApp.m_pSimBinSimData = NULL;

		theApp.m_pSimBinSimData = (SimBinSimData *)MapViewOfFile(m_SimDataMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (theApp.m_pSimBinSimData != NULL) {	// check to see if we have a valid view
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

void CSimBinSharedMem::MapVars()
{
	int i = 0;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.Padding1[0];
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.simrunning;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mLapTime;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mPosTotal;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mPos;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mEngineRPM;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mOilPressure;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mOilTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mWaterTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFuel;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mGear;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mSpeed;

	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFLWheelTemp.LTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFLWheelTemp.CTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFLWheelTemp.RTemp;

	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFRWheelTemp.LTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFRWheelTemp.CTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mFRWheelTemp.RTemp;

	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mRLWheelTemp.LTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mRLWheelTemp.CTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mRLWheelTemp.RTemp;

	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mRRWheelTemp.LTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mRRWheelTemp.CTemp;
	theApp.m_pSimBinVarToken[i++] = (double *)&theApp.m_SimBinSimData.mRRWheelTemp.RTemp;
}

void CSimBinSharedMem::UpdateSharedMem()
{
	if (theApp.m_pSimBinSimData != NULL) {
		theApp.m_SimBinSimData.simrunning = theApp.m_pSimBinSimData->simrunning;
		theApp.m_SimBinSimData.mLapTime = theApp.m_pSimBinSimData->mLapTime;
		theApp.m_SimBinSimData.mPosTotal = theApp.m_pSimBinSimData->mPosTotal;
		theApp.m_SimBinSimData.mPos = theApp.m_pSimBinSimData->mPos;
		theApp.m_SimBinSimData.mEngineRPM = theApp.m_pSimBinSimData->mEngineRPM;
		theApp.m_SimBinSimData.mOilPressure = theApp.m_pSimBinSimData->mOilPressure;
		theApp.m_SimBinSimData.mOilTemp = theApp.m_pSimBinSimData->mOilTemp;
		theApp.m_SimBinSimData.mWaterTemp = theApp.m_pSimBinSimData->mWaterTemp;
		theApp.m_SimBinSimData.mFuel = theApp.m_pSimBinSimData->mFuel;
		theApp.m_SimBinSimData.mGear = theApp.m_pSimBinSimData->mGear;
		theApp.m_SimBinSimData.mSpeed = theApp.m_pSimBinSimData->mSpeed;
		theApp.m_SimBinSimData.mFLWheelTemp.LTemp = theApp.m_pSimBinSimData->mFLWheelTemp.LTemp;
		theApp.m_SimBinSimData.mFLWheelTemp.CTemp = theApp.m_pSimBinSimData->mFLWheelTemp.CTemp;
		theApp.m_SimBinSimData.mFLWheelTemp.RTemp = theApp.m_pSimBinSimData->mFLWheelTemp.RTemp;
		theApp.m_SimBinSimData.mFRWheelTemp.LTemp = theApp.m_pSimBinSimData->mFRWheelTemp.LTemp;
		theApp.m_SimBinSimData.mFRWheelTemp.CTemp = theApp.m_pSimBinSimData->mFRWheelTemp.CTemp;
		theApp.m_SimBinSimData.mFRWheelTemp.RTemp = theApp.m_pSimBinSimData->mFRWheelTemp.RTemp;
		theApp.m_SimBinSimData.mRLWheelTemp.LTemp = theApp.m_pSimBinSimData->mRLWheelTemp.LTemp;
		theApp.m_SimBinSimData.mRLWheelTemp.CTemp = theApp.m_pSimBinSimData->mRLWheelTemp.CTemp;
		theApp.m_SimBinSimData.mRLWheelTemp.RTemp = theApp.m_pSimBinSimData->mRLWheelTemp.RTemp;
		theApp.m_SimBinSimData.mRRWheelTemp.LTemp = theApp.m_pSimBinSimData->mRRWheelTemp.LTemp;
		theApp.m_SimBinSimData.mRRWheelTemp.CTemp = theApp.m_pSimBinSimData->mRRWheelTemp.CTemp;
		theApp.m_SimBinSimData.mRRWheelTemp.RTemp = theApp.m_pSimBinSimData->mRRWheelTemp.RTemp;
	}
}
