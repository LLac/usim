// DoInputThread.cpp: implementation of the CDoInputThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bifs_mfc.h"
#include "DoInputThread.h"
#include "hid_dll\Biusb.h"
#include "FSVarList.h"

__int64 Mask_Array[] = {0x0, 0xFF, 0xFFFF, 0xFFFFFF, 0xFFFFFFFF, 0xFFFFFFFFFF, 0xFFFFFFFFFFFF, 0xFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

__int8 m_GlobalChar = 0;
__int16 m_GlobalShort = 0;
__int32	m_GlobalLong = 0;
__int64 m_GlobalInt64 = 0;
float m_GlobalFloat32 = 0;
long double m_GlobalFloat64 = 0;

void *VarsPtr[] = {	NULL,			// 0
					&m_GlobalChar,	// 1
					&m_GlobalShort,	// 2
					NULL,			// 3
					&m_GlobalLong,	// 4
					NULL,			// 5
					NULL,			// 6
					NULL,			// 7
					&m_GlobalInt64,	// 8
					&m_GlobalFloat32, 
					&m_GlobalFloat64};

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

//IMPLEMENT_DYNAMIC(CDoInputThread, CThreadDerived)

CDoInputThread::CDoInputThread(void* pOwnerObject, LPARAM lParam) 
	: CThreadDerived(pOwnerObject, lParam)
{
	m_delta_T = 0;
	theApp.SetThreadName(GetCurrentThreadId(),"DoInputThread");
}

CDoInputThread::~CDoInputThread()
{

}

void CDoInputThread::DoThreadWorker()
{
	UINT dIndex;
	long retval;
	static DWORD delta_T;

	// 10 ms elapsed
	if ( (timeGetTime() - m_delta_T) < 10) 
		return;
//	TRACE("DoInput Delta = %d\n", (timeGetTime() - m_delta_T));
	m_delta_T = timeGetTime();

	// read inputs from HID devices
//	Lock();
	for (dIndex = 0; dIndex < MAX_DEVICES; dIndex++) {
		if (theApp.m_DevArray[dIndex] == NULL || theApp.m_DevArray[dIndex]->m_DeleteMe) continue;

		if (theApp.m_DevArray[dIndex]->m_Connected &&	// This condition needs fix'n with non polled input devices
			theApp.m_DevArray[dIndex]->m_DeviceParam.NumberInputIndices > 0) {

			retval = theApp.m_BIUSB.ReadInputData(&theApp.m_DevArray[dIndex]->m_DeviceParam, (char *)theApp.m_DevArray[dIndex]->m_Inputs, FALSE);

			if (retval == DEV_INPUT) {
//				delta_T = timeGetTime();				

				ReadInputs(dIndex);
/*
				delta_T = timeGetTime() - delta_T;

				CString str;
				str.Format("%sfsim.log", theApp.m_Path);
				FILE* stream = fopen(str, "a");
				fprintf(stream, "DoInputThread Delta %d\n", delta_T);
				fclose(stream);
*/
			} else if (retval == DEV_FAILED) {
				theApp.m_DevArray[dIndex]->m_Connected = false;
				// write to log file
				CString str;
				str.Format("%sfsim.log", theApp.m_Path);
				FILE* stream = fopen(str, "a");
				fprintf(stream, "Device %s Failed! Returned error code : %d\n", theApp.m_DevArray[dIndex]->m_DeviceParam.DeviceName, retval);
				fclose(stream);
			} else if (retval == DEV_TIMEOUT) {

				TRACE("DoInputThread %s - DEV_TIMEOUT\n", theApp.m_DevArray[dIndex]->m_DeviceParam.DeviceName);

				CString str;
				str.Format("%sfsim.log", theApp.m_Path);
				FILE* stream = fopen(str, "a");
				fprintf(stream, "DoInputThread %s - DEV_TIMEOUT\n", theApp.m_DevArray[dIndex]->m_DeviceParam.DeviceName);
				fclose(stream);
			}
		}
	}
//	Unlock();
}

void CDoInputThread::ReadInputs(const UINT dIndex)
{
	UINT iIndex, nMultiplier;
	BOOL IsSet = 0;

	for (iIndex = 0; iIndex < theApp.m_DevArray[dIndex]->m_DeviceParam.NumberInputIndices; iIndex++) {
		// if active && set
		if (theApp.m_DevArray[dIndex]->m_InputActive[iIndex] && !theApp.m_DevArray[dIndex]->m_InputDisabled[iIndex]) {

			IsSet = InputState(dIndex, iIndex);

			if (!IsSet) continue;

			nMultiplier = 1;
			if (theApp.m_DevArray[dIndex]->m_InputsDelta[iIndex] < theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Threshold) {
				nMultiplier = theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Multiplier;
			}

			if (m_DoInputsTabs.OPFSKeysTab)			// ON PRESS
				FSKeysOnPress(&theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex], nMultiplier);
			else if (m_DoInputsTabs.ORFSKeysTab)	// DO ON RELEASE
				FSKeysOnRelease(&theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex]);
			else if (m_DoInputsTabs.ORepFSKeysTab)	// REPEAT. DO ON PRESS
				FSKeysOnRepeat(&theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex]);

			if (m_DoInputsTabs.OPOffsetsTab)		// ON PRESS
				OffsetsOnPress(&theApp.m_DevArray[dIndex]->m_OffsetsArray[iIndex], nMultiplier);
			else if (m_DoInputsTabs.OROffsetsTab)	// DO ON RELEASE
				OffsetsOnRelease(&theApp.m_DevArray[dIndex]->m_OffsetsArray[iIndex]);

		}
	}
}

