// LatchedTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LATCHEDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_LATCHEDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "LatchedTab.h"
#include "LatchedFlagsTab.h"
#include "LatchedNotesTab.h"

class CLatchedTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CLatchedTabCtrl();
	virtual ~CLatchedTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int				m_LATCHED, m_LATCHED_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	CLatchedNotesTab	m_NotesTab;
	CLatchedTab		m_LatchedTab;
	CLatchedFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CLatchedTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LATCHEDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
