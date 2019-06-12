// XPSharedMem.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "XPSharedMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXPSharedMem

CXPSharedMem::CXPSharedMem()
{

}

CXPSharedMem::~CXPSharedMem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CXPSharedMem message handlers

void CXPSharedMem::StopSharedMem()
{
	theApp.StopAllThreads();

	if (m_FlightDataMap != NULL) {
		CloseHandle(m_FlightDataMap);
		m_FlightDataMap = NULL;
		theApp.m_pXPFlightData = NULL;
	}

	theApp.StartAllThreads();
}

BOOL CXPSharedMem::StartSharedMem()
{
	if (m_FlightDataMap != NULL) {
		theApp.m_SimulationModel = SIM_XPLANE;
		return true;
	}

	m_FlightDataMap = NULL;
	m_FlightDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "XPlaneMemoryArea");

	if (m_FlightDataMap != NULL) {	// check if we have a valid mapping
		theApp.m_pXPFlightData = NULL;

		theApp.m_pXPFlightData = (XPFlightData *)MapViewOfFile(m_FlightDataMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (theApp.m_pXPFlightData != NULL) {	// check to see if we have a valid view
			// Copy the the data from the shared memory file into our local stucture.
			// This is typicaly done in a timer loop, but you might want to optimize
			// doing some kind of notification upon data changed.
			theApp.m_SimulationModel = SIM_XPLANE;
			return true;
		} else {
			// do exception handling here
			StopSharedMem();
		}
	}

	return false;
}

void CXPSharedMem::MapVars()
{
	UINT i;

	if (theApp.m_pXPFlightData == NULL) 
		return;

	for (i=0; i<ArraySize(XPCmdArray); i++) {
		theApp.m_pXPFlightData->Cmd[i].Active = 0;
		theApp.m_pXPFlightData->Cmd[i].Press = 0;
		theApp.m_pXPFlightData->Cmd[i].Release = 0;
	}

	for (i=0; i<ArraySize(XPDataArray); i++) {
		theApp.m_pXPFlightData->Data[i].Active = 0;
		theApp.m_pXPFlightData->Data[i].Write = 0;
		theApp.m_pXPFlightData->Data[i].inOffset = 0;
	}
}
