// VariablesPage.cpp: implementation of the CVariablesPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "VariablesPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVariablesPage::CVariablesPage()
{
	Clear();
}

CVariablesPage::~CVariablesPage()
{

}

void CVariablesPage::Serialize( CArchive &ar, UINT uiFileVersion )
{
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

		ar << m_OPVarType;
		ar << m_OPVarTokenName;
		ar << m_OPVarArrayTokenIndex;
		ar << m_OPSimType;
		ar << m_OPRadio;
		ar << m_OPOperator;
		ar << m_OPMask;
		ar << m_OPValue;
		ar << m_OPInc;
		ar << m_OPDec;
		ar << m_OPRead;
		ar << m_OPWrite;

		ar << m_ORVarType;
		ar << m_ORVarTokenName;
		ar << m_ORVarArrayTokenIndex;
		ar << m_ORSimType;
		ar << m_OROperator;
		ar << m_ORMask;
		ar << m_ORValue;
		ar << m_ORInc;
		ar << m_ORDec;
		ar << m_ORRead;
		ar << m_ORWrite;

		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		ar >> m_OPVarType;
		ar >> m_OPVarTokenName;
		// check if older file version loading
		switch (uiFileVersion) {
			case ID_SAVEFILEVERSION:
			case ID_SAVEFILEVERSION_2:
			case ID_SAVEFILEVERSION_3:
				ar >> m_OPVarArrayTokenIndex;
			break;
			case ID_SAVEFILEVERSION_1:
				m_OPVarArrayTokenIndex = 0;
			break;
		}
		ar >> m_OPSimType;
		ar >> m_OPRadio;
		ar >> m_OPOperator;
		ar >> m_OPMask;
		ar >> m_OPValue;
		ar >> m_OPInc;
		ar >> m_OPDec;
		ar >> m_OPRead;
		ar >> m_OPWrite;

		ar >> m_ORVarType;
		ar >> m_ORVarTokenName;
		// check if older file version loading
		switch (uiFileVersion) {
			case ID_SAVEFILEVERSION:
			case ID_SAVEFILEVERSION_2:
			case ID_SAVEFILEVERSION_3:
				ar >> m_ORVarArrayTokenIndex;
			break;
			case ID_SAVEFILEVERSION_1:
				m_ORVarArrayTokenIndex = 0;
			break;
		}
		ar >> m_ORSimType;
		ar >> m_OROperator;
		ar >> m_ORMask;
		ar >> m_ORValue;
		ar >> m_ORInc;
		ar >> m_ORDec;
		ar >> m_ORRead;
		ar >> m_ORWrite;

		ar >> m_Notes;

		m_OPVarTokenIndex = SearchIndex(m_OPVarTokenName, m_OPSimType, m_OPVarType);
		m_ORVarTokenIndex = SearchIndex(m_ORVarTokenName, m_ORSimType, m_ORVarType);
	}
}

void CVariablesPage::CopyData(CVariablesPage *pArray)
{
	m_OPVarType = pArray->m_OPVarType;
	m_OPVarTokenName = pArray->m_OPVarTokenName;
	m_OPVarTokenIndex = pArray->m_OPVarTokenIndex;
	m_OPVarArrayTokenIndex = pArray->m_OPVarArrayTokenIndex;
	m_OPSimType = pArray->m_OPSimType;
	m_OPRadio = pArray->m_OPRadio;
	m_OPOperator = pArray->m_OPOperator;
	m_OPMask = pArray->m_OPMask;
	m_OPValue = pArray->m_OPValue;
	m_OPInc = pArray->m_OPInc;
	m_OPDec = pArray->m_OPDec;
	m_OPRead = pArray->m_OPRead;
	m_OPWrite = pArray->m_OPWrite;

	m_ORVarType = pArray->m_ORVarType;
	m_ORVarTokenName = pArray->m_ORVarTokenName;
	m_ORVarTokenIndex = pArray->m_ORVarTokenIndex;
	m_ORVarArrayTokenIndex = pArray->m_ORVarArrayTokenIndex;
	m_ORSimType = pArray->m_ORSimType;
	m_ORRadio = pArray->m_ORRadio;
	m_OROperator = pArray->m_OROperator;
	m_ORMask = pArray->m_ORMask;
	m_ORValue = pArray->m_ORValue;
	m_ORInc = pArray->m_ORInc;
	m_ORDec = pArray->m_ORDec;
	m_ORRead = pArray->m_ORRead;
	m_ORWrite = pArray->m_ORWrite;

	m_Notes = pArray->m_Notes;
}

void CVariablesPage::PasteData(CVariablesPage *pArray)
{
	pArray->m_OPVarType = m_OPVarType;
	pArray->m_OPVarTokenName = m_OPVarTokenName;
	pArray->m_OPVarTokenIndex = m_OPVarTokenIndex;
	pArray->m_OPVarArrayTokenIndex = m_OPVarArrayTokenIndex;
	pArray->m_OPSimType = m_OPSimType;
	pArray->m_OPRadio = m_OPRadio;
	pArray->m_OPOperator = m_OPOperator;
	pArray->m_OPMask = m_OPMask;
	pArray->m_OPValue = m_OPValue;
	pArray->m_OPInc = m_OPInc;
	pArray->m_OPDec = m_OPDec;
	pArray->m_OPRead = m_OPRead;
	pArray->m_OPWrite = m_OPRead;

	pArray->m_ORVarType = m_ORVarType;
	pArray->m_ORVarTokenName = m_ORVarTokenName;
	pArray->m_ORVarTokenIndex = m_ORVarTokenIndex;
	pArray->m_ORVarArrayTokenIndex = m_ORVarArrayTokenIndex;
	pArray->m_ORSimType = m_ORSimType;
	pArray->m_ORRadio = m_ORRadio;
	pArray->m_OROperator = m_OROperator;
	pArray->m_ORMask = m_ORMask;
	pArray->m_ORValue = m_ORValue;
	pArray->m_ORInc = m_ORInc;
	pArray->m_ORDec = m_ORDec;
	pArray->m_ORRead = m_ORRead;
	pArray->m_ORWrite = m_ORWrite;

	pArray->m_Notes = m_Notes;
}

void CVariablesPage::Clear()
{
	m_OPVarType = 0;
	m_OPVarTokenName.Empty();
	m_OPVarTokenIndex = 0;
	m_OPVarArrayTokenIndex = 0;
	m_OPSimType = SIM_NONE;
	m_OPRadio = 0;
	m_OPOperator = 0;
	m_OPMask = 0;
	m_OPValue = 0;
	m_OPInc = 0;
	m_OPDec	= 0;
	m_OPRead = false;
	m_OPWrite = false;

	m_ORVarType = 0;
	m_ORVarTokenName.Empty();
	m_ORVarTokenIndex = 0;
	m_ORVarArrayTokenIndex = 0;
	m_ORSimType = SIM_NONE;
	m_ORRadio = 0;
	m_OROperator = 0;
	m_ORMask = 0;
	m_ORValue = 0;
	m_ORInc = 0;
	m_ORDec	= 0;
	m_ORRead = false;
	m_ORWrite = false;

	m_Notes.Empty();
}

BOOL CVariablesPage::IsSet()
{
	if (!m_OPVarTokenName.IsEmpty() || !m_ORVarTokenName.IsEmpty())
		return TRUE;
	if (!m_Notes.IsEmpty())
		return TRUE;

	return FALSE;
}