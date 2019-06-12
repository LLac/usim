// LatchedFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "LatchedFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLatchedFlagsTab dialog
CLatchedFlagsTab::CLatchedFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CLatchedFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLatchedFlagsTab)
	//}}AFX_DATA_INIT
}

void CLatchedFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLatchedFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLatchedFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CLatchedFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLatchedFlagsTab message handlers

void CLatchedFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CLatchedFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "Latched Output";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOFlagsArray[theApp.m_CurItemIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOActive[theApp.m_CurItemIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LODisabled[theApp.m_CurItemIndex];
}

void CLatchedFlagsTab::SetTreeItemDesc()
{
	CString str;

	str.Format(_T("%s %s : %s - Latched Output %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
		theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem))),
		theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	CFlagsTab::m_IOName.SetWindowText(str);
}