BOOL CDoInputThread::InputState(UINT dIndex, UINT iIndex)
{
	BOOL retval = false;

	m_DoInputsTabs.OPFSKeysTab = false;
	m_DoInputsTabs.ORFSKeysTab = false;
	m_DoInputsTabs.ORepFSKeysTab = false;
	m_DoInputsTabs.OPOffsetsTab = false;
	m_DoInputsTabs.OROffsetsTab = false;

	// if toggled
	if (theApp.m_DevArray[dIndex]->m_Inputs[iIndex] != theApp.m_DevArray[dIndex]->m_InputsOld[iIndex]) {
		// Toggled. Save old value
		theApp.m_DevArray[dIndex]->m_InputsOld[iIndex] = theApp.m_DevArray[dIndex]->m_Inputs[iIndex];

		// check delta time
		ULONG m_delta_T = timeGetTime();
		theApp.m_DevArray[dIndex]->m_InputsDelta[iIndex] = m_delta_T - theApp.m_DevArray[dIndex]->m_InputsDeltaOld[iIndex];
		theApp.m_DevArray[dIndex]->m_InputsDeltaOld[iIndex] = m_delta_T;

		retval = CheckInputState(dIndex, iIndex);
	} else {
		// check for repeat options on FSKeys	
		if (theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Tabs & DO_FSKEYS_TAB &&
			theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex].IsSet()) {
			if (theApp.m_DevArray[dIndex]->m_Inputs[iIndex] == 1 && theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex].m_OPRepeat) {
				m_DoInputsTabs.ORepFSKeysTab = true;
				retval = true;
			}
		}
	}

	return retval;
}

BOOL CDoInputThread::CheckInputState(UINT dIndex, UINT iIndex)
{
	BOOL retval = false;

	if (theApp.m_DevArray[dIndex]->m_Inputs[iIndex] == 1) {
		if (theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Tabs & DO_FSKEYS_TAB &&
			theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex].IsSet()) {
			m_DoInputsTabs.OPFSKeysTab = true;
			retval = true;
		}

		if (theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Tabs & DO_OFFSET_TAB &&
			theApp.m_DevArray[dIndex]->m_OffsetsArray[iIndex].IsSet()) {
			m_DoInputsTabs.OPOffsetsTab = true;
			retval = true;
		}
	} else {
		if (theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Tabs & DO_FSKEYS_TAB &&
			theApp.m_DevArray[dIndex]->m_FSKeysArray[iIndex].IsSet()) {
			m_DoInputsTabs.ORFSKeysTab = true;
			retval = true;
		}

		if (theApp.m_DevArray[dIndex]->m_CondArray[iIndex].m_Tabs & DO_OFFSET_TAB &&
			theApp.m_DevArray[dIndex]->m_OffsetsArray[iIndex].IsSet()) {
			m_DoInputsTabs.OROffsetsTab = true;
			retval = true;
		}
	}

	return retval;
}

