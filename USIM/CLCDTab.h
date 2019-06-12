#if !defined(AFX_CLCDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_CLCDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CLCDTab.h : header file
//
#include "ButtonSSL\ButtonSSL.h"
#include "CLCDLinesTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCLCDTab dialog
class CXTabCtrl;
class CCLCDTab : public CResizableDialog
{
// Construction
	friend CTreeDevices;

public:
	CCLCDTab(CWnd* pParent = NULL);   // standard constructor
	void InitCLCDTab();

	CXTabCtrl*	m_pTab;
	CEdit		m_Notes;
	CStatic		m_IOName;
	CStatic		m_Text3, m_Text4, m_Enable;
	CButton		m_OBClear;

// Dialog Data
	//{{AFX_DATA(CCLCDTab)
	enum { IDD = IDD_GLCD };
	CStatic	m_Static3;
	CStatic	m_Static11;
	CStatic	m_Static12;
	CCLCDLinesTabCtrl	m_CLCDLinesTabCtrl;
	//}}AFX_DATA

private:
	CButtonSSL	m_DEDEnable, m_CLCDTest, m_CLCDInvert;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLCDTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCLCDTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnCLCDTest();
	afx_msg void OnChangeNotes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLCDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
