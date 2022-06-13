// OutputThread.h: interface for the COutputThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_OUTPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadClass\Thread.h"

typedef struct _STRUCT_RWROUTPUT
{
	unsigned char symbol;
	unsigned char cnt;
} RWROUTPUT;

/////////////////////////////////////////////////////////////////////////////

class COutputThread : public CThread
{
public:
	DECLARE_DYNAMIC(COutputThread)

// Construction & Destruction
	COutputThread(void* pOwnerObject = NULL, LPARAM lParam = 0L);
	virtual ~COutputThread();

// Attributes, Operations
protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// Main Thread Handler
	// The only method that must be implemented
	virtual	DWORD ThreadHandler();
	/////////////////////////////////////////////////////////////////////////////////////

protected:
	int		m_delta_T;
	int		m_RWRcnt;
	int		m_GLCDcnt;
	int		m_CLCDcnt;
	int		m_SPIcnt;
	int		m_STEPcnt;
	BOOL	m_Run;

private:
	void DoOutputWorker();
	void InitOLED(UCHAR);
	void HIDOutput(UINT);
	void ReadDirectOutputs(UINT, UINT);
	void ReadMuxOutputs(UINT, UINT);
	void ReadDisplayOutputs(UINT, UINT);
	void ReadLatchedOutputs(UINT, UINT);
	void ReadDotMatrixOutputs(UINT, UINT);
	void UpdateCrtRWR(UINT, UINT);
	void UpdateLCD(CCLCDPage *, UINT, UINT, UINT);
//	void UpdateGLCD(UINT, UINT);
	void UpdateSPI(UINT, UINT);
	void UpdateSingleSPI(CSPIPage *, UINT, UINT, UINT);
	void UpdateSTEP(UINT, UINT);
	void UpdateSingleSTEP(CSTEPPage *, UINT, UINT, UINT);

	BOOL CheckConditions(double, double, COutputPage *);
	BOOL CheckFlags(CFlagsPage *);
	BOOL CheckLCDPageFlags(CFlagsPage *);
	CString ParseLCDText(CString, UCHAR);
	void FormatLCD(CCLCDPage *, UINT, UINT, UINT);

	BYTE m_LatchedOutputs[NUM_LATCHED_PER_PORT];
	UCHAR m_Line;
};

#endif // !defined(AFX_OUTPUTTHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
