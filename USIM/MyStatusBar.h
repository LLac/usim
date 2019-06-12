#if !defined(AFX_MYSTATUSBAR_H__B10578B8_0419_432A_8D68_4100A6FB4C67__INCLUDED_)
#define AFX_MYSTATUSBAR_H__B10578B8_0419_432A_8D68_4100A6FB4C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatusBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyStatusBar window

class CMyStatusBar : public CStatusBar
{
// Construction
public:
	CMyStatusBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStatusBar)
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyStatusBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyStatusBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATUSBAR_H__B10578B8_0419_432A_8D68_4100A6FB4C67__INCLUDED_)
