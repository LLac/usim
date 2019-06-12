#if !defined(AFX_F4SHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
#define AFX_F4SHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// F4SharedMem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CF4SharedMem window

class CF4SharedMem
{
// Construction
public:
	CF4SharedMem();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CF4SharedMem)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CF4SharedMem();
	void	StopSharedMem();
	void	StopSharedMem2();
	BOOL	StartSharedMem(UINT);
	void	UpdateSharedMem();
	void	MapVars();

	// Generated message map functions
private:
	HANDLE	m_hFlightData;
	void*	m_FlightDataMap;
	void*	m_FlightDataMap2;
	void*	m_ServerDataMap;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_F4SHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
