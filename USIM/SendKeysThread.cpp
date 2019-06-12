// SendKeysThread.cpp: implementation of the CSendKeysThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include <winuser.h> // Required for the ::SendInput function
#include "SendKeysThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define KEYEVENTF_SCANCODE 0x0008
#define TOGGLE	0x01
#define ON		0x02
#define REPEAT	0x03

static UINT __cdecl TSendKeysOnPress( LPVOID pParam );
static UINT __cdecl TSendKeysOnRelease( LPVOID pParam );

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSendKeysThread, CThread)

CSendKeysThread::CSendKeysThread(void* pOwnerObject, LPARAM lParam) 
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(CSendKeysThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_deltaT_SKW = m_deltaT_IW = 0;
	m_Run = FALSE;
}

CSendKeysThread::~CSendKeysThread()
{
	m_deltaT_SKW = m_deltaT_IW = 0;
}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD CSendKeysThread::ThreadHandler()
{
	BOOL bCanContinue = true;
	int nIncomingCommand;
	POSITION pos;
	CWinThread *tmpThread;
	DWORD dwExitCode;

#ifdef _DEBUG
		TRACE ("\nSendKeysThread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 10);

		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
//			if (theApp.m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
				DoSendKeysWorker();
			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
//			if (theApp.m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
				DoSendKeysWorker();
			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_STOP:
			pos = m_ThreadList.GetHeadPosition();
			while (pos != NULL) {
				tmpThread = m_ThreadList.GetNext(pos);

				try	{
					dwExitCode = 0;
					::TerminateThread(tmpThread->m_hThread, dwExitCode);
					try	{delete tmpThread;} catch (...) {}
				} catch (...) {}
			}

			m_ThreadList.RemoveAll();

			m_Run = FALSE;
			bCanContinue = FALSE;
			break;

		default:
			break;
		};

	} while (bCanContinue);

#ifdef _DEBUG
		TRACE ("\nSendKeysThread killed!\n");
#endif

	return 44;	// ... if Thread task completition OK
}

void CSendKeysThread::DoSendKeysWorker()
{
	int dIndex, iIndex;
	CWinThread *tmpThread;
	DWORD	dwThreadExitCode;

	if ( (timeGetTime() - m_deltaT_SKW) < 10) 
		return;

	// 10 ms elapsed
//	TRACE("SendKeys Delta = %d\n", (timeGetTime() - m_deltaT_SKW));

	m_deltaT_SKW = timeGetTime();

	if (theApp.m_SimulationModel == SIM_NONE) return;

	Lock();

	// Clean up thread pool;
	POSITION pos = m_ThreadList.GetHeadPosition();
	while (pos != NULL) {
		tmpThread = m_ThreadList.GetNext(pos);
		try {
			::GetExitCodeThread(tmpThread->m_hThread, &dwThreadExitCode);
			if (dwThreadExitCode != STILL_ACTIVE) {
				TRACE ("Remove thread list!\n");
				m_ThreadList.RemoveAt(m_ThreadList.Find(tmpThread));
			}
		} catch (...) {
			TRACE ("Remove thread list exception!\n");
			m_ThreadList.RemoveAt(m_ThreadList.Find(tmpThread));
		};		
	}

	for (dIndex = 0; dIndex < MAX_DEVICES; dIndex++) {
		if (theApp.m_pDevArray[dIndex] == NULL) continue;

		if (theApp.m_pDevArray[dIndex]->m_Connected) {
			for (iIndex = 0; iIndex < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices; iIndex++) {
				if (!theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex].IsSet())
					continue;

				if (theApp.m_pDevArray[dIndex]->m_InputActive[iIndex] && !theApp.m_pDevArray[dIndex]->m_InputDisabled[iIndex]) {
					// if toggled
					if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] != theApp.m_pDevArray[dIndex]->m_InputsSKOld[iIndex]) {
						// Toggled. Save old value
						theApp.m_pDevArray[dIndex]->m_InputsSKOld[iIndex] = theApp.m_pDevArray[dIndex]->m_Inputs[iIndex];

						// if ON
						if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1) {
							// DO ON PRESS
//							SendKeysOnPress (&theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex], TOGGLE);
							// start new thread
							tmpThread = AfxBeginThread(TSendKeysOnPress, static_cast<LPVOID>(&theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex]));
							m_ThreadList.AddTail(tmpThread);

							TRACE ("SendKeysOnPress.\n");							
						} else {
							// DO ON RELEASE
//							SendKeysOnRelease (&theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex]);
							// start new thread
							tmpThread = AfxBeginThread(TSendKeysOnRelease, static_cast<LPVOID>(&theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex]));
							m_ThreadList.AddTail(tmpThread);
							TRACE ("SendKeysOnRelease.\n");
						}
					} else {
						if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1 && 
							theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex].m_OPRepeat && 
							!theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex].m_OPHoldKey) {
							// DO ON PRESS
//							SendKeysOnPress (&theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex], REPEAT);
							// start new thread
							tmpThread = AfxBeginThread(TSendKeysOnPress, static_cast<LPVOID>(&theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex]));
							m_ThreadList.AddTail(tmpThread);
						}
					} 
				} 
			}
		}
	}

	Unlock();
}

