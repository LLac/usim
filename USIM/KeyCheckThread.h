// KeyCheckThread.h: interface for the CKeyCheckThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYCHECKTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_KEYCHECKTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"

/////////////////////////////////////////////////////////////////////////////

class CKeyCheckThread : public CThread
{
public:
	DECLARE_DYNAMIC(CKeyCheckThread)

// Attributes, Operations
	CKeyCheckThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CKeyCheckThread();

// Attributes, Operations
protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// Main Thread Handler
	// The only method that must be implemented
	virtual	DWORD ThreadHandler();
	/////////////////////////////////////////////////////////////////////////////////////

protected:
	BOOL	m_Run;
	UINT	m_DevCnt;

	DeviceParam	m_Device[MAX_DEVICES];

private:
	BOOL KeyCheck();
	void RandomValue(char *Value);
	void CryCodec(char *Value, char *EncValue, const char *Key);
};

#endif // !defined(AFX_KEYCHECKTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
