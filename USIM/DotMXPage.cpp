// DotMXPage.cpp: implementation of the CDotMXPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "DotMXPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDotMXPage::CDotMXPage()
{
	Clear();
}

CDotMXPage::~CDotMXPage()
{

}

void CDotMXPage::Serialize( CArchive &ar, UINT uiFileVersion )
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

		for (i = 0; i < 4; i++) {
			ar << m_Displays[i].VarTokenName;
			ar << m_Displays[i].VarArrayTokenIndex;
			ar << m_Displays[i].SimType;
			ar << m_Displays[i].Read;
			ar << m_Displays[i].Write;
			ar << m_Displays[i].UserValue;
			ar << m_Displays[i].Factor;
			ar << m_Displays[i].Constant;
			ar << m_Displays[i].LeadingZero;
			ar << m_Displays[i].Grouping;
			ar << m_Displays[i].Digits;
			ar << m_Displays[i].Minimum;
			ar << m_Displays[i].Maximum;

			ar << m_Displays[i].VarBlinkTokenName;
			ar << m_Displays[i].VarArrayBlinkTokenIndex;
			ar << m_Displays[i].BlinkMask;
			ar << m_Displays[i].BlinkSimType;
			ar << m_Displays[i].BlinkMaskName;
			ar << m_Displays[i].BlinkOnTime;
			ar << m_Displays[i].BlinkOffTime;
		}
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		// check if older file version loading
		switch (uiFileVersion) {
			case ID_SAVEFILEVERSION:
				for (i = 0; i < 4; i++) {
					ar >> m_Displays[i].VarTokenName;
					ar >> m_Displays[i].VarArrayTokenIndex;
					ar >> m_Displays[i].SimType;
					ar >> m_Displays[i].Read;
					ar >> m_Displays[i].Write;
					ar >> m_Displays[i].UserValue;
					ar >> m_Displays[i].Factor;
					ar >> m_Displays[i].Constant;
					ar >> m_Displays[i].LeadingZero;
					ar >> m_Displays[i].Grouping;
					ar >> m_Displays[i].Digits;
					ar >> m_Displays[i].Minimum;
					ar >> m_Displays[i].Maximum;

					m_Displays[i].VarTokenIndex = SearchIndex(m_Displays[i].VarTokenName, m_Displays[i].SimType, 0);

					ar >> m_Displays[i].VarBlinkTokenName;
					ar >> m_Displays[i].VarArrayBlinkTokenIndex;
					ar >> m_Displays[i].BlinkMask;
					ar >> m_Displays[i].BlinkSimType;
					ar >> m_Displays[i].BlinkMaskName;
					ar >> m_Displays[i].BlinkOnTime;
					ar >> m_Displays[i].BlinkOffTime;

					m_Displays[i].VarBlinkTokenIndex = SearchIndex(m_Displays[i].VarBlinkTokenName, m_Displays[i].BlinkSimType, 0);
				}
				break;
			case ID_SAVEFILEVERSION_2:
			case ID_SAVEFILEVERSION_3:
				for (i = 0; i < 4; i++) {
					ar >> m_Displays[i].VarTokenName;
					ar >> m_Displays[i].VarArrayTokenIndex;
					ar >> m_Displays[i].SimType;
					ar >> m_Displays[i].Read;
					ar >> m_Displays[i].Write;
					ar >> m_Displays[i].UserValue;
					ar >> m_Displays[i].Factor;
					ar >> m_Displays[i].Constant;
					ar >> m_Displays[i].LeadingZero;
					ar >> m_Displays[i].Grouping;
					ar >> m_Displays[i].Digits;
					ar >> m_Displays[i].Minimum;
					ar >> m_Displays[i].Maximum;

					m_Displays[i].VarTokenIndex = SearchIndex(m_Displays[i].VarTokenName, m_Displays[i].SimType, 0);
				}
			break;
			case ID_SAVEFILEVERSION_1:
				for (i = 0; i < 4; i++) {
					ar >> m_Displays[i].VarTokenName;
					m_Displays[i].VarArrayTokenIndex = 0;
					ar >> m_Displays[i].SimType;
					ar >> m_Displays[i].Read;
					ar >> m_Displays[i].Write;
					ar >> m_Displays[i].UserValue;
					ar >> m_Displays[i].Factor;
					ar >> m_Displays[i].Constant;
					ar >> m_Displays[i].LeadingZero;
					ar >> m_Displays[i].Grouping;
					ar >> m_Displays[i].Digits;
					ar >> m_Displays[i].Minimum;
					ar >> m_Displays[i].Maximum;

					m_Displays[i].VarTokenIndex = SearchIndex(m_Displays[i].VarTokenName, m_Displays[i].SimType, 0);
				}
			break;
		}

		ar >> m_Notes;
	}
}

