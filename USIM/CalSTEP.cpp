// CalSTEP.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "CalSTEP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalSTEP dialog


CCalSTEP::CCalSTEP(CWnd* pParent /*=NULL*/)
	: CDialog(CCalSTEP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalSTEP)
	//}}AFX_DATA_INIT
}


void CCalSTEP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalSTEP)
	DDX_Control(pDX, IDC_SPIN3, m_MaxSpin);
	DDX_Control(pDX, IDC_SPIN2, m_MinSpin);
	DDX_Control(pDX, IDC_SPIN1, m_OffsetSpin);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_MIN, m_MinValue);
	DDX_Control(pDX, IDC_MAX, m_MaxValue);
	DDX_Control(pDX, IDC_OFFSET, m_OffsetValue);
	DDX_Control(pDX, IDC_CALIBRATE_MIN, m_CalMin);
	DDX_Control(pDX, IDC_CALIBRATE_MAX, m_CalMax);
	DDX_Control(pDX, IDC_CALIBRATE_OFFSET, m_CalOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalSTEP, CDialog)
	//{{AFX_MSG_MAP(CCalSTEP)
	ON_BN_CLICKED(IDC_CALIBRATE_MIN, OnCalibrateMin)
	ON_BN_CLICKED(IDC_CALIBRATE_MAX, OnCalibrateMax)
	ON_BN_CLICKED(IDC_CALIBRATE_OFFSET, OnCalibrateOffset)
	ON_EN_KILLFOCUS(IDC_OFFSET, OnKillfocusOffset)
	ON_EN_KILLFOCUS(IDC_MIN, OnKillfocusMin)
	ON_EN_KILLFOCUS(IDC_MAX, OnKillfocusMax)
	ON_EN_CHANGE(IDC_OFFSET, OnChangeOffset)
	ON_EN_CHANGE(IDC_MIN, OnChangeMin)
	ON_EN_CHANGE(IDC_MAX, OnChangeMax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalSTEP message handlers

BOOL CCalSTEP::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CalMin.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_CalMin.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_CalMin.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	CRect r;
	m_CalMin.GetWindowRect(r);
	ScreenToClient(r);
	m_CalMin.MoveWindow(r.left, r.top, 15, 15);
	m_CalMin.SetCheck(false);

	m_CalMax.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_CalMax.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_CalMax.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_CalMax.GetWindowRect(r);
	ScreenToClient(r);
	m_CalMax.MoveWindow(r.left, r.top, 15, 15);
	m_CalMax.SetCheck(false);

	m_CalOffset.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_CalOffset.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_CalOffset.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_CalOffset.GetWindowRect(r);
	ScreenToClient(r);
	m_CalOffset.MoveWindow(r.left, r.top, 15, 15);
	m_CalOffset.SetCheck(false);

	m_OffsetSpin.SetDecimalPlaces (0);
	m_OffsetSpin.SetTrimTrailingZeros (FALSE);
	m_OffsetSpin.SetRangeAndDelta (m_ResMin, m_ResMax, 1);
	m_OffsetSpin.SetPos (*m_pOffset);
	m_OffsetSpin.SetFormatString("%10.0f");
	m_OffsetSpin.EnableWindow(false);
	m_OffsetValue.EnableWindow(false);
	
	m_MinSpin.SetDecimalPlaces (0);
	m_MinSpin.SetTrimTrailingZeros (FALSE);
	m_MinSpin.SetRangeAndDelta (m_ResMin, m_ResMax, 1);
	m_MinSpin.SetPos (*m_pMin);
	m_MinSpin.SetFormatString("%10.0f");
	m_MinSpin.EnableWindow(false);
	m_MinValue.EnableWindow(false);

	m_MaxSpin.SetDecimalPlaces (0);
	m_MaxSpin.SetTrimTrailingZeros (FALSE);
	m_MaxSpin.SetRangeAndDelta (m_ResMin, m_ResMax, 1);
	m_MaxSpin.SetPos (*m_pMax);
	m_MaxSpin.SetFormatString("%10.0f");
	m_MaxSpin.EnableWindow(false);
	m_MaxValue.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalSTEP::OnCalibrateMin() 
{
	m_CalMax.SetCheck(false);
	m_CalOffset.SetCheck(false);
	m_MaxSpin.EnableWindow(false);
	m_MaxValue.EnableWindow(false);
	m_OffsetSpin.EnableWindow(false);
	m_OffsetValue.EnableWindow(false);

	if (m_CalMin.GetCheck()) {
		m_MinSpin.EnableWindow(true);
		m_MinValue.EnableWindow(true);
		*m_pTemp = (long)(m_MinSpin.GetPos() + m_OffsetSpin.GetPos());
		*m_pCalibrate = 1;
	} else {
		m_MinSpin.EnableWindow(false);
		m_MinValue.EnableWindow(false);
		*m_pCalibrate = 0;
		*m_pTemp = 0;
	}
}

void CCalSTEP::OnCalibrateMax() 
{
	m_CalMin.SetCheck(false);
	m_CalOffset.SetCheck(false);
	m_MinSpin.EnableWindow(false);
	m_MinValue.EnableWindow(false);
	m_OffsetSpin.EnableWindow(false);
	m_OffsetValue.EnableWindow(false);

	if (m_CalMax.GetCheck()) {
		m_MaxSpin.EnableWindow(true);
		m_MaxValue.EnableWindow(true);
		*m_pTemp = (long)(m_MaxSpin.GetPos() + m_OffsetSpin.GetPos());
		*m_pCalibrate = 1;
	} else {
		m_MaxSpin.EnableWindow(false);
		m_MaxValue.EnableWindow(false);
		*m_pCalibrate = 0;
		*m_pTemp = 0;
	}
}

void CCalSTEP::OnCalibrateOffset() 
{
	m_CalMin.SetCheck(false);
	m_CalMax.SetCheck(false);
	m_MinSpin.EnableWindow(false);
	m_MinValue.EnableWindow(false);
	m_MaxSpin.EnableWindow(false);
	m_MaxValue.EnableWindow(false);

	if (m_CalOffset.GetCheck()) {
		m_OffsetSpin.EnableWindow(true);
		m_OffsetValue.EnableWindow(true);
		*m_pTemp = (long)m_OffsetSpin.GetPos();
		*m_pCalibrate = 1;
	} else {
		m_OffsetSpin.EnableWindow(false);
		m_OffsetValue.EnableWindow(false);
		*m_pCalibrate = 0;
		*m_pTemp = 0;
	}
}

void CCalSTEP::OnKillfocusMax() 
{
	double val = m_MaxSpin.GetPos();
	m_MaxSpin.SetValueForBuddy( val );
}

void CCalSTEP::OnKillfocusMin() 
{
	double val = m_MinSpin.GetPos();
	m_MinSpin.SetValueForBuddy( val );
}

void CCalSTEP::OnKillfocusOffset() 
{
	double val = m_OffsetSpin.GetPos();
	m_OffsetSpin.SetValueForBuddy( val );
}

void CCalSTEP::OnOK() 
{
	CString str;

	OnKillfocusMin();
	m_MinValue.GetWindowText(str);
	*m_pMin = atol(str);

	OnKillfocusMax();
	m_MaxValue.GetWindowText(str);
	*m_pMax = atol(str);

	OnKillfocusOffset();
	m_OffsetValue.GetWindowText(str);
	*m_pOffset = atol(str);

	*m_pCalibrate = 0;
	*m_pTemp = 0;


	CDialog::OnOK();
}

void CCalSTEP::OnCancel() 
{
	*m_pCalibrate = 0;
	*m_pTemp = 0;
	
	CDialog::OnCancel();
}

void CCalSTEP::OnChangeOffset() 
{
	OnCalibrateOffset();	
}

void CCalSTEP::OnChangeMin() 
{
	OnCalibrateMin();	
}

void CCalSTEP::OnChangeMax() 
{
	OnCalibrateMax();	
}
