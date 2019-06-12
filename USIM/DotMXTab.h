#if !defined(AFX_DOTMXTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_DOTMXTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DotMXTab.h : header file
//
#include "GridCtrl_src\GridCtrl.h"
#include "CEAdvButton\CEAdvButton.h"
#include "ButtonSSL\ButtonSSL.h"
//#include "VarListDlg.h"

#define DMX_NUM_ROWS	5
#define DMX_NUM_COLS	10
/////////////////////////////////////////////////////////////////////////////
// CDotMXTab dialog
class CXTabCtrl;
class CDotMXTab : public CResizableDialog
{
	friend CTreeDevices;

// Construction
public:
	CDotMXTab(CWnd* pParent = NULL);   // standard constructor
	void InitDotMXTab();

	CXTabCtrl*	m_pTab;
	CStatic		m_IOName;
	CComboBox	m_LBCombo, m_HSICombo;
	CEdit		m_Notes;
	CStatic		m_Text3, m_Text4, m_Enable;
	CButton		m_OBClear;
	CButton		m_Add, m_Remove, m_ClearData;
	CEAdvButton	m_OBGroup;

// Dialog Data
	//{{AFX_DATA(CDotMXTab)
	enum { IDD = IDD_7SEG };
		// NOTE: the ClassWizard will add data members here
	CSmartEdit	m_OBLevelText;
	CLinkSlider	m_OBLevel;
	CButton		m_Override;
	//}}AFX_DATA

private:
	void OnClear();
	void AddItem(BOOL);

	CGridCtrl	m_Grid;
	CImageList	m_ImageList;
	int			m_CurrentRow;
	CButtonSSL	m_LampTest;
	CVarListDlg	m_VarListDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDotMXTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDotMXTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnLampTest();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnClearData();
	afx_msg void OnChangeOBLevel();
	afx_msg void OnChangeNotes();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOTMXTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
