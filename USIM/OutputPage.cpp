// OutputPage.cpp: implementation of the COutputPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "OutputPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutputPage::COutputPage()
{
	Clear();
}

COutputPage::~COutputPage()
{

}

void COutputPage::Serialize( CArchive &ar, UINT uiFileVersion )
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

		ar << m_VarTokenName;
		ar << m_VarArrayTokenIndex;
		ar << m_SimType;
		ar << m_Read;
		ar << m_Write;
		ar << m_Condition1;
		ar << m_Condition2;
		ar << m_Condition3;
		ar << m_OPValue1;
		ar << m_OPValue2;
		ar << m_BitMask;
		ar << m_BitType;
		ar << m_BitMaskName;

		ar << m_VarBlinkTokenName;
		ar << m_VarArrayBlinkTokenIndex;
		ar << m_BlinkMask;
		ar << m_BlinkType;
		ar << m_BlinkMaskName;
		ar << m_BlinkOnTime;
		ar << m_BlinkOffTime;

		ar << m_OBLevel;
		ar << m_Invert;
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		ar >> m_VarTokenName;
		// check if older file version loading
		if (uiFileVersion == ID_SAVEFILEVERSION_1)
			m_VarArrayTokenIndex = 0;
		else
			ar >> m_VarArrayTokenIndex;
		ar >> m_SimType;
		ar >> m_Read;
		ar >> m_Write;
		ar >> m_Condition1;
		ar >> m_Condition2;
		ar >> m_Condition3;
		ar >> m_OPValue1;
		ar >> m_OPValue2;
		ar >> m_BitMask;
		ar >> m_BitType;
		ar >> m_BitMaskName;
		if (uiFileVersion > ID_SAVEFILEVERSION_2) {
			ar >> m_VarBlinkTokenName;
			ar >> m_VarArrayBlinkTokenIndex;
			ar >> m_BlinkMask;
			ar >> m_BlinkType;
			ar >> m_BlinkMaskName;
			ar >> m_BlinkOnTime;
			ar >> m_BlinkOffTime;
			m_VarBlinkTokenIndex = SearchIndex(m_VarBlinkTokenName, m_SimType, 0);
		}
		ar >> m_OBLevel;
		ar >> m_Invert;
		ar >> m_Notes;

		m_VarTokenIndex = SearchIndex(m_VarTokenName, m_SimType, 0);
	}
}

void COutputPage::CopyData(COutputPage *pArray)
{
	m_VarTokenName = pArray->m_VarTokenName;
	m_VarTokenIndex = pArray->m_VarTokenIndex;
	m_VarArrayTokenIndex = pArray->m_VarArrayTokenIndex;
	m_SimType = pArray->m_SimType;
	m_Read = pArray->m_Read;
	m_Write = pArray->m_Write;
	m_Condition1 = pArray->m_Condition1;
	m_Condition2 = pArray->m_Condition2;
	m_Condition3 = pArray->m_Condition3;
	m_OPValue1 = pArray->m_OPValue1;
	m_OPValue2 = pArray->m_OPValue2;
	m_BitMask = pArray->m_BitMask;
	m_BitType = pArray->m_BitType;
	m_BitMaskName = pArray->m_BitMaskName;

	m_VarBlinkTokenName = pArray->m_VarBlinkTokenName;
	m_VarArrayBlinkTokenIndex = pArray->m_VarArrayBlinkTokenIndex;
	m_VarBlinkTokenIndex = pArray->m_VarBlinkTokenIndex;
	m_BlinkMask = pArray->m_BlinkMask;
	m_BlinkType = pArray->m_BlinkType;
	m_BlinkMaskName = pArray->m_BlinkMaskName;
	m_BlinkOnTime = pArray->m_BlinkOnTime;
	m_BlinkOffTime = pArray->m_BlinkOffTime;

	m_OBLevel = pArray->m_OBLevel;
	m_LampTest = 0;
	m_Invert = pArray->m_Invert;
	m_Notes = pArray->m_Notes;
}

void COutputPage::PasteData(COutputPage *pArray)
{
	pArray->m_VarTokenName = m_VarTokenName;
	pArray->m_VarTokenIndex = m_VarTokenIndex;
	pArray->m_VarArrayTokenIndex = m_VarArrayTokenIndex;
	pArray->m_SimType = m_SimType;
	pArray->m_Read = m_Read;
	pArray->m_Write = m_Write;
	pArray->m_Condition1 = m_Condition1;
	pArray->m_Condition2 = m_Condition2;
	pArray->m_Condition3 = m_Condition3;
	pArray->m_OPValue1 = m_OPValue1;
	pArray->m_OPValue2 = m_OPValue2;
	pArray->m_BitMask = m_BitMask;
	pArray->m_BitType = m_BitType;
	pArray->m_BitMaskName = m_BitMaskName;

	pArray->m_VarBlinkTokenName = m_VarBlinkTokenName;
	pArray->m_VarArrayBlinkTokenIndex = m_VarArrayBlinkTokenIndex;
	pArray->m_VarBlinkTokenIndex = m_VarBlinkTokenIndex;
	pArray->m_BlinkMask = m_BlinkMask;
	pArray->m_BlinkType = m_BlinkType;
	pArray->m_BlinkMaskName = m_BlinkMaskName;
	pArray->m_BlinkOnTime = m_BlinkOnTime;
	pArray->m_BlinkOffTime = m_BlinkOffTime;

	pArray->m_OBLevel = m_OBLevel;
	pArray->m_Invert = m_Invert;
	pArray->m_Notes = m_Notes;
}

void COutputPage::Clear()
{
	m_VarTokenName.Empty();
	m_VarTokenIndex = 0;
	m_VarArrayTokenIndex = 0;
	m_SimType = SIM_NONE;
	m_Read = false;
	m_Write = false;
	m_Condition1 = 0;
	m_Condition2 = 0;
	m_Condition3 = 0;
	m_OPValue1 = 0;
	m_OPValue2 = 0;
	m_BitMask = 0;
	m_BitType = 0;
	m_BitMaskName.Empty();

	m_VarBlinkTokenName.Empty();
	m_VarArrayBlinkTokenIndex = 0;
	m_VarBlinkTokenIndex = 0;
	m_BlinkMask = 0;
	m_BlinkType = 0;
	m_BlinkMaskName.Empty();
	m_BlinkOnTime = 0;
	m_BlinkOffTime = 0;
	m_BlinkOnFlag = false;

	m_OBLevel = 10;
	m_LampTest = 0;
	m_Invert = 0;
	m_Notes = _T("");
}

BOOL COutputPage::IsSet()
{
	if (!m_VarTokenName.IsEmpty())
		return TRUE;
	if (!m_Notes.IsEmpty())
		return TRUE;

	return FALSE;
}

