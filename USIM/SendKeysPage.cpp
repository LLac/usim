// SendKeysPage.cpp: implementation of the CSendKeysPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "SendKeysPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSendKeysPage::CSendKeysPage()
{
	Clear();
}

CSendKeysPage::~CSendKeysPage()
{

}

void CSendKeysPage::Serialize( CArchive &ar )
{
	UINT i;
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

		for (i=0; i<MAX_MACROS; i++) {
			ar << m_OPKeyMacros[i].iAlt;
			ar << m_OPKeyMacros[i].iShift;
			ar << m_OPKeyMacros[i].iCtrl;
			ar << m_OPKeyMacros[i].iHoldDelay;
			ar << m_OPKeyMacros[i].iNum;
			ar << m_OPKeyMacros[i].iCaps;
			ar << m_OPKeyMacros[i].iScroll;
			ar << m_OPKeyMacros[i].iMacroDelay;
			ar << m_OPKeyMacros[i].iVKCode;
			ar << m_OPKeyMacros[i].iSKCode;
			ar << m_OPKeyMacros[i].iMacroString;
			ar << m_OPKeyMacros[i].iExtended;
		}

		for (i=0; i<MAX_MACROS; i++) {
			ar << m_ORKeyMacros[i].iAlt;
			ar << m_ORKeyMacros[i].iShift;
			ar << m_ORKeyMacros[i].iCtrl;
			ar << m_ORKeyMacros[i].iHoldDelay;
			ar << m_ORKeyMacros[i].iNum;
			ar << m_ORKeyMacros[i].iCaps;
			ar << m_ORKeyMacros[i].iScroll;
			ar << m_ORKeyMacros[i].iMacroDelay;
			ar << m_ORKeyMacros[i].iVKCode;
			ar << m_ORKeyMacros[i].iSKCode;
			ar << m_ORKeyMacros[i].iMacroString;
			ar << m_ORKeyMacros[i].iExtended;
		}

		ar << m_OPHoldKey;
		ar << m_OPRepeat;
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		for (i=0; i<MAX_MACROS; i++) {
			ar >> m_OPKeyMacros[i].iAlt;
			ar >> m_OPKeyMacros[i].iShift;
			ar >> m_OPKeyMacros[i].iCtrl;
			ar >> m_OPKeyMacros[i].iHoldDelay;
			ar >> m_OPKeyMacros[i].iNum;
			ar >> m_OPKeyMacros[i].iCaps;
			ar >> m_OPKeyMacros[i].iScroll;
			ar >> m_OPKeyMacros[i].iMacroDelay;
			ar >> m_OPKeyMacros[i].iVKCode;
			ar >> m_OPKeyMacros[i].iSKCode;
			ar >> m_OPKeyMacros[i].iMacroString;
			ar >> m_OPKeyMacros[i].iExtended;
		}

		for (i=0; i<MAX_MACROS; i++) {
			ar >> m_ORKeyMacros[i].iAlt;
			ar >> m_ORKeyMacros[i].iShift;
			ar >> m_ORKeyMacros[i].iCtrl;
			ar >> m_ORKeyMacros[i].iHoldDelay;
			ar >> m_ORKeyMacros[i].iNum;
			ar >> m_ORKeyMacros[i].iCaps;
			ar >> m_ORKeyMacros[i].iScroll;
			ar >> m_ORKeyMacros[i].iMacroDelay;
			ar >> m_ORKeyMacros[i].iVKCode;
			ar >> m_ORKeyMacros[i].iSKCode;
			ar >> m_ORKeyMacros[i].iMacroString;
			ar >> m_ORKeyMacros[i].iExtended;
		}

		ar >> m_OPHoldKey;
		ar >> m_OPRepeat;
		ar >> m_Notes;
	}
}

