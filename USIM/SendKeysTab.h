#if !defined(AFX_SENDKEYSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_SENDKEYSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendKeysTab.h : header file
//
#include "GridCtrl_src\GridCtrl.h"
#include "ButtonSSL\ButtonSSL.h"
#include "CEAdvButton\CEAdvButton.h"
#include "KeyboardHook.h"
#include "SendKeysPage.h"

#define SK_NUM_ROWS	9
#define SK_NUM_COLS	7

#define PRESSGRID		0
#define RELEASEGRID		1
/////////////////////////////////////////////////////////////////////////////
// CSendKeysTab dialog
class CXTabCtrl;
class CSendKeysTab : public CResizableDialog
{
	friend CTreeDevices;

// Construction
public:
	CSendKeysTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSendKeysTab();
	void	RecordKey(tagKeyResultA*, CString);
	void	InitSendKeysTab();
	void	KeyMacroRecord(BOOL);
	void	SetIOStatus();
	void	GridOnSize();

	CXTabCtrl*	m_pTab;
	CStatic		m_IOName, m_Enable;
	CEAdvButton	m_SendKeysGroup;

// Dialog Data
	//{{AFX_DATA(CSendKeysTab)
	enum { IDD = IDD_SEND_KEYS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	void		OnAddButton(CGridCtrl *);
	void		OnRemoveButton(CGridCtrl *);
	void		OnClear();
	void		OnClearGridRow(CGridCtrl *);
	void		CreateGrid(CGridCtrl *);
	void		OnGridClick(CGridCtrl *, NMHDR *);
	void		OnGridEndEdit(CGridCtrl *, NMHDR *, LRESULT*);
	void		SaveToSendKeysArray(CGridCtrl *, TAGKEYMACRO *, UINT nRow = NULL);
	void		LoadData(CGridCtrl *, TAGKEYMACRO *, UINT);
	void		RecordData(CGridCtrl *, TAGKEYMACRO *, tagKeyResultA*, CString);

	CImageList	m_ImageList;
	CButton		m_P_Add, m_P_Remove, m_P_Clear, m_R_Add, m_R_Remove, m_R_Clear;
	CButton		m_HoldKey;
	CStatic		m_P_Frame, m_R_Frame;
	CButton		m_Repeat, m_Clear;
	CButtonSSL	m_PressRec, m_ReleaseRec;
	CEdit		m_Notes;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendKeysTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CGridCtrl		m_PressGrid;
	CGridCtrl		m_ReleaseGrid;
	CKeyboardHook	m_KeyHook;
//	TAGKEYMACRO		m_OPKeyResults[MAX_MACROS], m_ORKeyResults[MAX_MACROS];

	// Generated message map functions
	//{{AFX_MSG(CSendKeysTab)
	afx_msg void OnHoldKey();
	afx_msg void OnPAdd();
	afx_msg void OnPRemove();
	afx_msg void OnPClear();
	afx_msg void OnRAdd();
	afx_msg void OnRRemove();
	afx_msg void OnRClear();
	virtual BOOL OnInitDialog();
	afx_msg void OnRepeat();
	afx_msg void OnClearButton();
	afx_msg void OnRecord();
	afx_msg void OnChangeNotes();
	afx_msg void OnPGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnPGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRGridSize(tagNMHDR *,long *);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSKEYSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
