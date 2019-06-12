// CLCDFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "CLCDFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLCDFlagsTab dialog
CCLCDFlagsTab::CCLCDFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CCLCDFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLCDFlagsTab)
	//}}AFX_DATA_INIT
}

void CCLCDFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLCDFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLCDFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CCLCDFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLCDFlagsTab message handlers

void CCLCDFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CCLCDFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "Character LCD";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDFlagsArray[theApp.m_CurPortIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDActive[theApp.m_CurPortIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDDisabled[theApp.m_CurPortIndex];
}

void CCLCDFlagsTab::SetTreeItemDesc()
{
	CString str, sPort, sID;
	CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

	// KLUDGE for PLV2
	sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
	sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem));
	if (sPort.Find(sID) != -1) {
		str.Format(_T("%s %s : %s"), 
					sID,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
					theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	} else {
		str.Format(	_T("%s %s : Port %s - Character LCD : %s"), 
					sID,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
					Alpha[theApp.m_CurPortIndex],
					theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	}

	m_IOName.SetWindowText(str);
}