void CSendKeysPage::CopyData(CSendKeysPage *pArray)
{
	UINT i;

	for (i=0; i<MAX_MACROS; i++) {
		m_OPKeyMacros[i].iAlt = pArray->m_OPKeyMacros[i].iAlt;
		m_OPKeyMacros[i].iShift = pArray->m_OPKeyMacros[i].iShift;
		m_OPKeyMacros[i].iCtrl = pArray->m_OPKeyMacros[i].iCtrl;
		m_OPKeyMacros[i].iHoldDelay = pArray->m_OPKeyMacros[i].iHoldDelay;
		m_OPKeyMacros[i].iNum = pArray->m_OPKeyMacros[i].iNum;
		m_OPKeyMacros[i].iCaps = pArray->m_OPKeyMacros[i].iCaps;
		m_OPKeyMacros[i].iScroll = pArray->m_OPKeyMacros[i].iScroll;
		m_OPKeyMacros[i].iMacroDelay = pArray->m_OPKeyMacros[i].iMacroDelay;
		m_OPKeyMacros[i].iVKCode = pArray->m_OPKeyMacros[i].iVKCode;
		m_OPKeyMacros[i].iSKCode = pArray->m_OPKeyMacros[i].iSKCode;
		m_OPKeyMacros[i].iMacroString = pArray->m_OPKeyMacros[i].iMacroString;
		m_OPKeyMacros[i].iExtended = pArray->m_OPKeyMacros[i].iExtended;
	}

	for (i=0; i<MAX_MACROS; i++) {
		m_ORKeyMacros[i].iAlt = pArray->m_ORKeyMacros[i].iAlt;
		m_ORKeyMacros[i].iShift = pArray->m_ORKeyMacros[i].iShift;
		m_ORKeyMacros[i].iCtrl = pArray->m_ORKeyMacros[i].iCtrl;
		m_ORKeyMacros[i].iHoldDelay = pArray->m_ORKeyMacros[i].iHoldDelay;
		m_ORKeyMacros[i].iNum = pArray->m_ORKeyMacros[i].iNum;
		m_ORKeyMacros[i].iCaps = pArray->m_ORKeyMacros[i].iCaps;
		m_ORKeyMacros[i].iScroll = pArray->m_ORKeyMacros[i].iScroll;
		m_ORKeyMacros[i].iMacroDelay = pArray->m_ORKeyMacros[i].iMacroDelay;
		m_ORKeyMacros[i].iVKCode = pArray->m_ORKeyMacros[i].iVKCode;
		m_ORKeyMacros[i].iSKCode = pArray->m_ORKeyMacros[i].iSKCode;
		m_ORKeyMacros[i].iMacroString = pArray->m_ORKeyMacros[i].iMacroString;
		m_ORKeyMacros[i].iExtended = pArray->m_ORKeyMacros[i].iExtended;
	}

	m_OPHoldKey = pArray->m_OPHoldKey;
	m_OPRepeat = pArray->m_OPRepeat;
	m_Notes = pArray->m_Notes;
}

