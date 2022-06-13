// SimConnectThread.cpp: implementation of the CSimConnectThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "USIMDlg.h"
#include <winuser.h> // Required for the ::SendInput function
#include "SimConnectThread.h"
#include "w32process\w32process.h"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSimConnectThread, CThread)

CSimConnectThread::CSimConnectThread(void* pOwnerObject, LPARAM lParam) 
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(CSimConnectThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_deltaT_SKW = m_deltaT_IW = 0;
	m_Run = FALSE;
	m_OS64 = Is64BitWindows();
	m_Running32 = false;
	m_SimIdx = 0;
}

CSimConnectThread::~CSimConnectThread()
{
	m_deltaT_SKW = m_deltaT_IW = 0;
}

BOOL CSimConnectThread::Is64BitWindows()
{
#if defined(_WIN64)
    return TRUE;  // 64-bit programs run only on Win64
#elif defined(_WIN32)
    // 32-bit programs run on both 32-bit and 64-bit Windows
    // so must sniff
    BOOL f64 = FALSE;
    return IsWow64Process(GetCurrentProcess(), &f64) && f64;
#else
    return FALSE; // Win64 does not support Win16
#endif
}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD CSimConnectThread::ThreadHandler()
{
	BOOL bCanContinue = true;
	int nIncomingCommand;
	static UCHAR Dispatcher = 0;
	static UCHAR pulseme = 0;
	STARTUPINFO info={sizeof(info)};

	SECURITY_ATTRIBUTES saAttr; 
	BOOL ErrorFound = false;

	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

#ifdef _DEBUG
		TRACE ("\nSimConnect Thread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 10);

		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
			if (Dispatcher++ > 50) {
				Dispatcher = 0; // 1 sec ellapsed
				if (theApp.m_Disable) {
					pUSIMDlg->TraySetIcon(IDI_USIM_DIS);
					pUSIMDlg->TrayUpdate();
				} else if (theApp.m_SimulationModel == SIM_NONE) {
					pUSIMDlg->TraySetIcon(IDI_USIM_OFF);
					pUSIMDlg->TrayUpdate();
				} else {
					if (pulseme) {
						if (theApp.m_SimulationModel == SIM_TEST)
							pUSIMDlg->TraySetIcon(IDI_USIM_ONGR);
						else
							pUSIMDlg->TraySetIcon(IDI_USIM_ON);
					} else {
						if (theApp.m_SimulationModel == SIM_TEST)
							pUSIMDlg->TraySetIcon(IDI_USIM_ONGR1);
						else
							pUSIMDlg->TraySetIcon(IDI_USIM_ON1);
					}
					pulseme = !pulseme;
					pUSIMDlg->TrayUpdate();
				}
				ScanProcesses();
			}
			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
			if (m_OS64) {
				// launch 64 exe
				if(!m_processInfo.hProcess) {
					// Create a pipe for the child process's STDOUT. 
					if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
						ErrorFound = true;
#ifdef _DEBUG
						TRACE ("\nError - StdoutRd CreatePipe\n");
#endif
					}

					// Ensure the read handle to the pipe for STDOUT is not inherited.
					if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
						ErrorFound = true;
#ifdef _DEBUG
						TRACE ("\nError - Stdout SetHandleInformation\n");
#endif
					}

					// Create a pipe for the child process's STDIN. 
					if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
						ErrorFound = true;
#ifdef _DEBUG
						TRACE ("\nError - Stdin CreatePipe\n");
#endif
					}

					// Ensure the write handle to the pipe for STDIN is not inherited. 
					if (! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
						ErrorFound = true;
#ifdef _DEBUG
						TRACE ("\nError - Stdin SetHandleInformation\n");
#endif
					}

					ZeroMemory(&info, sizeof(STARTUPINFO));
					info.cb = sizeof(STARTUPINFO); 
					info.hStdError = g_hChildStd_OUT_Wr;
					info.hStdOutput = g_hChildStd_OUT_Wr;
					info.hStdInput = g_hChildStd_IN_Rd;
					info.dwFlags |= STARTF_USESTDHANDLES;

					CString cPath = (LPTSTR)(LPCTSTR)IDS_CPATH;

					CreateProcess((LPTSTR)(LPCTSTR)cPath,	// Launch USIM x64 child process
						NULL, 
						NULL, 
						NULL, 
						TRUE, 
						0, 
						NULL, 
						NULL, 
						&info, 
						&m_processInfo);
				} // if(!m_processInfo.hProcess)
			}
			ScanProcesses();
			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_CONTINUE:
			m_Run = TRUE;
			SetActivityStatus(CThread::THREAD_CONTINUING);
			break;

		case CThread::CMD_STOP:
			m_Run = FALSE;
			bCanContinue = FALSE;
			if (m_OS64 && m_processInfo.hProcess) {
				// Running on 64 bit windows. Kill 64 bit process.
				PostThreadMessage(m_processInfo.dwThreadId, WM_QUIT, NULL, NULL);
				CloseHandle(m_processInfo.hProcess);
				CloseHandle(m_processInfo.hThread);
			}
			SimDisconnect(SIM_TEST);
			break;

		default:
			break;
		};

	} while (bCanContinue);

