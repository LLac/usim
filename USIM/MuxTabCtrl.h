// OutputTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUXTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_MUXTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "MuxTab.h"
#include "MuxFlagsTab.h"
#include "MuxNotesTab.h"

class CMuxTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CMuxTabCtrl();
	virtual ~CMuxTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_MUX, m_MUX_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	CMuxNotesTab	m_NotesTab;
	CMuxTab			m_MuxTab;
	CMUXFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMuxTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MUXTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
