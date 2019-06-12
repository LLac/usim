// DoInputThread.h: interface for the CDoInputThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOINPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_DOINPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadDerived.h"
#include "cSound\cSound.h"
#include "FSKeysPage.h"
#include "OffsetsPage.h"
#include "FSConnect.h"

typedef struct tagDoInputTabs
{
	BOOL	OPFSKeysTab;
	BOOL	ORFSKeysTab;
	BOOL	ORepFSKeysTab;
	BOOL	OPOffsetsTab;
	BOOL	OROffsetsTab;
} DOINPUTTABS;

/////////////////////////////////////////////////////////////////////////////

class CDoInputThread : public CThreadDerived
{
public:
//	DECLARE_DYNAMIC(CDoInputThread)

// Construction & Destruction
	CDoInputThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~CDoInputThread();

// Operations
public:
	virtual void DoThreadWorker();

// Attributes, Operations
protected:
	DWORD	m_delta_T;
	HWND	m_Audio;

private:
	cSound i_Sound;
	DOINPUTTABS	m_DoInputsTabs;
	
	CFSConnect	m_FSConnect;

	double FormatVar(MODULE_VAR *);
	__int64 CastVar(double *, UINT);

	void ReadInputs(const UINT);
	BOOL InputState(UINT, UINT);
	BOOL CheckInputState(UINT, UINT);

	void FSKeysOnPress(CFSKeysPage *, UINT);
	void FSKeysOnRelease(CFSKeysPage *);
	void FSKeysOnRepeat(CFSKeysPage *);
	void PlayFile(CString);
	void OffsetsOnPress(COffsetsPage *, UINT);
	void OffsetsOnRelease(COffsetsPage *);
};

#endif // !defined(AFX_DOINPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
