// InputThread.cpp: implementation of the CInputThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "InputThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CInputThread, CThread)

CInputThread::CInputThread(void* pOwnerObject, LPARAM lParam) 
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(CInputThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_deltaT_SKW = m_deltaT_IW = 0;
	m_Run = FALSE;
}

CInputThread::~CInputThread()
{
	m_deltaT_SKW = m_deltaT_IW = 0;
}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD CInputThread::ThreadHandler()
{
	BOOL bCanContinue = true;
	int nIncomingCommand;

#ifdef _DEBUG
		TRACE ("\nInput Thread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 10);

		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
//			if (theApp.m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
				DoInputWorker();
			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
//			if (theApp.m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
				DoInputWorker();
			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_STOP:
			m_Run = FALSE;
			bCanContinue = FALSE;
			break;

		default:
			break;
		};

	} while (bCanContinue);

#ifdef _DEBUG
		TRACE ("\nInputThread killed!\n");
#endif

	return 11;	// ... if Thread task completition OK
}

void CInputThread::DoInputWorker()
{
	UINT dIndex;
	long retval;
	POSITION pos;

	// 10 ms elapsed
	if ( (timeGetTime() - m_deltaT_IW) < 10) 
		return;

	m_deltaT_IW = timeGetTime();

	if (theApp.m_SimulationModel == SIM_NONE) return;

	// read inputs from HID devices
	Lock();

	// read from HID devices
	pos = theApp.m_DevArrayList.GetHeadPosition();
	while (pos != NULL) {
		dIndex = theApp.m_DevArrayList.GetNext(pos);

		if (theApp.m_pDevArray[dIndex]->m_Connected &&	// This condition needs fix'n with non polled input devices
			theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices > 0) {

			retval = ReadInputData(	&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
									(char *)theApp.m_pDevArray[dIndex]->m_Inputs, 
									FALSE);

			if (retval == DEV_INPUT) {
				ReadInputs(dIndex);
			} else if (retval == DEV_FAILED) {
				theApp.m_pDevArray[dIndex]->m_Connected = false;
				// write to log file
				CString str;
				str.Format("%susim.log", theApp.m_AppPath);
				FILE* stream = fopen(str, "a");
				fprintf(stream, "Device %s Failed! Returned error code : %d\n", theApp.m_pDevArray[dIndex]->m_DeviceParam.DeviceName, retval);
				fclose(stream);
			} else if (retval == DEV_TIMEOUT) {
				TRACE("InputThread %s - DEV_TIMEOUT\n", theApp.m_pDevArray[dIndex]->m_DeviceParam.DeviceName);

				CString str;
				str.Format("%susim.log", theApp.m_AppPath);
				FILE* stream = fopen(str, "a");
				fprintf(stream, "InputThread %s - DEV_TIMEOUT\n", theApp.m_pDevArray[dIndex]->m_DeviceParam.DeviceName);
				fclose(stream);
			}
		}
	} // while (pos != NULL)

	Unlock();
}

void CInputThread::ReadInputs(const UINT dIndex)
{
	UINT iIndex;
	BOOL IsSet = 0;
	
	for (iIndex = 0; iIndex < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices; iIndex++) {
		// if active && set
		if (theApp.m_pDevArray[dIndex]->m_InputActive[iIndex] && !theApp.m_pDevArray[dIndex]->m_InputDisabled[iIndex]) {
			IsSet = InputState(dIndex, iIndex);
		}
	}
}

BOOL CInputThread::InputState(UINT dIndex, UINT iIndex)
{
	BOOL retval = false;

	// if toggled
	if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] != theApp.m_pDevArray[dIndex]->m_InputsOld[iIndex]) {
		// Toggled. Save old value
		theApp.m_pDevArray[dIndex]->m_InputsOld[iIndex] = theApp.m_pDevArray[dIndex]->m_Inputs[iIndex];
		retval = theApp.m_pDevArray[dIndex]->m_Inputs[iIndex];

		if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1)
			VariablesOnPress(&theApp.m_pDevArray[dIndex]->m_VariablesArray[iIndex]);
		else
			VariablesOnRelease(&theApp.m_pDevArray[dIndex]->m_VariablesArray[iIndex]);
	}

	// Check if assigned to Flag list.
	for (int i = 0; i<MAX_INPUT_FLAGS; i++) {
		if (theApp.m_UserVarsArray.m_IOFlags[i].Active) {
			if (theApp.m_UserVarsArray.m_IOFlags[i].ProductID == theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID &&
				theApp.m_UserVarsArray.m_IOFlags[i].IOIndex == iIndex) {
				theApp.m_UserVarsArray.m_IOFlags[i].FlagValue = theApp.m_pDevArray[dIndex]->m_Inputs[iIndex];
			}
		}
	}

	return retval;
}

