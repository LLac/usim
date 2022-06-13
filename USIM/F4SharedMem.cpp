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
		theApp.m_pBMSFlightData2 = NULL;
	}

	memset(&theApp.m_F4FlightData, 0, sizeof(theApp.m_F4FlightData));
	Sleep(100);	// wait for outputs to update
}

void CF4SharedMem::StopSharedMem2()
{
	if (m_FlightDataMap2 != NULL) {
		CloseHandle(m_FlightDataMap2);
		m_FlightDataMap2 = NULL;
		theApp.m_pBMSFlightData2 = NULL;
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
		theApp.m_pBMSFlightData2 = NULL;

		if (SimType == SIM_F4BMS) {
			theApp.m_pBMSFlightData = (BMSFlightData*)MapViewOfFile(m_FlightDataMap, FILE_MAP_WRITE, 0, 0, 0);
			if (theApp.m_pBMSFlightData == NULL) {	// check to see if we have a valid view
				StopSharedMem();
				return false;
			}
			theApp.m_pBMSFlightData2 = (BMSFlightData2*)MapViewOfFile(m_FlightDataMap2, FILE_MAP_WRITE, 0, 0, 0);
			if (theApp.m_pBMSFlightData2 == NULL) {	// check to see if we have a valid view
				StopSharedMem2();
				return false;
			}
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
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.LOX;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.internalFuel;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.externalFuel;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.fuelFlow;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.fuelFlow2;
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
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.cabinAlt;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hydPressureA;
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.hydPressureB;
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
	theApp.m_pF4VarToken[i++] = (double *)&theApp.m_F4FlightData.miscBits;
	
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
}

void CF4SharedMem::UpdateSharedMem()
{
	if (theApp.m_SimulationModel == SIM_F4BMS && theApp.m_pBMSFlightData != NULL && theApp.m_pBMSFlightData2 != NULL) {
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
		memcpy(theApp.m_F4FlightData.newDetection, theApp.m_pBMSFlightData->newDetection, sizeof(theApp.m_pBMSFlightData->newDetection));

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
		theApp.m_F4FlightData.nozzlePos2 = theApp.m_pBMSFlightData2->nozzlePos2;
		theApp.m_F4FlightData.internalFuel = theApp.m_pBMSFlightData->internalFuel;
		theApp.m_F4FlightData.externalFuel = theApp.m_pBMSFlightData->externalFuel;
		theApp.m_F4FlightData.fuelFlow = theApp.m_pBMSFlightData->fuelFlow;
		theApp.m_F4FlightData.rpm = theApp.m_pBMSFlightData->rpm;
		theApp.m_F4FlightData.rpm2 = theApp.m_pBMSFlightData2->rpm2;
		theApp.m_F4FlightData.ftit = theApp.m_pBMSFlightData->ftit;
		theApp.m_F4FlightData.ftit2 = theApp.m_pBMSFlightData2->ftit2;
		theApp.m_F4FlightData.gearPos = theApp.m_pBMSFlightData->gearPos;
		theApp.m_F4FlightData.speedBrake = theApp.m_pBMSFlightData->speedBrake;
		theApp.m_F4FlightData.epuFuel = theApp.m_pBMSFlightData->epuFuel;
		theApp.m_F4FlightData.oilPressure = theApp.m_pBMSFlightData->oilPressure;
		theApp.m_F4FlightData.oilPressure2 = theApp.m_pBMSFlightData2->oilPressure2;
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
		theApp.m_F4FlightData.MainPower = theApp.m_pBMSFlightData->MainPower;

		// Add the new EXTRA SM section because programmers smarter than me thought it would be a good idea. o_O
		theApp.m_F4FlightData.navMode = (F4FlightData::NavModes)theApp.m_pBMSFlightData2->navMode;
		theApp.m_F4FlightData.AAUZ = theApp.m_pBMSFlightData2->AAUZ;
		theApp.m_F4FlightData.tacanInfo[0] = theApp.m_pBMSFlightData2->tacanInfo[0];
		theApp.m_F4FlightData.tacanInfo[1] = theApp.m_pBMSFlightData2->tacanInfo[1];
		theApp.m_F4FlightData.AltCalReading = theApp.m_pBMSFlightData2->AltCalReading;
		theApp.m_F4FlightData.altBits = theApp.m_pBMSFlightData2->altBits;
		theApp.m_F4FlightData.powerBits = theApp.m_pBMSFlightData2->powerBits;
		theApp.m_F4FlightData.blinkBits = theApp.m_pBMSFlightData2->blinkBits;
		theApp.m_F4FlightData.miscBits = theApp.m_pBMSFlightData2->miscBits;
		theApp.m_F4FlightData.cmdsMode = (F4FlightData::CmdsModes)theApp.m_pBMSFlightData2->cmdsMode;
		theApp.m_F4FlightData.BupUhfPreset = theApp.m_pBMSFlightData2->BupUhfPreset;
		theApp.m_F4FlightData.BupUhfFreq = theApp.m_pBMSFlightData2->BupUhfFreq;
		theApp.m_F4FlightData.cabinAlt = theApp.m_pBMSFlightData2->cabinAlt;
		theApp.m_F4FlightData.hydPressureA = theApp.m_pBMSFlightData2->hydPressureA;
		theApp.m_F4FlightData.hydPressureB = theApp.m_pBMSFlightData2->hydPressureB;
		theApp.m_F4FlightData.currentTime = theApp.m_pBMSFlightData2->currentTime;
		theApp.m_F4FlightData.vehicleACD = theApp.m_pBMSFlightData2->vehicleACD;
		theApp.m_F4FlightData.VersionNum2 = theApp.m_pBMSFlightData2->VersionNum;
		theApp.m_F4FlightData.fuelFlow2 = theApp.m_pBMSFlightData2->fuelFlow2;
		theApp.m_F4FlightData.lefPos = theApp.m_pBMSFlightData2->lefPos;
		theApp.m_F4FlightData.tefPos = theApp.m_pBMSFlightData2->tefPos;
		theApp.m_F4FlightData.vtolPos = theApp.m_pBMSFlightData2->vtolPos;
	}

	// Custom VARs Section

	// PRKMag
	BOOL WOW = (theApp.m_F4FlightData.lightBits3 & F4FlightData::OnGround);
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

	// Liquid Oxygen
	theApp.m_F4FlightData.LOX = (theApp.m_F4FlightData.total / 8400.0F) * 5.0F;
}
