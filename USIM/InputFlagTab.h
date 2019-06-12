#if !defined(AFX_INPUTFLAGTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_INPUTFLAGTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputFlagTab.h : header file
//
#include "GridCtrl_src\GridCtrl.h"
#include "CEAdvButton\CEAdvButton.h"
#include "ButtonSSL\ButtonSSL.h"

#define FLAG_NUM_ROWS	MAX_INPUT_FLAGS + 1
#define FLAG_NUM_COLS	4
/////////////////////////////////////////////////////////////////////////////
// CInputFlagTab dialog
class CXTabCtrl;
class CInputFlagTab : public CResizableDialog
{
	friend CTreeDevices;

// Construction
public:
	CInputFlagTab(CWnd* pParent = NULL);   // standard constructor
	void InitInputFlagTab();

	CXTabCtrl*	m_pTab;
	CStatic		m_IOName;
	CStatic		m_Enable;
	CButton		m_OBClear;
	CButton		m_Add, m_Remove;
	CEAdvButton	m_OBGroup;

// Dialog Data
	//{{AFX_DATA(CInputFlagTab)
	enum { IDD = IDD_INPUT_FLAGS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	void OnClear();

	CGridCtrl	m_Grid;
	CImageList	m_ImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputFlagTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CInputFlagTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
//	afx_msg void OnAdd();
//	afx_msg void OnRemove();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTFLAGTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
