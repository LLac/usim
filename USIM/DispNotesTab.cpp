// DispNotesTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "DispNotesTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispNotesTab dialog


CDispNotesTab::CDispNotesTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDispNotesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDispNotesTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDispNotesTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispNotesTab)
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_DESC, m_Desc);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispNotesTab, CResizableDialog)
	//{{AFX_MSG_MAP(CDispNotesTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_EN_CHANGE(IDC_DESC, OnChangeDesc)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispNotesTab message handlers

BOOL CDispNotesTab::OnInitDialog() 
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

void CDispNotesTab::InitNotesTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("Multiplexed Output: "));
		m_Enable.SetWindowText(_T(""));

		OnClear();
	} else {
		// Check IO Status
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispActive[theApp.m_CurPortIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispDisabled[theApp.m_CurPortIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Desc.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);
		SetTreeItemDesc();
		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Notes);
	}
}

void CDispNotesTab::SetTreeItemDesc()
{
	CString str, sPort, sID;
	CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

	// KLUDGE for PLV2
	str.Format("%s : %s",
				theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[theApp.m_CurPortIndex]),
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[theApp.m_CurPortIndex], str);

	sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
	sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem));
	if (sPort.Find(sID) != -1) {
		str.Format(_T("%s %s : 7-Segment Display : %s"), 
					sID,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);
	} else {
		str.Format(	_T("%s %s : Port %s - 7-Segment Display : %s"), 
					sID,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
					Alpha[theApp.m_CurPortIndex],
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);
	}

	m_IOName.SetWindowText(str);
}

void CDispNotesTab::OnChangeDesc()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Desc.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);
	SetTreeItemDesc();
}

void CDispNotesTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Notes);
}

void CDispNotesTab::OnClear() 
{
	m_Desc.SetWindowText("");
	m_Notes.SetWindowText("");
}

void CDispNotesTab::OnClearButton() 
{
	OnClear();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].Clear();

	CString str;
	str.Format("%s : ",	theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[theApp.m_CurPortIndex]));
	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[theApp.m_CurPortIndex], str);
}

BOOL CDispNotesTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
