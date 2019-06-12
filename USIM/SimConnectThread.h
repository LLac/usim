// SimConnectThread.h: interface for the CSimConnectThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMCONNECTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_SIMCONNECTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"

/////////////////////////////////////////////////////////////////////////////

class CSimConnectThread : public CThread
{
public:
	DECLARE_DYNAMIC(CSimConnectThread)

// Attributes, Operations
	CSimConnectThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CSimConnectThread();

// Attributes, Operations
protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// Main Thread Handler
	// The only method that must be implemented
	virtual	DWORD ThreadHandler();
	/////////////////////////////////////////////////////////////////////////////////////

protected:
	DWORD	m_deltaT_SKW, m_deltaT_IW;
	BOOL	m_Run;
	BOOL	m_OS64;
	BOOL	m_Running32;
	UINT	m_SimIdx;
	PROCESS_INFORMATION m_processInfo;

private:
	void ScanProcesses();
	void SimConnect(UINT);
	void SimDisconnect(UINT);
	void SimUpdate();
	BOOL Is64BitWindows();
};

#endif // !defined(AFX_SIMCONNECTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
