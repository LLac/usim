#if !defined(AFX_SPITAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_SPITAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SPITab.h : header file
//
#include "CalSPI.h"
#include "VarListDlg.h"

#define SPI_NUM_ROWS	9
#define SPI_NUM_COLS	8

/////////////////////////////////////////////////////////////////////////////
// CSPITab dialog
class CXTabCtrl;
class CSPITab : public CResizableDialog
{
// Construction
	friend CTreeDevices;

public:
	CSPITab(CWnd* pParent = NULL);   // standard constructor
	void InitSPITab();

	CXTabCtrl*	m_pTab;
	CEdit		m_Notes;
	CStatic		m_IOName;
	CStatic		m_Text3, m_Text4, m_Enable;
	CButton		m_OBClear;
	int			m_CurrentRow;

// Dialog Data
	//{{AFX_DATA(CSPITab)
	enum { IDD = IDD_SPIDAC };
	CButton	m_FlightData;
	CButton	m_ClearData;
	CButton	m_Remove;
	CButton	m_Add;
	//}}AFX_DATA

private:
	void OnClear();
	UINT AddRow();

	CButtonSSL		m_SPITest;
	CGridCtrl		m_Grid;
	CImageList		m_ImageList;
	CCalSPI			m_CalSPIDlg;
	CVarListDlg		m_VarListDlg;
	CStatic			m_Data;
	USHORT			m_Resolution;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPITab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSPITab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnSPITest();
	afx_msg void OnChangeNotes();
	afx_msg void OnFlightData();
	afx_msg void OnClearData();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPITAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
