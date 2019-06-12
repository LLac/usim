#if !defined(AFX_VARLISTDLG_H__7000E559_7802_4D2D_AACC_D6DF7F343EED__INCLUDED_)
#define AFX_VARLISTDLG_H__7000E559_7802_4D2D_AACC_D6DF7F343EED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VarListDlg.h : header file
//

//#include "COptionTree\OptionTree.h"
#include "libtree\OptionTree.h"
#include "ResizableLib\ResizableDialog.h"
#include "amsEdit\amsEdit.h"

#define TREEDEPTH			8
#define	VARTYPE_VAR			0
#define	VARTYPE_CMD			1
#define	VARTYPE_VAR_ONLY	2
#define VARTYPE_REALTIME	3
#define VARTYPE_BITS		4

/////////////////////////////////////////////////////////////////////////////
// CVarListDlg dialog

class CVarListDlg : public CResizableDialog
{
// Construction
public:
	CVarListDlg(CWnd* pParent = NULL);   // standard constructor
	CString	m_VarTokenName;
	UINT	m_SimType;
	UINT	m_DataType;
	UINT	m_ArrayIndex;
	UINT	m_ListTypeVar;
	UINT	m_BitMask;
	UINT	m_BitType;
	int		m_nTimer;
	int		m_OldIndex;
	UINT	m_VarArrayIndex;

// Dialog Data
	//{{AFX_DATA(CVarListDlg)
	enum { IDD = IDD_VARSLIST };
	CButton		m_Cancel;
	CButton		m_OK;
	CStatic		m_ValueText;
	CAMSEdit	m_SearchText;
	CButton		m_ClearData;
	CButton		m_Radio1;
	CButton		m_Radio2;
	CButton		m_Radio3;
	CButton		m_Search;
	CButton		m_Reset;
	CEdit		m_EVarArrayIndex;
	CStatic		m_IndexMinMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarListDlg)
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillVarList();
	void FillCmdList();
	void FillBitsList();
	void SearchItem(CString);

	COptionTree m_otTree;
	COptionTreeItem* m_pSearchOffset;

	// Generated message map functions
	//{{AFX_MSG(CVarListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnClearData();
	virtual void OnCancel();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnBtSearchItem();
	afx_msg void OnBtResetSearch();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeMinMax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARLISTDLG_H__7000E559_7802_4D2D_AACC_D6DF7F343EED__INCLUDED_)
