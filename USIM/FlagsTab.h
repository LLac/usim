#if !defined(AFX_FLAGSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_FLAGSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlagsTab.h : header file
//

#include "FlagsPage.h"
#include "GridCtrl_src\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFlagsTab dialog

class CFlagsTab : public CResizableDialog
{
// Construction
public:
	CFlagsTab(UINT id, CWnd *pParent = NULL);
	void InitFlagsTab();
	
	CEdit		m_Notes;
	CStatic		m_IOName;
	CStatic		m_Enable;
	CButton		m_OBClear;

	CString		m_StrIO;
	CFlagsPage 	*m_pIOArray;
	BOOL		*m_pIOActive;
	BOOL		*m_pIODisabled;

// Dialog Data
	//{{AFX_DATA(CFlagsTab)
	enum { IDD = IDD_OUTPUT_FLAGS };
	//}}AFX_DATA

private:
	CGridCtrl		m_Grid;
	CButtonSSL		m_PowerFlag;
	CImageList		m_ImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlagsTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlagsTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearButton();
	afx_msg void OnChangeNotes();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnPowerFlag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLAGSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
