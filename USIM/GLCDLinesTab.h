#if !defined(AFX_GLCDLINESTAB_H__8CC18FE5_5B63_4EA4_8C7E_E5AAE861F03C__INCLUDED_)
#define AFX_GLCDLINESTAB_H__8CC18FE5_5B63_4EA4_8C7E_E5AAE861F03C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLCDLinesTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGLCDLinesTab dialog
class CXTabCtrl;
class CGLCDLinesTab : public CResizableDialog
{
// Construction
public:
	CGLCDLinesTab(CWnd* pParent = NULL);   // standard constructor
	void InitLCDLinesTab();
	void SetTabIcons();
	void SetTabIcon(int);
	void PopulateCombos();
	void AddLineText(CEdit *, UINT);
	BOOL SetClipboardText(LPCTSTR);

	CXTabCtrl*	m_pTab;
	UINT		m_CurPageSel;

// Dialog Data
	//{{AFX_DATA(CGLCDLinesTab)
	enum { IDD = IDD_LCD_LINES };
	CButton	m_Button5;
	CButton	m_Button4;
	CButton	m_Button3;
	CButton	m_Button2;
	CButton	m_Button1;
	CEdit	m_LCDLine5;
	CEdit	m_LCDLine4;
	CEdit	m_LCDLine3;
	CEdit	m_LCDLine2;
	CEdit	m_LCDLine1;
	CButton	m_Clear;
	CButton		m_Enable;
	CButton		m_Radio1;
	CButton		m_Radio2;
	CComboBox	m_ComboPrev;
	CComboBox	m_ComboNext;
	CComboBox	m_ComboDisplay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLCDLinesTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CVarListDlg		m_VarListDlg;

protected:
	int				m_PAGE, m_PAGE_P, m_PAGE_DIS, m_PAGE_DIS_P;
	CImageList		m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CGLCDLinesTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnSelchangeComboPrev();
	afx_msg void OnSelchangeComboNext();
	afx_msg void OnSelchangeComboDisplay();
	afx_msg void OnEnable();
	afx_msg void OnClear();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLCDLINESTAB_H__8CC18FE5_5B63_4EA4_8C7E_E5AAE861F03C__INCLUDED_)
