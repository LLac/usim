// GLCDLinesTabCtrl.cpp: implementation of the CGLCDLinesTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "GLCDLinesTabCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLCDLinesTabCtrl::CGLCDLinesTabCtrl()
{
	m_Init = false;
}

CGLCDLinesTabCtrl::~CGLCDLinesTabCtrl()
{

}

BEGIN_MESSAGE_MAP(CGLCDLinesTabCtrl, CXTabCtrl)
	//{{AFX_MSG_MAP(CGLCDLinesTabCtrl)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CGLCDLinesTabCtrl::InitPages()
{
	//Setup the Image list for Misc Icons
	m_ImageList.DeleteImageList();
	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_PAGE = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE));
	m_PAGE_P = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE_P));
	m_PAGE_DIS = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE_DIS));
	m_PAGE_DIS_P = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE_DIS_P));
	
	// Setup the tab control
	SetImageList ( &m_ImageList );

	CString str;
	m_LCDLinesTab.Create (IDD_LCD_LINES, this);
	m_LCDLinesTab.m_pTab = this;
	for (int i=0; i<LCD_MAX_PAGES; i++) {
		str.Format("Page %d", i+1);
		AddTab (&m_LCDLinesTab, (LPSTR)(LPCTSTR)str, m_PAGE_DIS);
	}

	m_Init = true;
	SelectTab(0);
	m_LCDLinesTab.ShowWindow(SW_SHOW);
}

void CGLCDLinesTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CXTabCtrl::OnSize(nType, cx, cy);

	if (!m_Init) return;

	RECT lpRect;

	m_LCDLinesTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_LCDLinesTab.MoveWindow(&lpRect, TRUE);
}

void CGLCDLinesTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CXTabCtrl::OnSelchange(pNMHDR, pResult);

	m_LCDLinesTab.InitLCDLinesTab();
}