double CDoInputThread::FormatVar(MODULE_VAR *mvar) 
{
	switch (mvar->var_type) {
		case TYPE_BOOL:
		case TYPE_BOOL8:
		case TYPE_BOOL16:
		case TYPE_BOOL32:
			return (double)mvar->var_value.b;
		break;
    
		case TYPE_ENUM:
		case TYPE_ENUM8:
		case TYPE_ENUM16:
		case TYPE_ENUM32:
			return (double)mvar->var_value.e;
		break;
		
		case TYPE_BCD16:
			return (double)mvar->var_value.b;
		break;

		case TYPE_BCO16:
			return (double)mvar->var_value.o;
		break;

		default:
			return (double)mvar->var_value.n;
	}
}

void CDoInputThread::FSKeysOnPress(CFSKeysPage *nFSKeys, UINT nCount)
{
	// DO ON PRESS
	if (nFSKeys->m_OPRepeat)
		nFSKeys->m_Time_Out = timeGetTime();

	// Trigger FS Key Event
	for (UINT i = 0; i < nCount; i++) {
		trigger_key_event(nFSKeys->m_OPKey, nFSKeys->m_OPParamVal);
	}

	// Execute file if set
	if (!nFSKeys->m_OPExe.IsEmpty()) {
		PlayFile(nFSKeys->m_OPExe);
	}
}

void CDoInputThread::FSKeysOnRepeat(CFSKeysPage *nFSKeys)
{
	// Trigger FS Key Event
	DWORD Delta_Time = (ULONG)(timeGetTime() - nFSKeys->m_Time_Out);

	if (nFSKeys->m_OPRate <= Delta_Time) {
#ifdef _DEBUG
//	TRACE("DoInput Delta = %d\n", Delta_Time);
#endif
		nFSKeys->m_Time_Out = timeGetTime();
		trigger_key_event(nFSKeys->m_OPKey, nFSKeys->m_OPParamVal);
	}
}

void CDoInputThread::FSKeysOnRelease(CFSKeysPage *nFSKeys)
{
	// Trigger FS Key Event
	trigger_key_event(nFSKeys->m_ORKey, nFSKeys->m_ORParamVal);

	// Execute file if set
	if (!nFSKeys->m_ORExe.IsEmpty()) {
		CString str = nFSKeys->m_ORExe;
		PlayFile(str);
	}
}

void CDoInputThread::PlayFile(CString nFSKeys)
{
	DWORD u32_Err;

	u32_Err = i_Sound.PlaySoundFile( (LPTSTR)(LPCTSTR)nFSKeys );
	switch(u32_Err) {
		// errors defined in cSound.h
		case ERR_INVALID_FILETYPE:
			TRACE("\nThis filetype is not supported!\n");
			ShellExecute(NULL, "open", nFSKeys, NULL, NULL, SW_SHOWMINIMIZED );
			break;
		case ERR_PLAY_WAV:
			TRACE("\nWindows could not play the Wav file!\n");
			break;
		case MCIERR_SEQ_NOMIDIPRESENT:
			TRACE("\nThere is no Midi device installed or it is in use by another application!\n");
			break;
	}
}

