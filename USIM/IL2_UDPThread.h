// IL2_UDPThread.h: interface for the CIL2_UDPThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IL2_UDPTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_IL2_UDPTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"

typedef struct {
    char *ip;
    int port;
} Config;

/////////////////////////////////////////////////////////////////////////////

class CIL2_UDPThread : public CThread
{
public:
	DECLARE_DYNAMIC(CIL2_UDPThread)

// Attributes, Operations
	CIL2_UDPThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CIL2_UDPThread();

// Attributes, Operations
protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// Main Thread Handler
	// The only method that must be implemented
	virtual	DWORD ThreadHandler();
	/////////////////////////////////////////////////////////////////////////////////////

protected:
	DWORD	m_deltaT_IW;
	BOOL	m_Run;
	fd_set	m_ActiveReadFds;	// Static vars for socket code
	struct	hostent *localhost;
	struct	sockaddr_in server;
	struct	sockaddr_in client;
	SOCKET	m_sock;
	Config	m_conf;
	CString m_RequestBuf, m_AnswerBuf;
	BOOL	m_Status;

private:
	void	RequestBuffer();
	void	AnswerBuffer();
	BOOL	InitIL2_UDP();
	void	StopIL2_UDP();
	BOOL	SendPacket();
	BOOL	ReadPacket();
	void	MapVars();
	UINT	SearchVarID(UINT);
};

#endif // !defined(AFX_IL2_UDPTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
