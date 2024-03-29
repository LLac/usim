// TrayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrayDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrayDialog dialog


CTrayDialog::CTrayDialog(UINT uIDD,CWnd* pParent /*=NULL*/)
	: CResizableDialog(uIDD, pParent)
{
	m_nidIconData.cbSize			= sizeof(NOTIFYICONDATA);
	
	m_nidIconData.hWnd				= 0;
	m_nidIconData.uID				= 1;

	m_nidIconData.uCallbackMessage	= WM_TRAY_ICON_NOTIFY_MESSAGE;

	m_nidIconData.hIcon				= 0;
	m_nidIconData.szTip[0]			= 0;	
	m_nidIconData.uFlags			= NIF_MESSAGE;

	m_bTrayIconVisible				= FALSE;

	m_nDefaultMenuItem				= 0;

	m_bMinimizeToTray				= TRUE;

	m_Visible						= FALSE;

}


BEGIN_MESSAGE_MAP(CTrayDialog, CResizableDialog)
	//{{AFX_MSG_MAP(CTrayDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE, OnTrayNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayDialog message handlers

int CTrayDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_nidIconData.hWnd = this->m_hWnd;
	m_nidIconData.uID = 1;
	
	return 0;
}

void CTrayDialog::OnDestroy() 
{
	CResizableDialog::OnDestroy();
	if(m_nidIconData.hWnd && m_nidIconData.uID>0 && TrayIsVisible())
	{
		Shell_NotifyIcon(NIM_DELETE,&m_nidIconData);
	}
}

void  CTrayDialog::TrayShowWindow(BOOL bVisible)
{
	m_Visible = bVisible;
}

BOOL CTrayDialog::TrayIsVisible()
{
	return m_bTrayIconVisible;
}

void CTrayDialog::TraySetIcon(HICON hIcon)
{
	ASSERT(hIcon);

	m_nidIconData.hIcon = hIcon;
	m_nidIconData.uFlags |= NIF_ICON;
}

void CTrayDialog::TraySetIcon(UINT nResourceID)
{
	ASSERT(nResourceID>0);
	HICON hIcon = 0;
	hIcon = AfxGetApp()->LoadIcon(nResourceID);
	if(hIcon)
	{
		m_nidIconData.hIcon = hIcon;
		m_nidIconData.uFlags |= NIF_ICON;
	}
	else
	{
		TRACE0("FAILED TO LOAD ICON\n");
	}
}

void CTrayDialog::TraySetIcon(LPCTSTR lpszResourceName)
{
	HICON hIcon = 0;
	hIcon = AfxGetApp()->LoadIcon(lpszResourceName);
	if(hIcon)
	{
		m_nidIconData.hIcon = hIcon;
		m_nidIconData.uFlags |= NIF_ICON;
	}
	else
	{
		TRACE0("FAILED TO LOAD ICON\n");
	}
}

void CTrayDialog::TraySetToolTip(LPCTSTR lpszToolTip)
{
	ASSERT(strlen(lpszToolTip) > 0 && strlen(lpszToolTip) < 64);

	strcpy(m_nidIconData.szTip,lpszToolTip);
	m_nidIconData.uFlags |= NIF_TIP;
}

BOOL CTrayDialog::TrayShow()
{
	BOOL bSuccess = FALSE;
	if(!m_bTrayIconVisible)
	{
		bSuccess = Shell_NotifyIcon(NIM_ADD,&m_nidIconData);
		if(bSuccess)
			m_bTrayIconVisible= TRUE;
	}
	else
	{
		TRACE0("ICON ALREADY VISIBLE\n");
	}
	return bSuccess;
}

BOOL CTrayDialog::TrayHide()
{
	BOOL bSuccess = FALSE;
	if(m_bTrayIconVisible)
	{
		bSuccess = Shell_NotifyIcon(NIM_DELETE,&m_nidIconData);
		if(bSuccess)
			m_bTrayIconVisible= FALSE;
	}
	else
	{
		TRACE0("ICON ALREADY HIDDEN\n");
	}
	return bSuccess;
}

