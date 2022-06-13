#if !defined(AFX_LATCHEDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_LATCHEDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LatchedTab.h : header file
//
#include "ButtonSSL\ButtonSSL.h"
#include "amsEdit\amsEdit.h"
#include "HexEdit\HexEdit.h"
#include "VarListDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CLatchedTab dialog
class CXTabCtrl;
class CLatchedTab : public CResizableDialog
{
	friend CTreeDevices;
// Construction
public:
	CLatchedTab(CWnd* pParent = NULL);   // standard constructor
	void InitLatchedTab();

	CXTabCtrl*	m_pTab;

// Dialog Data
	//{{AFX_DATA(CLatchedTab)
	enum { IDD = IDD_OUTPUTS };
	CButton			m_MaskData;
	CButton			m_BlinkMaskData;
	CStatic			m_IOName;
	CEAdvButton		m_OVGroup1;
	CEAdvButton		m_OVGroup2;
	CEAdvButton		m_OBGroup2;
	CEAdvButton		m_OBGroup3;
	CEAdvButton		m_OBGroup4;
	CStatic			m_Text3;
	CStatic			m_Text4;
	CStatic			m_Leveltxt;
	CStatic			m_Enable;
	CComboBox		m_OPCombo1;
	CComboBox		m_OPCombo2;
	CComboBox		m_OPCombo3;
	CAMSNumericEdit	m_OPValue1;
	CAMSNumericEdit	m_OPValue2;
	CEdit			m_Notes;
	CHexEdit		m_HexMask;
	CHexEdit		m_BlinkMask;
	CAMSNumericEdit	m_BlinkOnTime;
	CAMSNumericEdit	m_BlinkOffTime;
	CSmartEdit		m_OBLevelText;
	CLinkSlider		m_OBLevel;
	CButton			m_OBClear;
	CButton			m_FlightData;
	CButton			m_ClearData;
	CButton			m_FlightBlinkData;
	CButton			m_ClearBlinkData;
	CButton			m_ClearMask;
	CButton			m_ClearBlinkMask;
	//}}AFX_DATA

private:
	UCHAR			m_BitType;
	CButtonSSL		m_LampTest, m_Invert;
	CVarListDlg		m_VarListDlg;
	CStatic			m_Data, m_Mask, m_Blink, m_BlinkData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLatchedTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLatchedTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnLampTest();
	afx_msg void OnInvert();
	afx_msg void OnChangeOBLevel();
	afx_msg void OnChangeNotes();
	afx_msg void OnFlightData();
	afx_msg void OnClearData();
	afx_msg void OnSelchangeOpCombo();
	afx_msg void OnSelchangeOpCombo2();
	afx_msg void OnSelchangeOpCombo3();
	afx_msg void OnChangeOpValue();
	afx_msg void OnChangeOpValue2();
	afx_msg void OnChangeHexMask();
	afx_msg void OnFlightBlinkData();
	afx_msg void OnClearBlinkData();
	afx_msg void OnChangeBlinkMask();
	afx_msg void OnChangeOnTime();
	afx_msg void OnChangeOffTime();
	afx_msg void OnMaskdata();
	afx_msg void OnBlinkMaskData();
	afx_msg void OnClearMask();
	afx_msg void OnClearBlinkMask();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LATCHEDTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
