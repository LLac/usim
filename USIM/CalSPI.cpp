// CalSPI.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "CalSPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalSPI dialog


CCalSPI::CCalSPI(CWnd* pParent /*=NULL*/)
	: CDialog(CCalSPI::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalSPI)
	//}}AFX_DATA_INIT
}


void CCalSPI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalSPI)
	DDX_Control(pDX, IDC_STATIC1, m_MinText);
	DDX_Control(pDX, IDC_STATIC2, m_MaxText);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_SLIDER1, m_MinSlider);
	DDX_Control(pDX, IDC_SLIDER2, m_MaxSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_OffsetSlider);
	DDX_Control(pDX, IDC_MIN, m_MinValue);
	DDX_Control(pDX, IDC_MAX, m_MaxValue);
	DDX_Control(pDX, IDC_OFFSET, m_OffsetValue);
	DDX_Control(pDX, IDC_CALIBRATE_MIN, m_CalMin);
	DDX_Control(pDX, IDC_CALIBRATE_MAX, m_CalMax);
	DDX_Control(pDX, IDC_CALIBRATE_OFFSET, m_CalOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalSPI, CDialog)
	//{{AFX_MSG_MAP(CCalSPI)
	ON_BN_CLICKED(IDC_CALIBRATE_MIN, OnCalibrateMin)
	ON_BN_CLICKED(IDC_CALIBRATE_MAX, OnCalibrateMax)
	ON_BN_CLICKED(IDC_CALIBRATE_OFFSET, OnCalibrateOffset)
	ON_EN_CHANGE(IDC_OFFSET, OnChangeOffset)
	ON_EN_CHANGE(IDC_MIN, OnChangeMin)
	ON_EN_CHANGE(IDC_MAX, OnChangeMax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalSPI message handlers

BOOL CCalSPI::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString str;
	str.Format("%d", m_ResMax);
	m_MaxText.SetWindowText (str);
	str.Format("%d", m_ResMin);
	m_MinText.SetWindowText (str);

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

	m_OffsetValue.SetSlideLink( this, IDC_SLIDER3 );
	m_OffsetValue.SetParams(m_ResMin, m_ResMax, 10);
	m_OffsetValue.SetValue(*m_pOffset);
	m_OffsetValue.EnableWindow(false);
	m_OffsetSlider.EnableWindow(false);
	
	m_MinValue.SetSlideLink(this, IDC_SLIDER1 );
	m_MinValue.SetParams(m_ResMin, m_ResMax, 10);
	m_MinValue.SetValue(*m_pMin);
	m_MinValue.EnableWindow(false);
	m_MinSlider.EnableWindow(false);

	m_MaxValue.SetSlideLink( this, IDC_SLIDER2 );
	m_MaxValue.SetParams(m_ResMin, m_ResMax, 10);
	m_MaxValue.SetValue(*m_pMax);
	m_MaxValue.EnableWindow(false);
	m_MaxSlider.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalSPI::OnCalibrateMin() 
{
	m_CalMax.SetCheck(false);
	m_CalOffset.SetCheck(false);
	m_MaxSlider.EnableWindow(false);
	m_MaxValue.EnableWindow(false);
	m_OffsetSlider.EnableWindow(false);
	m_OffsetValue.EnableWindow(false);

	if (m_CalMin.GetCheck()) {
		m_MinSlider.EnableWindow(true);
		m_MinValue.EnableWindow(true);
		*m_pTemp = (long)(m_MinSlider.GetPos() + m_OffsetSlider.GetPos());
		*m_pCalibrate = 1;
	} else {
		m_MinSlider.EnableWindow(false);
		m_MinValue.EnableWindow(false);
		*m_pCalibrate = 0;
		*m_pTemp = 0;
	}
}

void CCalSPI::OnCalibrateMax() 
{
	m_CalMin.SetCheck(false);
	m_CalOffset.SetCheck(false);
	m_MinSlider.EnableWindow(false);
	m_MinValue.EnableWindow(false);
	m_OffsetSlider.EnableWindow(false);
	m_OffsetValue.EnableWindow(false);

	if (m_CalMax.GetCheck()) {
		m_MaxSlider.EnableWindow(true);
		m_MaxValue.EnableWindow(true);
		*m_pTemp = (long)(m_MaxSlider.GetPos() + m_OffsetSlider.GetPos());
		*m_pCalibrate = 1;
	} else {
		m_MaxSlider.EnableWindow(false);
		m_MaxValue.EnableWindow(false);
		*m_pCalibrate = 0;
		*m_pTemp = 0;
	}
}

void CCalSPI::OnCalibrateOffset() 
{
	m_CalMin.SetCheck(false);
	m_CalMax.SetCheck(false);
	m_MinSlider.EnableWindow(false);
	m_MinValue.EnableWindow(false);
	m_MaxSlider.EnableWindow(false);
	m_MaxValue.EnableWindow(false);

	if (m_CalOffset.GetCheck()) {
		m_OffsetSlider.EnableWindow(true);
		m_OffsetValue.EnableWindow(true);
		*m_pTemp = (long)m_OffsetSlider.GetPos();
		*m_pCalibrate = 1;
	} else {
		m_OffsetSlider.EnableWindow(false);
		m_OffsetValue.EnableWindow(false);
		*m_pCalibrate = 0;
		*m_pTemp = 0;
	}
}

void CCalSPI::OnOK() 
{
	CString str;

	m_MinValue.GetWindowText(str);
	*m_pMin = atol(str);

	m_MaxValue.GetWindowText(str);
	*m_pMax = atol(str);

	m_OffsetValue.GetWindowText(str);
	*m_pOffset = atol(str);

	*m_pCalibrate = 0;
	*m_pTemp = 0;

	CDialog::OnOK();
}

void CCalSPI::OnCancel() 
{
	*m_pCalibrate = 0;
	*m_pTemp = 0;
	
	CDialog::OnCancel();
}


void CCalSPI::OnChangeOffset() 
{
	OnCalibrateOffset();
}

void CCalSPI::OnChangeMin() 
{
	OnCalibrateMin();
}

void CCalSPI::OnChangeMax() 
{
	OnCalibrateMax();
}
