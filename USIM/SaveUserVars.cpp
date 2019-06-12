// SaveUserVars.cpp: implementation of the CSaveUserVars class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "SaveUserVars.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const UINT CSaveUserVars::m_uiFileClass = ID_FILECLASS;
const UINT CSaveUserVars::m_uiFileVersion = ID_SAVEFILEVERSION;
const UINT CSaveUserVars::m_uiFileVersion_1 = ID_SAVEFILEVERSION_1;

IMPLEMENT_SERIAL( CSaveUserVars, CObject, CSaveUserVars::m_uiFileVersion);

CSaveUserVars::CSaveUserVars()
{
	ClearUserVarsData();
}

CSaveUserVars::~CSaveUserVars()
{

}

void CSaveUserVars::Serialize( CArchive &ar )
{	// MSDN says we ne need to call ancestor's serialize
	CObject::Serialize( ar );

	UINT uiFileVersion, uiFileClass;
	int i;
	UCHAR DataFlag;

	// standard procedure when serializing
	if ( ar.IsStoring() ) {	// store the rest of our data
		ar << m_uiFileClass;
		ar << m_uiFileVersion;

		if (IsSet()) {
			DataFlag = true;
			ar << DataFlag;
		} else {
			DataFlag = false;
			ar << DataFlag;
			return;
		}

		for (i=0; i<MAX_INPUT_FLAGS; i++) {
			ar << m_IOFlags[i].Active;
			ar << m_IOFlags[i].FlagValue;
			ar << m_IOFlags[i].IOIndex;
			ar << m_IOFlags[i].ItemPathString;
			ar << m_IOFlags[i].ProductID;
			ar << m_IOFlags[i].VarTokenString;
		}
	} else {
		// we are loading data, so we first need to check
		// if it's the correct version
		ar >> uiFileClass;
		if (uiFileClass != ID_FILECLASS) {
			AfxMessageBox("Incompatible file!", MB_OK );
			AfxThrowArchiveException(CArchiveException::badClass);
			return;		// wrong file class
		}

		ar >> uiFileVersion;
		ar >> DataFlag;
		if (!DataFlag) {
			ClearUserVarsData();
			return;
		}

		switch (uiFileVersion) {
			case m_uiFileVersion:
			case m_uiFileVersion_1:
				// everything is okay, load data
				// in the EXACT order as saving
				for (i=0; i<MAX_INPUT_FLAGS; i++) {
					ar >> m_IOFlags[i].Active;
					ar >> m_IOFlags[i].FlagValue;
					ar >> m_IOFlags[i].IOIndex;
					ar >> m_IOFlags[i].ItemPathString;
					ar >> m_IOFlags[i].ProductID;
					ar >> m_IOFlags[i].VarTokenString;
				}
				break;

			default:	// version mismatch, so inform the user
				// WARN USER about old file version
#ifndef _DEBUG
				AfxMessageBox("Obsolete data found in IMC file.\nResave your configuration file!", MB_ICONEXCLAMATION);
#endif
				AfxThrowArchiveException(CArchiveException::badClass);
				break;
		}	// switch (uiFileVersion)
	}
}

void CSaveUserVars::ClearUserVarsData()
{
	for (int i=0; i<MAX_INPUT_FLAGS; i++) {
		m_IOFlags[i].Active = false;
		m_IOFlags[i].FlagValue = 0;
		m_IOFlags[i].IOIndex = 0;
		m_IOFlags[i].ItemPathString.Empty();
		m_IOFlags[i].ProductID = 0x0000;
		m_IOFlags[i].VarTokenString.Format(_T("Flag %i"), i+1);
	}
}

BOOL CSaveUserVars::IsSet()
{
	int i;

	for (i=0; i<MAX_INPUT_FLAGS; i++) {
		if (m_IOFlags[i].Active) return true;
	}

	return false;
}

