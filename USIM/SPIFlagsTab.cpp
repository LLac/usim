// SPIFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "SPIFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPIFlagsTab dialog
CSPIFlagsTab::CSPIFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CSPIFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPIFlagsTab)
	//}}AFX_DATA_INIT
}

void CSPIFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPIFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPIFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CSPIFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPIFlagsTab message handlers

void CSPIFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CSPIFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "SPI";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIFlagsArray[theApp.m_CurItemIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIActive[theApp.m_CurItemIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIDisabled[theApp.m_CurItemIndex];
}

void CSPIFlagsTab::SetTreeItemDesc()
{
	CString str, sPort, sID;
	str.Format("%s : %s",
		theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIhItem[theApp.m_CurItemIndex]),
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPINotesArray[theApp.m_CurItemIndex].m_Desc);

	theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIhItem[theApp.m_CurItemIndex], str);

	// KLUDGE for PLV2
	sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
	sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem)));
	if (sPort.Find(sID) != -1) {
		str.Format(_T("%s %s : SPI %s"), sID,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	} else {
		str.Format(_T("%s %s : %s - SPI %s"), sID,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			sPort,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
	}
	m_IOName.SetWindowText(str);
}