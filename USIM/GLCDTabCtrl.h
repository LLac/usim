// GLCDTabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_GLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "GLCDTab.h"
#include "GLCDFlagsTab.h"
#include "GLCDNotesTab.h"

class CGLCDTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CGLCDTabCtrl();
	virtual ~CGLCDTabCtrl();
	void InitTabs();
	void InitAllTabs();

protected:
	int m_GLCD, m_GLCD_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;
	CImageList		m_ImageList;
	BOOL			m_Init;
	CGLCDNotesTab	m_NotesTab;
	CGLCDTab		m_GLCDTab;
	CGLCDFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLCDTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GLCDTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
