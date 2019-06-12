// DotMXFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "DotMXFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDotMXFlagsTab dialog
CDotMXFlagsTab::CDotMXFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CDotMXFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDotMXFlagsTab)
	//}}AFX_DATA_INIT
}

void CDotMXFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDotMXFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDotMXFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CDotMXFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDotMXFlagsTab message handlers

void CDotMXFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CDotMXFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "Dot Matrix";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXFlagsArray[theApp.m_CurItemIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXActive[theApp.m_CurItemIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXDisabled[theApp.m_CurItemIndex];
}

void CDotMXFlagsTab::SetTreeItemDesc()
{
	CString str;

	str.Format(_T("%s %s : %s - Dot Matrix %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
		theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem))),
		theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	CFlagsTab::m_IOName.SetWindowText(str);
}