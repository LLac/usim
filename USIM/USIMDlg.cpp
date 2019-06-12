// USIMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "USIMDlg.h"
#include "ReadOnlyEdit\ReadOnlyEdit.h"
#include "UpdateCheck\UpdateCheck.h"
#include "F4SharedMem.h"
#include "PathSplit\CPathSplit.h"
#include "Version\Version.h"
#include "w32process\w32process.h"

#include <Windows.h>
#include <Dbt.h>
#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CNewDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CReadOnlyEdit		m_EditCtrl;
	CStatic				m_Version;

	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CNewDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EDIT2, m_EditCtrl);
	DDX_Control(pDX, IDC_VERSION, m_Version);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CNewDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	str.Format ("Disclaimer:\r\n\r\nTHIS SOFTWARE AND THE ACCOMPANYING FILES "
		"ARE PROVIDED 'AS IS' AND WITHOUT WARRANTIES AS TO PERFORMANCE "
		"WHETHER EXPRESSED OR IMPLIED.\r\n\r\nBecause of the various hardware "
		"and software environments into which %s may be put, "
		"there is NO WARRANTY OF OPERATION OR GUARANTEE THAT %s "
		"will work in all environments.\r\n\r\n"
		"The user assumes the entire risk of using this software.", 
		AfxGetAppName(), AfxGetAppName());

	m_EditCtrl.SetWindowText(str);
	m_EditCtrl.SetBackColor(RGB(255,255,255));

	// Get some version information
	CString strVersion;
	TCHAR szFullPath[_MAX_PATH];
	HINSTANCE hInstance = AfxGetInstanceHandle();
	GetModuleFileName(hInstance, szFullPath, sizeof(szFullPath));
	GetVersionInfo(szFullPath, _T("FileVersion"), &strVersion);

	str.Format (IDS_VERSION, strVersion);
	m_Version.SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CUSIMDlg dialog

CUSIMDlg::CUSIMDlg(CWnd* pParent /*=NULL*/)
	: CTrayDialog(CUSIMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUSIMDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USIM_OFF);
	TrayShowWindow(theApp.GetProfileInt("Options", "Visible", false));
}

void CUSIMDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSIMDlg)
	DDX_Control(pDX, IDC_TAB1, m_DefTabCtrl);
	DDX_Control(pDX, IDC_TAB2, m_OutputTabCtrl);
	DDX_Control(pDX, IDC_TAB3, m_MuxTabCtrl);
	DDX_Control(pDX, IDC_TAB4, m_DispTabCtrl);
	DDX_Control(pDX, IDC_TAB5, m_RWRTabCtrl);
	DDX_Control(pDX, IDC_TAB6, m_GLCDTabCtrl);
	DDX_Control(pDX, IDC_TAB7, m_STEPTabCtrl);
	DDX_Control(pDX, IDC_TAB8, m_InputsTabCtrl);
	DDX_Control(pDX, IDC_TAB9, m_LatchedTabCtrl);
	DDX_Control(pDX, IDC_TAB10, m_SPITabCtrl);
	DDX_Control(pDX, IDC_TAB11, m_DotMXTabCtrl);
	DDX_Control(pDX, IDC_TAB12, m_CLCDTabCtrl);
	DDX_Control(pDX, IDC_DEVICESTREE, m_TreeDevices);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSIMDlg, CTrayDialog)
	//{{AFX_MSG_MAP(CUSIMDlg)
	ON_COMMAND(IDD_ABOUTBOX, OnAboutbox)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDS_UPDATE, OnUpdate)
	ON_UPDATE_COMMAND_UI(IDS_UPDATE, OnUpdateUpdate)
	ON_COMMAND(ID_FILE_LOAD, OnLoad)
	ON_COMMAND(ID_FILE_SAVE, OnSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD, OnUpdateFileLoad)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_F4SHOWDATA, OnF4Showdata)
	ON_UPDATE_COMMAND_UI(ID_F4SHOWDATA, OnF4UpdateShowdata)
	ON_COMMAND(ID_SHOWDATA, OnShowdata)
	ON_UPDATE_COMMAND_UI(ID_SHOWDATA, OnUpdateShowdata)
	ON_COMMAND(ID_AUTOCONNECT, OnAutoconnect)
	ON_UPDATE_COMMAND_UI(ID_AUTOCONNECT, OnUpdateAutoconnect)
	ON_COMMAND(ID_DISABLE, OnDisable)
	ON_UPDATE_COMMAND_UI(ID_DISABLE, OnUpdateDisable)
	ON_COMMAND(ID_TRAY_RESTORE, OnRestore)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_WM_SIZE()
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SPLITTER_MOVED, OnSplitterMoved)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSIMDlg message handlers