void CInputThread::VariablesOnPress(CVariablesPage *nVariables)
{
	// DO ON PRESS

	double dVal;

	if (nVariables->m_OPVarTokenName.IsEmpty()) 
		return;

	switch (nVariables->m_OPSimType) {
		case SIM_XPLANE:
			if (theApp.m_pXPFlightData == NULL) return;

			if (nVariables->m_OPVarType == xplmType_Cmd) {
				theApp.m_pXPFlightData->Cmd[nVariables->m_OPVarTokenIndex].Active = true;
				theApp.m_pXPFlightData->Cmd[nVariables->m_OPVarTokenIndex].Press = true;
				return;
			}

			theApp.m_pXPFlightData->Data[nVariables->m_OPVarTokenIndex].Active = true;
			dVal = theApp.m_pXPFlightData->Data[nVariables->m_OPVarTokenIndex].dDataRead;
			break;

		case SIM_FS9:
			break;

		case SIM_FSX:
			break;

		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
			if (nVariables->m_OPVarType == xplmType_Cmd) {
				theApp.m_IL2FlightData.Cmd[nVariables->m_OPVarTokenIndex].Active = true;
				theApp.m_IL2FlightData.Cmd[nVariables->m_OPVarTokenIndex].Press = true;
				return;
			}

			theApp.m_IL2FlightData.Data[nVariables->m_OPVarTokenIndex].Active = true;
			dVal = theApp.m_IL2FlightData.Data[nVariables->m_OPVarTokenIndex].dDataRead;
			break;

		default:
			return;
	}

	dVal = 0;
	switch (nVariables->m_OPRadio) {
		case 0:		// Mask
			if (nVariables->m_OPOperator == 0)
				dVal = (double)((DWORD)dVal & nVariables->m_OPMask);
			else
				dVal = (double)((DWORD)dVal | nVariables->m_OPMask);
			break;
		case 1:		// set value
			dVal = (double)nVariables->m_OPValue;
			break;
		case 2:		// inc
			dVal += (double)nVariables->m_OPInc;
			break;
		case 3:		// dec
			dVal -= (double)nVariables->m_OPDec;
			break;
	}

	switch(nVariables->m_OPSimType) {
		case SIM_XPLANE:
			if (theApp.m_pXPFlightData != NULL) {
				theApp.m_pXPFlightData->Data[nVariables->m_OPVarTokenIndex].dDataWrite = dVal;
				theApp.m_pXPFlightData->Data[nVariables->m_OPVarTokenIndex].Write = true;
			}
			break;

		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
			theApp.m_IL2FlightData.Data[nVariables->m_OPVarTokenIndex].dDataWrite = dVal;
			theApp.m_IL2FlightData.Data[nVariables->m_OPVarTokenIndex].Write = true;
			break;
	}
}

void CInputThread::VariablesOnRelease(CVariablesPage *nVariables)
{
	// DO ON RELEASE

	double dVal;

	if (nVariables->m_ORVarTokenName.IsEmpty()) 
		return;

	switch (nVariables->m_ORSimType) {
		case SIM_XPLANE:
			if (theApp.m_pXPFlightData == NULL) return;

			if (nVariables->m_ORVarType == xplmType_Cmd) {
				theApp.m_pXPFlightData->Cmd[nVariables->m_ORVarTokenIndex].Active = true;
				theApp.m_pXPFlightData->Cmd[nVariables->m_OPVarTokenIndex].Release = true;
				return;
			}

			theApp.m_pXPFlightData->Data[nVariables->m_ORVarTokenIndex].Active = true;
			dVal = theApp.m_pXPFlightData->Data[nVariables->m_ORVarTokenIndex].dDataRead;
			break;

		case SIM_FS9:
			break;

		case SIM_FSX:
			break;

		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
			if (nVariables->m_ORVarType == xplmType_Cmd) {
				theApp.m_IL2FlightData.Cmd[nVariables->m_ORVarTokenIndex].Active = true;
				theApp.m_IL2FlightData.Cmd[nVariables->m_ORVarTokenIndex].Release = true;
				return;
			}

			theApp.m_IL2FlightData.Data[nVariables->m_ORVarTokenIndex].Active = true;
			dVal = theApp.m_IL2FlightData.Data[nVariables->m_ORVarTokenIndex].dDataRead;
			break;

		default:
			return;
	}

	dVal = 0;
	switch (nVariables->m_ORRadio) {
		case 0:		// Mask
			if (nVariables->m_OROperator == 0)
				dVal = (double)((DWORD)dVal & nVariables->m_ORMask);
			else
				dVal = (double)((DWORD)dVal | nVariables->m_ORMask);
			break;
		case 1:		// set value
			dVal = (double)nVariables->m_ORValue;
			break;
		case 2:		// inc
			dVal += (double)nVariables->m_ORInc;
			break;
		case 3:		// dec
			dVal -= (double)nVariables->m_ORDec;
			break;
	}

	switch(nVariables->m_ORSimType) {
		case SIM_XPLANE:
			if (theApp.m_pXPFlightData != NULL) {
				theApp.m_pXPFlightData->Data[nVariables->m_ORVarTokenIndex].dDataWrite = dVal;
				theApp.m_pXPFlightData->Data[nVariables->m_ORVarTokenIndex].Write = true;
			}
			break;

		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
			theApp.m_IL2FlightData.Data[nVariables->m_ORVarTokenIndex].dDataWrite = dVal;
			theApp.m_IL2FlightData.Data[nVariables->m_ORVarTokenIndex].Write = true;
			break;
	}
}
