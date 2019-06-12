// OutputTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_RWRTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "RWRTab.h"
#include "RWRFlagsTab.h"
#include "RWRNotesTab.h"

class CRWRTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CRWRTabCtrl();
	virtual ~CRWRTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_RWR, m_RWR_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	CRWRNotesTab	m_NotesTab;
	CRWRTab			m_RWRTab;
	CRWRFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CRWRTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RWRTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
