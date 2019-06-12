// OutputTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_OUTPUTTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "OutputTab.h"
#include "OutputFlagsTab.h"
#include "OutputNotesTab.h"

class COutputTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	COutputTabCtrl();
	virtual ~COutputTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_OUTPUT, m_OUTPUT_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	COutputNotesTab	m_NotesTab;
	COutputTab		m_OutputTab;
	COutputFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OUTPUTTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