void CDoInputThread::OffsetsOnPress(COffsetsPage *nOffsets, UINT nCount)
{
	UINT i;
	BIFS_ACTION_READWRITE_HDR pHdrRW;
	// DO ON PRESS

	if (nOffsets->m_OPOffset == 0) return;
	
	if (nOffsets->m_OPOffset != 0) {
		pHdrRW.action = BIFS_ACTION_READ;
		pHdrRW.offset = nOffsets->m_OPOffset;
	} else if (nOffsets->m_OPAddress != 0) {
		pHdrRW.action = BIFS_ACTION_READ_TRUE;
		pHdrRW.offset = nOffsets->m_OPAddress;
	} else {
		return;
	}

	pHdrRW.size = nOffsets->m_OPSize;
	pHdrRW.data = VarsPtr[nOffsets->m_OPSize];
	try {
		for (UINT i = 0; i < nCount; i++)
			m_FSConnect.ReadWriteFSVar(&pHdrRW);
	} catch (...) {
		// cleanup
		return;
	}

	if (nOffsets->m_OPSize < 9)
		*(__int64 *)pHdrRW.data = *(__int64 *)pHdrRW.data & Mask_Array[nOffsets->m_OPSize];

	switch (nOffsets->m_OPRadio) {
		case 0:		// Mask
			if (nOffsets->m_OPOperator == 0)
				*(DWORD *)pHdrRW.data = *(DWORD *)pHdrRW.data & nOffsets->m_OPMask;
			else
				*(DWORD *)pHdrRW.data = *(DWORD *)pHdrRW.data | nOffsets->m_OPMask;
			break;
		case 1:		// set value
			*(__int64 *)pHdrRW.data = (__int64)nOffsets->m_OPValue;
			break;
		case 2:		// inc
			*(UINT *)pHdrRW.data = *(UINT *)pHdrRW.data + nOffsets->m_OPInc;
			break;
		case 3:		// dec
			*(UINT *)pHdrRW.data = *(UINT *)pHdrRW.data - nOffsets->m_OPDec;
			break;
	}

	// write offset
	if (nOffsets->m_OPOffset != 0)
		pHdrRW.action = BIFS_ACTION_WRITE;
	else if (nOffsets->m_OPAddress != 0)
		pHdrRW.action = BIFS_ACTION_WRITE_TRUE;

	try {
		for (i = 0; i < nCount; i++)
			m_FSConnect.ReadWriteFSVar(&pHdrRW);
	} catch (...) {
		// cleanup
		return;
	}
}

void CDoInputThread::OffsetsOnRelease(COffsetsPage *nOffsets)
{
	BIFS_ACTION_READWRITE_HDR pHdrRW;

	if (nOffsets->m_OROffset == 0) return;

	if (nOffsets->m_OROffset != 0) {
		pHdrRW.action = BIFS_ACTION_READ;
		pHdrRW.offset = nOffsets->m_OROffset;
	} else if (nOffsets->m_ORAddress != 0) {
		pHdrRW.action = BIFS_ACTION_READ_TRUE;
		pHdrRW.offset = nOffsets->m_ORAddress;
	} else {
		return;
	}

	pHdrRW.size = nOffsets->m_ORSize;
	pHdrRW.data = VarsPtr[nOffsets->m_ORSize];
	try {
		m_FSConnect.ReadWriteFSVar(&pHdrRW);
	} catch (...) {
		// cleanup
		return;
	}

	if (nOffsets->m_ORSize < 9)
		*(__int64 *)pHdrRW.data = *(__int64 *)pHdrRW.data & Mask_Array[nOffsets->m_ORSize];

	switch (nOffsets->m_ORRadio) {
		case 0:		// Mask
			if (nOffsets->m_OROperator == 0)
				*(DWORD *)pHdrRW.data = *(DWORD *)pHdrRW.data & nOffsets->m_ORMask;
			else
				*(DWORD *)pHdrRW.data = *(DWORD *)pHdrRW.data | nOffsets->m_ORMask;
			break;
		case 1:		// set value
			*(__int64 *)pHdrRW.data = (__int64)nOffsets->m_ORValue;
			break;
		case 2:		// inc
			*(UINT *)pHdrRW.data = *(UINT *)pHdrRW.data + nOffsets->m_ORInc;
			break;
		case 3:		// dec
			*(UINT *)pHdrRW.data = *(UINT *)pHdrRW.data - nOffsets->m_ORDec;
			break;
	}

	// write offset
	if (nOffsets->m_OROffset != 0)
		pHdrRW.action = BIFS_ACTION_WRITE;
	else if (nOffsets->m_ORAddress != 0)
		pHdrRW.action = BIFS_ACTION_WRITE_TRUE;

	try {
		m_FSConnect.ReadWriteFSVar(&pHdrRW);
	} catch (...) {
		// cleanup
		return;
	}
}
