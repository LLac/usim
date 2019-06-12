// DefTabCtrl.cpp: implementation of the CDefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "DefTabCtrl.h"
#include "ResizableLib\ResizableDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDefTabCtrl::CDefTabCtrl()
{
	m_Init = false;
}

CDefTabCtrl::~CDefTabCtrl()
{

}

BEGIN_MESSAGE_MAP(CDefTabCtrl, CXTabCtrl)
	//{{AFX_MSG_MAP(CDefTabCtrl)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDefTabCtrl::InitTabs()
{
	//Setup the Image list for Misc Icons
	m_ImageList.DeleteImageList();
	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_ImageList.Add(theApp.LoadIcon(IDI_USIM_OFF));		// 0
	m_ImageList.Add(theApp.LoadIcon(IDI_SIMS));			// 1
	
	// Setup the tab control
	SetImageList ( &m_ImageList );

	m_DefaultTab.Create (IDD_DEFAULT, this);
	m_SimPathTab.Create (IDD_SIMPATH, this);

	AddTab (&m_DefaultTab, _T("Application Settings"), 0);
	m_DefaultTab.m_pTab = this;
	AddTab (&m_SimPathTab, _T("Simulator Paths"), 1);
	m_SimPathTab.m_pTab = this;

	m_Init = true;
}

void CDefTabCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CXTabCtrl::OnShowWindow(bShow, nStatus);
}

void CDefTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CXTabCtrl::OnSize(nType, cx, cy);

	if (!m_Init) return;

	RECT lpRect;
	m_DefaultTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_DefaultTab.MoveWindow(&lpRect, TRUE);

	m_SimPathTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_SimPathTab.MoveWindow(&lpRect, TRUE);
}
