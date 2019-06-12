#if !defined(AFX_REPLACEDEVICE_H__2B3FC65B_6A3A_49FF_9E49_A50B1906CAE6__INCLUDED_)
#define AFX_REPLACEDEVICE_H__2B3FC65B_6A3A_49FF_9E49_A50B1906CAE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplaceDevice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReplaceDevice dialog

class CReplaceDevice : public CDialog
{
// Construction
public:
	CReplaceDevice(CWnd* pParent = NULL);   // standard constructor
	POSITION	m_ReplacePos;

// Dialog Data
	//{{AFX_DATA(CReplaceDevice)
	enum { IDD = IDD_REPLACE };
	CComboBox	m_PickList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplaceDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReplaceDevice)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeOpCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLACEDEVICE_H__2B3FC65B_6A3A_49FF_9E49_A50B1906CAE6__INCLUDED_)
