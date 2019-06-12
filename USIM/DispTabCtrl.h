// DispTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_DISPTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "DispTab.h"
#include "DispFlagsTab.h"
#include "DispNotesTab.h"

class CDispTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CDispTabCtrl();
	virtual ~CDispTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_DISP, m_DISP_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;;

	CImageList		m_ImageList;
	BOOL			m_Init;
	CDispNotesTab	m_NotesTab;
	CDispFlagsTab	m_FlagsTab;
	CDispTab		m_DispTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDispTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DISPTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
