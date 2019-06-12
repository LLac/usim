// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code, this includes 
// commercial libraries and anything else for profit.
//
// I would appreciate a notification of any bugs or bug fixes to help the control grow.
//
// History:
// --------
//	See License.txt for full history information.
//
//
// Copyright (c) 1999-2002 
// ComputerSmarts.net 
// mattrmiller@computersmarts.net
//////////////////////////////////////////////////////////////////////////////////////
// hb/ec-logic
// Create an edit item with a button to launch a dialogbox for the edit field
// Copied source from COptionTreeItemEdit and added some code for the button
//////////////////////////////////////////////////////////////////////////////////////

#ifndef OT_ITEMEDITBUTTON
#define OT_ITEMEDITBUTTON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemEditButton.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeItemEdit.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemEditButton window

class COptionTreeItemEditButton : public COptionTreeItemEdit
{
// Construction
public:
	COptionTreeItemEditButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemEditButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOption(DWORD dwOption, BOOL bSet);
	void SetEditLong(long lValue);
	void SetEditDword(DWORD dwValue);
	void SetEditFloat(float fValue);
	void SetEditInt(int nValue);
	void SetEditDouble(double dValue);
	virtual BOOL CreateEditItem(  DWORD dwOptions, 
                                DWORD dwAddStyle = 0,
                                const CString& inputmask="");
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
  virtual void OnShow(BOOL bShow);

	BOOL GetEditDword(DWORD &dwReturn);
	BOOL GetEditInt(int &nReturn);
	BOOL GetEditLong(long &lReturn);
	BOOL GetEditDouble(double &dReturn);
	BOOL GetEditFloat(float &fReturn);
	virtual ~COptionTreeItemEditButton();

protected:
  BOOL PreTranslateMessage(MSG* pMsg) ;
	// Generated message map functions
protected:
	BOOL IsStringNumeric(CString strString);
	//{{AFX_MSG(COptionTreeItemEditButton)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void OptionsChanges();
	BOOL m_bFocus;
   CButton m_ctrlButton;
public:
  afx_msg void OnEnChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !OT_ITEMEDITBUTTON