static UINT __cdecl TSendKeysOnPress( LPVOID pParam )
//void CSendKeysThread::SendKeysOnPress(CSendKeysPage *nSendKeys, UINT iStatus)
{
	CSendKeysPage *nSendKeys = (CSendKeysPage *)pParam;
	INPUT vKInput[2];

    if (nSendKeys == NULL)
		return 1;   // if pObject is not valid

	// Trigger SendKeys Event
	for (int i=0; i<MAX_MACROS; i++) {
		if (nSendKeys->m_OPKeyMacros[i].iVKCode == 0) continue;

		// set Caps, Num and Scroll keys
		if (nSendKeys->m_OPKeyMacros[i].iCaps == 0) {
			if (GetKeyState(VK_CAPITAL) & 0x0001) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_CAPITAL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		} else {
			if (!(GetKeyState(VK_CAPITAL) & 0x0001)) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_CAPITAL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		}

		if (nSendKeys->m_OPKeyMacros[i].iNum == 0) {
			if (GetKeyState(VK_NUMLOCK) & 0x0001) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_NUMLOCK, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		} else {
			if (!(GetKeyState(VK_NUMLOCK) & 0x0001)) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_NUMLOCK, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		}

		if (nSendKeys->m_OPKeyMacros[i].iScroll == 0) {
			if (GetKeyState(VK_SCROLL) & 0x0001) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_SCROLL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		} else {
			if (!(GetKeyState(VK_SCROLL) & 0x0001)) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_SCROLL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		}

		// set Alt, Shift and Ctrl keys
		if (nSendKeys->m_OPKeyMacros[i].iAlt != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iAlt, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		if (nSendKeys->m_OPKeyMacros[i].iShift != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iShift, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		if (nSendKeys->m_OPKeyMacros[i].iCtrl != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iCtrl, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		// set virtual key
		if (nSendKeys->m_OPKeyMacros[i].iVKCode != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));       
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = nSendKeys->m_OPKeyMacros[i].iSKCode;
			if (nSendKeys->m_OPKeyMacros[i].iExtended)
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_EXTENDEDKEY;
			else
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));

			TRACE ("Sendinput wScan 0x%x  %d\n", vKInput[0].ki.wScan, vKInput[0].ki.wScan);
		}

		if (!nSendKeys->m_OPHoldKey) {
			Sleep(nSendKeys->m_OPKeyMacros[i].iHoldDelay);

			// release virtual key
			::ZeroMemory(vKInput, sizeof(vKInput));        
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = nSendKeys->m_OPKeyMacros[i].iSKCode;
			if (nSendKeys->m_OPKeyMacros[i].iExtended)
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY; // THIS IS IMPORTANT
			else
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // THIS IS IMPORTANT
			::SendInput(1, vKInput, sizeof(INPUT));

			if (nSendKeys->m_OPKeyMacros[i].iCtrl != 0) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iCtrl, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(1, vKInput, sizeof(INPUT));
			}

			if (nSendKeys->m_OPKeyMacros[i].iShift != 0) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iShift, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(1, vKInput, sizeof(INPUT));
			}

			if (nSendKeys->m_OPKeyMacros[i].iAlt != 0) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iAlt, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(1, vKInput, sizeof(INPUT));
			}
		}

		Sleep(nSendKeys->m_OPKeyMacros[i].iMacroDelay);
	}

	TRACE ("TSendKeysOnPress Thread exit: 111\n");
	return 1;   // thread completed successfully
}

