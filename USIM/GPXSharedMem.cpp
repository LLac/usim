// GPXSharedMem.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "GPXSharedMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGPXSharedMem

CGPXSharedMem::CGPXSharedMem()
{
	MapVars();	// default for pull down list if not connected
}

CGPXSharedMem::~CGPXSharedMem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGPXSharedMem message handlers

void CGPXSharedMem::StopSharedMem()
{
//	theApp.StopAllThreads();

	if (m_SimDataMap != NULL) {
		CloseHandle(m_SimDataMap);
		m_SimDataMap = NULL;
		theApp.m_pGPXSimData = NULL;
	}

	memset(&theApp.m_GPXSimData, 0, sizeof(theApp.m_GPXSimData));
	Sleep(100);	// wait for outputs to update
//	theApp.StartAllThreads();
}

BOOL CGPXSharedMem::StartSharedMem (UINT nIndex)
{
	m_SimDataMap = NULL;
	m_SimDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "GPxCInfo");

	if (m_SimDataMap != NULL) {	// check if we have a valid mapping
		theApp.m_pGPXSimData = NULL;

		theApp.m_pGPXSimData = (GPXSimData *)MapViewOfFile(m_SimDataMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (theApp.m_pGPXSimData != NULL) {	// check to see if we have a valid view
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

void CGPXSharedMem::MapVars()
{
	int i = 0;

	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.spare;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.flags;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.lap;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.position;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.fuel;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.temp;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.speed;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.splittime;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.gear;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.revs;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.time;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.timeFront;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.timeBehind;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.driverFront;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.driverBehind;
	theApp.m_pGPXVarToken[i++] = (double *)&theApp.m_GPXSimData.m_GPxCInfoData.flag_string;
}

void CGPXSharedMem::UpdateSharedMem()
{
	CString str;

	str.Empty();
	if (theApp.m_pGPXSimData != NULL) {
		if (theApp.m_pGPXSimData->m_GPxCInfoData.flags & GPX_CIF_PITLIGHT)
			str = "Pit ";
		if (theApp.m_pGPXSimData->m_GPxCInfoData.flags & GPX_CIF_REDLED)
			str += "Red ";
		if (theApp.m_pGPXSimData->m_GPxCInfoData.flags & GPX_CIF_YELLOWLED)
			str += "Yel ";
		if (theApp.m_pGPXSimData->m_GPxCInfoData.flags & GPX_CIF_BLUELED)
			str += "Blu ";
		if (str.IsEmpty()) str = " ";
		strcpy(theApp.m_pGPXSimData->m_GPxCInfoData.flag_string, str);

		memcpy(&theApp.m_GPXSimData.m_GPxCInfoData, &theApp.m_pGPXSimData->m_GPxCInfoData, sizeof(GPxCInfoData));
	}
}
