#if !defined(AFX_SIMBINSHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
#define AFX_SIMBINSHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimBinSharedMem.h : header file
//

typedef struct _SIMBIN_SIMTYPE
{
	CString		Name;
	CString		SharedMemName;
} SIMBIN_SIMTYPE;

/////////////////////////////////////////////////////////////////////////////
// CSimBinSharedMem window

class CSimBinSharedMem
{
// Construction
public:
	CSimBinSharedMem();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimBinSharedMem)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimBinSharedMem();
	void	StopSharedMem();
	BOOL	StartSharedMem(UINT);
	void	UpdateSharedMem();
	void	MapVars();

	// Generated message map functions
private:
	HANDLE	m_hSimData;
	void*	m_SimDataMap;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMBINSHAREDMEM_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
