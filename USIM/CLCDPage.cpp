// CLCDPage.cpp: implementation of the CCLCDPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "CLCDPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCLCDPage::CCLCDPage()
{
	Clear();
}

CCLCDPage::~CCLCDPage()
{

}

void CCLCDPage::Serialize( CArchive &ar )
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
		
		ar << m_DEDEnable;
		ar << m_LCDRadio;
		ar << m_LCDPageNext;
		ar << m_LCDPagePrev;

		for (i=0; i<LCD_MAX_PAGES; i++)
			ar << m_LCDPageEnable[i];
		
		for (i=0; i<LCD_MAX_PAGES; i++)
			ar << m_LCDPageDisplay[i];

		int j;
		for (i=0; i<LCD_MAX_PAGES; i++)
			for (j=0; j<GLCD_MAX_LINES; j++)
				ar << m_LCDPages[i][j];

		ar << m_LCDInvert;
		ar << m_Notes;
	} else {
		ar >> DataFlag;
		if (!DataFlag) {
			Clear();
			return;
		}

		ar >> m_DEDEnable;
		ar >> m_LCDRadio;
		ar >> m_LCDPageNext;
		ar >> m_LCDPagePrev;

		int i;
		for (i=0; i<LCD_MAX_PAGES; i++)
			ar >> m_LCDPageEnable[i];

		for (i=0; i<LCD_MAX_PAGES; i++)
			ar >> m_LCDPageDisplay[i];

		int j;
		for (i=0; i<LCD_MAX_PAGES; i++)
			for (j=0; j<GLCD_MAX_LINES; j++)
				ar >> m_LCDPages[i][j];

		ar >> m_LCDInvert;
		ar >> m_Notes;
	}
}

void CCLCDPage::CopyData(CCLCDPage *pArray)
{
	m_DEDEnable = pArray->m_DEDEnable;
	m_LCDRadio = pArray->m_LCDRadio;
	m_LCDPageNext = pArray->m_LCDPageNext;
	m_LCDPagePrev = pArray->m_LCDPagePrev;

	int j;
	for (int i=0; i<LCD_MAX_PAGES; i++) {
		m_LCDPageEnable[i] = pArray->m_LCDPageEnable[i];
		m_LCDPageDisplay[i] = pArray->m_LCDPageDisplay[i];
		for (j=0; j<GLCD_MAX_LINES; j++)
			m_LCDPages[i][j] = pArray->m_LCDPages[i][j];
	}

	m_LCDInvert = pArray->m_LCDInvert;
	m_LCDInvertOld = pArray->m_LCDInvertOld;
	m_LCDTest = 0;
	m_LCDTestOld = 0;
	m_Notes = pArray->m_Notes;
}

void CCLCDPage::PasteData(CCLCDPage *pArray)
{
	pArray->m_DEDEnable = m_DEDEnable;
	pArray->m_LCDRadio = m_LCDRadio;
	pArray->m_LCDPageNext = m_LCDPageNext;
	pArray->m_LCDPagePrev = m_LCDPagePrev;

	int j;
	for (int i=0; i<LCD_MAX_PAGES; i++) {
		pArray->m_LCDPageEnable[i] = m_LCDPageEnable[i];
		pArray->m_LCDPageDisplay[i] = m_LCDPageDisplay[i];
		for (j=0; j<GLCD_MAX_LINES; j++)
			pArray->m_LCDPages[i][j] = m_LCDPages[i][j];
	}

	pArray->m_LCDInvert = m_LCDInvert;
	pArray->m_LCDInvertOld = m_LCDInvertOld;
	pArray->m_LCDTest = 0;
	pArray->m_LCDTestOld = 0;
	pArray->m_Notes = m_Notes;
}

void CCLCDPage::Clear()
{
	m_DEDEnable = 0;
	m_LCDRadio = 0;
	m_LCDPageNext = 0;
	m_LCDPagePrev = 0;

	int j;
	for (int i=0; i<LCD_MAX_PAGES; i++) {
		m_LCDPageEnable[i] = 0;
		m_LCDPageDisplay[i] = 0;
		for (j=0; j<GLCD_MAX_LINES; j++)
			m_LCDPages[i][j].Empty();
	}

	m_LCDInvert = 0;
	m_LCDInvertOld = 0;
	m_LCDTest = 0;
	m_LCDTestOld = 0;
	m_Notes.Empty();

	m_LCDCurrentPage = 0;
	m_FlagNextOld = m_FlagPrevOld = 0;
	m_FlagDisplayOld = 0;
}

void CCLCDPage::ClearLCDPage(UINT nPage)
{
	m_LCDPageEnable[nPage] = 0;
	m_LCDPageDisplay[nPage] = 0;

	for (int j=0; j<GLCD_MAX_LINES; j++)
		m_LCDPages[nPage][j].Empty();
}

BOOL CCLCDPage::IsSet()
{
	for (int i=0; i<LCD_MAX_PAGES; i++) {
		if (IsPageSet(i))
			return TRUE;
	}

	if (!m_Notes.IsEmpty())
		return TRUE;

	return FALSE;
}

BOOL CCLCDPage::IsPageSet(UINT nPage)
{
	if (m_LCDPageEnable[nPage] == 1)
		return TRUE;

	if (m_LCDRadio == 0 && (m_LCDPagePrev > 0 || m_LCDPageNext > 0))
		return TRUE;
	
	if (m_LCDRadio == 1 && m_LCDPageDisplay[nPage] > 0)
		return TRUE;

	for (int i=0; i<GLCD_MAX_LINES; i++) {
		if (!m_LCDPages[nPage][i].IsEmpty())
			return TRUE;
	}

	return FALSE;
}