void CDotMXPage::CopyData(CDotMXPage *pArray)
{
	for (int i = 0; i < 4; i++) {
		m_Displays[i].VarTokenName = pArray->m_Displays[i].VarTokenName;
		m_Displays[i].VarTokenIndex = pArray->m_Displays[i].VarTokenIndex;
		m_Displays[i].VarArrayTokenIndex = pArray->m_Displays[i].VarArrayTokenIndex;
		m_Displays[i].SimType = pArray->m_Displays[i].SimType;
		m_Displays[i].Read = pArray->m_Displays[i].Read;
		m_Displays[i].Write = pArray->m_Displays[i].Write;
		m_Displays[i].UserValue = pArray->m_Displays[i].UserValue;
		m_Displays[i].Factor = pArray->m_Displays[i].Factor;
		m_Displays[i].Constant = pArray->m_Displays[i].Constant;
		m_Displays[i].LeadingZero = pArray->m_Displays[i].LeadingZero;
		m_Displays[i].Grouping = pArray->m_Displays[i].Grouping;
		m_Displays[i].Digits = pArray->m_Displays[i].Digits;
		m_Displays[i].Minimum = pArray->m_Displays[i].Minimum;
		m_Displays[i].Maximum = pArray->m_Displays[i].Maximum;

		m_Displays[i].VarBlinkTokenName = pArray->m_Displays[i].VarBlinkTokenName;
		m_Displays[i].VarArrayBlinkTokenIndex = pArray->m_Displays[i].VarArrayBlinkTokenIndex;
		m_Displays[i].VarBlinkTokenIndex = pArray->m_Displays[i].VarBlinkTokenIndex;
		m_Displays[i].BlinkMask = pArray->m_Displays[i].BlinkMask;
		m_Displays[i].BlinkSimType = pArray->m_Displays[i].BlinkSimType;
		m_Displays[i].BlinkMaskName = pArray->m_Displays[i].BlinkMaskName;
		m_Displays[i].BlinkOnTime = pArray->m_Displays[i].BlinkOnTime;
		m_Displays[i].BlinkOffTime = pArray->m_Displays[i].BlinkOffTime;
	}

	m_LampTest = 0;
	m_Notes = pArray->m_Notes;
}

void CDotMXPage::PasteData(CDotMXPage *pArray)
{
	for (int i = 0; i < 4; i++) {
		pArray->m_Displays[i].VarTokenName = m_Displays[i].VarTokenName;
		pArray->m_Displays[i].VarTokenIndex = m_Displays[i].VarTokenIndex;
		pArray->m_Displays[i].VarArrayTokenIndex = m_Displays[i].VarArrayTokenIndex;
		pArray->m_Displays[i].SimType = m_Displays[i].SimType;
		pArray->m_Displays[i].Read = m_Displays[i].Read;
		pArray->m_Displays[i].Write = m_Displays[i].Write;
		pArray->m_Displays[i].UserValue = m_Displays[i].UserValue;
		pArray->m_Displays[i].Factor = m_Displays[i].Factor;
		pArray->m_Displays[i].Constant = m_Displays[i].Constant;
		pArray->m_Displays[i].LeadingZero = m_Displays[i].LeadingZero;
		pArray->m_Displays[i].Grouping = m_Displays[i].Grouping;
		pArray->m_Displays[i].Digits = m_Displays[i].Digits;
		pArray->m_Displays[i].Minimum = m_Displays[i].Minimum;
		pArray->m_Displays[i].Maximum = m_Displays[i].Maximum;

		pArray->m_Displays[i].VarBlinkTokenName = m_Displays[i].VarBlinkTokenName;
		pArray->m_Displays[i].VarArrayBlinkTokenIndex = m_Displays[i].VarArrayBlinkTokenIndex;
		pArray->m_Displays[i].VarBlinkTokenIndex = m_Displays[i].VarBlinkTokenIndex;
		pArray->m_Displays[i].BlinkMask = m_Displays[i].BlinkMask;
		pArray->m_Displays[i].BlinkSimType = m_Displays[i].BlinkSimType;
		pArray->m_Displays[i].BlinkMaskName = m_Displays[i].BlinkMaskName;
		pArray->m_Displays[i].BlinkOnTime = m_Displays[i].BlinkOnTime;
		pArray->m_Displays[i].BlinkOffTime = m_Displays[i].BlinkOffTime;
	}

	pArray->m_LampTest = 0;
	pArray->m_Notes = m_Notes;
}

void CDotMXPage::Clear()
{
	for (int i = 0; i < 4; i++) {
		m_Displays[i].VarTokenName.Empty();
		m_Displays[i].VarTokenIndex = 0;
		m_Displays[i].VarArrayTokenIndex = 0;
		m_Displays[i].SimType = SIM_NONE;
		m_Displays[i].Read = false;
		m_Displays[i].Write = false;
		m_Displays[i].UserValue.Empty();
		m_Displays[i].Factor = 1;
		m_Displays[i].Constant = 0;
		m_Displays[i].LeadingZero = 0;
		m_Displays[i].Grouping = 0;
		m_Displays[i].Digits = 0;
		m_Displays[i].Minimum = 0;
		m_Displays[i].Maximum = 0;

		m_Displays[i].VarBlinkTokenName.Empty();
		m_Displays[i].VarArrayBlinkTokenIndex = 0;
		m_Displays[i].VarBlinkTokenIndex = 0;
		m_Displays[i].BlinkMask = 0;
		m_Displays[i].BlinkSimType = SIM_NONE;
		m_Displays[i].BlinkMaskName.Empty();
		m_Displays[i].BlinkOnTime = 0;
		m_Displays[i].BlinkOffTime = 0;
		m_Displays[i].BlinkOnFlag = false;
	}

	m_LampTest = 0;
	m_Notes = _T("");
}

BOOL CDotMXPage::IsSet()
{
	for (int i = 0; i < 4; i++) {
		if (m_Displays[i].Grouping != 0 || m_Displays[i].Digits != 0)
			return true;
	}

	return FALSE;
}