// USIM.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#include <vld.h>
#include "USIM.h"
#include "USIMDlg.h"
#include "VarListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//COutputThread CUSIMApp::m_OutputThread;

/////////////////////////////////////////////////////////////////////////////
// CUSIMApp

BEGIN_MESSAGE_MAP(CUSIMApp, CWinApp)
	//{{AFX_MSG_MAP(CUSIMApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	//ON_COMMAND(ID_HELP_FINDER, CWinApp::OnHelpFinder)	// menu help command
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSIMApp construction

CUSIMApp::CUSIMApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUSIMApp object

CUSIMApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUSIMApp initialization

BOOL CUSIMApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	HANDLE hMutex;
	// Create mutex
	hMutex = CreateMutex(NULL, false, "Universal Simulation Interface Manager");

	// if it exist tell user
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
    	// display a message
		MessageBox(NULL,"Application already running!","Universal Simulation Interface Manager",MB_OK);
        return (FALSE);
	} // if mutex already exist then we there are already a instance running

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

#ifndef _AFXDLL
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Set drawing style of all menus to XP-Mode
	CNewMenu::SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	CNewMenu::SetXpBlending(FALSE);
	CNewMenu::SetNewMenuBorderAllMenu(TRUE);
	CNewMenu::SetSelectDisableMode(FALSE);

    TCHAR path_buffer[_MAX_PATH];
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];

	GetModuleFileName(AfxGetInstanceHandle(), path_buffer, sizeof(path_buffer));

	_splitpath( path_buffer, drive, dir, fname, ext );
	m_AppPath.Format("%s%s", drive, dir);
	m_AppFilePath = path_buffer;

	for (int i=0; i < MAX_DEVICES; i++)
		theApp.m_pDevArray[i] = NULL;

	m_SimulationModel = SIM_TEST;

	SetRegistryKey(_T("Beta Innovations"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	CUSIMDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CUSIMApp::StartSimConnectThread()
{
	if (m_SimConnectThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_SimConnectThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};
}

void CUSIMApp::StopSimConnectThread()
{
	DWORD dwExitCode;

	try	{
		m_SimConnectThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CUSIMApp::StartOutputThread()
{
	if (m_OutputThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_OutputThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};
}

void CUSIMApp::StopOutputThread()
{
	DWORD dwExitCode;

	try	{
		m_OutputThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CUSIMApp::StartInputThread()
{
	if (m_InputThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_InputThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};
}

void CUSIMApp::StopInputThread()
{
	DWORD dwExitCode;

	try	{
		m_InputThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CUSIMApp::StartSendKeysThread()
{
	if (m_SendKeysThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_SendKeysThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};
}

void CUSIMApp::StopSendKeysThread()
{
	DWORD dwExitCode;

	try	{
		m_SendKeysThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CUSIMApp::StartUDPThread()
{
	if (m_IL2_UDPThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_IL2_UDPThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};

	if (m_LFS_UDPThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_LFS_UDPThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};
}

void CUSIMApp::StopUDPThread()
{
	DWORD dwExitCode;

	try	{
		m_IL2_UDPThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}

	try	{
		m_LFS_UDPThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CUSIMApp::StartKeyCheckThread()
{
	if (m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
		return;

	try	{
		m_KeyCheckThread.Start();
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	};
}

void CUSIMApp::StopKeyCheckThread()
{
	DWORD dwExitCode;

	try	{
		m_KeyCheckThread.Stop(dwExitCode, 1);
	} catch (CThreadException* pe) {
		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CUSIMApp::StopAllThreads()
{
	// Stop all threads
	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;
	pUSIMDlg->StopTreeThread();
	StopOutputThread();
	StopInputThread();
	StopSendKeysThread();
	StopUDPThread();
//	StopKeyCheckThread();
	Sleep (500);	// wait until all threads finish
}

void CUSIMApp::StartAllThreads()
{
	// Start all threads
//	StartKeyCheckThread();
	StartOutputThread();
	StartInputThread();
	StartSendKeysThread();
	StartUDPThread();
	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;
	pUSIMDlg->StartTreeThread();
}

double VarTypeConversion(int vartype, double *varvalue)
{
	double retval = 0;

	if (varvalue == NULL) 
		return retval;

	switch(vartype) {
		case xplmType_Byte:
		case xplmType_ByteHex:
			retval = *(unsigned char*)varvalue;
		break;

		case xplmType_Char:
			retval = *(char*)varvalue;
		break;

		case xplmType_Int:
		case xplmType_IntHex:
			retval = *(int*)varvalue;
		break;

		case xplmType_Float:
			retval = *(float*)varvalue;
		break;

		case xplmType_Double:
			retval = *varvalue;
		break;

		case xplmType_Word:
			retval = *(unsigned short*)varvalue;
		break;

		case xplmType_DWord:
			retval = *(DWORD*)varvalue;
		break;
	}

	return retval;
}

UINT SearchIndex(CString TokenName, UINT SimType, UINT VarType)
{
	UINT i;

	switch (SimType) {
		case SIM_XPLANE:
			if (VarType == xplmType_Cmd) {
				for (i = 0; i < ArraySize(XPCmdArray); i++) {
					if (TokenName == XPCmdArray[i].VarTokenName) {
						return i;
					}
				}
			} else {
				for (i = 0; i < ArraySize(XPDataArray); i++) {
					if (TokenName == XPDataArray[i].VarTokenName) {
						return i;
					}
				}
			}
			break;

		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
			if (VarType == xplmType_Cmd) {
				for (i = 0; i < ArraySize(IL2CmdArray); i++) {
					if (TokenName == IL2CmdArray[i].VarTokenName) {
						return i;
					}
				}
			} else {
				for (i = 0; i < ArraySize(IL2DataArray); i++) {
					if (TokenName == IL2DataArray[i].VarTokenName) {
						return i;
					}
				}
			}
			break;

		case SIM_F4BMS:
		case SIM_F4AF:
		case SIM_F4FF:
		case SIM_F4OF:
		case SIM_F4USIM:
			for (i = 0; i < ArraySize(F4DataArray); i++) {
				if (TokenName == F4DataArray[i].VarTokenName)
					return i;
			}
			break;

		case SIM_GTR:
		case SIM_GTR2:
		case SIM_EVO:
		case SIM_GTL:
		case SIM_RACE:
		case SIM_RACE07:
		case SIM_SIMBIN:
		case SIM_RACEON:
		case SIM_VOLVO:
			for (i = 0; i < ArraySize(SimBinDataArray); i++) {
				if (TokenName == SimBinDataArray[i].VarTokenName)
					return i;
			}
			break;

		case SIM_RF:
			for (i = 0; i < ArraySize(RFDataArray); i++) {
				if (TokenName == RFDataArray[i].VarTokenName)
					return i;
			}
			break;

		case SIM_GPX:
			for (i = 0; i < ArraySize(GPXDataArray); i++) {
				if (TokenName == GPXDataArray[i].VarTokenName)
					return i;
			}
			break;

		case SIM_LFS:
			for (i = 0; i < ArraySize(LFSDataArray); i++) {
				if (TokenName == LFSDataArray[i].VarTokenName)
					return i;
			}
			break;

		case USIM_INPUT_FLAGS:
			for (i = 0; i < MAX_INPUT_FLAGS; i++) {
				if (TokenName == theApp.m_UserVarsArray.m_IOFlags[i].VarTokenString)
					return i;
			}
			break;
	}
	
	return 0;
}
