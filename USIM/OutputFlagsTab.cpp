// OutputFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "OutputFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputFlagsTab dialog
COutputFlagsTab::COutputFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(COutputFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutputFlagsTab)
	//}}AFX_DATA_INIT
}

void COutputFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutputFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(COutputFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputFlagsTab message handlers

void COutputFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void COutputFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "Output";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputFlagsArray[theApp.m_CurItemIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputActive[theApp.m_CurItemIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputDisabled[theApp.m_CurItemIndex];
}

void COutputFlagsTab::SetTreeItemDesc()
{
	CString str, sPort, sID;
	str.Format("%s : %s",
		theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputhItem[theApp.m_CurItemIndex]),
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputNotesArray[theApp.m_CurItemIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputhItem[theApp.m_CurItemIndex], str);

	// KLUDGE for PLV2
	sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
	sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem)));
	if (sPort.Find(sID) != -1) {
		str.Format(_T("%s %s : Output %s"), sID,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	} else {
		str.Format(_T("%s %s : %s - Output %s"), sID,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			sPort,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	}
	CFlagsTab::m_IOName.SetWindowText(str);
}