BOOL CTrayDialog::TrayUpdate()
{
	BOOL bSuccess = FALSE;
	if(m_bTrayIconVisible)
	{
		bSuccess = Shell_NotifyIcon(NIM_MODIFY,&m_nidIconData);
	}
	else
	{
//		TRACE0("ICON NOT VISIBLE\n");
	}
	return bSuccess;
}


BOOL CTrayDialog::TraySetMenu(UINT nResourceID,UINT nDefaultPos)
{
	m_nDefaultMenuItem = nDefaultPos;
	BOOL bSuccess;
	bSuccess = m_mnuTrayMenu.LoadMenu(nResourceID);
	return bSuccess;
}


BOOL CTrayDialog::TraySetMenu(LPCTSTR lpszMenuName,UINT nDefaultPos)
{
	m_nDefaultMenuItem = nDefaultPos;
	BOOL bSuccess;
	bSuccess = m_mnuTrayMenu.LoadMenu(lpszMenuName);
	return bSuccess;
}

BOOL CTrayDialog::TraySetMenu(HMENU hMenu,UINT nDefaultPos)
{
	m_nDefaultMenuItem = nDefaultPos;
	m_mnuTrayMenu.Attach(hMenu);
	return TRUE;
}

long CTrayDialog::OnTrayNotify(WPARAM wParam, LPARAM lParam) 
{ 
    UINT uID; 
    UINT uMsg; 
 
    uID = (UINT) wParam; 
    uMsg = (UINT) lParam; 
 
	if (uID != 1)
		return 0;
	
	CPoint pt;	

    switch (uMsg ) 
	{ 
	case WM_MOUSEMOVE:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayMouseMove(pt);
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayLButtonDown(pt);
		break;
	case WM_LBUTTONDBLCLK:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayLButtonDblClk(pt);
		break;
	
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
		GetCursorPos(&pt);
		//ClientToScreen(&pt);
		OnTrayRButtonDown(pt);
		break;
	case WM_RBUTTONDBLCLK:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayRButtonDblClk(pt);
		break;
    } 
    return 0; 
}

void CTrayDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(m_bMinimizeToTray) {
		if ((nID & 0xFFF0) == SC_MINIMIZE) {
			if(TrayShow())
				m_Visible = false;
				this->ShowWindow(SW_HIDE);		
		} else {
			CResizableDialog::OnSysCommand(nID, lParam);
		}
	} else {
		CResizableDialog::OnSysCommand(nID, lParam);
	}
}

void CTrayDialog::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if(!m_Visible) {
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}

    CResizableDialog::OnWindowPosChanging(lpwndpos);
}

void CTrayDialog::TraySetMinimizeToTray(BOOL bMinimizeToTray)
{
	m_bMinimizeToTray = bMinimizeToTray;
}

void CTrayDialog::OnTrayRButtonDown(CPoint pt)
{
	CMenu *pcSubMenu = m_mnuTrayMenu.GetSubMenu(0);
	if (pcSubMenu) {
		pcSubMenu->SetDefaultItem(m_nDefaultMenuItem, TRUE);

		SetForegroundWindow();
		pcSubMenu->TrackPopupMenu(TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this);
		PostMessage(WM_NULL, 0, 0);
	}
}

void CTrayDialog::OnTrayLButtonDown(CPoint pt)
{
	pt;
}

void CTrayDialog::OnTrayLButtonDblClk(CPoint pt)
{
	pt;
	if(m_bMinimizeToTray) {
		if(TrayHide()) {
			m_Visible = true;
			this->ShowWindow(SW_SHOW);
		}
	}
}

void CTrayDialog::OnTrayRButtonDblClk(CPoint pt)
{
	pt;
}

void CTrayDialog::OnTrayMouseMove(CPoint pt)
{
	pt;
}

