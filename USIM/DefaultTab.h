#if !defined(AFX_DEFAULTTAB_H__83C66988_66ED_4A09_B06C_ADF3A7C7E584__INCLUDED_)
#define AFX_DEFAULTTAB_H__83C66988_66ED_4A09_B06C_ADF3A7C7E584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefaultTab.h : header file
//

#include "CEAdvButton\CEAdvButton.h"
#include "ReadOnlyEdit\ReadOnlyEdit.h"
#include "ButtonSSL\ButtonSSL.h"
//#include "ResizableLib\ResizableChild.h"
#include "ResizableLib\ResizableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDefaultTab dialog
class CDefTabCtrl;

class CDefaultTab : public CResizableDialog
{
	friend class CTreeDevices;

// Construction
public:
	CDefaultTab(CWnd* pParent = NULL);   // standard constructor
	void SimUpdate();
	void UpdateWindow();
	void OnHelpButton();

	CDefTabCtrl*	m_pTab;

// Global Variables
public:

// Dialog Data
	//{{AFX_DATA(CDefaultTab)
	enum { IDD = IDD_DEFAULT };
	CButton	m_AutoUpdate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefaultTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CButton			m_Refresh, m_Web, m_Help, m_Update;
	CReadOnlyEdit	m_Status;
	CButton			m_Disable;
	CButton			m_AutoConnect;
	CButton			m_AutoStart;
	CButton			m_Minimized;

	// Generated message map functions
	//{{AFX_MSG(CDefaultTab)
	afx_msg void OnRefreshButton();
	afx_msg void OnUpdateButton();
	afx_msg void OnWebButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnDisable();
	afx_msg void OnAutoConnect();
	afx_msg void OnMinimized();
	afx_msg void OnPaint();
	afx_msg void OnAutoupdate();
	afx_msg void OnAutostart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFAULTTAB_H__83C66988_66ED_4A09_B06C_ADF3A7C7E584__INCLUDED_)

