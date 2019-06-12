// FlagsPage.cpp: implementation of the CFlagsPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "FlagsPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlagsPage::CFlagsPage()
{
	Clear();
}

CFlagsPage::~CFlagsPage()
{

}

void CFlagsPage::Serialize( CArchive &ar )
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

		for (i=0; i<MAX_INPUT_FLAGS; i++) {
			ar << Active[i];
		}
		ar << m_Notes;
		ar << m_PowerFlag;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		for (i=0; i<MAX_INPUT_FLAGS; i++) {
			ar >> Active[i];
		}
		ar >> m_Notes;
		ar >> m_PowerFlag;
	}
}

void CFlagsPage::CopyData(CFlagsPage *pArray)
{
	for (int i=0; i<MAX_INPUT_FLAGS; i++) {	
		Active[i] = pArray->Active[i];
	}
	m_Notes = pArray->m_Notes;
	m_PowerFlag = pArray->m_PowerFlag;
}

void CFlagsPage::PasteData(CFlagsPage *pArray)
{
	for (int i=0; i<MAX_INPUT_FLAGS; i++) {	
		 pArray->Active[i] = Active[i];
	}
	pArray->m_Notes = m_Notes;
	pArray->m_PowerFlag = m_PowerFlag;
}

void CFlagsPage::Clear()
{
	for (int i=0; i<MAX_INPUT_FLAGS; i++) {	
		Active[i] = 0;
	}
	m_Notes.Empty(); 
	m_PowerFlag = 0;
}

BOOL CFlagsPage::IsSet()
{
	if (m_PowerFlag != 0)
		return TRUE;

	for (int i=0; i<MAX_INPUT_FLAGS; i++) {	
		if (Active[i] != 0) return TRUE;
	}

	return FALSE;
}