#ifdef _DEBUG
		TRACE ("\nSimConnectThread killed!\n");
#endif
	if (ErrorFound) {
		// write to log file
		CString str;
		TRACE("CreatePipe - error\n");
		str.Format("%susim.log", theApp.m_AppPath);
		FILE* stream = fopen(str, "a");
		fprintf(stream, "CreatePipe - error\n");
		fclose(stream);
	}

	return 33;	// ... if Thread task completion OK
}

void CSimConnectThread::ScanProcesses()
{
	UINT i;
	CString s_Buf;
	CHAR chBuf;
	DWORD dwRead = 0, dwWritten;
	BOOL bSuccess = 0;

	// 50 ms elapsed
	if ((timeGetTime() - m_deltaT_IW) < 500) // check sim status every 0.5 sec
		return;

	m_deltaT_IW = timeGetTime();

	if (theApp.m_Disable) {
		theApp.m_SimulationModel = SIM_NONE;
		theApp.m_pDefTabCtrl->m_DefaultTab.SimUpdate();		// Update status field
		return;
	} else if (theApp.m_SimulationModel == SIM_NONE) {
		theApp.m_SimulationModel = SIM_TEST;
		theApp.m_pDefTabCtrl->m_DefaultTab.SimUpdate();		// Update status field
	}

	if (!theApp.m_AutoConnect)
		return;

	CProcessList m_ProcessList;
	if (theApp.m_SimulationModel <= SIM_TEST) {
		for (i=0; i< ArraySize(SimTypes); i++) {
			if (!theApp.SimList[i].SimPath.IsEmpty() && theApp.SimList[i].Enable) {			
				// Check if 32 bit SIM running
				if (m_ProcessList.FindModule((LPTSTR)(LPCTSTR)theApp.SimList[i].SimPath) != NULL) {
					SimConnect(i);
					m_SimIdx = i;
					m_Running32 = true;
					break;
				} else if (m_OS64) {
					// Running on 64 bit windows. Check for 64 bit processes.
					PostThreadMessage(m_processInfo.dwThreadId, NULL, NULL, NULL);	// send wake up call
					bSuccess = WriteFile(g_hChildStd_IN_Wr, theApp.SimList[i].SimPath, theApp.SimList[i].SimPath.GetLength(), &dwWritten, NULL);

					chBuf = 0;
					bSuccess = ReadFile(g_hChildStd_OUT_Rd, &chBuf, 1, &dwRead, NULL);
					if (bSuccess && (dwRead > 0)) {
						if (chBuf == 1) {
							SimConnect(i);
							m_SimIdx = i;
							break;
						}
					}
				}
			} // if (!theApp.SimList[i].SimFileName.IsEmpty() && theApp.SimList[i].Enable)
		} // for (i=0; i< ArraySize(SimTypes); i++)
	} else {
		// Check if SIM is NOT running
		for (i=0; i< ArraySize(SimTypes); i++) {
			if (!theApp.SimList[i].SimFileName.IsEmpty()) {
				if (m_Running32 && m_ProcessList.FindModule((LPTSTR)(LPCTSTR)theApp.SimList[i].SimPath) == NULL) {
					// 32 bit Process is NOT running
					SimDisconnect(theApp.SimList[i].SimType);
					m_Running32 = false;
					m_SimIdx = 0;
				} else if (m_OS64 && !m_Running32) {
					// Running on 64 bit windows. Check for 64 bit processes.
					PostThreadMessage(m_processInfo.dwThreadId, NULL, NULL, NULL);	// send wake up call
					bSuccess = WriteFile(g_hChildStd_IN_Wr, theApp.SimList[i].SimPath, theApp.SimList[i].SimPath.GetLength(), &dwWritten, NULL);

					chBuf = 0;
					bSuccess = ReadFile(g_hChildStd_OUT_Rd, &chBuf, 1, &dwRead, NULL);
					if (chBuf == 0) {
						// 64 bit Process is NOT running
						SimDisconnect(theApp.SimList[i].SimType);
						m_SimIdx = 0;
					}
				}
			}
		}
	} // if (theApp.m_SimulationModel == SIM_NONE)

	theApp.m_pDefTabCtrl->m_DefaultTab.SimUpdate();		// Update status field
}

