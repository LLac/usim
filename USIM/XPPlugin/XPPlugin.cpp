/*
 * XPPlugin.c
 * 
 * This example plugin demonstrates how to use the timed processing callbacks
 * to continuously record sim data to disk.
 * 
 * This technique can be used to record data to disk or to the network.  Unlike
 * UDP data output, we can increase our frequency to capture data every single
 * sim frame.  (This example records once per second.)
 * 
 * Use the timed processing APIs to do any periodic or asynchronous action in
 * your plugin.
 * 
 */

#include <stdio.h>
#include <string.h>
#define APL	0
#define IBM 1
#define XPLM200
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPPlugin.h"

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	int i;

	strcpy(outName, "BI XP Plugin");
	strcpy(outSig, "bixplane.plugin");
	strcpy(outDesc, "BI interface plugin.");

	StartSharedMemServer();
	if (m_pXPlaneData == NULL) return 0;

	/* Find the data refs we want to record. */
	// Dataref type data
	for (i=0; i<ArraySize(XPDataArray); i++) {
		m_pXPlaneData->Data[i].DataRef = XPLMFindDataRef(XPDataArray[i].VarTokenName);
	}

	// Cmd type data
	for (i=0; i<ArraySize(XPCmdArray); i++) {
		m_pXPlaneData->Cmd[i].DataRef = XPLMFindCommand(XPCmdArray[i].VarTokenName);
	}

	/* Register our callback for once a second.  Positive intervals
	 * are in seconds, negative are the negative of sim frames.  Zero
	 * registers but does not schedule a callback for time. */

	XPLMRegisterFlightLoopCallback(		
			MyFlightLoopCallback,	/* Callback */
			(float)iInterval,		/* Interval */
			NULL);					/* refcon not used. */
	
	return (m_ServerDataMap != NULL) ? 1 : 0;
}

void StartSharedMemServer()
{
/*
    if (m_hMutex) {
        CloseHandle(m_hMutex);
    }

	m_hMutex = CreateMutex(
		NULL,                       // security attributes
		false,                      // initial owner
		sMutexFileName);            // name
*/
	if (m_ServerDataMap != NULL) return;

	m_ServerDataMap = CreateFileMapping(INVALID_HANDLE_VALUE,
							 NULL,
							 PAGE_READWRITE,
							 0,
							 sizeof(*m_pXPlaneData),
							 "XPlaneMemoryArea");

	m_pXPlaneData = (XPFlightData *)MapViewOfFile(m_ServerDataMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
}

void StopSharedMemServer()
{
/*
	if (m_hMutex) {
		CloseHandle(m_hMutex);
	}
*/

	if (m_ServerDataMap != NULL) {
		if (m_pXPlaneData != NULL) {	// check to see if we have a valid view
			UnmapViewOfFile(m_pXPlaneData);
			m_pXPlaneData = NULL;
		}
		CloseHandle(m_ServerDataMap);
		m_ServerDataMap = NULL;
	}
}

PLUGIN_API void	XPluginStop(void)
{
	/* Unregister the callback */
	XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);

	// Stop Sharedmem
	StopSharedMemServer();
}

PLUGIN_API void XPluginDisable(void)
{

}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					long			inMessage,
					void *			inParam)
{

}

