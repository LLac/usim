// STEPTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEPTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_STEPTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "STEPTab.h"
#include "STEPFlagsTab.h"
#include "STEPNotesTab.h"

class CSTEPTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CSTEPTabCtrl();
	virtual ~CSTEPTabCtrl();
	void InitTabs();
	void InitAllTabs();

	int m_STEP, m_STEP_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;

	CImageList		m_ImageList;
	BOOL			m_Init;
	CSTEPNotesTab	m_NotesTab;
	CSTEPTab		m_STEPTab;
	CSTEPFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSTEPTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_STEPTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
