// GLCDTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "GLCDTab.h"
//#include "F4VarList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLCDTab dialog


CGLCDTab::CGLCDTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CGLCDTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGLCDTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CGLCDTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGLCDTab)
	DDX_Control(pDX, IDC_TAB1, m_GLCDLinesTabCtrl);
	DDX_Control(pDX, IDC_OUTPUTNAME, m_IOName);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_ENABLE_DED, m_DEDEnable);
	DDX_Control(pDX, IDC_LAMPTEST, m_GLCDTest);
	DDX_Control(pDX, IDC_INVERT, m_GLCDInvert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGLCDTab, CResizableDialog)
	//{{AFX_MSG_MAP(CGLCDTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_ENABLE_DED, OnDEDEnable)
	ON_BN_CLICKED(IDC_LAMPTEST, OnGLCDTest)
	ON_BN_CLICKED(IDC_INVERT, OnGLCDInvert)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLCDTab message handlers

BOOL CGLCDTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_DEDEnable.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_DEDEnable.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_DEDEnable.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	CRect r;
	m_DEDEnable.GetWindowRect(r);
	ScreenToClient(r);
	m_DEDEnable.MoveWindow(r.left, r.top, 15, 15);

	m_GLCDTest.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_GLCDTest.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_GLCDTest.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_GLCDTest.GetWindowRect(r);
	ScreenToClient(r);
	m_GLCDTest.MoveWindow(r.left, r.top, 15, 15);

	m_GLCDInvert.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_GLCDInvert.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_GLCDInvert.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_GLCDInvert.GetWindowRect(r);
	ScreenToClient(r);
	m_GLCDInvert.MoveWindow(r.left, r.top, 15, 15);

	m_GLCDLinesTabCtrl.InitPages();
	InitGLCDTab();

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

void CGLCDTab::InitGLCDTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("Graphic LCD : "));
		m_Enable.SetWindowText(_T(""));

		OnClearButton();
	} else {
		CString str;
		CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

		str.Format(_T("%s %s : Port %s - Graphic LCD : %s"),
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
					Alpha[theApp.m_CurPortIndex],
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDNotesArray[theApp.m_CurPortIndex].m_Desc);
		m_IOName.SetWindowText(str);
		m_Notes.SetWindowText(_T(""));
		
		m_DEDEnable.SetCheck(false);
		m_GLCDTest.SetCheck(false);
		m_GLCDInvert.SetCheck(false);

		// Set all data from DevArray
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_DEDEnable == 1) {
			m_DEDEnable.SetCheck(1);
		} else {
			m_DEDEnable.SetCheck(0);
		}

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_LCDInvert == 1) {
			m_GLCDInvert.SetCheck(true);
		} else {
			m_GLCDInvert.SetCheck(false);
		}

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDActive[theApp.m_CurPortIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDDisabled[theApp.m_CurPortIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_Notes);
	}

	m_GLCDLinesTabCtrl.m_LCDLinesTab.InitLCDLinesTab();
}

void CGLCDTab::OnDEDEnable() 
{
	if (m_DEDEnable.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_DEDEnable = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_DEDEnable = 0;
	}
}

void CGLCDTab::OnGLCDTest() 
{
	if (m_GLCDTest.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_LCDTest = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_LCDTest = 0;
	}
}

void CGLCDTab::OnGLCDInvert() 
{
	if (m_GLCDInvert.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_LCDInvert = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_LCDInvert = 0;
	}
}

void CGLCDTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_Notes);
}

void CGLCDTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.SetWindowText(_T(""));
	OnChangeNotes();

	m_DEDEnable.SetCheck(0);
	OnDEDEnable();
	
	m_GLCDTest.SetCheck(0);
	OnGLCDTest();

	m_GLCDInvert.SetCheck(0);
	OnGLCDInvert();
}


BOOL CGLCDTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

