// DEFTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_DEFTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "DefaultTab.h"
#include "SimPathTab.h"

class CDefTabCtrl : public CXTabCtrl  
{
public:
	CDefTabCtrl();
	virtual ~CDefTabCtrl();
	void InitTabs();

	CDefaultTab		m_DefaultTab;
	CSimPathTab		m_SimPathTab;

protected:
	BOOL			m_Init;
	CImageList		m_ImageList;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDefTabCtrl)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DEFTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
