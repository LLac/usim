// RwrPage.cpp: implementation of the CRwrPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "RwrPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRwrPage::CRwrPage()
{
	Clear();
}

CRwrPage::~CRwrPage()
{

}

void CRwrPage::Serialize( CArchive &ar )
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

		ar << m_RWREnable;
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		ar >> m_RWREnable;
		ar >> m_Notes;
	}
}

void CRwrPage::CopyData(CRwrPage *pArray)
{
	m_RWREnable = pArray->m_RWREnable;
	m_RWRTest = 0;
	m_Notes = pArray->m_Notes;
}

void CRwrPage::PasteData(CRwrPage *pArray)
{
	pArray->m_RWREnable = m_RWREnable;
	pArray->m_RWRTest = 0;
	pArray->m_Notes = m_Notes;
}

void CRwrPage::Clear()
{
	m_RWREnable = 0;
	m_RWRTest = 0;
	m_Notes.Empty();
}

BOOL CRwrPage::IsSet()
{
	if (m_RWREnable != 0)
		return TRUE;
	if (!m_Notes.IsEmpty())
		return TRUE;

	return FALSE;
}