BOOL CUSIMDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_DefaultNewMenu.LoadToolBar(IDR_MAINFRAME);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// set up status bar
	m_bar.Create(this); //We create the status bar
	m_bar.SetIndicators(indicators, 2); //Set the number of panes 

	CRect rect;
	GetClientRect(&rect);
	//Size the two panes
	
	m_bar.SetPaneInfo(0, ID_INDICATOR_1, SBPS_NOBORDERS, 75);
	m_bar.SetPaneInfo(1, ID_INDICATOR_2, SBPS_STRETCH, 0);

	//This is where we actually draw it on the screen
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_2);

	CString str;
	str.LoadString(ID_INDICATOR_1);
	m_bar.SetPaneText(0, str);

	theApp.m_FilePath = "NONE";
	m_bar.SetPaneText(1, theApp.m_FilePath);

	// Setup the tab control
	RECT lpRect;
	m_DefTabCtrl.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
//	lpRect.right = lpRect.left + 430;

//	m_DefTabCtrl.MoveWindow(&lpRect, TRUE);
	m_DefTabCtrl.InitTabs();
	theApp.m_pDefTabCtrl = &m_DefTabCtrl;
	m_DefTabCtrl.ShowWindow(SW_SHOW);
/*
	m_UserVarsTabCtrl.MoveWindow(&lpRect, TRUE);
	m_UserVarsTabCtrl.InitTabs();
	theApp.m_pUserVarsTabCtrl = &m_UserVarsTabCtrl;
*/
	m_OutputTabCtrl.MoveWindow(&lpRect, TRUE);
	m_OutputTabCtrl.InitTabs();
	theApp.m_pOutputTabCtrl = &m_OutputTabCtrl;

	m_MuxTabCtrl.MoveWindow(&lpRect, TRUE);
	m_MuxTabCtrl.InitTabs();
	theApp.m_pMuxTabCtrl = &m_MuxTabCtrl;

	m_DispTabCtrl.MoveWindow(&lpRect, TRUE);
	m_DispTabCtrl.InitTabs();
	theApp.m_pDispTabCtrl = &m_DispTabCtrl;

	m_RWRTabCtrl.MoveWindow(&lpRect, TRUE);
	m_RWRTabCtrl.InitTabs();
	theApp.m_pRWRTabCtrl = &m_RWRTabCtrl;

	m_CLCDTabCtrl.MoveWindow(&lpRect, TRUE);
	m_CLCDTabCtrl.InitTabs();
	theApp.m_pCLCDTabCtrl = &m_CLCDTabCtrl;

	m_GLCDTabCtrl.MoveWindow(&lpRect, TRUE);
	m_GLCDTabCtrl.InitTabs();
	theApp.m_pGLCDTabCtrl = &m_GLCDTabCtrl;

	m_STEPTabCtrl.MoveWindow(&lpRect, TRUE);
	m_STEPTabCtrl.InitTabs();
	theApp.m_pSTEPTabCtrl = &m_STEPTabCtrl;

	m_InputsTabCtrl.MoveWindow(&lpRect, TRUE);
	m_InputsTabCtrl.InitTabs();
	theApp.m_pInputsTabCtrl = &m_InputsTabCtrl;

	m_LatchedTabCtrl.MoveWindow(&lpRect, TRUE);
	m_LatchedTabCtrl.InitTabs();
	theApp.m_pLatchedTabCtrl = &m_LatchedTabCtrl;

	m_SPITabCtrl.MoveWindow(&lpRect, TRUE);
	m_SPITabCtrl.InitTabs();
	theApp.m_pSPITabCtrl = &m_SPITabCtrl;

	m_DotMXTabCtrl.MoveWindow(&lpRect, TRUE);
	m_DotMXTabCtrl.InitTabs();
	theApp.m_pDotMXTabCtrl = &m_DotMXTabCtrl;

	// Devices Tree View Initialization
	theApp.m_AutoConnect = theApp.GetProfileInt("Options", "AutoConnect", true);
	theApp.m_HideItems = theApp.GetProfileInt("Options", "HideItems", false);
	theApp.m_HidePorts = theApp.GetProfileInt("Options", "HidePorts", false);
	theApp.m_HideModes = theApp.GetProfileInt("Options", "HideModes", false);

	m_DefTabCtrl.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	int rightpane = lpRect.right - lpRect.left + 200;

	m_TreeDevices.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	int leftpane = lpRect.right - lpRect.left;

	m_xSplitter.BindWithControl(this, IDC_XBAR);	// bind the splitter object to the puppet control
    m_xSplitter.SetMinWidth(leftpane, rightpane);	// set the minimum width of the left/right pane 

	m_xSplitter.AttachAsLeftPane(IDC_DEVICESTREE);	// attach a control within the same dialogbox as a pane of the splitter
	m_xSplitter.AttachAsRightPane(IDC_TAB1);		// you can even attach another splitter as a pane
    m_xSplitter.AttachAsRightPane(IDC_TAB2);
	m_xSplitter.AttachAsRightPane(IDC_TAB3);
	m_xSplitter.AttachAsRightPane(IDC_TAB4);
	m_xSplitter.AttachAsRightPane(IDC_TAB5);
    m_xSplitter.AttachAsRightPane(IDC_TAB6);
	m_xSplitter.AttachAsRightPane(IDC_TAB7);
	m_xSplitter.AttachAsRightPane(IDC_TAB8);
	m_xSplitter.AttachAsRightPane(IDC_TAB9);
	m_xSplitter.AttachAsRightPane(IDC_TAB10);
	m_xSplitter.AttachAsRightPane(IDC_TAB11);
	m_xSplitter.AttachAsRightPane(IDC_TAB12);

	// preset layout
	OnSplitterMoved(0,0);

	SetupMinimizeToTray();

	theApp.m_pTreeDevices = &m_TreeDevices;
	TreeView_SetUnicodeFormat(m_TreeDevices.GetSafeHwnd(), FALSE);

	m_TreeDevices.OnRefresh();
	theApp.StartSimConnectThread();
	StopTreeThread();

	SetPriorityClass(	GetCurrentProcess(),			// handle to the thread
						ABOVE_NORMAL_PRIORITY_CLASS);	// thread priority level

	LoadWindowRect("MainDlg Settings", true);
	m_xSplitter.RestoreSplitter("MainDlg Settings");

	HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	int i;
	for(i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) {
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if( !hDevNotify ) {
//			AfxMessageBox(CString("Can't register device notification: ")
//				+ _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);

			// write to log file
			CString str;
			str.Format("%susim.log", theApp.m_AppPath);
			FILE* stream = fopen(str, "a");
			fprintf(stream, CString("Can't register device notification: ") + _com_error(GetLastError()).ErrorMessage());
			fclose(stream);
			TRACE("Can't register device notification");
			return FALSE;
		}
	}

	// Auto Launch sim
	i = theApp.GetProfileInt("Sim Paths", "Auto Launch", 0);
	if (i > 0) {
		CString FilePath = theApp.GetProfileString("Sim Paths", SimTypes[i-1].SimName, "");
		CPathSplit l_oPathSplit(FilePath);
		CString FileName = l_oPathSplit.GetFileName();
		FilePath = l_oPathSplit.GetDrive() + l_oPathSplit.GetDirectory();		
		if (!FileName.IsEmpty()) {
			CProcessList m_ProcessList;
			if ( m_ProcessList.Find((LPTSTR)(LPCTSTR)FileName) == NULL) {
				FileName = l_oPathSplit.GetFileName() + l_oPathSplit.GetExtension();
				HINSTANCE hInst = ShellExecute(NULL, "open", FileName, NULL, FilePath, SW_SHOWNORMAL);
				if ((int)hInst == ERROR_FILE_NOT_FOUND) {
					TRACE("Error Opening file. %s File Not Found!", FileName);
				}
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

long CUSIMDlg::OnSplitterMoved(UINT xDiff, long nID)
{
	xDiff;
	nID;
	RECT lpRect;

	RemoveAllAnchors();
	
	m_DefTabCtrl.GetWindowRect(&lpRect);
	ScreenToClient(&lpRect);
	int minx = lpRect.left + RIGHTPANE_MINX;
	if (minx < DIALOG_MINX) minx = DIALOG_MINX;

	CSize sz;
	sz.cx = minx;
	sz.cy = DIALOG_MINY;
	SetMinTrackSize(sz);

	AddAnchor(IDC_DEVICESTREE, TOP_LEFT, BOTTOM_LEFT);

	AddAnchor(IDC_XBAR, TOP_LEFT, BOTTOM_LEFT);

	AddAnchor(IDC_TAB1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB2, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB3, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB4, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB5, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB6, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB7, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB8, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB9, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB10, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB11, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB12, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(AFX_IDW_STATUS_BAR, BOTTOM_LEFT, BOTTOM_RIGHT);

	return 0;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUSIMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CTrayDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUSIMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUSIMDlg::OnAboutbox() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

BOOL CUSIMDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if ( nID == 0x01 )
		return FALSE;
	if ( nID == 0x02 ) {	// ESC command
//		return FALSE;
	}

	return CTrayDialog::OnCommand(wParam, lParam);
}

void CUSIMDlg::OnUpdate() 
{
	CUpdateCheck checkUpdate;
//	checkUpdate.Check(IDS_UPDATE);	
}

void CUSIMDlg::OnUpdateUpdate(CCmdUI* pCmdUI) 
{
	// ********* UPDATE Feature Disabled *********** //
//	pCmdUI->Enable(true);
}

void CUSIMDlg::OnLoad() 
{
	char strFilter[] = { "USIM Configuration Files (*.imc)|*.imc|All Files (*.*)|*.*||" };
	
	CFileDialog FileDlg(TRUE, ".imc", NULL, OFN_HIDEREADONLY | OFN_READONLY | OFN_NOCHANGEDIR, strFilter);

	if( FileDlg.DoModal() == IDOK )	{
		theApp.m_FileName = FileDlg.GetFileName();
		theApp.m_FilePath = FileDlg.GetPathName();
	} else {
		return;
	}

	theApp.StopSimConnectThread();
	theApp.StopAllThreads();

	OnLoadFile();
	theApp.m_pTreeDevices->OnRefresh();
	theApp.StartSimConnectThread();
}

void CUSIMDlg::OnLoadFile() 
{
	int i;
	CString str, str1;

	if (theApp.m_FilePath.IsEmpty()) return;

	str.Format("%susim.log", theApp.m_AppPath);
	FILE* stream = fopen(str, "w");
	if (stream != NULL)
		fprintf(stream, "Log File Started.\n");

	CFile *file = new CFile();
	CFileException exception;

	// attempt opening a file
	str.Format("%s", theApp.m_FilePath);
	if ( !file->Open( str, CFile::modeRead, &exception ) ) {
		// an error occured opening file
//		MessageBox (_T("Cannot open ") + theApp.m_FileName, _T("File Load Error"), MB_ICONEXCLAMATION);
		fprintf(stream, "\nError opening file : Exception %d\n", exception.m_cause);
		TRACE("\nError opening file : Exception %d\n", exception.m_cause);
		delete file;
		if (stream != NULL) fclose(stream);

		return;
	}

	// load all file data into array
	theApp.m_DevArrayList.RemoveAll();

	for (i=0; i < MAX_DEVICES; i++) {
		if ( theApp.m_pDevArray[i] != NULL) 
			delete theApp.m_pDevArray[i];
		theApp.m_pDevArray[i] = NULL;
	}

	// now create a CArchive object
	CArchive archive( file, CArchive::load );

	try {
		theApp.m_UserVarsArray.Serialize( archive );
		// everything went good, so close
		// and return success
		archive.Close();
	} catch ( CArchiveException *pArchEx) {
		if (pArchEx->m_cause == CArchiveException::badClass) {
			archive.Close();
			file->SeekToBegin();
			pArchEx->Delete();
		} else {
			archive.Close();
		}

		file->Close();
		delete file;
		if (stream != NULL) fclose(stream);
		return;
	}

	theApp.m_WarningFlag = FALSE;
	for (i=0; i < MAX_DEVICES; i++) {
		theApp.m_pDevArray[i] = new CSaveDevice();

		if ( theApp.m_pDevArray[i] == NULL )
			break;

		// now create a CArchive object
		CArchive archive( file, CArchive::load );

		try {
			theApp.m_pDevArray[i]->Serialize( archive );
		} catch ( CException *err ) {
			err->Delete();
			
			// cleanup
			archive.Close();
			delete theApp.m_pDevArray[i];
			theApp.m_pDevArray[i] = NULL;

			break;
		}

		// everything went good, so close
		// and return success
		archive.Close();

		theApp.m_DevArrayList.AddTail(i);
	}

	file->Close();
	delete file;
	if (stream != NULL) fclose(stream);

//	theApp.m_bartext = theApp.m_FilePath;
	TRACE(theApp.m_FilePath);
}

void CUSIMDlg::OnSave() 
{
	// attempt opening a file
	if (theApp.m_FileName.GetLength() == 0) {
		char strFilter[] = { "USIM Configuration Files (*.imc)|*.imc|All Files (*.*)|*.*||" };

		CFileDialog FileDlg(false, ".imc", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, strFilter);

		if( FileDlg.DoModal() == IDOK )	{
			theApp.m_FileName = FileDlg.GetFileName();
			theApp.m_FilePath = FileDlg.GetPathName();
		} else {
			return;
		}
	}

	OnFileSave();
}

void CUSIMDlg::OnSaveAs() 
{
	CString str;
	char strFilter[] = { "USIM Configuration Files (*.imc)|*.imc|All Files (*.*)|*.*||" };

	if (theApp.m_FileName.GetLength() == 0) {
		theApp.m_FileName = "usim.imc";
	}

	CFileDialog FileDlg(FALSE, ".imc", theApp.m_FileName, OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, strFilter);

	if( FileDlg.DoModal() == IDOK )	{
		theApp.m_FileName = FileDlg.GetFileName();
		theApp.m_FilePath = FileDlg.GetPathName();
	} else {
		return;
	}

	OnFileSave();
}

void CUSIMDlg::OnFileSave() 
{
	CString str;
	str.Format ("%susim.log", theApp.m_AppPath);
	FILE* stream = fopen(str, "a");

	// Save all changes to file
	CFile *file = new CFile();
	CFileException exception;

	// attempt opening a file
	str.Format ("%s", theApp.m_FilePath);
	
	if ( !file->Open( str, CFile::modeCreate | CFile::modeWrite, &exception ) ) {
		// an error occured opening file
		str.Format("\nERROR : %d - IMC save failed!\n", exception.m_cause);
		TRACE(str);
		fprintf(stream, str);
		fclose(stream);
		return;
	}

	// now create a CArchive object
	CArchive archive( file, CArchive::store );

	try {
		theApp.m_UserVarsArray.Serialize( archive );
		// everything went fine, so close
		// and return success
		archive.Close();
	} catch ( CException *err ) {
		// cleanup
		err->Delete();
		file->Close();
		delete file;
		fclose(stream);
		str.Format("\nERROR : %d - IMC save failed!\n", exception.m_cause);
		TRACE(str);
		m_bar.SetPaneText(1, str);
		return;
	}

	for (int i=0; i < MAX_DEVICES; i++) {
		if (theApp.m_pDevArray[i] != NULL) {
			if (!theApp.m_pDevArray[i]->IsSet()) 
				continue;

			// now create a CArchive object
			CArchive archive( file, CArchive::store );

			try {
				theApp.m_pDevArray[i]->Serialize( archive );
			} catch ( CException *err ) {
				err->Delete();
				// cleanup
				archive.Close();
				break;
			}

			// everything went fine, so close
			// and return success
			archive.Close();
		}
	}

	file->Close();
	delete file;
	if (stream != NULL) fclose(stream);

	m_bar.SetPaneText(1, str);
}

void CUSIMDlg::OnAppExit() 
{
	DestroyWindow();
}

void CUSIMDlg::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI;
	if (theApp.m_DevCnt == 0) {
		pCmdUI->Enable(false);
	}
}

void CUSIMDlg::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI;
	if (theApp.m_DevCnt == 0) {
		pCmdUI->Enable(false);
	}
}

void CUSIMDlg::OnUpdateFileLoad(CCmdUI* pCmdUI) 
{
	pCmdUI;
//	if (theApp.m_DevCnt == 0) {
//		pCmdUI->Enable(false);
//	}	
}

void CUSIMDlg::OnF4Showdata() 
{
	// TODO: Add your command handler code here
	if (m_F4ShowData) {
		m_F4ShowData.ShowWindow(true);
	} else {
		m_F4ShowData.Create(IDD_DATAOUT);
		m_F4ShowData.ShowWindow(true);
	}
}

void CUSIMDlg::OnF4UpdateShowdata(CCmdUI* pCmdUI) 
{
	if (theApp.m_SimulationModel == SIM_NONE) {
		pCmdUI->Enable(false);
	}
}

void CUSIMDlg::OnShowdata() 
{
	// TODO: Add your command handler code here
	m_ShowData.m_ListTypeVar = VARTYPE_REALTIME;
	if (m_ShowData) {
		m_ShowData.ShowWindow(true);
	} else {
		m_ShowData.Create(IDD_VARSLIST);
		m_ShowData.ShowWindow(true);
	}
}

void CUSIMDlg::OnUpdateShowdata(CCmdUI* pCmdUI) 
{
	if (theApp.m_SimulationModel == SIM_NONE) {
		pCmdUI->Enable(false);
	}
}

void CUSIMDlg::OnRestore() 
{
	CPoint pt;
	pt.x = pt.y = 0;
	OnTrayLButtonDblClk(pt);
}

void CUSIMDlg::OnAutoconnect() 
{
	theApp.m_AutoConnect = !theApp.m_AutoConnect;
	theApp.WriteProfileInt("Options", "AutoConnect", theApp.m_AutoConnect);
}

void CUSIMDlg::OnUpdateAutoconnect(CCmdUI* pCmdUI) 
{
	if (theApp.m_AutoConnect) {
		pCmdUI->SetCheck(true);
	} else {
		pCmdUI->SetCheck(false);
	}
}

void CUSIMDlg::OnDisable() 
{
	theApp.m_Disable = !theApp.m_Disable;
	theApp.WriteProfileInt("Options", "Disable", theApp.m_Disable);
}

void CUSIMDlg::OnUpdateDisable(CCmdUI* pCmdUI) 
{
	if (theApp.m_Disable) {
		pCmdUI->SetCheck(true);
	} else {
		pCmdUI->SetCheck(false);
	}
}

LRESULT CUSIMDlg::OnDeviceChange(WPARAM wParam, LPARAM lParam)  
{
	if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam ) {
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		PDEV_BROADCAST_DEVICEINTERFACE pDevInf;
		//PDEV_BROADCAST_HANDLE pDevHnd;
		//PDEV_BROADCAST_OEM pDevOem;
		//PDEV_BROADCAST_PORT pDevPort;
		//PDEV_BROADCAST_VOLUME pDevVolume;
		if( pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
			pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
			CString str;
			str = pDevInf->dbcc_name;
			str.MakeUpper();
			if (str.Find(_T("VID_12DA")) != -1 || str.Find(_T("VID_6666")) != -1) {
				if (str.Find(_T("PID_1900")) == -1) // Skip USIM USB Dongle
					theApp.m_pTreeDevices->OnRefreshList();
			}
		}
	}
	return 0;
}

void CUSIMDlg::OnHelpFinder() 
{
	m_DefTabCtrl.m_DefaultTab.OnHelpButton();
}

void CUSIMDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if (bShow) {
		StartTreeThread();
	} else {
		StopTreeThread();
	}

	CTrayDialog::OnShowWindow(bShow, nStatus);
}

