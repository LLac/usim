// InputsTabCtrl.h: interface for the InputsTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTSTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
#define AFX_INPUTSTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTabCtrl\XTabCtrl.h"
#include "NotesTab.h"
#include "InputFlagTab.h"
#include "SendKeysTab.h"
#include "VariablesTab.h"

class CInputsTabCtrl : public CXTabCtrl  
{
	friend CTreeThread;
	friend CKeyboardHook;
	friend CTreeDevices;

public:
	void InitTabs();
	CInputsTabCtrl();
	virtual ~CInputsTabCtrl();
	void InitAllTabs();

	int		m_NOTES, m_NOTES_P, m_COND, m_COND_P, m_SENDKEYS, m_SENDKEYS_P;
	int		m_IOFLAGS, m_IOFLAGS_P, m_VARS, m_VARS_P;

protected:
	BOOL			m_Init;
	CImageList		m_ImageList;
	CNotesTab		m_NotesTab;
	CInputFlagTab	m_InputFlagTab;
	CSendKeysTab	m_SendKeysTab;
	CVariablesTab	m_VariablesTab;

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputsTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_INPUTSTABCTRL_H__C0CD0040_3F45_4214_8641_6BFD24571E9F__INCLUDED_)
