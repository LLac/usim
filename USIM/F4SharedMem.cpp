// F4SharedMem.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "F4SharedMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CF4SharedMem

CF4SharedMem::CF4SharedMem()
{
	MapVars();	// default for pull down list if not connected
}

CF4SharedMem::~CF4SharedMem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CF4SharedMem message handlers

void CF4SharedMem::StopSharedMem()
{
	if (m_FlightDataMap != NULL) {
		CloseHandle(m_FlightDataMap);
		m_FlightDataMap = NULL;
		theApp.m_pBMSFlightData = NULL;
		theApp.m_pAFFlightData = NULL;
	}

	memset(&theApp.m_F4FlightData, 0, sizeof(theApp.m_F4FlightData));
	Sleep(100);	// wait for outputs to update
}

void CF4SharedMem::StopSharedMem2()
{
	if (m_FlightDataMap2 != NULL) {
		CloseHandle(m_FlightDataMap2);
		m_FlightDataMap2 = NULL;
		theApp.m_pFlightData2 = NULL;
	}

	memset(&theApp.m_F4FlightData, 0, sizeof(theApp.m_F4FlightData));
	Sleep(100);	// wait for outputs to update
}

BOOL CF4SharedMem::StartSharedMem (UINT SimType)
{
	m_FlightDataMap = NULL;
	m_FlightDataMap = OpenFileMapping(FILE_MAP_WRITE , true, "FalconSharedMemoryArea");

	m_FlightDataMap2 = NULL;
	m_FlightDataMap2 = OpenFileMapping(FILE_MAP_WRITE , true, "FalconSharedMemoryArea2");

	if (m_FlightDataMap != NULL) {	// check if we have a valid mapping
		theApp.m_pBMSFlightData = NULL;
		theApp.m_pAFFlightData = NULL;
		theApp.m_pFlightData2 = NULL;

		switch (SimType) {
			case SIM_F4FF:
				theApp.m_pBMSFlightData = (BMSFlightData*)MapViewOfFile(m_FlightDataMap, FILE_MAP_WRITE, 0, 0, 0);
				if (theApp.m_pBMSFlightData == NULL) {	// check to see if we have a valid view
					StopSharedMem();
					return false;
				}
				break;

			case SIM_F4BMS:
				theApp.m_pFlightData2 = (FlightData2*)MapViewOfFile(m_FlightDataMap2, FILE_MAP_WRITE, 0, 0, 0);
				if (theApp.m_pFlightData2 == NULL) {	// check to see if we have a valid view
					StopSharedMem2();
//					return false;
				}
			case SIM_F4AF:
			case SIM_F4OF:
				theApp.m_pAFFlightData = (AFFlightData*)MapViewOfFile(m_FlightDataMap, FILE_MAP_WRITE, 0, 0, 0);
				if (theApp.m_pAFFlightData == NULL) {	// check to see if we have a valid view
					StopSharedMem();
					return false;
				}
				break;
		}
	}

	return true;
}