void CUSIMDlg::SetupMinimizeToTray()
{
	TraySetIcon(IDI_USIM_OFF);
    TraySetToolTip("Universal Simulation Interface Manager");
    
    BOOL bSuccess = m_menu.LoadMenu(IDR_POPMENU);
    if (!(bSuccess)) TRACE("Unabled to load menu", "Error");
	else TraySetMenu(m_menu, 6);
	m_menu.LoadToolBar(IDR_MAINFRAME);

	if (!theApp.GetProfileInt("Options", "Visible", false)) 
		TrayShow();
}

void CUSIMDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (nType != SIZE_MINIMIZED) {
		CTrayDialog::OnSize(nType, cx, cy);
	}
}

void CUSIMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CTrayDialog::OnSysCommand(nID, lParam);
	}
}

void CUSIMDlg::OnOK() 
{
}

void CUSIMDlg::OnCancel() 
{
	this->ShowWindow(SW_HIDE);
	TrayShow();
}

void CUSIMDlg::OnDestroy()
{
	int i;

	// clear all outputs
	for (i=0; i < MAX_DEVICES; i++) {
		if (theApp.m_pDevArray[i] != NULL)
			theApp.m_pDevArray[i]->ClearDeviceData();
	}

	Sleep(500);	// delay for threads to clear outputs.

	theApp.StopSimConnectThread();
	theApp.StopAllThreads();

	// clear all outputs
	for (i=0; i < MAX_DEVICES; i++) {
		if (theApp.m_pDevArray[i] != NULL)
			theApp.m_pDevArray[i]->ClearDeviceData();
	}

	// Close all HID devices
	if (!CloseDevices(theApp.m_DevCnt, theApp.m_DeviceList))
		MessageBox ("An error occured while attempting to close device!", _T("Close Device Failed"), MB_ICONEXCLAMATION);

	for (i=0; i < MAX_DEVICES; i++) {
		if (theApp.m_pDevArray[i] != NULL)
			delete theApp.m_pDevArray[i];
		theApp.m_pDevArray[i] = NULL;
	}

	SaveWindowRect("MainDlg Settings", true);
	m_xSplitter.SaveSplitter("MainDlg Settings");

	RemoveAllAnchors();

	WinHelp(0L, HELP_QUIT);
	CTrayDialog::OnDestroy();
}

void CUSIMDlg::OnTimer(UINT nIDEvent) 
{
	nIDEvent;

	try {
		m_TreeThread.TreeWorker();
		m_bar.SetPaneText(1, theApp.m_FilePath);
	} catch (CThreadException* pe) {
		// write to log file
		CString str;
		TRACE("TreeDevices - OnTimer error\n");
		str.Format("%susim.log", theApp.m_AppPath);
		FILE* stream = fopen(str, "a");
		fprintf(stream, "TreeDevices - OnTimer error\n");
		fclose(stream);

		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}