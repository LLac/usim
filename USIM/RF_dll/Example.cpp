//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Source File                                   ﬁ
//›                                                                         ﬁ
//› Description: Declarations for the Internals Example Plugin              ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› This source code module, and all information, data, and algorithms      ﬁ
//› associated with it, are part of CUBE technology (tm).                   ﬁ
//›                           PROPRIETARY                                   ﬁ
//› Copyright (c) 1996-2006 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›   jmm.2006.01.06: modified for public                                   ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

//***************************************************************************//

#include "Example.hpp"          // corresponding header file
#include <stdio.h>              // for sample output

//***************************************************************************//

//***************************************************************************//
// plugin information
// Change this information to siut your needs. rFactor will use this information
// to ensure that it is using the most recent version
unsigned g_uPluginID          = 1;
char     g_szPluginName[]     = "BI Interface Plugin - 2010 July 4";
unsigned g_uPluginVersion     = 003;
unsigned g_uPluginObjectCount = 1;
InternalsPluginInfo g_PluginInfo;

//***************************************************************************//

//***************************************************************************//
// interface to plugin information
extern "C" __declspec(dllexport)
const char* __cdecl GetPluginName() { return g_szPluginName; }

extern "C" __declspec(dllexport)
unsigned __cdecl GetPluginVersion() { return g_uPluginVersion; }

extern "C" __declspec(dllexport)
unsigned __cdecl GetPluginObjectCount() { return g_uPluginObjectCount; }

// get the plugin-info object used to create the plugin.
extern "C" __declspec(dllexport)
PluginObjectInfo* __cdecl GetPluginObjectInfo( const unsigned uIndex )
{
	switch(uIndex) {
		case 0:
			return  &g_PluginInfo;
		default:
			return 0;
	}
}
//***************************************************************************//

//***************************************************************************//
// InternalsPluginInfo class
InternalsPluginInfo::InternalsPluginInfo()
{
	// put together a name for this plugin
	sprintf( m_szFullName, "%s - %s", g_szPluginName, InternalsPluginInfo::GetName() );
}
//***************************************************************************//

//***************************************************************************//
const char*    InternalsPluginInfo::GetName()     const { return ExampleInternalsPlugin::GetName(); }
const char*    InternalsPluginInfo::GetFullName() const { return m_szFullName; }
// Change this to suit your needs
const char*    InternalsPluginInfo::GetDesc()     const { return "Example Internals Plugin"; }
const unsigned InternalsPluginInfo::GetType()     const { return ExampleInternalsPlugin::GetType(); }
const char*    InternalsPluginInfo::GetSubType()  const { return ExampleInternalsPlugin::GetSubType(); }
const unsigned InternalsPluginInfo::GetVersion()  const { return ExampleInternalsPlugin::GetVersion(); }
void*          InternalsPluginInfo::Create()      const { return new ExampleInternalsPlugin(); }
//***************************************************************************//

//***************************************************************************//
// InternalsPlugin class
const char ExampleInternalsPlugin::m_szName[] = "InternalsPlugin";
const char ExampleInternalsPlugin::m_szSubType[] = "Internals";
const unsigned ExampleInternalsPlugin::m_uID = 1;
const unsigned ExampleInternalsPlugin::m_uVersion = 3;
//***************************************************************************//

//***************************************************************************//
PluginObjectInfo *ExampleInternalsPlugin::GetInfo() { return &g_PluginInfo; }
//***************************************************************************//


#define	TIMEOUT_MSEC	10
#define	TIMEOUT_SEC		900

//***************************************************************************//

void ExampleInternalsPlugin::Startup()
{
	// Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
	// example output data.

	FILE *fo = fopen( "birfip.log", "w" );
	fclose( fo );

	StartSharedMemServer();
}

void ExampleInternalsPlugin::StartSharedMemServer()
{
	FILE *fo;

	m_ServerDataMap = NULL;
	RFSimData = NULL;
	m_ServerDataMap = CreateFileMapping(INVALID_HANDLE_VALUE,
							 NULL,
							 PAGE_READWRITE,
							 0,
							 sizeof(RF_Telem),
							 "rFactorMemoryArea");

	// If the shared memory file already exists, close it.
	if (m_ServerDataMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		fo = fopen( "birfip.log", "a" );
		fprintf( fo, "Init sharedmem already exists.\n");
		fclose( fo );
	}

	if (m_ServerDataMap != NULL) {
		RFSimData = (RF_Telem *)MapViewOfFile(m_ServerDataMap, FILE_MAP_WRITE, 0, 0, 0);
		if (RFSimData == NULL) {	// check to see if we have a valid view
			UnmapViewOfFile(RFSimData);

			CloseHandle(m_ServerDataMap);
			m_ServerDataMap = NULL;

			fo = fopen( "birfip.log", "a" );
			fprintf( fo, "Init sharedmem failed.\n");
			fclose( fo );
		} else {
			fo = fopen( "birfip.log", "a" );
			fprintf( fo, "Init sharedmem OK.\n");
			fclose( fo );
		}
	}
}

void ExampleInternalsPlugin::UpdateTelemetry( const TelemInfoV2 &info )
{
	static WORD			Time_Out = 0;
	WORD				Delta_Time = 0;
	SYSTEMTIME			mySYSTEMTIME;

	GetSystemTime(&mySYSTEMTIME);
	Delta_Time = (unsigned short)(mySYSTEMTIME.wSecond*1000 + mySYSTEMTIME.wMilliseconds - Time_Out);

	if (Delta_Time > TIMEOUT_MSEC) {
		Time_Out = (unsigned short)(mySYSTEMTIME.wSecond*1000 + mySYSTEMTIME.wMilliseconds);
		
		if (RFSimData != NULL) {
			memcpy(&RFSimData->m_TelemInfoV2, (void *)&info, sizeof(TelemInfoV2));
		}
	}
}

void ExampleInternalsPlugin::UpdateScoring( const ScoringInfoV2 &info )
{
	static WORD			Time_Out = 0;
	WORD				Delta_Time = 0;
	SYSTEMTIME			mySYSTEMTIME;

	GetSystemTime(&mySYSTEMTIME);
	Delta_Time = (unsigned short)(mySYSTEMTIME.wSecond*1000 + mySYSTEMTIME.wMilliseconds - Time_Out);

	if (Delta_Time > TIMEOUT_SEC) {
		Time_Out = (unsigned short)(mySYSTEMTIME.wSecond*1000 + mySYSTEMTIME.wMilliseconds);
		
		if (RFSimData != NULL) {
			memcpy(&RFSimData->m_ScoringInfoV2, (void *)&info, sizeof(ScoringInfoV2));
			memcpy(&RFSimData->m_VehicleScoringInfoV2, (void *)&info, sizeof(VehicleScoringInfoV2));
		}
	}
}

void ExampleInternalsPlugin::Shutdown()     // game shutdown
{
	if (RFSimData != NULL) {
		CloseHandle(RFSimData);
		RFSimData = NULL;
	}

	FILE *fo = fopen( "birfip.log", "a" );
	fprintf( fo, "thread closing\n");
	fclose( fo );
}
