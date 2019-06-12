// InputThread.h: interface for the CInputThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_INPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"
#include "SendKeysPage.h"

/////////////////////////////////////////////////////////////////////////////

class CInputThread : public CThread
{
public:
	DECLARE_DYNAMIC(CInputThread)

// Attributes, Operations
	CInputThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CInputThread();

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

private:
	void DoInputWorker();
	void ReadInputs(const UINT);
	BOOL InputState(UINT, UINT);
	void VariablesOnPress(CVariablesPage *);
	void VariablesOnRelease(CVariablesPage *);
};

#endif // !defined(AFX_INPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
