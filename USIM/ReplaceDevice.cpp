// ReplaceDevice.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "ReplaceDevice.h"
#include "TreeDevices.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplaceDevice dialog


CReplaceDevice::CReplaceDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplaceDevice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReplaceDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplaceDevice)
	DDX_Control(pDX, IDC_OP_COMBO, m_PickList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplaceDevice, CDialog)
	//{{AFX_MSG_MAP(CReplaceDevice)
	ON_CBN_SELCHANGE(IDC_OP_COMBO, OnSelchangeOpCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplaceDevice message handlers

void CReplaceDevice::OnOK() 
{

	theApp.m_pTreeDevices->m_ReplacePos = m_ReplacePos;

	CDialog::OnOK();
}

void CReplaceDevice::OnCancel() 
{
	// TODO: Add extra cleanup here
	theApp.m_pTreeDevices->m_ReplacePos = NULL;
	CDialog::OnCancel();
}

BOOL CReplaceDevice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	int i, nItem;
	HTREEITEM hItem = theApp.m_pTreeDevices->m_CurrenthItem;
	int nIndex = theApp.m_pTreeDevices->m_CurrentIndex;

	i = 0;
		
	m_PickList.ResetContent();
	m_ReplacePos = NULL;

	POSITION pos = theApp.m_DevArrayList.GetHeadPosition();
	while (pos != NULL) {
		i = theApp.m_DevArrayList.GetNext(pos);
		if (theApp.m_pDevArray[i]->m_hItem != hItem) {
			if (theApp.m_pDevArray[i]->m_DeviceParam.VendorID == theApp.m_pDevArray[nIndex]->m_DeviceParam.VendorID &&
				theApp.m_pDevArray[i]->m_DeviceParam.ProductID == theApp.m_pDevArray[nIndex]->m_DeviceParam.ProductID &&
				theApp.m_pDevArray[i]->m_DeviceParam.ConfigFlag == theApp.m_pDevArray[nIndex]->m_DeviceParam.ConfigFlag) {
				
				// add to pick list
				str.Format("%s : %s", theApp.m_pDevArray[i]->m_DeviceParam.DeviceName, theApp.m_pDevArray[i]->m_DeviceParam.SerialNum);
				nItem = m_PickList.AddString(str);
				if (nItem >= 0)
					m_PickList.SetItemDataPtr(nItem, theApp.m_DevArrayList.Find(i));
			}
		}
	}
	
	if (m_PickList.GetCount() == 0) {
		nItem = m_PickList.AddString(_T("No Matching Devices Found"));
		if (nItem >= 0) {
			m_PickList.SetItemDataPtr(nItem, NULL);
			m_PickList.SetCurSel(nItem);
			m_PickList.EnableWindow(FALSE); 
			
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReplaceDevice::OnSelchangeOpCombo() 
{
	// TODO: Add your control notification handler code here
	m_ReplacePos = (POSITION)m_PickList.GetItemDataPtr(m_PickList.GetCurSel());
}
