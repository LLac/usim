// DefaultTab.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "USIM.h"
#include "USIMDlg.h"
#include "DefaultTab.h"
#include "TreeDevices.h"
#include "UpdateCheck\UpdateCheck.h"
//#include "F4VarList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefaultTab dialog


CDefaultTab::CDefaultTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDefaultTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefaultTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDefaultTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefaultTab)
	DDX_Control(pDX, IDC_AUTOUPDATE, m_AutoUpdate);
	DDX_Control(pDX, IDC_EDIT1, m_Status);
	DDX_Control(pDX, IDC_REFRESH, m_Refresh);
	DDX_Control(pDX, IDC_WEB, m_Web);
	DDX_Control(pDX, IDC_HELP2, m_Help);
	DDX_Control(pDX, IDC_UPDATE, m_Update);
	DDX_Control(pDX, IDC_DISABLE, m_Disable);
	DDX_Control(pDX, IDC_AUTOCONNECT, m_AutoConnect);
	DDX_Control(pDX, IDC_AUTOSTART, m_AutoStart);
	DDX_Control(pDX, IDC_MINIMIZED, m_Minimized);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDefaultTab, CResizableDialog)
	//{{AFX_MSG_MAP(CDefaultTab)
	ON_BN_CLICKED(IDC_REFRESH, OnRefreshButton)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdateButton)
	ON_BN_CLICKED(IDC_WEB, OnWebButton)
	ON_BN_CLICKED(IDC_DISABLE, OnDisable)
	ON_BN_CLICKED(IDC_AUTOCONNECT, OnAutoConnect)
	ON_BN_CLICKED(IDC_MINIMIZED, OnMinimized)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AUTOUPDATE, OnAutoupdate)
	ON_BN_CLICKED(IDC_HELP2, OnHelpButton)
	ON_BN_CLICKED(IDC_AUTOSTART, OnAutostart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefaultTab message handlers

void CDefaultTab::SimUpdate () 
{
	static UCHAR mSimulationModel = 0xff;

	if (mSimulationModel == theApp.m_SimulationModel) return;

	switch(theApp.m_SimulationModel) {
		case SIM_NONE:
			m_Status.SetWindowText(_T("Disabled"));
			m_Status.SetBackColor(RGB(240,240,240));
			break;
		case SIM_TEST:
			m_Status.SetWindowText(_T("TEST MODE"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_KBS:
			m_Status.SetWindowText(_T("Keyboard Emulation"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_XPLANE:
			m_Status.SetWindowText(_T("X-Plane Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_FS9:
			m_Status.SetWindowText(_T("FS9 Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_FSX:
			m_Status.SetWindowText(_T("FSX Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_IL2:
			m_Status.SetWindowText(_T("IL-2 Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_IL2FB:
			m_Status.SetWindowText(_T("IL-2 FB Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_IL2PF:
			m_Status.SetWindowText(_T("Pacific Fighters Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_F4BMS:
			m_Status.SetWindowText(_T("F4:BMS Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_F4AF:
			m_Status.SetWindowText(_T("F4:AF Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_F4FF:
			m_Status.SetWindowText(_T("F4:FF Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_F4OF:
			m_Status.SetWindowText(_T("F4:OF Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_GTR:
			m_Status.SetWindowText(_T("GTR Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_GTR2:
			m_Status.SetWindowText(_T("GTR2 Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_GTL:
			m_Status.SetWindowText(_T("GTL Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_EVO:
			m_Status.SetWindowText(_T("GTR EVO Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_RACE:
			m_Status.SetWindowText(_T("RACE Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_RACE07:
			m_Status.SetWindowText(_T("RACE 07 Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_RACEON:
			m_Status.SetWindowText(_T("RACE ON Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_VOLVO:
			m_Status.SetWindowText(_T("VOLVO Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_RF:
			m_Status.SetWindowText(_T("rFactor Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_GPX:
			m_Status.SetWindowText(_T("GPx Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
		case SIM_LFS:
			m_Status.SetWindowText(_T("LFS Connected"));
			m_Status.SetBackColor(RGB(100,240,100));
			break;
	}

	mSimulationModel = theApp.m_SimulationModel;
	TRACE("USIM Updated\n");
}

void CDefaultTab::OnRefreshButton () {
	theApp.m_pTreeDevices->OnRefreshList();
}

void CDefaultTab::OnWebButton () {
	CUpdateCheck checkUpdate;
	checkUpdate.GotoURL("www.betainnovations.ca", SW_SHOW);
}

void CDefaultTab::OnHelpButton () {
	CString strPath(MAKEINTRESOURCE(IDS_HELPFILE));
	HINSTANCE retval = ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOWNORMAL );

	if ((UINT)retval <= 32)
		MessageBox("The help file could not be located!", "USIM: Help File", MB_ICONWARNING);
}

void CDefaultTab::OnUpdateButton () {
	CUpdateCheck checkUpdate;
	checkUpdate.Check(IDS_UPDATE);
}

BOOL CDefaultTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	AddAnchor(IDC_STATIC1, TOP_CENTER);
	
	AddAnchor(IDC_STATIC2, TOP_CENTER);
	AddAnchor(IDC_EDIT1, TOP_CENTER);

	AddAnchor(IDC_STATIC5, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_DISABLE, TOP_LEFT);
	AddAnchor(IDC_AUTOCONNECT, TOP_LEFT);
	AddAnchor(IDC_MINIMIZED, TOP_LEFT);
	AddAnchor(IDC_AUTOUPDATE, TOP_LEFT);

	AddAnchor(IDC_REFRESH, MIDDLE_CENTER);
	AddAnchor(IDC_STATIC6, MIDDLE_CENTER);

	AddAnchor(IDC_UPDATE, MIDDLE_CENTER);
	AddAnchor(IDC_STATIC7, MIDDLE_CENTER);

	AddAnchor(IDC_HELP2, MIDDLE_CENTER);
	AddAnchor(IDC_STATIC8, MIDDLE_CENTER);

	AddAnchor(IDC_WEB, MIDDLE_CENTER);
	AddAnchor(IDC_STATIC9, MIDDLE_CENTER);

	m_Refresh.SetIcon(theApp.LoadIcon(MAKEINTRESOURCE(IDI_USB)));
	m_Web.SetIcon(theApp.LoadIcon(MAKEINTRESOURCE(IDI_WEB)));
	m_Help.SetIcon(theApp.LoadIcon(MAKEINTRESOURCE(IDI_HELP)));
	m_Update.SetIcon(theApp.LoadIcon(MAKEINTRESOURCE(IDI_UPDATE)));
	
	m_Status.SetWindowText(_T("Not Connected."));
	m_Status.SetBackColor(RGB(240,240,240));

	m_Disable.SetCheck(theApp.GetProfileInt("Options", "Disable", false));
	theApp.m_Disable = m_Disable.GetCheck();

	m_AutoConnect.SetCheck(theApp.GetProfileInt("Options", "AutoConnect", true));
	theApp.m_AutoConnect = m_AutoConnect.GetCheck();
	UpdateWindow();

	m_AutoStart.SetCheck(theApp.GetProfileInt("Options", "AutoStart", true));
	OnAutostart();

	m_Minimized.SetCheck(!theApp.GetProfileInt("Options", "Visible", false));

	m_AutoUpdate.SetCheck(theApp.GetProfileInt("Options", "AutoUpdate", false));
	if (m_AutoUpdate.GetCheck()) {
		CUpdateCheck checkUpdate;
		checkUpdate.Check(IDS_UPDATE, false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDefaultTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}


void CDefaultTab::OnAutoupdate() 
{
	theApp.WriteProfileInt("Options", "AutoUpdate", m_AutoUpdate.GetCheck());
}

void CDefaultTab::OnDisable() 
{
	theApp.WriteProfileInt("Options", "Disable", m_Disable.GetCheck());
	theApp.m_Disable = m_Disable.GetCheck();
	UpdateWindow();
}

void CDefaultTab::OnAutoConnect() 
{
	theApp.WriteProfileInt("Options", "AutoConnect", m_AutoConnect.GetCheck());
	theApp.m_AutoConnect = m_AutoConnect.GetCheck();
	UpdateWindow();
}

void CDefaultTab::OnAutostart() 
{
	HKEY hKey;
	LONG lnRes;
	char buffer[MAX_PATH];

	theApp.WriteProfileInt("Options", "AutoStart", m_AutoStart.GetCheck());

	lnRes = RegOpenKeyEx(	HKEY_LOCAL_MACHINE,  // handle of open key
								// The following is the address of name of subkey to open
								"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
								0L,KEY_WRITE,
								&hKey            // address of handle of open key
	);
	if (m_AutoStart.GetCheck()) {
		// now add program path to the RUN key
		if(lnRes == ERROR_SUCCESS) {
			lstrcpy(buffer, theApp.m_AppFilePath);
			lnRes = RegSetValueEx(	hKey,
									LPCTSTR("USIM"),  // handle of the opened key to set value for
									0,
									REG_SZ,
									(const unsigned char *)buffer,
									lstrlen(buffer)+1);

			TRACE ("RegSetValueEx succeeded\n");
		}
	} else {
		lnRes = RegDeleteValue (hKey, LPCTSTR("USIM"));
		lnRes = RegDeleteKey (hKey, LPCTSTR("USIM"));
	}
}

void CDefaultTab::OnMinimized() 
{
	theApp.WriteProfileInt("Options", "Visible", !m_Minimized.GetCheck());
}

void CDefaultTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	UpdateWindow();
	// Do not call CResizableDialog::OnPaint() for painting messages
}

void CDefaultTab::UpdateWindow()
{
	m_Disable.SetCheck(theApp.GetProfileInt("Options", "Disable", false));
	m_AutoConnect.SetCheck(theApp.GetProfileInt("Options", "AutoConnect", true));
}

