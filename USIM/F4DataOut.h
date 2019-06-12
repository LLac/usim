#if !defined(AFX_F4DATAOUT_H__A6053F69_8383_46F7_9BD7_BFEF14026D73__INCLUDED_)
#define AFX_F4DATAOUT_H__A6053F69_8383_46F7_9BD7_BFEF14026D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// F4DataOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CF4DataOut dialog

class CF4DataOut : public CDialog
{
// Construction
public:
	CF4DataOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CF4DataOut)
	enum { IDD = IDD_DATAOUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CF4DataOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrl	m_DataList;
	UINT		m_nTimer;
	void		OnStartTimer();
	void		OnStopTimer();
	void		DisplayData(); 

	// Generated message map functions
	//{{AFX_MSG(CF4DataOut)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_F4DATAOUT_H__A6053F69_8383_46F7_9BD7_BFEF14026D73__INCLUDED_)
