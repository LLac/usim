// LFS_UDPThread.h: interface for the CLFS_UDPThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LFS_UDPTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_LFS_UDPTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"

/////////////////////////////////////////////////////////////////////////////

class CLFS_UDPThread : public CThread
{
public:
	DECLARE_DYNAMIC(CLFS_UDPThread)

// Attributes, Operations
	CLFS_UDPThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CLFS_UDPThread();

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
	OutGaugePack m_AnswerBuf;
	CString m_RequestBuf;
	BOOL	m_Status;

private:
	void	RequestBuffer();
	void	AnswerBuffer();
	BOOL	InitLFS_UDP();
	void	StopLFS_UDP();
	BOOL	SendPacket();
	BOOL	ReadPacket();
	void	MapVars();
};

#endif // !defined(AFX_LFS_UDPTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
