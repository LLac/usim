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

#ifndef OT_ITEMFONT
#define OT_ITEMFONT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemFont.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"
#include "OptionTreeFontSel.h"

///////////////////////////////
// CDefFont
///////////////////////////////
class CDefFont : public LOGFONT
{
  public:
    CDefFont(const CString& sFaceName="Arial",const LONG height=12);
};
///////////////////////////////
// COptionTreeItemFont
///////////////////////////////
class COptionTreeItemFont : public COptionTreeItem
{
protected:
  static CDefFont __lfDefault;

public:
	COptionTreeItemFont();
	void SetApplyWindow(CWnd *pWnd);
	void SetCustomSample(const CString& strSample);
	COLORREF GetDefaultTextColor();
	COLORREF GetTextColor();
	void SetTextColor(COLORREF crColor);
	void SetCurFont(LOGFONT* plf);
	BOOL CreateFontItem(LOGFONT* plfFont, COLORREF crFontColor, DWORD dwOptions=OT_FS_USEDEFAULT);
	virtual ~COptionTreeItemFont();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
  virtual void OnShow(BOOL bShow){;}

  void  GetCurFont(LOGFONT& lf);

protected:
	COLORREF m_crDefTextColor;
	COLORREF m_crTextColor;
	LOGFONT m_lfFont;
  CFont   _Font;
	CString m_strCustomSample;
	CWnd *m_pApplyWindow;
	long m_lDefaultHeight;
  
};

#endif // !OT_ITEMFONT
