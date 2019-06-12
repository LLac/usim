// DotMXTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTMXTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_DOTMXTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "DotMXTab.h"
#include "DotMXFlagsTab.h"
#include "DotMXNotesTab.h"

class CDotMXTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CDotMXTabCtrl();
	virtual ~CDotMXTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_DMX, m_DMX_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;

	CImageList		m_ImageList;
	BOOL			m_Init;
	CDotMXNotesTab	m_NotesTab;
	CDotMXTab		m_DotMXTab;
	CDotMXFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDotMXTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DOTMXTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
