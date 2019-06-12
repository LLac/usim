#if !defined(AFX_XPSHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
#define AFX_XPSHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XPSharedMem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXPSharedMem window

class CXPSharedMem
{
// Construction
public:
	CXPSharedMem();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPSharedMem)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXPSharedMem();
	void	StopSharedMem();
	BOOL	StartSharedMem();
	void	MapVars();

	// Generated message map functions
private:
	HANDLE	m_hFlightData;
	void*	m_FlightDataMap;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPSHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
