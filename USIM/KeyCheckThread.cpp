// KeyCheckThread.cpp: implementation of the CKeyCheckThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "USIMDlg.h"
#include <winuser.h> // Required for the ::SendInput function
#include "hid_dll\intel_hex.h"
#include "KeyCheckThread.h"
#include "w32process\w32process.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CKeyCheckThread, CThread)

CKeyCheckThread::CKeyCheckThread(void* pOwnerObject, LPARAM lParam) 
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(CKeyCheckThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_Run = FALSE;
}

CKeyCheckThread::~CKeyCheckThread()
{

}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD CKeyCheckThread::ThreadHandler()
{
	BOOL bCanContinue = true;
	int nIncomingCommand;
	static UCHAR Dispatcher = 0;
	static UCHAR pulseme = 0;

//	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;

#ifdef _DEBUG
		TRACE ("\nKeyCheck Thread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 1000);

		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
			if (m_DevCnt == 0)
				DetectDevice(&m_DevCnt, m_Device, PID_USIM_V1, DT_SECURITY);

			if (KeyCheck()) {
				SetActivityStatus(CThread::THREAD_RUNNING);
			} else {
				SetActivityStatus(CThread::THREAD_CONTINUING);
				CloseDevices(m_DevCnt, m_Device);
				m_DevCnt = 0;
			}
			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
			SetActivityStatus(CThread::THREAD_CONTINUING);
			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_STOP:
			CloseDevices(m_DevCnt, m_Device);
			m_DevCnt = 0;

			m_Run = FALSE;
			bCanContinue = FALSE;
			break;

		default:
			break;
		};

	} while (bCanContinue);

	CloseDevices(m_DevCnt, m_Device);
	m_DevCnt = 0;

#ifdef _DEBUG
		TRACE ("\nKeyCheckThread killed!\n");
#endif

	return 99;	// ... if Thread task completition OK
}

BOOL CKeyCheckThread::KeyCheck()
{
	ULONG retval;
	BOOL retflag = false;
	char Value1[59], Value2[59];
	char EncValue1[65], EncValue2[65];

	// Generate Random Value1
	RandomValue(Value1);

	// Encrypt Value1 with Key1
	EncValue1[0] = 0;
	CryCodec(Value1, &EncValue1[SOD], KEY1);

	// write encrypted Value1 to USIM dongle
	EncValue1[REPORT_ID] = 0;	// Report ID
    EncValue1[REC_TYPE] = (UCHAR)CODEC;

	retval = 0;
	retval = WriteData(m_Device, EncValue1);

	if (retval == DEV_FAILED || retval == 0)
		return false;

	// Read Value2 from USIM dongle
    retval = 0;
	retval = ReadData(m_Device, EncValue2);
	if (retval == DEV_FAILED || retval == DEV_TIMEOUT)
		return false;

/********* TEST ONLY ***********/
// Encrypt Value1 with Key2
//CryCodec(Value1, EncValue2, KEY2);
/********* TEST ONLY ***********/

	// Decrypt Value2 with Key2
	CryCodec(EncValue2, Value2, KEY2);

	// Compare Value2 with Value1
	if (memcmp((const char *)Value1, (const char *)Value2, 59) == 0) 
		retflag = true;

	return retflag;
}

void CKeyCheckThread::RandomValue(char *Value)
{
	srand((UINT)time(0)); // set initial seed value to system clock
	for (int nCount=0; nCount < 59; ++nCount) {
		Value[nCount] = (char)(rand() % 256);
	}
}

void CKeyCheckThread::CryCodec(char *Value, char *EncValue, const char *Key)
{
	// XOR schar[0] and pchar[0] to get dchar[0]
	EncValue[0] = (char)(Value[0] ^ Key[0]);

	// For each i > 0, XOR schar[i] and pchar[i] and dchar[i-1]
	for (int i=1; i<59; i++) {
		EncValue[i] = (char)(Value[i] ^ Key[i] ^ EncValue[i-1]);
		EncValue[i] ^= EncValue[i-1];
	}
}
