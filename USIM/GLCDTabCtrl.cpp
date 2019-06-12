// GLCDTabCtrl.cpp: implementation of the CGLCDTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "GLCDTabCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLCDTabCtrl::CGLCDTabCtrl()
{
	m_Init = false;
}

CGLCDTabCtrl::~CGLCDTabCtrl()
{

}

BEGIN_MESSAGE_MAP(CGLCDTabCtrl, CXTabCtrl)
	//{{AFX_MSG_MAP(CGLCDTabCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CGLCDTabCtrl::InitTabs()
{
	//Setup the Image list for Misc Icons
	m_ImageList.DeleteImageList();
	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_NOTES = m_ImageList.Add(theApp.LoadIcon(IDI_NOTES));
	m_NOTES_P = m_ImageList.Add(theApp.LoadIcon(IDI_NOTES_B));
	m_FLAGS = m_ImageList.Add(theApp.LoadIcon(IDI_COND));
	m_FLAGS_P = m_ImageList.Add(theApp.LoadIcon(IDI_COND_B));
	m_GLCD = m_ImageList.Add(theApp.LoadIcon(IDI_GLCD));
	m_GLCD_P = m_ImageList.Add(theApp.LoadIcon(IDI_GLCD_P));
	
	// Setup the tab control
	SetImageList ( &m_ImageList );

	m_NotesTab.Create (IDD_NOTES, this);
	AddTab (&m_NotesTab, _T("Notes"), m_NOTES);
	m_NotesTab.m_pTab = this;

	m_FlagsTab.Create (IDD_OUTPUT_FLAGS, this);
	AddTab (&m_FlagsTab, _T("Output Conditions"), m_FLAGS);
	m_FlagsTab.m_pTab = this;

	m_GLCDTab.Create (IDD_GLCD, this);	
	AddTab (&m_GLCDTab, _T("Graphic LCD"), m_GLCD);
	m_GLCDTab.m_pTab =this;

	m_Init = true;
}

void CGLCDTabCtrl::InitAllTabs()
{
	m_NotesTab.InitNotesTab();
	m_FlagsTab.InitFlagsTab();
	m_GLCDTab.InitGLCDTab();
}

void CGLCDTabCtrl::OnSize(UINT nType, int cx, int cy) 
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

	m_GLCDTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_GLCDTab.MoveWindow(&lpRect, TRUE);
}