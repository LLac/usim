// InputsTabCtrl.cpp: implementation of the CInputsTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "InputsTabCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInputsTabCtrl::CInputsTabCtrl()
{
	m_Init = false;
}

CInputsTabCtrl::~CInputsTabCtrl()
{

}

BEGIN_MESSAGE_MAP(CInputsTabCtrl, CXTabCtrl)
	//{{AFX_MSG_MAP(CInputsTabCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInputsTabCtrl::InitTabs()
{
	//Setup the Image list for Misc Icons
	m_ImageList.DeleteImageList();
	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_NOTES = m_ImageList.Add(theApp.LoadIcon(IDI_NOTES));
	m_NOTES_P = m_ImageList.Add(theApp.LoadIcon(IDI_NOTES_P));
	m_IOFLAGS = m_ImageList.Add(theApp.LoadIcon(IDI_COND));
	m_IOFLAGS_P = m_ImageList.Add(theApp.LoadIcon(IDI_COND_P));
	m_SENDKEYS = m_ImageList.Add(theApp.LoadIcon(IDI_KBS2));
	m_SENDKEYS_P = m_ImageList.Add(theApp.LoadIcon(IDI_KBS2_P));
	m_VARS = m_ImageList.Add(theApp.LoadIcon(IDI_VARS));
	m_VARS_P = m_ImageList.Add(theApp.LoadIcon(IDI_VARS_P));
	
	// Setup the tab control
	SetImageList ( &m_ImageList );

	m_NotesTab.Create (IDD_NOTES, this);
	AddTab (&m_NotesTab, _T("Notes"), m_NOTES);
	m_NotesTab.m_pTab = this;

	m_InputFlagTab.Create (IDD_INPUT_FLAGS, this);
	AddTab (&m_InputFlagTab, _T("Input Flags"), m_IOFLAGS);
	m_InputFlagTab.m_pTab = this;

	m_SendKeysTab.Create (IDD_SEND_KEYS, this);
	AddTab (&m_SendKeysTab, _T("Keys"), m_SENDKEYS);
	m_SendKeysTab.m_pTab = this;

	m_VariablesTab.Create (IDD_VARIABLES, this);
	AddTab (&m_VariablesTab, _T("Variables"), m_VARS);
	m_VariablesTab.m_pTab = this;

	m_Init = true;
}

void CInputsTabCtrl::InitAllTabs()
{
	m_NotesTab.InitNotesTab();
	m_InputFlagTab.InitInputFlagTab();
	m_SendKeysTab.InitSendKeysTab();
	m_VariablesTab.InitVariablesTab();
}

void CInputsTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CXTabCtrl::OnSize(nType, cx, cy);

	if (!m_Init) return;

	RECT lpRect;
	m_NotesTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_NotesTab.MoveWindow(&lpRect, TRUE);

	m_InputFlagTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_InputFlagTab.MoveWindow(&lpRect, TRUE);

	m_SendKeysTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_SendKeysTab.MoveWindow(&lpRect, TRUE);

	m_VariablesTab.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	lpRect.bottom = cy - 10;
	lpRect.right = cx - 10;
	m_VariablesTab.MoveWindow(&lpRect, TRUE);
}
