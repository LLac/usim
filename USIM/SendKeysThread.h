// SendKeysThread.h: interface for the CSendKeysThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDKEYSTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_SENDKEYSTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"
#include "SendKeysPage.h"

/////////////////////////////////////////////////////////////////////////////

class CSendKeysThread : public CThread
{
public:
	DECLARE_DYNAMIC(CSendKeysThread)

// Attributes, Operations
	CSendKeysThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CSendKeysThread();

// Attributes, Operations
protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// Main Thread Handler
	// The only method that must be implemented
	virtual	DWORD ThreadHandler();
	/////////////////////////////////////////////////////////////////////////////////////

public:
	void DoThreadWorker();

protected:
	DWORD	m_deltaT_SKW, m_deltaT_IW;
	BOOL	m_Run;

private:
	void DoSendKeysWorker();
//	void SendKeysOnPress(CSendKeysPage *);
//	void SendKeysOnRelease(CSendKeysPage *);
//	void SendKeysOnRepeat(CSendKeysPage *);

	CList<CWinThread*, CWinThread*> m_ThreadList;
};

#endif // !defined(AFX_SENDKEYSTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