void CSendKeysPage::PasteData(CSendKeysPage *pArray)
{
	UINT i;

	for (i=0; i<MAX_MACROS; i++) {
		pArray->m_OPKeyMacros[i].iAlt = m_OPKeyMacros[i].iAlt;
		pArray->m_OPKeyMacros[i].iShift = m_OPKeyMacros[i].iShift;
		pArray->m_OPKeyMacros[i].iCtrl = m_OPKeyMacros[i].iCtrl;
		pArray->m_OPKeyMacros[i].iHoldDelay = m_OPKeyMacros[i].iHoldDelay;
		pArray->m_OPKeyMacros[i].iNum = m_OPKeyMacros[i].iNum;
		pArray->m_OPKeyMacros[i].iCaps = m_OPKeyMacros[i].iCaps;
		pArray->m_OPKeyMacros[i].iScroll = m_OPKeyMacros[i].iScroll;
		pArray->m_OPKeyMacros[i].iMacroDelay = m_OPKeyMacros[i].iMacroDelay;
		pArray->m_OPKeyMacros[i].iVKCode = m_OPKeyMacros[i].iVKCode;
		pArray->m_OPKeyMacros[i].iSKCode = m_OPKeyMacros[i].iSKCode;
		pArray->m_OPKeyMacros[i].iMacroString = m_OPKeyMacros[i].iMacroString;
		pArray->m_OPKeyMacros[i].iExtended = m_OPKeyMacros[i].iExtended;
	}

	for (i=0; i<MAX_MACROS; i++) {
		pArray->m_ORKeyMacros[i].iAlt = m_ORKeyMacros[i].iAlt;
		pArray->m_ORKeyMacros[i].iShift = m_ORKeyMacros[i].iShift;
		pArray->m_ORKeyMacros[i].iCtrl = m_ORKeyMacros[i].iCtrl;
		pArray->m_ORKeyMacros[i].iHoldDelay = m_ORKeyMacros[i].iHoldDelay;
		pArray->m_ORKeyMacros[i].iNum = m_ORKeyMacros[i].iNum;
		pArray->m_ORKeyMacros[i].iCaps = m_ORKeyMacros[i].iCaps;
		pArray->m_ORKeyMacros[i].iScroll = m_ORKeyMacros[i].iScroll;
		pArray->m_ORKeyMacros[i].iMacroDelay = m_ORKeyMacros[i].iMacroDelay;
		pArray->m_ORKeyMacros[i].iVKCode = m_ORKeyMacros[i].iVKCode;
		pArray->m_ORKeyMacros[i].iSKCode = m_ORKeyMacros[i].iSKCode;
		pArray->m_ORKeyMacros[i].iMacroString = m_ORKeyMacros[i].iMacroString;
		pArray->m_ORKeyMacros[i].iExtended = m_ORKeyMacros[i].iExtended;
	}

	pArray->m_OPHoldKey = m_OPHoldKey;
	pArray->m_OPRepeat = m_OPRepeat;
	pArray->m_Notes = m_Notes;
}

void CSendKeysPage::Clear()
{
	OPClear();
	ORClear();

	m_OPHoldKey = false;
	m_OPRepeat = false;
	m_Notes.Empty();
}

void CSendKeysPage::OPClear()
{
	UINT i;
	for (i=0; i<MAX_MACROS; i++) {
		m_OPKeyMacros[i].iAlt = 0;
		m_OPKeyMacros[i].iShift = 0;
		m_OPKeyMacros[i].iCtrl = 0;
		m_OPKeyMacros[i].iHoldDelay = 0;
		m_OPKeyMacros[i].iNum = 0;
		m_OPKeyMacros[i].iCaps = 0;
		m_OPKeyMacros[i].iScroll = 0;
		m_OPKeyMacros[i].iMacroDelay = 0;
		m_OPKeyMacros[i].iVKCode = 0;
		m_OPKeyMacros[i].iSKCode = 0;
		m_OPKeyMacros[i].iMacroString.Empty();
		m_OPKeyMacros[i].iExtended = false;
	}
}

void CSendKeysPage::ORClear()
{
	UINT i;
	for (i=0; i<MAX_MACROS; i++) {
		m_ORKeyMacros[i].iAlt = 0;
		m_ORKeyMacros[i].iShift = 0;
		m_ORKeyMacros[i].iCtrl = 0;
		m_ORKeyMacros[i].iHoldDelay = 0;
		m_ORKeyMacros[i].iNum = 0;
		m_ORKeyMacros[i].iCaps = 0;
		m_ORKeyMacros[i].iScroll = 0;
		m_ORKeyMacros[i].iMacroDelay = 0;
		m_ORKeyMacros[i].iVKCode = 0;
		m_ORKeyMacros[i].iSKCode = 0;
		m_ORKeyMacros[i].iMacroString.Empty();
		m_ORKeyMacros[i].iExtended = false;
	}
}

BOOL CSendKeysPage::IsSet()
{
	if (m_OPKeyMacros[0].iVKCode != 0)
		return TRUE;
	if (m_ORKeyMacros[0].iVKCode != 0)
		return TRUE;
	if (!m_Notes.IsEmpty())
		return TRUE;

	return FALSE;
}