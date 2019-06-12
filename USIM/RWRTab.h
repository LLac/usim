#if !defined(AFX_RWRTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_RWRTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RWRTab.h : header file
//
#include "ButtonSSL\ButtonSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CRWRTab dialog
class CXTabCtrl;
class CRWRTab : public CResizableDialog
{
	friend CTreeDevices;

// Construction
public:
	CRWRTab(CWnd* pParent = NULL);   // standard constructor
	void InitRWRTab();

	CXTabCtrl*	m_pTab;
	CEdit		m_Notes;
	CStatic		m_IOName;
	CStatic		m_Text3, m_Text4, m_Enable;
	CButton		m_OBClear;

// Dialog Data
	//{{AFX_DATA(CRWRTab)
	enum { IDD = IDD_RWR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	CButtonSSL	m_RWREnable, m_RWRTest;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWRTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRWRTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnEnable();
	afx_msg void OnRWRTest();
	afx_msg void OnChangeNotes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWRTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