void CF4SharedMem::MapVars()
{
	// These pointers must be in the same order as F4DataArray[] in F4VarList.h
	int i = 0;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.NONE;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.x;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.y;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.z;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.xDot;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.yDot;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.zDot;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.alpha;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.beta;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.gamma;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.pitch;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.roll;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.yaw;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.mach;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.kias;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.vt;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.gs;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.windOffset;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.AAUZ;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.AltCalReading;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.nozzlePos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.nozzlePos2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.USIMnozzlePos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.LOX;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.internalFuel;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.externalFuel;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.fuelFlow;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.rpm;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.rpm2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.ftit;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.ftit2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.gearPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.speedBrake;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.epuFuel;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.oilPressure;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.oilPressure2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.fwd;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.aft;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.total;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hydA;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hydB;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.CabinPressure;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.cabinAlt;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hydPressureA;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hydPressureB;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.fuelFlow2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.lefPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.tefPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.vtolPos;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.TrimPitch;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.TrimRoll;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.TrimYaw;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.ChaffCount;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.FlareCount;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.NoseGearPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.LeftGearPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.RightGearPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.cmdsMode;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.AdiIlsHorPos;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.AdiIlsVerPos;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.courseState;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.headingState;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.totalStates;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.courseDeviation;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.desiredCourse;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.distanceToBeacon;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.bearingToBeacon;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.currentHeading;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.desiredHeading;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.deviationLimit;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.halfDeviationLimit;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.localizerCourse;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.airbaseX;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.airbaseY;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.totalValues;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hsiBits;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.navMode;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.UFCTChan;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.AUXTChan;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.tacanInfo[0];
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.tacanInfo[1];
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.BupUhfPreset;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.BupUhfFreq;

	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.MainPower;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.lightBits;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.lightBits2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.lightBits3;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.PRKMag;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.altBits;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.powerBits;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.blinkBits;
	
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.DEDLines[0];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.DEDLines[1];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.DEDLines[2];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.DEDLines[3];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.DEDLines[4];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.PFLLines[0];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.PFLLines[1];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.PFLLines[2];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.PFLLines[3];
	theApp.m_pF4VarToken[i++] = (double *)theApp.m_F4FlightData.PFLLines[4];
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.O1;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.O2;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.O3Chaff;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.O4Flare;
	
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.currentTime;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.vehicleACD;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.VersionNum2;
}

