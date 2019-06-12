#if !defined(AFX_SPIFLAGSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
#define AFX_SPIFLAGSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SPIFlagsTab.h : header file
//

#include "FlagsPage.h"
#include "FlagsTab.h"

/////////////////////////////////////////////////////////////////////////////
// CSPIFlagsTab dialog

class CXTabCtrl;
class CSPIFlagsTab : public CFlagsTab
{
// Construction
	friend CTreeDevices;

public:
	CSPIFlagsTab(CWnd* pParent = NULL);
	CXTabCtrl*	m_pTab;
	void SetTreeItemDesc();

// Dialog Data
	//{{AFX_DATA(CSPIFlagsTab)
	enum { IDD = IDD_OUTPUT_FLAGS };
	//}}AFX_DATA

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPIFlagsTab)
	public:
	virtual void InitFlagsTab();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void InitVars();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSPIFlagsTab)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPIFLAGSTAB_H__7C617D2B_1550_47B2_A63A_5B4775C526D1__INCLUDED_)
