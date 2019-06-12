// STEPFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "STEPFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTEPFlagsTab dialog
CSTEPFlagsTab::CSTEPFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CSTEPFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSTEPFlagsTab)
	//}}AFX_DATA_INIT
}

void CSTEPFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTEPFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTEPFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CSTEPFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEPFlagsTab message handlers

void CSTEPFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CSTEPFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "STEP";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPFlagsArray[theApp.m_CurItemIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPActive[theApp.m_CurItemIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPDisabled[theApp.m_CurItemIndex];
}

void CSTEPFlagsTab::SetTreeItemDesc()
{
	CString str, sPort, sID;
	str.Format("%s : %s",
		theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPhItem[theApp.m_CurItemIndex]),
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPNotesArray[theApp.m_CurItemIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPhItem[theApp.m_CurItemIndex], str);

	// KLUDGE for PLV2
	sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
	sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem)));
	if (sPort.Find(sID) != -1) {
		str.Format(_T("%s %s : STEP %s"), sID,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	} else {
		str.Format(_T("%s %s : %s - STEP %s"), sID,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			sPort,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	}
	m_IOName.SetWindowText(str);
}