// STEPPage.cpp: implementation of the CSTEPPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "STEPPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTEPPage::CSTEPPage()
{
	Clear();
}

CSTEPPage::~CSTEPPage()
{

}

void CSTEPPage::Serialize( CArchive &ar, UINT uiFileVersion )
{
	int i;
	UCHAR DataFlag;

	if (ar.IsStoring()) {
		if (IsSet()) {
			DataFlag = true;
			ar << DataFlag;
		} else {
			DataFlag = false;
			ar << DataFlag;
			return;
		}

		ar << m_Sections;
		for (i=0; i<m_Sections; i++) {
			ar << m_MinValue[i];
			ar << m_MaxValue[i];
			ar << m_Formula[i];
			ar << m_STEPMin[i];
			ar << m_STEPMax[i];
			ar << m_STEPOffset[i];
		}
		ar << m_VarTokenName;
		ar << m_VarArrayTokenIndex;
		ar << m_SimType;
		ar << m_Read;
		ar << m_Write;
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		ar >> m_Sections;
		for (i=0; i<m_Sections; i++) {
			ar >> m_MinValue[i];
			ar >> m_MaxValue[i];
			ar >> m_Formula[i];
			ar >> m_STEPMin[i];
			ar >> m_STEPMax[i];
			ar >> m_STEPOffset[i];
		}
		ar >> m_VarTokenName;
		// check if older file version loading
		switch (uiFileVersion) {
			case ID_SAVEFILEVERSION:
				ar >> m_VarArrayTokenIndex;
			break;
			case ID_SAVEFILEVERSION_1:
				m_VarArrayTokenIndex = 0;
			break;
		}
		ar >> m_SimType;
		ar >> m_Read;
		ar >> m_Write;
		ar >> m_Notes;

		m_VarTokenIndex = SearchIndex(m_VarTokenName, m_SimType, 0);
	}
}

void CSTEPPage::CopyData(CSTEPPage *pArray)
{
	m_Sections = pArray->m_Sections;
	for (int i=0; i<8; i++) {	
		m_MinValue[i] = pArray->m_MinValue[i];
		m_MaxValue[i] = pArray->m_MaxValue[i];
		m_Formula[i] = pArray->m_Formula[i];
		m_STEPMin[i] = pArray->m_STEPMin[i];
		m_STEPMax[i] = pArray->m_STEPMax[i];
		m_STEPOffset[i] = pArray->m_STEPOffset[i];
	}
	m_VarTokenName = pArray->m_VarTokenName;
	m_VarTokenIndex = pArray->m_VarTokenIndex;
	m_SimType = pArray->m_SimType;
	m_Read = pArray->m_Read;
	m_Write = pArray->m_Write;
	m_STEPTest = 0;
	m_STEPTemp = 0;
	m_STEPCalibrate = 0;
	m_Notes = pArray->m_Notes;
	m_Sections = pArray->m_Sections;
}

void CSTEPPage::PasteData(CSTEPPage *pArray)
{
	pArray->m_Sections = m_Sections;
	for (int i=0; i<8; i++) {	
		pArray->m_MinValue[i] = m_MinValue[i];
		pArray->m_MaxValue[i] = m_MaxValue[i];
		pArray->m_Formula[i] = m_Formula[i];
		pArray->m_STEPMin[i] = m_STEPMin[i];
		pArray->m_STEPMax[i] = m_STEPMax[i];
		pArray->m_STEPOffset[i] = m_STEPOffset[i];
	}
	pArray->m_VarTokenName = m_VarTokenName;
	pArray->m_VarTokenIndex = m_VarTokenIndex;
	pArray->m_SimType = m_SimType;
	pArray->m_Read = m_Read;
	pArray->m_Write = m_Write;
	pArray->m_STEPTest = 0;
	pArray->m_STEPTemp = 0;
	pArray->m_STEPCalibrate = 0;
	pArray->m_Notes = m_Notes;
	pArray->m_Sections = m_Sections;
}

void CSTEPPage::Clear()
{
	memset(m_MinValue, 0, 8);
	memset(m_MaxValue, 0, 8);
	for (int i=0; i<8; i++)
		m_Formula[i].Empty();
	memset(m_STEPMin, 0, 8);
	memset(m_STEPMax, 0, 8);
	memset(m_STEPOffset, 0, 8);
	m_VarTokenName.Empty();
	m_VarTokenIndex = 0;
	m_SimType = SIM_NONE;
	m_Read = false;
	m_Write = false;
	m_STEPTest = 0;
	m_STEPTemp = 0;
	m_STEPCalibrate = 0;
	m_Notes.Empty(); 
	m_Sections = 0;
}

BOOL CSTEPPage::IsSet()
{
	if (!m_VarTokenName.IsEmpty())
		return TRUE;
	if (!m_Notes.IsEmpty())
		return TRUE;
	if (m_Sections > 0)
		return TRUE;

	return FALSE;
}