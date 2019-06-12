// CLCDLinesTabCtrl.h: interface for the DefPagesCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLCDLINESTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_CLCDLINESTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "CLCDLinesTab.h"
#include "CLCDPage.h"

class CCLCDLinesTabCtrl : public CXTabCtrl  
{

public:
	CCLCDLinesTabCtrl();
	virtual ~CCLCDLinesTabCtrl();
	void InitPages();

	CCLCDLinesTab	m_LCDLinesTab;

protected:
	int				m_PAGE, m_PAGE_P, m_PAGE_DIS, m_PAGE_DIS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCLCDLinesTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CCLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
