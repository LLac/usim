// NotesPage.cpp: implementation of the CNotesPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "NotesPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNotesPage::CNotesPage()
{
	Clear();
}

CNotesPage::~CNotesPage()
{

}

void CNotesPage::Serialize( CArchive &ar )
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

		ar << m_Desc;
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		ar >> m_Desc;
		ar >> m_Notes;
	}
}

void CNotesPage::CopyData(CNotesPage *pArray)
{
	m_Desc = pArray->m_Desc;
	m_Notes = pArray->m_Notes;
}

void CNotesPage::PasteData(CNotesPage *pArray)
{
	pArray->m_Desc = m_Desc;
	pArray->m_Notes = m_Notes;
}

void CNotesPage::Clear()
{
	m_Desc.Empty();
	m_Notes.Empty();
}

BOOL CNotesPage::IsSet()
{
	if (!m_Desc.IsEmpty())
		return TRUE;
	if (!m_Notes.IsEmpty())
		return TRUE;

	return FALSE;
}