// CLCDTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "CLCDTab.h"
//#include "F4VarList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLCDTab dialog


CCLCDTab::CCLCDTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CCLCDTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLCDTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCLCDTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLCDTab)
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_TAB1, m_CLCDLinesTabCtrl);
	DDX_Control(pDX, IDC_OUTPUTNAME, m_IOName);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_ENABLE_DED, m_DEDEnable);
	DDX_Control(pDX, IDC_LAMPTEST, m_CLCDTest);
	DDX_Control(pDX, IDC_INVERT, m_CLCDInvert);
	DDX_Control(pDX, IDC_STATIC11, m_Static11);
	DDX_Control(pDX, IDC_STATIC12, m_Static12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLCDTab, CResizableDialog)
	//{{AFX_MSG_MAP(CCLCDTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_LAMPTEST, OnCLCDTest)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLCDTab message handlers

BOOL CCLCDTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_DEDEnable.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_DEDEnable.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_DEDEnable.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	CRect r;
	m_DEDEnable.GetWindowRect(r);
	ScreenToClient(r);
	m_DEDEnable.MoveWindow(r.left, r.top, 15, 15);
	m_DEDEnable.EnableWindow(false);

	m_CLCDTest.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_CLCDTest.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_CLCDTest.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_CLCDTest.GetWindowRect(r);
	ScreenToClient(r);
	m_CLCDTest.MoveWindow(r.left, r.top, 15, 15);

	m_CLCDInvert.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_CLCDInvert.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_CLCDInvert.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_CLCDInvert.GetWindowRect(r);
	ScreenToClient(r);
	m_CLCDInvert.MoveWindow(r.left, r.top, 15, 15);

	m_CLCDInvert.EnableWindow(false);
	m_Static11.EnableWindow(false);
	m_Static12.EnableWindow(false);
	m_Static3.EnableWindow(false);

	m_CLCDLinesTabCtrl.InitPages();
	InitCLCDTab();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC11, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_OUTPUTNAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC5, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB1, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_STATIC1, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LAMPTEST, BOTTOM_LEFT);
	AddAnchor(IDC_STATIC2, BOTTOM_LEFT);
	AddAnchor(IDC_INVERT, BOTTOM_CENTER);
	AddAnchor(IDC_STATIC3, BOTTOM_CENTER);

	AddAnchor(IDC_STATIC4, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCLCDTab::InitCLCDTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("Character LCD : "));
		m_Enable.SetWindowText(_T(""));

		OnClearButton();
	} else {
		CString str, sPort, sID;
		CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

		// KLUDGE for PLV2
		sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
		sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem));
		if (sPort.Find(sID) != -1) {
			str.Format(_T("%s %s : %s"), 
						sID,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
						theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		} else {
			str.Format(	_T("%s %s : Port %s - Character LCD : %s"), 
						sID,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
						Alpha[theApp.m_CurPortIndex],
						theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		}

		m_IOName.SetWindowText(str);
		m_Notes.SetWindowText(_T(""));
		
		m_DEDEnable.SetCheck(false);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_DEDEnable = 0;
		m_CLCDTest.SetCheck(false);
		m_CLCDInvert.SetCheck(false);

		// Set all data from DevArray
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDInvert == 1) {
			m_CLCDInvert.SetCheck(true);
		} else {
			m_CLCDInvert.SetCheck(false);
		}

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDActive[theApp.m_CurPortIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDDisabled[theApp.m_CurPortIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_Notes);
	}

	m_CLCDLinesTabCtrl.m_LCDLinesTab.InitLCDLinesTab();
}

void CCLCDTab::OnCLCDTest() 
{
	if (m_CLCDTest.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDTest = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDTest = 0;
	}
}

void CCLCDTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_Notes);
}

void CCLCDTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.SetWindowText(_T(""));
	OnChangeNotes();

	m_DEDEnable.SetCheck(0);
	
	m_CLCDTest.SetCheck(0);
	OnCLCDTest();
}

BOOL CCLCDTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

