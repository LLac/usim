#if !defined(AFX_GLCDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_GLCDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLCDTab.h : header file
//
#include "ButtonSSL\ButtonSSL.h"
#include "GLCDLinesTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGLCDTab dialog
class CXTabCtrl;
class CGLCDTab : public CResizableDialog
{
// Construction
	friend CTreeDevices;

public:
	CGLCDTab(CWnd* pParent = NULL);   // standard constructor
	void InitGLCDTab();

	CXTabCtrl*	m_pTab;
	CEdit		m_Notes;
	CStatic		m_IOName;
	CStatic		m_Text3, m_Text4, m_Enable;
	CButton		m_OBClear;

// Dialog Data
	//{{AFX_DATA(CGLCDTab)
	enum { IDD = IDD_GLCD };
	CGLCDLinesTabCtrl	m_GLCDLinesTabCtrl;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	CButtonSSL	m_DEDEnable, m_GLCDTest, m_GLCDInvert;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLCDTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGLCDTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnDEDEnable();
	afx_msg void OnPFLEnable();
	afx_msg void OnGLCDTest();
	afx_msg void OnGLCDInvert();
	afx_msg void OnChangeNotes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLCDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
