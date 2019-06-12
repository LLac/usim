// CLCDTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_CLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "CLCDTab.h"
#include "CLCDFlagsTab.h"
#include "CLCDNotesTab.h"

class CCLCDTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CCLCDTabCtrl();
	virtual ~CCLCDTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_CLCD, m_CLCD_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	CCLCDNotesTab	m_NotesTab;
	CCLCDTab		m_CLCDTab;
	CCLCDFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCLCDTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
