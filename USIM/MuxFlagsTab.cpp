// MUXFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "MUXFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMUXFlagsTab dialog
CMUXFlagsTab::CMUXFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CMUXFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMUXFlagsTab)
	//}}AFX_DATA_INIT
}

void CMUXFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMUXFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMUXFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CMUXFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMUXFlagsTab message handlers

void CMUXFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CMUXFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "Output";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxFlagsArray[theApp.m_CurItemIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxActive[theApp.m_CurItemIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxDisabled[theApp.m_CurItemIndex];
}

void CMUXFlagsTab::SetTreeItemDesc()
{
	CString str;

	str.Format(_T("%s %s : %s - Output %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
		theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem))),
		theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	CFlagsTab::m_IOName.SetWindowText(str);
}