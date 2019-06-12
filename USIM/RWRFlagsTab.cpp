// RWRFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "RWRFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRWRFlagsTab dialog
CRWRFlagsTab::CRWRFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CRWRFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRWRFlagsTab)
	//}}AFX_DATA_INIT
}

void CRWRFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRWRFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRWRFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CRWRFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWRFlagsTab message handlers

void CRWRFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CRWRFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "RWR";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrFlagsArray[theApp.m_CurPortIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrActive[theApp.m_CurPortIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrDisabled[theApp.m_CurPortIndex];
}

void CRWRFlagsTab::SetTreeItemDesc()
{
	CString str;
	CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

	str.Format(_T("%s %s : Port %s - CRT RWR : %s"), 
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
				Alpha[theApp.m_CurPortIndex],
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrNotesArray[theApp.m_CurPortIndex].m_Desc);
	CFlagsTab::m_IOName.SetWindowText(str);
}