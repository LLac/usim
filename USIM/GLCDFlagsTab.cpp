// GLCDFlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "GLCDFlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLCDFlagsTab dialog
CGLCDFlagsTab::CGLCDFlagsTab(CWnd* pParent /*=NULL*/)
	: CFlagsTab(CGLCDFlagsTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGLCDFlagsTab)
	//}}AFX_DATA_INIT
}

void CGLCDFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CFlagsTab::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGLCDFlagsTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGLCDFlagsTab, CFlagsTab)
	//{{AFX_MSG_MAP(CGLCDFlagsTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLCDFlagsTab message handlers

void CGLCDFlagsTab::InitFlagsTab() 
{
	InitVars();
	SetTreeItemDesc();
	CFlagsTab::InitFlagsTab();
}

void CGLCDFlagsTab::InitVars()
{
	CFlagsTab::m_StrIO = "Graphic LCD";
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) return;

	CFlagsTab::m_pIOArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDFlagsArray[theApp.m_CurPortIndex];
	CFlagsTab::m_pIOActive = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDActive[theApp.m_CurPortIndex];
	CFlagsTab::m_pIODisabled = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDDisabled[theApp.m_CurPortIndex];
}

void CGLCDFlagsTab::SetTreeItemDesc()
{
	CString str;
	CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

	str.Format(_T("%s %s : Port %s - Graphic LCD : %s"),
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
					Alpha[theApp.m_CurPortIndex],
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDNotesArray[theApp.m_CurPortIndex].m_Desc);
	CFlagsTab::m_IOName.SetWindowText(str);
}