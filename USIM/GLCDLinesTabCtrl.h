// GLCDLinesTabCtrl.h: interface for the DefPagesCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLCDLINESTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_GLCDLINESTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "GLCDLinesTab.h"
#include "CLCDPage.h"

class CGLCDLinesTabCtrl : public CXTabCtrl  
{

public:
	CGLCDLinesTabCtrl();
	virtual ~CGLCDLinesTabCtrl();
	void InitPages();

	CGLCDLinesTab	m_LCDLinesTab;

protected:
	int				m_PAGE, m_PAGE_P, m_PAGE_DIS, m_PAGE_DIS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CGLCDLinesTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CGLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
