// SPITabCtrl.cpp: implementation of the CSPITabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "SPITabCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPITabCtrl::CSPITabCtrl()
{
	m_Init = false;
}

CSPITabCtrl::~CSPITabCtrl()
{

}

BEGIN_MESSAGE_MAP(CSPITabCtrl, CXTabCtrl)
	//{{AFX_MSG_MAP(CSPITabCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSPITabCtrl::InitTabs()
{
	//Setup the Image list for Misc Icons
	m_ImageList.DeleteImageList();
	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_NOTES = m_ImageList.Add(theApp.LoadIcon(IDI_NOTES));
	m_NOTES_P = m_ImageList.Add(theApp.LoadIcon(IDI_NOTES_B));
	m_FLAGS = m_ImageList.Add(theApp.LoadIcon(IDI_COND));
	m_FLAGS_P = m_ImageList.Add(theApp.LoadIcon(IDI_COND_B));
	m_SPI = m_ImageList.Add(theApp.LoadIcon(IDI_SPIS));
	m_SPI_P = m_ImageList.Add(theApp.LoadIcon(IDI_SPIS_P));

	// Setup the tab control
	SetImageList ( &m_ImageList );

	m_NotesTab.Create (IDD_NOTES, this);
	AddTab (&m_NotesTab, _T("Notes"), m_NOTES);
	m_NotesTab.m_pTab = this;

	m_FlagsTab.Create (IDD_OUTPUT_FLAGS, this);
	AddTab (&m_FlagsTab, _T("Output Conditions"), m_FLAGS);
	m_FlagsTab.m_pTab = this;

	m_SPITab.Create (IDD_SPIDAC, this);
	AddTab (&m_SPITab, _T("SPI Output"), m_SPI);
	m_SPITab.m_pTab = this;

	m_Init = true;
}

void CSPITabCtrl::InitAllTabs()
{
	m_NotesTab.InitNotesTab();
	m_FlagsTab.InitFlagsTab();
	m_SPITab.InitSPITab();
}

void CSPITabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CXTabCtrl::OnSize(nType, cx, cy);

	if (!m_Init) return;

	RECT lpRect;
	m_NotesTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_NotesTab.MoveWindow(&lpRect, TRUE);

	m_FlagsTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_FlagsTab.MoveWindow(&lpRect, TRUE);

	m_SPITab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_SPITab.MoveWindow(&lpRect, TRUE);
}