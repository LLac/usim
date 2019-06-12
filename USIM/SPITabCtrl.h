// SPITabCtrl.h: interface for the DefTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPITABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_SPITABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "SPITab.h"
#include "SPIFlagsTab.h"
#include "SPINotesTab.h"

class CSPITabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CTreeDevices;

public:
	CSPITabCtrl();
	virtual ~CSPITabCtrl();
	void InitTabs();
	void InitAllTabs();

	int m_SPI, m_SPI_P, m_NOTES, m_NOTES_P, m_FLAGS, m_FLAGS_P;

	CImageList		m_ImageList;
	BOOL			m_Init;
	CSPINotesTab	m_NotesTab;
	CSPITab			m_SPITab;
	CSPIFlagsTab	m_FlagsTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSPITabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SPITABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
