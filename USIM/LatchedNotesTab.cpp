// LatchedNotesTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "LatchedNotesTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLatchedNotesTab dialog


CLatchedNotesTab::CLatchedNotesTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CLatchedNotesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLatchedNotesTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CLatchedNotesTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLatchedNotesTab)
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_DESC, m_Desc);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLatchedNotesTab, CResizableDialog)
	//{{AFX_MSG_MAP(CLatchedNotesTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_EN_CHANGE(IDC_DESC, OnChangeDesc)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLatchedNotesTab message handlers

BOOL CLatchedNotesTab::OnInitDialog() 
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

void CLatchedNotesTab::InitNotesTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("Latched Output : "));
		m_Enable.SetWindowText("");

		OnClear();

	} else {
		// Check IO Status
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOActive[theApp.m_CurItemIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LODisabled[theApp.m_CurItemIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Desc.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LONotesArray[theApp.m_CurItemIndex].m_Desc);
		SetTreeItemDesc();
		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LONotesArray[theApp.m_CurItemIndex].m_Notes);
	}
}

void CLatchedNotesTab::SetTreeItemDesc()
{
	CString str;
	str.Format("%s : %s",
		theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOhItem[theApp.m_CurItemIndex]),
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LONotesArray[theApp.m_CurItemIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOhItem[theApp.m_CurItemIndex], str);

	str.Format(_T("%s %s : %s - Latched Output %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
		theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem))),
		theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	m_IOName.SetWindowText(str);
}

void CLatchedNotesTab::OnChangeDesc()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Desc.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LONotesArray[theApp.m_CurItemIndex].m_Desc);
	SetTreeItemDesc();
}

void CLatchedNotesTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LONotesArray[theApp.m_CurItemIndex].m_Notes);
}

void CLatchedNotesTab::OnClear() 
{
	m_Desc.SetWindowText("");
	m_Notes.SetWindowText("");
}

void CLatchedNotesTab::OnClearButton() 
{
	OnClear();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LONotesArray[theApp.m_CurItemIndex].Clear();

	CString str;
	str.Format("%s : ",	theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOhItem[theApp.m_CurItemIndex]));
	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOhItem[theApp.m_CurItemIndex], str);
}

BOOL CLatchedNotesTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
