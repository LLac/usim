#if !defined(AFX_CALSPI_H__A00E0E58_7AA2_4B83_8BE7_D619166161EA__INCLUDED_)
#define AFX_CALSPI_H__A00E0E58_7AA2_4B83_8BE7_D619166161EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalSPI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalSPI dialog

class CCalSPI : public CDialog
{
// Construction
public:
	CCalSPI(CWnd* pParent = NULL);   // standard constructor

	long	*m_pTemp;
	UCHAR	*m_pCalibrate;
	long	*m_pMin;
	long	*m_pMax;
	long	*m_pOffset;

	long	m_ResMin;
	long	m_ResMax;

// Dialog Data
	//{{AFX_DATA(CCalSPI)
	enum { IDD = IDD_CALSPI };
	CStatic	m_MinText;
	CStatic	m_MaxText;
	CButton	m_OK;
	CButton	m_Cancel;
	CLinkSlider	m_MinSlider;
	CLinkSlider	m_MaxSlider;
	CLinkSlider	m_OffsetSlider;
	CSmartEdit	m_MinValue;
	CSmartEdit	m_MaxValue;
	CSmartEdit	m_OffsetValue;
	CButtonSSL	m_CalMin;
	CButtonSSL	m_CalMax;
	CButtonSSL	m_CalOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalSPI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalSPI)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCalibrateMin();
	afx_msg void OnCalibrateMax();
	afx_msg void OnCalibrateOffset();
	afx_msg void OnChangeOffset();
	afx_msg void OnChangeMin();
	afx_msg void OnChangeMax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALSPI_H__A00E0E58_7AA2_4B83_8BE7_D619166161EA__INCLUDED_)
