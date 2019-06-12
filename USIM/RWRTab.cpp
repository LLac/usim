// RWRTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "RWRTab.h"
//#include "F4VarList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRWRTab dialog


CRWRTab::CRWRTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CRWRTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRWRTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CRWRTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRWRTab)
	DDX_Control(pDX, IDC_OUTPUTNAME, m_IOName);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_ENABLE_RWR, m_RWREnable);
	DDX_Control(pDX, IDC_LAMPTEST, m_RWRTest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRWRTab, CResizableDialog)
	//{{AFX_MSG_MAP(CRWRTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_ENABLE_RWR, OnEnable)
	ON_BN_CLICKED(IDC_LAMPTEST, OnRWRTest)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWRTab message handlers

BOOL CRWRTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_RWREnable.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_RWREnable.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_RWREnable.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	CRect r;
	m_RWREnable.GetWindowRect(r);
	ScreenToClient(r);
	m_RWREnable.MoveWindow(r.left, r.top, 15, 15);

	m_RWRTest.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_RWRTest.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_RWRTest.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_RWRTest.GetWindowRect(r);
	ScreenToClient(r);
	m_RWRTest.MoveWindow(r.left, r.top, 15, 15);

	InitRWRTab();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_STATIC11, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_OUTPUTNAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC2, BOTTOM_LEFT);
	AddAnchor(IDC_LAMPTEST, BOTTOM_LEFT);

	AddAnchor(IDC_STATIC3, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRWRTab::InitRWRTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("CRT RWR : "));
		m_Enable.SetWindowText(_T(""));

		OnClearButton();
	} else {
		CString str;
		CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

		str.Format(_T("%s %s : Port %s - CRT RWR : %s"), 
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
				Alpha[theApp.m_CurPortIndex],
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Desc);
		m_IOName.SetWindowText(str);
		m_Notes.SetWindowText(_T(""));
		
		m_RWRTest.SetCheck(0);

		// Set all data from DevArray
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_RWREnable == 1)
			m_RWREnable.SetCheck(1);
		else 
			m_RWREnable.SetCheck(0);

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrActive[theApp.m_CurPortIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrDisabled[theApp.m_CurPortIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_Notes);

	}
}


void CRWRTab::OnEnable() 
{
	if (m_RWREnable.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_RWREnable = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_RWREnable = 0;
	}

}

void CRWRTab::OnRWRTest() 
{
	if (m_RWRTest.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_RWRTest = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_RWRTest = 0;
	}
}

void CRWRTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_Notes);
}

void CRWRTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.SetWindowText(_T(""));
	OnChangeNotes();

	m_RWREnable.SetCheck(0);
	OnEnable();

	m_RWRTest.SetCheck(0);
	OnRWRTest();
}


BOOL CRWRTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
