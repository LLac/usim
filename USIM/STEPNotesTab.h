#if !defined(AFX_STEPNOTESTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_STEPNOTESTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STEPNotesTab.h : header file
//
//#include "ResizableLib\ResizableChild.h"

/////////////////////////////////////////////////////////////////////////////
// CSTEPNotesTab dialog
class CXTabCtrl;
class CSTEPNotesTab : public CResizableDialog
{
// Construction
public:
	CSTEPNotesTab(CWnd* pParent = NULL);   // standard constructor
	void InitNotesTab();
	void SetTreeItemDesc();

	CXTabCtrl*	m_pTab;
	CStatic		m_IOName;
	CEdit		m_Desc, m_Notes, m_Enable;
	CButton		m_Clear;

// Dialog Data
	//{{AFX_DATA(CSTEPNotesTab)
	enum { IDD = IDD_NOTES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	void OnClear();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTEPNotesTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSTEPNotesTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnChangeDesc();
	afx_msg void OnChangeNotes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEPNOTESTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
