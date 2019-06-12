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
// ce: the item edit uses input mask for proper handle of various datatypes
// ce: the item edit creates itself automatically on clicked, if it is not yet create -> this is for performance
//     reasons.

#ifndef OT_ITEMEDIT
#define OT_ITEMEDIT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemEdit.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemEdit window

class COptionTreeItemEdit : public CEditMask, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void SetOption(DWORD dwOption, BOOL bSet);

	virtual void SetEditLong(long lValue);
	virtual void SetEditDword(DWORD dwValue);
	virtual void SetEditFloat(float fValue);
	virtual void SetEditInt(int nValue);
	virtual void SetEditDouble(double dValue);
	BOOL PrepareEditItem(const CString& sWindowText="",
                              DWORD dwOptions=0, 
                              DWORD dwAddStyle = 0,
                              const CString& inputmask="");

  BOOL CreateEditItem(DWORD dwOptions, 
                      DWORD dwAddStyle = 0,
                      const CString& inputmask="");

  ///
  /// Attention: Use this instead of SetWindowText
  ///
  virtual void    SetEditText(const CString& sText);

  virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
  virtual void OnShow(BOOL bShow);
  virtual CString GetAttributeText(void);

	BOOL GetEditDword(DWORD &dwReturn);
	BOOL GetEditInt(int &nReturn);
	BOOL GetEditLong(long &lReturn);
	BOOL GetEditDouble(double &dReturn);
	BOOL GetEditFloat(float &fReturn);
	virtual ~COptionTreeItemEdit();

protected:
  BOOL PreTranslateMessage(MSG* pMsg) ;

	// Generated message map functions
protected:
	BOOL IsStringNumeric(CString strString);
	//{{AFX_MSG(COptionTreeItemEdit)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void OptionsChanges();
	BOOL m_bFocus;
  // these members store creation params
  DWORD   _dwExStyleBuffer;
  CString _InputMaskBuffer;
  CString _WindowTextBuffer;
public:
  afx_msg void OnEnChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !OT_ITEMEDIT
