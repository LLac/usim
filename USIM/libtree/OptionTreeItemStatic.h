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


#ifndef OT_ITEMSTATIC
#define OT_ITEMSTATIC

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added Headers
#include "CommonRes.h"
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Classes
class COptionTree;

class COptionTreeItemStatic : public COptionTreeItem
{
public:
	BOOL CreateStaticItem(DWORD dwOptions);
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnShow(BOOL bShow){;}
	virtual CString GetAttributeText(void);

	void SetStaticText(CString strStaticText);
	CString GetStaticText();
	COptionTreeItemStatic();
	virtual void AddIcon (HICON hIcon){m_hIcon = hIcon;}; // hb - macro to add icon
	virtual void AddBitmap (CBitmap *pBitmap){m_pBitmap = pBitmap;}; // hb - macro to add icon
	virtual ~COptionTreeItemStatic();

	void SetStaticDataIndex(UINT iIndex);
	UINT GetStaticDataIndex();

	void SetStaticListType(UINT iType);
	UINT GetStaticListType();

protected:
	CString m_strStaticText;
	HICON m_hIcon; // hb - added an icon 
	CBitmap *m_pBitmap; // hb - added a bitmap
   	UINT	m_DataIndex;
	UINT	m_ListType;
};

#endif // !OT_ITEMSTATIC
