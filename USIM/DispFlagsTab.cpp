// DispFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "DispFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispFlagsTab dialog
CDispFlagsTab::CDispFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CDispFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDispFlagsTab)
	//}}AFX_DATA_INIT
}

void CDispFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CDispFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispFlagsTab message handlers

void CDispFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CDispFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "7-Segment Display";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayFlagsArray[theApp.m_CurPortIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispActive[theApp.m_CurPortIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispDisabled[theApp.m_CurPortIndex];
}

void CDispFlagsTab::SetTreeItemDesc()
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
