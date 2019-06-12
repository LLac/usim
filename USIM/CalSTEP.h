#if !defined(AFX_CALSTEP_H__A00E0E58_7AA2_4B83_8BE7_D619166161EA__INCLUDED_)
#define AFX_CALSTEP_H__A00E0E58_7AA2_4B83_8BE7_D619166161EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalSTEP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalSTEP dialog

class CCalSTEP : public CDialog
{
// Construction
public:
	CCalSTEP(CWnd* pParent = NULL);   // standard constructor

	long	*m_pTemp;
	UCHAR	*m_pCalibrate;
	long	*m_pMin;
	long	*m_pMax;
	long	*m_pOffset;

	long	m_ResMin;
	long	m_ResMax;

// Dialog Data
	//{{AFX_DATA(CCalSTEP)
	enum { IDD = IDD_CALSTEP };
	CNumSpinCtrl	m_MaxSpin;
	CNumSpinCtrl	m_MinSpin;
	CNumSpinCtrl	m_OffsetSpin;
	CStatic	m_MinText;
	CStatic	m_MaxText;
	CButton	m_OK;
	CButton	m_Cancel;
	CEdit	m_MinValue;
	CEdit	m_MaxValue;
	CEdit	m_OffsetValue;
	CButtonSSL	m_CalMin;
	CButtonSSL	m_CalMax;
	CButtonSSL	m_CalOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalSTEP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalSTEP)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCalibrateMin();
	afx_msg void OnCalibrateMax();
	afx_msg void OnCalibrateOffset();
	afx_msg void OnKillfocusOffset();
	afx_msg void OnKillfocusMin();
	afx_msg void OnKillfocusMax();
	afx_msg void OnChangeOffset();
	afx_msg void OnChangeMin();
	afx_msg void OnChangeMax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALSTEP_H__A00E0E58_7AA2_4B83_8BE7_D619166161EA__INCLUDED_)
