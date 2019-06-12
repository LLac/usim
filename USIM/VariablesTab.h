#if !defined(AFX_VARIABLESTAB_H__8858A0F6_B86B_4261_AB1F_1FC4EA2892ED__INCLUDED_)
#define AFX_VARIABLESTAB_H__8858A0F6_B86B_4261_AB1F_1FC4EA2892ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VariablesTab.h : header file
//
#include "hexedit\HexEdit.h"
#include "CEAdvButton\CEAdvButton.h"
#include "NumSpinCtrl\NumSpinCtrl.h"
#include "VarListDlg.h"

/////////////////////////////////////////////////////////////////////////////
// VariablesTab dialog
class CXTabCtrl;
class CVariablesTab : public CResizableDialog
{
// Construction
public:
	CVariablesTab(CWnd* pParent = NULL);   // standard constructor
	void InitVariablesTab();
	void SetIOStatus();

	CXTabCtrl*	m_pTab;
	CStatic		m_IOName;
	CEdit		m_Notes, m_Enable;
	CButton		m_Clear;
	CNumSpinCtrl	m_ORIncSpin;
	CNumSpinCtrl	m_ORDecSpin;
	CNumSpinCtrl	m_OPIncSpin;
	CNumSpinCtrl	m_OPDecSpin;
	CAMSNumericEdit	m_OPValue;
	CAMSNumericEdit	m_ORValue;
	CComboBox		m_OPSize;
	CComboBox		m_ORSize;

// Dialog Data
	//{{AFX_DATA(CVariablesTab)
	enum { IDD = IDD_VARIABLES };
	CEdit	m_ORInc;
	CEdit	m_ORDec;
	CEdit	m_OPInc;
	CEdit	m_OPDec;
	CComboBox	m_OROperator;
	CComboBox	m_OPOperator;
	CButton	m_OPRadio1;
	CButton	m_OPRadio2;
	CButton	m_OPRadio3;
	CButton	m_OPRadio4;
	CButton	m_ORRadio1;
	CButton	m_ORRadio2;
	CButton	m_ORRadio3;
	CButton	m_ORRadio4;
	//}}AFX_DATA

private:
	void OnClear();
	CHexEdit		m_OPMask, m_ORMask;
	CEAdvButton		m_OPGroup, m_ORGroup;
	CButton			m_OPFlightData, m_ORFlightData;
	CButton			m_OPClearData, m_ORClearData;
	CStatic			m_OPText, m_ORText;
	CVarListDlg		m_VarListDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVariablesTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVariablesTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnChangeNotes();
	afx_msg void OnOPFlightData();
	afx_msg void OnOPClearData();
	afx_msg void OnORFlightData();
	afx_msg void OnORClearData();
	afx_msg void OnOPRadio1();
	afx_msg void OnOPRadio2();
	afx_msg void OnOPRadio3();
	afx_msg void OnOPRadio4();
	afx_msg void OnORRadio1();
	afx_msg void OnORRadio2();
	afx_msg void OnORRadio3();
	afx_msg void OnORRadio4();
	afx_msg void OnChangeOPMask();
	afx_msg void OnChangeORMask();
	afx_msg void OnSelchangeOPCombo1();
	afx_msg void OnSelchangeORCombo1();
	afx_msg void OnChangeOPValue();
	afx_msg void OnChangeORValue();
	afx_msg void OnChangeOPInc();
	afx_msg void OnChangeOPDec();
	afx_msg void OnChangeORInc();
	afx_msg void OnChangeORDec();
	afx_msg void OnSelchangeOpSize();
	afx_msg void OnSelchangeOrSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARIABLESTAB_H__8858A0F6_B86B_4261_AB1F_1FC4EA2892ED__INCLUDED_)