static UINT __cdecl TSendKeysOnRelease ( LPVOID pParam )
//void CInputThread::SendKeysOnRelease(CSendKeysPage *nSendKeys)
{
	CSendKeysPage *nSendKeys = (CSendKeysPage *)pParam;
	INPUT vKInput[2];
	int i;

    if (nSendKeys == NULL)
		return 2;   // if pObject is not valid

	if (nSendKeys->m_OPHoldKey) {
		for (i=0; i<MAX_MACROS; i++) {
			if (nSendKeys->m_OPKeyMacros[i].iVKCode == 0) continue;

			// release OPKeyMacros
			::ZeroMemory(vKInput, sizeof(vKInput));        
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = nSendKeys->m_OPKeyMacros[i].iSKCode;
			if (nSendKeys->m_OPKeyMacros[i].iExtended)
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY; // THIS IS IMPORTANT
			else
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // THIS IS IMPORTANT
			::SendInput(1, vKInput, sizeof(INPUT));

			if (nSendKeys->m_OPKeyMacros[i].iCtrl != 0) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iCtrl, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(1, vKInput, sizeof(INPUT));
			}

			if (nSendKeys->m_OPKeyMacros[i].iShift != 0) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iShift, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(1, vKInput, sizeof(INPUT));
			}

			if (nSendKeys->m_OPKeyMacros[i].iAlt != 0) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_OPKeyMacros[i].iAlt, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(1, vKInput, sizeof(INPUT));
			}
		}
	}

	for (i=0; i<MAX_MACROS; i++) {
		// Do ORKeyMacros
		if (nSendKeys->m_ORKeyMacros[i].iVKCode == 0) continue;

		// set Caps, Num and Scroll keys
		if (nSendKeys->m_ORKeyMacros[i].iCaps == 0) {
			if (GetKeyState(VK_CAPITAL) & 0x0001) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_CAPITAL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		} else {
			if (!(GetKeyState(VK_CAPITAL) & 0x0001)) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_CAPITAL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		}

		if (nSendKeys->m_OPKeyMacros[i].iNum == 0) {
			if (GetKeyState(VK_NUMLOCK) & 0x0001) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_NUMLOCK, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		} else {
			if (!(GetKeyState(VK_NUMLOCK) & 0x0001)) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_NUMLOCK, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		}

		if (nSendKeys->m_ORKeyMacros[i].iScroll == 0) {
			if (GetKeyState(VK_SCROLL) & 0x0001) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_SCROLL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		} else {
			if (!(GetKeyState(VK_SCROLL) & 0x0001)) {
				::ZeroMemory(vKInput, sizeof(vKInput));
				vKInput[0].type = vKInput[1].type = INPUT_KEYBOARD;
				vKInput[0].ki.wScan = vKInput[1].ki.wScan = (USHORT)MapVirtualKey(VK_SCROLL, 0);
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				vKInput[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				::SendInput(2, vKInput, sizeof(INPUT));
			}
		}

		// set Alt, Shift and Ctrl keys
		if (nSendKeys->m_ORKeyMacros[i].iAlt != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(VK_MENU, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		if (nSendKeys->m_ORKeyMacros[i].iShift != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_ORKeyMacros[i].iShift, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		if (nSendKeys->m_ORKeyMacros[i].iCtrl != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_ORKeyMacros[i].iCtrl, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		// set virtual key
		if (nSendKeys->m_ORKeyMacros[i].iVKCode != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));       
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = nSendKeys->m_ORKeyMacros[i].iSKCode;
			if (nSendKeys->m_ORKeyMacros[i].iExtended)
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_EXTENDEDKEY;
			else
				vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		Sleep(nSendKeys->m_ORKeyMacros[i].iHoldDelay);

		// release virtual key
		::ZeroMemory(vKInput, sizeof(vKInput));
		vKInput[0].type = INPUT_KEYBOARD;
		vKInput[0].ki.wScan = vKInput[1].ki.wScan = nSendKeys->m_ORKeyMacros[i].iSKCode;
		if (nSendKeys->m_ORKeyMacros[i].iExtended)
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
		else
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		::SendInput(1, vKInput, sizeof(INPUT));

		if (nSendKeys->m_ORKeyMacros[i].iCtrl != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_ORKeyMacros[i].iCtrl, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		if (nSendKeys->m_ORKeyMacros[i].iShift != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_ORKeyMacros[i].iShift, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		if (nSendKeys->m_ORKeyMacros[i].iAlt != 0) {
			::ZeroMemory(vKInput, sizeof(vKInput));
			vKInput[0].type = INPUT_KEYBOARD;
			vKInput[0].ki.wScan = (USHORT)MapVirtualKey(nSendKeys->m_ORKeyMacros[i].iAlt, 0);
			vKInput[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			::SendInput(1, vKInput, sizeof(INPUT));
		}

		Sleep(nSendKeys->m_ORKeyMacros[i].iMacroDelay);
	}

	TRACE ("TSendKeysOnRelease Thread exit: 222\n");
	return 2;   // thread completed successfully
}