float	MyFlightLoopCallback(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon)
{
//	ULONG retval;
	int i;
	int ioutValues;
	float foutValues;
	UCHAR boutValues;

	/* The actual callback.  First we read the sim's time and the data. */
//	if (m_ServerDataMap == NULL || m_pXPlaneData == NULL) { // || !m_hMutex) {
	if (m_ServerDataMap == NULL || m_pXPlaneData == NULL) {
		// Something went wrong. Try starting sharedmem again...and again...and again.
		StartSharedMemServer();
		return 1.0;
	}

/*
	// get exclusive access to common memory block
	retval = WaitForSingleObject(m_hMutex, 5);
	
	if (retval == WAIT_TIMEOUT) 
		return (float)iInterval;

	if (retval == WAIT_OBJECT_0 || retval == WAIT_ABANDONED) {
*/
	// DataRefs
	for (i=0; i<ArraySize(XPDataArray); i++) {
		if (m_pXPlaneData->Data[i].Active) {
			// Read values from SharedMem
			if (m_pXPlaneData->Data[i].Write) {
				if (XPDataArray[i].VarArraySize == 0) {
					switch(XPDataArray[i].VarType) {
						case xplmType_Int:
							XPLMSetDatai(m_pXPlaneData->Data[i].DataRef, (int)m_pXPlaneData->Data[i].dDataWrite);
							break;

						case xplmType_Float:
							XPLMSetDataf(m_pXPlaneData->Data[i].DataRef, (float)m_pXPlaneData->Data[i].dDataWrite);
							break;

						case xplmType_Double:
							XPLMSetDatad(m_pXPlaneData->Data[i].DataRef, m_pXPlaneData->Data[i].dDataWrite);
							break;
					} // switch(m_pXPlaneData->Data[i].Type) {
				} else {
					switch(XPDataArray[i].VarType) {
						case xplmType_Int:
							ioutValues = (int)m_pXPlaneData->Data[i].dDataWrite;
							XPLMSetDatavi(	m_pXPlaneData->Data[i].DataRef, 
											&ioutValues,
											m_pXPlaneData->Data[i].inOffset,
											1);
							break;

						case xplmType_Float:
							foutValues = (float)m_pXPlaneData->Data[i].dDataWrite;
							XPLMSetDatavf(	m_pXPlaneData->Data[i].DataRef, 
											&foutValues,
											m_pXPlaneData->Data[i].inOffset,
											1);
							break;

						case xplmType_Byte:
							boutValues = (UCHAR)m_pXPlaneData->Data[i].dDataWrite;
							XPLMSetDatab(	m_pXPlaneData->Data[i].DataRef, 
											&boutValues,
											m_pXPlaneData->Data[i].inOffset,
											1);
							break;
					} // switch(XPDataArray[i].VarType)
				}
				m_pXPlaneData->Data[i].Write = 0;

			} // if (m_pXPlaneData->Data[i].Write) {

			// Write values into SharedMem
			if (XPDataArray[i].VarArraySize == 0) {
				switch(XPDataArray[i].VarType) {
					case xplmType_Int:
						m_pXPlaneData->Data[i].dDataRead = (double)XPLMGetDatai(m_pXPlaneData->Data[i].DataRef);
						break;
					case xplmType_Float:
						m_pXPlaneData->Data[i].dDataRead = XPLMGetDataf(m_pXPlaneData->Data[i].DataRef);
						break;
					case xplmType_Double:
						m_pXPlaneData->Data[i].dDataRead = XPLMGetDatad(m_pXPlaneData->Data[i].DataRef);
						break;
				}
			} else {
				switch(XPDataArray[i].VarType) {
					case xplmType_Int:
						XPLMGetDatavi(	m_pXPlaneData->Data[i].DataRef,
										&ioutValues,
										m_pXPlaneData->Data[i].inOffset,
										1);
						m_pXPlaneData->Data[i].dDataRead = (double)ioutValues;
						break;
					case xplmType_Float:
						XPLMGetDatavf(	m_pXPlaneData->Data[i].DataRef,
										&foutValues,
										m_pXPlaneData->Data[i].inOffset,
										1);
						m_pXPlaneData->Data[i].dDataRead = foutValues;
						break;
					case xplmType_Byte:
						XPLMGetDatab(	m_pXPlaneData->Data[i].DataRef,
										&boutValues,
										m_pXPlaneData->Data[i].inOffset,
										1);
						m_pXPlaneData->Data[i].dDataRead = (int)boutValues;
						break;
				}
			}
		} // if (m_pXPlaneData->Data[i].Active) {
	} // for (UINT i=0; i<ArraySize(XPDataArray); i++) {

	// Commands
	for (i=0; i<ArraySize(XPCmdArray); i++) {
		if (m_pXPlaneData->Cmd[i].Active) {
			// Start command
			if (m_pXPlaneData->Cmd[i].Press) {
				XPLMCommandBegin(m_pXPlaneData->Cmd[i].DataRef);
				m_pXPlaneData->Cmd[i].Press = 0;
			}

			if (m_pXPlaneData->Cmd[i].Release) {
				XPLMCommandEnd(m_pXPlaneData->Cmd[i].DataRef);
				m_pXPlaneData->Cmd[i].Release = 0;
			}
		}
	} // for (UINT i=0; i<ArraySize(XPCmdArray); i++) {
/*
	}

	ReleaseMutex(m_hMutex);
*/	
	/* Return 1.0 to indicate that we want to be called again in 1 second. */
	return (float)iInterval;
}
