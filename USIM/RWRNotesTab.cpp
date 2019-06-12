// RWRNotesTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "RWRNotesTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRWRNotesTab dialog


CRWRNotesTab::CRWRNotesTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CRWRNotesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRWRNotesTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CRWRNotesTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRWRNotesTab)
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_DESC, m_Desc);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRWRNotesTab, CResizableDialog)
	//{{AFX_MSG_MAP(CRWRNotesTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_EN_CHANGE(IDC_DESC, OnChangeDesc)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWRNotesTab message handlers

BOOL CRWRNotesTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	InitNotesTab();

	AddAnchor(IDC_STATIC1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_IONAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_DESC, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_NOTES, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRWRNotesTab::InitNotesTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("CRT RWR : "));
		m_Enable.SetWindowText(_T(""));

		OnClear();
	} else {
		// Check IO Status
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrActive[theApp.m_CurPortIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrDisabled[theApp.m_CurPortIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Desc.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Desc);
		SetTreeItemDesc();
		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Notes);
	}
}

void CRWRNotesTab::SetTreeItemDesc()
{
	CString str;
	CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

	str.Format("%s : %s",
		theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrhItem[theApp.m_CurPortIndex]),
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrhItem[theApp.m_CurPortIndex], str);

	str.Format(_T("%s %s : Port %s - CRT RWR : %s"), 
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
				Alpha[theApp.m_CurPortIndex],
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Desc);
	m_IOName.SetWindowText(str);
}

void CRWRNotesTab::OnChangeDesc()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Desc.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Desc);
	SetTreeItemDesc();
}

void CRWRNotesTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Notes);
}

void CRWRNotesTab::OnClear() 
{
	m_Desc.SetWindowText("");
	m_Notes.SetWindowText("");
}

void CRWRNotesTab::OnClearButton() 
{
	OnClear();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].Clear();

	CString str;
	str.Format("%s : ",	theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrhItem[theApp.m_CurPortIndex]));
	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrhItem[theApp.m_CurPortIndex], str);
}

BOOL CRWRNotesTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
