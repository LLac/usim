// NotesTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "NotesTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotesTab dialog


CNotesTab::CNotesTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CNotesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotesTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CNotesTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotesTab)
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_DESC, m_Desc);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotesTab, CResizableDialog)
	//{{AFX_MSG_MAP(CNotesTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_EN_CHANGE(IDC_DESC, OnChangeDesc)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotesTab message handlers

BOOL CNotesTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	InitNotesTab();

	AddAnchor(IDC_STATIC1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);

	AddAnchor(IDC_DESC, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_NOTES, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotesTab::InitNotesTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText("Input: ");
		m_Enable.SetWindowText("");

		OnClear();

	} else {
		// Check IO Status
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputActive[theApp.m_CurItemIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputDisabled[theApp.m_CurItemIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Desc.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_NotesArray[theApp.m_CurItemIndex].m_Desc);
		SetTreeItemDesc();
		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_NotesArray[theApp.m_CurItemIndex].m_Notes);
	}
}

void CNotesTab::SetTreeItemDesc()
{
	CString str;
	str.Format("%s : %s",
		theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputhItem[theApp.m_CurItemIndex]),
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_NotesArray[theApp.m_CurItemIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputhItem[theApp.m_CurItemIndex], str);

	str.Format(_T("%s %s : Input %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
		theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	m_IOName.SetWindowText(str);
}

void CNotesTab::OnChangeDesc()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Desc.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_NotesArray[theApp.m_CurItemIndex].m_Desc);
	SetTreeItemDesc();
}

void CNotesTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_NotesArray[theApp.m_CurItemIndex].m_Notes);
}

void CNotesTab::OnClear() 
{
	m_Desc.SetWindowText("");
	m_Notes.SetWindowText("");
}

void CNotesTab::OnClearButton() 
{
	OnClear();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_NotesArray[theApp.m_CurItemIndex].Clear();

	CString str;
	str.Format("%s : ",	theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputhItem[theApp.m_CurItemIndex]));
	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputhItem[theApp.m_CurItemIndex], str);
}

BOOL CNotesTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