void CF4SharedMem::UpdateSharedMem()
{
	if (theApp.m_SimulationModel == SIM_F4FF && theApp.m_pBMSFlightData != NULL) {

		theApp.m_F4FlightData.VersionNum = theApp.m_pBMSFlightData->VersionNum;

		memcpy(theApp.m_F4FlightData.DEDLines, theApp.m_pBMSFlightData->DEDLines, 5*26);
		memcpy(theApp.m_F4FlightData.Invert, theApp.m_pBMSFlightData->Invert, 5*26);
		memcpy(theApp.m_F4FlightData.PFLLines, theApp.m_pBMSFlightData->PFLLines, 5*26);
		memcpy(theApp.m_F4FlightData.PFLInvert, theApp.m_pBMSFlightData->PFLInvert, 5*26);

		theApp.m_F4FlightData.RwrObjectCount = theApp.m_pBMSFlightData->RwrObjectCount;
		memcpy(theApp.m_F4FlightData.RWRsymbol, theApp.m_pBMSFlightData->RWRsymbol, sizeof(theApp.m_pBMSFlightData->RWRsymbol));
		memcpy(theApp.m_F4FlightData.bearing, theApp.m_pBMSFlightData->bearing, sizeof(theApp.m_pBMSFlightData->bearing));
		memcpy(theApp.m_F4FlightData.missileActivity, theApp.m_pBMSFlightData->missileActivity, sizeof(theApp.m_pBMSFlightData->missileActivity));
		memcpy(theApp.m_F4FlightData.missileLaunch, theApp.m_pBMSFlightData->missileLaunch, sizeof(theApp.m_pBMSFlightData->missileLaunch));
		memcpy(theApp.m_F4FlightData.selected, theApp.m_pBMSFlightData->selected, sizeof(theApp.m_pBMSFlightData->selected));
		memcpy(theApp.m_F4FlightData.lethality, theApp.m_pBMSFlightData->lethality, sizeof(theApp.m_pBMSFlightData->lethality));

		theApp.m_F4FlightData.x = theApp.m_pBMSFlightData->x;
		theApp.m_F4FlightData.y = theApp.m_pBMSFlightData->y;
		theApp.m_F4FlightData.z = theApp.m_pBMSFlightData->z;
		theApp.m_F4FlightData.xDot = theApp.m_pBMSFlightData->xDot;
		theApp.m_F4FlightData.yDot = theApp.m_pBMSFlightData->yDot;
		theApp.m_F4FlightData.zDot = theApp.m_pBMSFlightData->zDot;
		theApp.m_F4FlightData.alpha = theApp.m_pBMSFlightData->alpha;
		theApp.m_F4FlightData.beta = theApp.m_pBMSFlightData->beta;
		theApp.m_F4FlightData.gamma = theApp.m_pBMSFlightData->gamma;
		theApp.m_F4FlightData.pitch = theApp.m_pBMSFlightData->pitch;
		theApp.m_F4FlightData.roll = theApp.m_pBMSFlightData->roll;
		theApp.m_F4FlightData.yaw = theApp.m_pBMSFlightData->yaw;
		theApp.m_F4FlightData.mach = theApp.m_pBMSFlightData->mach;
		theApp.m_F4FlightData.kias = theApp.m_pBMSFlightData->kias;
		theApp.m_F4FlightData.vt = theApp.m_pBMSFlightData->vt;
		theApp.m_F4FlightData.gs = theApp.m_pBMSFlightData->gs;
		theApp.m_F4FlightData.windOffset = theApp.m_pBMSFlightData->windOffset;
		theApp.m_F4FlightData.nozzlePos = theApp.m_pBMSFlightData->nozzlePos;
		theApp.m_F4FlightData.nozzlePos2 = theApp.m_pBMSFlightData->nozzlePos2;
		theApp.m_F4FlightData.internalFuel = theApp.m_pBMSFlightData->internalFuel;
		theApp.m_F4FlightData.externalFuel = theApp.m_pBMSFlightData->externalFuel;
		theApp.m_F4FlightData.fuelFlow = theApp.m_pBMSFlightData->fuelFlow;
		theApp.m_F4FlightData.rpm = theApp.m_pBMSFlightData->rpm;
		theApp.m_F4FlightData.rpm2 = theApp.m_pBMSFlightData->rpm2;
		theApp.m_F4FlightData.ftit = theApp.m_pBMSFlightData->ftit;
		theApp.m_F4FlightData.ftit2 = theApp.m_pBMSFlightData->ftit2;
		theApp.m_F4FlightData.gearPos = theApp.m_pBMSFlightData->gearPos;
		theApp.m_F4FlightData.speedBrake = theApp.m_pBMSFlightData->speedBrake;
		theApp.m_F4FlightData.epuFuel = theApp.m_pBMSFlightData->epuFuel;
		theApp.m_F4FlightData.oilPressure = theApp.m_pBMSFlightData->oilPressure;
		theApp.m_F4FlightData.oilPressure2 = theApp.m_pBMSFlightData->oilPressure2;
		theApp.m_F4FlightData.lightBits = theApp.m_pBMSFlightData->lightBits;
		theApp.m_F4FlightData.lightBits2 = theApp.m_pBMSFlightData->lightBits2;
		theApp.m_F4FlightData.lightBits3 = theApp.m_pBMSFlightData->lightBits3;
		theApp.m_F4FlightData.ChaffCount = theApp.m_pBMSFlightData->ChaffCount;
		theApp.m_F4FlightData.FlareCount = theApp.m_pBMSFlightData->FlareCount;
		theApp.m_F4FlightData.NoseGearPos = theApp.m_pBMSFlightData->NoseGearPos;
		theApp.m_F4FlightData.LeftGearPos = theApp.m_pBMSFlightData->LeftGearPos;
		theApp.m_F4FlightData.RightGearPos = theApp.m_pBMSFlightData->RightGearPos;
		theApp.m_F4FlightData.AdiIlsHorPos = theApp.m_pBMSFlightData->AdiIlsHorPos;
		theApp.m_F4FlightData.AdiIlsVerPos = theApp.m_pBMSFlightData->AdiIlsVerPos;
		theApp.m_F4FlightData.courseState = theApp.m_pBMSFlightData->courseState;
		theApp.m_F4FlightData.headingState = theApp.m_pBMSFlightData->headingState;
		theApp.m_F4FlightData.totalStates = theApp.m_pBMSFlightData->totalStates;
		theApp.m_F4FlightData.courseDeviation = theApp.m_pBMSFlightData->courseDeviation;
		theApp.m_F4FlightData.desiredCourse = theApp.m_pBMSFlightData->desiredCourse;
		theApp.m_F4FlightData.distanceToBeacon = theApp.m_pBMSFlightData->distanceToBeacon;
		theApp.m_F4FlightData.bearingToBeacon = theApp.m_pBMSFlightData->bearingToBeacon;
		theApp.m_F4FlightData.currentHeading = theApp.m_pBMSFlightData->currentHeading;
		theApp.m_F4FlightData.desiredHeading = theApp.m_pBMSFlightData->desiredHeading;
		theApp.m_F4FlightData.deviationLimit = theApp.m_pBMSFlightData->deviationLimit;
		theApp.m_F4FlightData.halfDeviationLimit = theApp.m_pBMSFlightData->halfDeviationLimit;
		theApp.m_F4FlightData.localizerCourse = theApp.m_pBMSFlightData->localizerCourse;
		theApp.m_F4FlightData.airbaseX = theApp.m_pBMSFlightData->airbaseX;
		theApp.m_F4FlightData.airbaseY = theApp.m_pBMSFlightData->airbaseY;
		theApp.m_F4FlightData.totalValues = theApp.m_pBMSFlightData->totalValues;
		theApp.m_F4FlightData.hsiBits = theApp.m_pBMSFlightData->hsiBits;
		theApp.m_F4FlightData.TrimPitch = theApp.m_pBMSFlightData->TrimPitch;
		theApp.m_F4FlightData.TrimRoll = theApp.m_pBMSFlightData->TrimRoll;
		theApp.m_F4FlightData.TrimYaw = theApp.m_pBMSFlightData->TrimYaw;
		theApp.m_F4FlightData.UFCTChan = theApp.m_pBMSFlightData->UFCTChan;
		theApp.m_F4FlightData.AUXTChan = theApp.m_pBMSFlightData->AUXTChan;
		theApp.m_F4FlightData.fwd = theApp.m_pBMSFlightData->fwd;
		theApp.m_F4FlightData.aft = theApp.m_pBMSFlightData->aft;
		theApp.m_F4FlightData.total = theApp.m_pBMSFlightData->total;
		theApp.m_F4FlightData.MainPower = (theApp.m_F4FlightData.lightBits3 & F4FlightData::Power_Off ? 1:0);
	} else 	if ((theApp.m_SimulationModel == SIM_F4BMS ||
				theApp.m_SimulationModel == SIM_F4AF || 
				theApp.m_SimulationModel == SIM_F4OF) 
				&& theApp.m_pAFFlightData != NULL) {
		if (theApp.m_SimulationModel == SIM_F4BMS && theApp.m_pFlightData2 != NULL) {
			// Add the new EXTRA SM section because programmers smarter than me thought it would be a good idea. o_O
			theApp.m_F4FlightData.navMode = theApp.m_pFlightData2->navMode;
			theApp.m_F4FlightData.AAUZ = theApp.m_pFlightData2->AAUZ;
			theApp.m_F4FlightData.tacanInfo[0] = theApp.m_pFlightData2->tacanInfo[0];
			theApp.m_F4FlightData.tacanInfo[1] = theApp.m_pFlightData2->tacanInfo[1];
			theApp.m_F4FlightData.AltCalReading = theApp.m_pFlightData2->AltCalReading;
			theApp.m_F4FlightData.altBits = theApp.m_pFlightData2->altBits;
			theApp.m_F4FlightData.powerBits = theApp.m_pFlightData2->powerBits;
			theApp.m_F4FlightData.blinkBits = theApp.m_pFlightData2->blinkBits;
			theApp.m_F4FlightData.cmdsMode = theApp.m_pFlightData2->cmdsMode;
			theApp.m_F4FlightData.BupUhfPreset = theApp.m_pFlightData2->BupUhfPreset;
			theApp.m_F4FlightData.BupUhfFreq = theApp.m_pFlightData2->BupUhfFreq;
			theApp.m_F4FlightData.cabinAlt = theApp.m_pFlightData2->cabinAlt;
			theApp.m_F4FlightData.hydPressureA = theApp.m_pFlightData2->hydPressureA;
			theApp.m_F4FlightData.hydPressureB = theApp.m_pFlightData2->hydPressureB;
			theApp.m_F4FlightData.currentTime = theApp.m_pFlightData2->currentTime;
			theApp.m_F4FlightData.vehicleACD = theApp.m_pFlightData2->vehicleACD;
			theApp.m_F4FlightData.VersionNum2 = theApp.m_pFlightData2->VersionNum;
			theApp.m_F4FlightData.fuelFlow2 = theApp.m_pFlightData2->fuelFlow2;
			theApp.m_F4FlightData.lefPos = theApp.m_pFlightData2->lefPos;
			theApp.m_F4FlightData.tefPos = theApp.m_pFlightData2->tefPos;
			theApp.m_F4FlightData.vtolPos = theApp.m_pFlightData2->vtolPos;
		}

		theApp.m_F4FlightData.VersionNum = theApp.m_pAFFlightData->VersionNum;

		memcpy(theApp.m_F4FlightData.DEDLines, theApp.m_pAFFlightData->DEDLines, 5*26);
		memcpy(theApp.m_F4FlightData.Invert, theApp.m_pAFFlightData->Invert, 5*26);
		memcpy(theApp.m_F4FlightData.PFLLines, theApp.m_pAFFlightData->PFLLines, 5*26);
		memcpy(theApp.m_F4FlightData.PFLInvert, theApp.m_pAFFlightData->PFLInvert, 5*26);

		theApp.m_F4FlightData.RwrObjectCount = theApp.m_pAFFlightData->RwrObjectCount;
		memcpy(theApp.m_F4FlightData.RWRsymbol, theApp.m_pAFFlightData->RWRsymbol, sizeof(theApp.m_pAFFlightData->RWRsymbol));
		memcpy(theApp.m_F4FlightData.bearing, theApp.m_pAFFlightData->bearing, sizeof(theApp.m_pAFFlightData->bearing));
		memcpy(theApp.m_F4FlightData.missileActivity, theApp.m_pAFFlightData->missileActivity, sizeof(theApp.m_pAFFlightData->missileActivity));
		memcpy(theApp.m_F4FlightData.missileLaunch, theApp.m_pAFFlightData->missileLaunch, sizeof(theApp.m_pAFFlightData->missileLaunch));
		memcpy(theApp.m_F4FlightData.selected, theApp.m_pAFFlightData->selected, sizeof(theApp.m_pAFFlightData->selected));
		memcpy(theApp.m_F4FlightData.lethality, theApp.m_pAFFlightData->lethality, sizeof(theApp.m_pAFFlightData->lethality));
		memcpy(theApp.m_F4FlightData.newDetection, theApp.m_pAFFlightData->newDetection, sizeof(theApp.m_pAFFlightData->newDetection));

		theApp.m_F4FlightData.x = theApp.m_pAFFlightData->x;
		theApp.m_F4FlightData.y = theApp.m_pAFFlightData->y;
		theApp.m_F4FlightData.z = theApp.m_pAFFlightData->z;
		theApp.m_F4FlightData.xDot = theApp.m_pAFFlightData->xDot;
		theApp.m_F4FlightData.yDot = theApp.m_pAFFlightData->yDot;
		theApp.m_F4FlightData.zDot = theApp.m_pAFFlightData->zDot;
		theApp.m_F4FlightData.alpha = theApp.m_pAFFlightData->alpha;
		theApp.m_F4FlightData.beta = theApp.m_pAFFlightData->beta;
		theApp.m_F4FlightData.gamma = theApp.m_pAFFlightData->gamma;
		theApp.m_F4FlightData.pitch = theApp.m_pAFFlightData->pitch;
		theApp.m_F4FlightData.roll = theApp.m_pAFFlightData->roll;
		theApp.m_F4FlightData.yaw = theApp.m_pAFFlightData->yaw;
		theApp.m_F4FlightData.mach = theApp.m_pAFFlightData->mach;
		theApp.m_F4FlightData.kias = theApp.m_pAFFlightData->kias;
		theApp.m_F4FlightData.vt = theApp.m_pAFFlightData->vt;
		theApp.m_F4FlightData.gs = theApp.m_pAFFlightData->gs;
		theApp.m_F4FlightData.windOffset = theApp.m_pAFFlightData->windOffset;
		theApp.m_F4FlightData.nozzlePos = theApp.m_pAFFlightData->nozzlePos;
		theApp.m_F4FlightData.nozzlePos2 = 0;
		theApp.m_F4FlightData.internalFuel = theApp.m_pAFFlightData->internalFuel;
		theApp.m_F4FlightData.externalFuel = theApp.m_pAFFlightData->externalFuel;
		theApp.m_F4FlightData.fuelFlow = theApp.m_pAFFlightData->fuelFlow;
		theApp.m_F4FlightData.rpm = theApp.m_pAFFlightData->rpm;
		theApp.m_F4FlightData.rpm2 = 0;
		theApp.m_F4FlightData.ftit = theApp.m_pAFFlightData->ftit;
		theApp.m_F4FlightData.ftit2 = 0;
		theApp.m_F4FlightData.gearPos = theApp.m_pAFFlightData->gearPos;
		theApp.m_F4FlightData.speedBrake = theApp.m_pAFFlightData->speedBrake;
		theApp.m_F4FlightData.epuFuel = theApp.m_pAFFlightData->epuFuel;
		theApp.m_F4FlightData.oilPressure = theApp.m_pAFFlightData->oilPressure;
		theApp.m_F4FlightData.oilPressure2 = 0;
		theApp.m_F4FlightData.lightBits = theApp.m_pAFFlightData->lightBits;
		theApp.m_F4FlightData.lightBits2 = theApp.m_pAFFlightData->lightBits2;
		theApp.m_F4FlightData.lightBits3 = theApp.m_pAFFlightData->lightBits3;
		theApp.m_F4FlightData.ChaffCount = theApp.m_pAFFlightData->ChaffCount;
		theApp.m_F4FlightData.FlareCount = theApp.m_pAFFlightData->FlareCount;
		theApp.m_F4FlightData.NoseGearPos = theApp.m_pAFFlightData->NoseGearPos;
		theApp.m_F4FlightData.LeftGearPos = theApp.m_pAFFlightData->LeftGearPos;
		theApp.m_F4FlightData.RightGearPos = theApp.m_pAFFlightData->RightGearPos;
		theApp.m_F4FlightData.AdiIlsHorPos = theApp.m_pAFFlightData->AdiIlsHorPos;
		theApp.m_F4FlightData.AdiIlsVerPos = theApp.m_pAFFlightData->AdiIlsVerPos;
		theApp.m_F4FlightData.courseState = theApp.m_pAFFlightData->courseState;
		theApp.m_F4FlightData.headingState = theApp.m_pAFFlightData->headingState;
		theApp.m_F4FlightData.totalStates = theApp.m_pAFFlightData->totalStates;
		theApp.m_F4FlightData.courseDeviation = theApp.m_pAFFlightData->courseDeviation;
		theApp.m_F4FlightData.desiredCourse = theApp.m_pAFFlightData->desiredCourse;
		theApp.m_F4FlightData.distanceToBeacon = theApp.m_pAFFlightData->distanceToBeacon;
		theApp.m_F4FlightData.bearingToBeacon = theApp.m_pAFFlightData->bearingToBeacon;
		theApp.m_F4FlightData.currentHeading = theApp.m_pAFFlightData->currentHeading;
		theApp.m_F4FlightData.desiredHeading = theApp.m_pAFFlightData->desiredHeading;
		theApp.m_F4FlightData.deviationLimit = theApp.m_pAFFlightData->deviationLimit;
		theApp.m_F4FlightData.halfDeviationLimit = theApp.m_pAFFlightData->halfDeviationLimit;
		theApp.m_F4FlightData.localizerCourse = theApp.m_pAFFlightData->localizerCourse;
		theApp.m_F4FlightData.airbaseX = theApp.m_pAFFlightData->airbaseX;
		theApp.m_F4FlightData.airbaseY = theApp.m_pAFFlightData->airbaseY;
		theApp.m_F4FlightData.totalValues = theApp.m_pAFFlightData->totalValues;
		theApp.m_F4FlightData.hsiBits = theApp.m_pAFFlightData->hsiBits;
		theApp.m_F4FlightData.TrimPitch = theApp.m_pAFFlightData->TrimPitch;
		theApp.m_F4FlightData.TrimRoll = theApp.m_pAFFlightData->TrimRoll;
		theApp.m_F4FlightData.TrimYaw = theApp.m_pAFFlightData->TrimYaw;
		theApp.m_F4FlightData.UFCTChan = theApp.m_pAFFlightData->UFCTChan;
		theApp.m_F4FlightData.AUXTChan = theApp.m_pAFFlightData->AUXTChan;
		theApp.m_F4FlightData.fwd = theApp.m_pAFFlightData->fwd;
		theApp.m_F4FlightData.aft = theApp.m_pAFFlightData->aft;
		theApp.m_F4FlightData.total = theApp.m_pAFFlightData->total;
		if (theApp.m_pAFFlightData->MainPower || (theApp.m_F4FlightData.lightBits3 & F4FlightData::PowerOffFlag))
			theApp.m_F4FlightData.MainPower = 1;
		else
			theApp.m_F4FlightData.MainPower = 0;
	}

	// Custom VARs Section

	// HYD A & B
	int EPUFlag = theApp.m_F4FlightData.lightBits2 & F4FlightData::EPUOn;

	if (theApp.m_F4FlightData.lightBits2 & F4FlightData::HYD) {
		theApp.m_F4FlightData.hydB = theApp.m_F4FlightData.hydA = 0;
	} else if (!EPUFlag && theApp.m_F4FlightData.rpm < 40) {
		theApp.m_F4FlightData.hydB = theApp.m_F4FlightData.hydA = 10*theApp.m_F4FlightData.rpm;
	} else if (EPUFlag && theApp.m_F4FlightData.rpm < 40) {
		theApp.m_F4FlightData.hydB = 10*theApp.m_F4FlightData.rpm;
		theApp.m_F4FlightData.hydA = 3200;
	} else if (theApp.m_F4FlightData.rpm >= 40) {
		theApp.m_F4FlightData.hydB = theApp.m_F4FlightData.hydA = 3500;
	}

	// Cabin Pressure
	if (theApp.m_F4FlightData.z > -8000 || theApp.m_F4FlightData.lightBits & F4FlightData::CAN) {
		theApp.m_F4FlightData.CabinPressure = -theApp.m_F4FlightData.z;
	} else if (theApp.m_F4FlightData.z > -23000) {
		theApp.m_F4FlightData.CabinPressure = 8000;
	} else {
		// Ambient Pressure + 5 PSI (Differential) 
		theApp.m_F4FlightData.CabinPressure = (float)(-2300 - 0.46*theApp.m_F4FlightData.z);
	}

	// PRKMag
	BOOL WOW = (theApp.m_F4FlightData.lightBits3 & F4FlightData::OnGround) || (theApp.m_F4FlightData.lightBits & F4FlightData::WOW);
	if ((theApp.m_F4FlightData.lightBits3 & F4FlightData::MainGen) == 0 ||
		(theApp.m_F4FlightData.lightBits3 & F4FlightData::StbyGen) == 0 ||
		!WOW ||	theApp.m_F4FlightData.rpm > 83) {
		// turn coil OFF
		theApp.m_F4FlightData.PRKMag = 0;
	} else if ((theApp.m_F4FlightData.lightBits3 & F4FlightData::MainGen) > 0 && 
		(theApp.m_F4FlightData.lightBits3 & F4FlightData::StbyGen) > 0 &&
		WOW && theApp.m_F4FlightData.rpm < 83) {
		// turn coil ON
		theApp.m_F4FlightData.PRKMag = 1;
	}

	// OS1, OS2, OS3Chaff, OS4Flare
	CString str;
	memset(theApp.m_F4FlightData.O1, 0, 5);
	memset(theApp.m_F4FlightData.O2, 0, 5);
	memset(theApp.m_F4FlightData.O3Chaff, 0, 5);
	memset(theApp.m_F4FlightData.O4Flare, 0, 5);

	if (theApp.m_F4FlightData.lightBits2 & F4FlightData::Degr) {
		// display "Auto Degr" on all OSRAMS
		str = "Auto";
		memcpy(theApp.m_F4FlightData.O2, str, 4);
		
		str = "Degr";
		memcpy(theApp.m_F4FlightData.O3Chaff, str, 4);
	} else {
		str = "Lo 0";
		memcpy(theApp.m_F4FlightData.O1, str, 4);
		memcpy(theApp.m_F4FlightData.O2, str, 4);

		if (theApp.m_F4FlightData.lightBits2 & F4FlightData::ChaffLo)
			str.Format("Lo%2d", (UCHAR)(theApp.m_F4FlightData.ChaffCount >= 0 ? theApp.m_F4FlightData.ChaffCount:0));
		else
			str.Format(" %.3d", (UCHAR)(theApp.m_F4FlightData.ChaffCount >= 0 ? theApp.m_F4FlightData.ChaffCount:0));
		memcpy(theApp.m_F4FlightData.O3Chaff, str, 4);

		if (theApp.m_F4FlightData.lightBits2 & F4FlightData::FlareLo)
			str.Format("Lo%2d", (UCHAR)(theApp.m_F4FlightData.FlareCount >= 0 ? theApp.m_F4FlightData.FlareCount:0));
		else
			str.Format(" %.3d", (UCHAR)(theApp.m_F4FlightData.FlareCount >= 0 ? theApp.m_F4FlightData.FlareCount:0));
		memcpy(theApp.m_F4FlightData.O4Flare, str, 4);		
	}

	// USIM NOZ
	float rpmVal = theApp.m_F4FlightData.rpm;

	if (rpmVal <= 10.0F) {
		rpmVal   = 90.0F;
	} else if (rpmVal <= 20.0F) {
		rpmVal   = 90.0F + ((rpmVal - 20.0F) / 20.0F) * 30.0F;
	} else if (rpmVal <= 70.0F) {
		rpmVal   = 70.0F + ((rpmVal - 70.0F) / 70.0F) * 52.0F;
	} else if (rpmVal <= 90.0F) {
		rpmVal   = 0.0F;
	} else if (rpmVal < 97.0F) {
		rpmVal   = 30.0F;
	} else if (rpmVal >= 97.0F && theApp.m_F4FlightData.fuelFlow >= 30000.0F) {
		rpmVal   = 100.0F;
	} else {
		rpmVal   = 30.0F;
	}

	// updated every 10 ms from Output Thread
	theApp.m_F4FlightData.USIMnozzlePos = theApp.m_F4FlightData.USIMnozzlePos + (rpmVal - theApp.m_F4FlightData.USIMnozzlePos) * 0.1F;

	// Liquid Oxygen
	theApp.m_F4FlightData.LOX = (theApp.m_F4FlightData.total / 8400.0F) * 5.0F;
}