void CSimConnectThread::SimConnect(UINT nIndex)
{
	BOOL retflag = false;

	if (theApp.m_SimulationModel == theApp.SimList[nIndex].SimType) 
		return;
	if (theApp.m_SimulationModel > SIM_TEST) 
		return;

	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;

	switch (theApp.SimList[nIndex].SimType) {
		case SIM_KBS:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			retflag = true;
			break;

		case SIM_XPLANE:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			// set active datarefs
			retflag = theApp.m_XPSharedMem.StartSharedMem();
			theApp.m_XPSharedMem.MapVars();
			break;

		case SIM_FS9:

			break;

		case SIM_FSX:

			break;

		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
//			theApp.StartUDPThread();	// called in OnRefresh()
			retflag = true;
			break;

		case SIM_F4BMS:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			// set active datarefs
			retflag = theApp.m_F4SharedMem.StartSharedMem(theApp.SimList[nIndex].SimType);
			break;

		case SIM_GTR:
		case SIM_GTR2:
		case SIM_EVO:
		case SIM_GTL:
		case SIM_RACE:
		case SIM_RACE07:
		case SIM_RACEON:
		case SIM_VOLVO:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			// set active datarefs
			retflag = theApp.m_SimBinSharedMem.StartSharedMem(nIndex);
			break;

		case SIM_RF:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			// set active datarefs
			retflag = theApp.m_RFSharedMem.StartSharedMem(nIndex);
			break;

		case SIM_GPX:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			// set active datarefs
			retflag = theApp.m_GPXSharedMem.StartSharedMem(nIndex);
			break;

		case SIM_LFS:
			// load config file
			if (!theApp.SimList[nIndex].USCFile.IsEmpty())
				theApp.m_FilePath = theApp.SimList[nIndex].USCFile;

			// Stop all threads
			theApp.StopAllThreads();
			pUSIMDlg->OnLoadFile();
			theApp.m_pTreeDevices->OnRefresh();
			
			// set active datarefs
//			theApp.StartUDPThread();	// called in OnRefresh()
			retflag = true;
			break;
	}

	if (retflag) {
		theApp.m_SimulationModel = (UCHAR)theApp.SimList[nIndex].SimType;

		pUSIMDlg->TraySetIcon(IDI_USIM_ON);
		pUSIMDlg->TrayUpdate();
		TRACE("Simtype %d running...\n", theApp.SimList[nIndex].SimType);
	} else {
		TRACE("Simtype %d sharedmem failed!\n", theApp.SimList[nIndex].SimType);
	}
}

void CSimConnectThread::SimDisconnect(UINT SimType)
{
	if (theApp.m_SimulationModel != SimType)
		return;
	if (theApp.m_SimulationModel <= SIM_TEST)
		return;

	switch (SimType) {
		case SIM_KBS:

			break;

		case SIM_XPLANE:
			theApp.m_XPSharedMem.StopSharedMem();
			break;

		case SIM_FS9:

			break;

		case SIM_FSX:

			break;

		case SIM_IL2:
			theApp.StopUDPThread();
			break;

		case SIM_IL2FB:

			break;

		case SIM_IL2PF:

			break;

		case SIM_F4BMS:
			theApp.m_F4SharedMem.StopSharedMem();
			break;

		case SIM_GTR:
		case SIM_GTR2:
		case SIM_EVO:
		case SIM_GTL:
		case SIM_RACE:
		case SIM_RACE07:
		case SIM_RACEON:
		case SIM_VOLVO:
			theApp.m_SimBinSharedMem.StopSharedMem();
			break;

		case SIM_RF:
			theApp.m_RFSharedMem.StopSharedMem();
			break;

		case SIM_GPX:
			theApp.m_GPXSharedMem.StopSharedMem();
			break;

		case SIM_LFS:
			theApp.StopUDPThread();
			break;
	}

	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;

	if (theApp.m_Disable) {
		theApp.m_SimulationModel = SIM_NONE;
		pUSIMDlg->TraySetIcon(IDI_USIM_DIS);
	} else {
		theApp.m_SimulationModel = SIM_TEST;
		pUSIMDlg->TraySetIcon(IDI_USIM_OFF);
	}

	pUSIMDlg->TrayUpdate();
	TRACE("SimType %d not running...\n", SimType);
}
