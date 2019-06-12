// OutputTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "OutputTab.h"
//#include "F4VarList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputTab dialog


COutputTab::COutputTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(COutputTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutputTab)
	//}}AFX_DATA_INIT
}

void COutputTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputTab)
	DDX_Control(pDX, IDC_MASKDATA, m_MaskData);
	DDX_Control(pDX, IDC_OUTPUTNAME, m_IOName);
	DDX_Control(pDX, IDC_GROUP1, m_OVGroup1);
	DDX_Control(pDX, IDC_GROUP2, m_OBGroup2);
	DDX_Control(pDX, IDC_OB_TEXT3, m_Text3);
	DDX_Control(pDX, IDC_OB_TEXT4, m_Text4);
	DDX_Control(pDX, IDC_STATIC8, m_Leveltxt);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_OP_COMBO, m_OPCombo1);
	DDX_Control(pDX, IDC_OP_COMBO2, m_OPCombo2);
	DDX_Control(pDX, IDC_OP_COMBO3, m_OPCombo3);
	DDX_Control(pDX, IDC_OP_VALUE, m_OPValue1);
	DDX_Control(pDX, IDC_OP_VALUE2, m_OPValue2);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_EDIT1, m_HexMask);
	DDX_Control(pDX, IDC_OB_LEVEL, m_OBLevelText);
	DDX_Control(pDX, IDC_OB_LEVEL_SLIDER, m_OBLevel);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_FLIGHTDATA, m_FlightData);
	DDX_Control(pDX, IDC_CLEARDATA, m_ClearData);
	DDX_Control(pDX, IDC_CLEARMASK, m_ClearMask);
	DDX_Control(pDX, IDC_TEXT1, m_Data);
	DDX_Control(pDX, IDC_TEXT2, m_Mask);
	DDX_Control(pDX, IDC_LAMPTEST, m_LampTest);
	DDX_Control(pDX, IDC_INVERT, m_Invert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutputTab, CResizableDialog)
	//{{AFX_MSG_MAP(COutputTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_LAMPTEST, OnLampTest)
	ON_BN_CLICKED(IDC_INVERT, OnInvert)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	ON_BN_CLICKED(IDC_FLIGHTDATA, OnFlightData)
	ON_BN_CLICKED(IDC_CLEARDATA, OnClearData)
	ON_CBN_SELCHANGE(IDC_OP_COMBO, OnSelchangeOpCombo)
	ON_CBN_SELCHANGE(IDC_OP_COMBO2, OnSelchangeOpCombo2)
	ON_CBN_SELCHANGE(IDC_OP_COMBO3, OnSelchangeOpCombo3)
	ON_EN_CHANGE(IDC_OP_VALUE, OnChangeOpValue)
	ON_EN_CHANGE(IDC_OP_VALUE2, OnChangeOpValue2)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeHexMask)
	ON_BN_CLICKED(IDC_MASKDATA, OnMaskdata)
	ON_BN_CLICKED(IDC_CLEARMASK, OnClearMask)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputTab message handlers

BOOL COutputTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_LampTest.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_LampTest.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_LampTest.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	CRect r;
	m_LampTest.GetWindowRect(r);
	ScreenToClient(r);
	m_LampTest.MoveWindow(r.left, r.top, 15, 15);

	m_Invert.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_Invert.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_Invert.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_Invert.GetWindowRect(r);
	ScreenToClient(r);
	m_Invert.MoveWindow(r.left, r.top, 15, 15);

	m_OBLevelText.SetSlideLink( this, IDC_OB_LEVEL_SLIDER );
	m_OBLevelText.SetParams( 0, 10, 10);
	m_OBLevelText.SetValue(10);
	m_OBLevelText.EnableWindow(false);
	m_OBLevel.EnableWindow(false);
	m_Text3.EnableWindow(false);
	m_Text4.EnableWindow(false);
	m_Leveltxt.EnableWindow(false);

	m_HexMask.SetHexColor(RGB(255,255,255),RGB(0,0,0));

	m_OVGroup1.AddControlToList (IDC_OP_VALUE);
	m_OVGroup1.AddControlToList (IDC_OP_COMBO2);
	m_OVGroup1.AddControlToList (IDC_OP_COMBO3);
	m_OVGroup1.AddControlToList (IDC_OP_VALUE2);

	m_OBGroup2.AddControlToList (IDC_MASKDATA);
	m_OBGroup2.AddControlToList (IDC_CLEARMASK);
	m_OBGroup2.AddControlToList (IDC_EDIT1);

	InitOutputTab();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_OUTPUTNAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_TEXT1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CLEARDATA, TOP_RIGHT);

	AddAnchor(IDC_TEXT2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CLEARMASK, TOP_RIGHT);

	AddAnchor(IDC_STATIC5, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC6, BOTTOM_LEFT);
	AddAnchor(IDC_STATIC7, BOTTOM_LEFT);
	AddAnchor(IDC_STATIC8, BOTTOM_LEFT);
	AddAnchor(IDC_LAMPTEST, BOTTOM_LEFT);
	AddAnchor(IDC_INVERT, BOTTOM_LEFT);
	AddAnchor(IDC_OB_LEVEL, BOTTOM_LEFT);
	AddAnchor(IDC_OB_TEXT3, BOTTOM_LEFT);
	AddAnchor(IDC_OB_LEVEL_SLIDER, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_OB_TEXT4, BOTTOM_RIGHT);

	AddAnchor(IDC_STATIC9, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutputTab::InitOutputTab() 
{
	int i;

	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("Output : "));
		m_Enable.SetWindowText(_T(""));
		m_Data.SetWindowText(_T(""));
		m_Mask.SetWindowText("");
		m_HexMask.SetValue(0, true);

		OnClearButton();
	} else {
		CString str, sPort, sID;

		// KLUDGE for PLV2
		sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
		sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem)));
		if (sPort.Find(sID) != -1) {
			str.Format(_T("%s %s : Output %s"), sID,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
				theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		} else {
			str.Format(_T("%s %s : %s - Output %s"), sID,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
				sPort,
				theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		}
		m_IOName.SetWindowText(str);
		m_Notes.SetWindowText(_T(""));
		
		// Fill in Sim Values
		str.Empty();
		switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_SimType) {
			case SIM_XPLANE:
				if (XPDataArray[theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenIndex].VarArraySize > 0) {
					str.Format("X-Plane/%s[%d]", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex + 1);
				} else {
					str.Format("X-Plane/%s", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName);
				}
				break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_F4USIM:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_SIMBIN:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_RF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_GPX:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_LFS:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case USIM_INPUT_FLAGS:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;
		}
		m_Data.SetWindowText(str);

		// Set all data from DevArray
		m_OPCombo1.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Condition1);
		m_OPCombo2.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Condition2);
		m_OPCombo3.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Condition3);

		m_OPValue1.SetDouble(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_OPValue1, true);
		m_OPValue2.SetDouble(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_OPValue2, true);

		m_HexMask.SetValue(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMask, true);
		int mask_end;
		_BITS_LIST const *pBitsArray;

		switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitType) {
			case LFSBITS:
				mask_end = ArraySize(LFSBitsArray);
				pBitsArray = LFSBitsArray;
				break;

			case GPXBITS:
				mask_end = ArraySize(GPXBitsArray);
				pBitsArray = GPXBitsArray;
				break;

			default:
				mask_end = ArraySize(F4BitsArray);
				pBitsArray = F4BitsArray;
				break;
		}

		for (i=0; i<mask_end; i++) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMask == pBitsArray[i].BitMask &&
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitType == pBitsArray[i].BitType) {
				m_Mask.SetWindowText(pBitsArray[i].TokenString);
			}
		}

		OnSelchangeOpCombo();

		m_LampTest.SetCheck(0);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_LampTest = 0;

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Invert == 1)
			m_Invert.SetCheck(1);
		else 
			m_Invert.SetCheck(0);
		
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputActive[theApp.m_CurItemIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputDisabled[theApp.m_CurItemIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Notes);
	}
}

void COutputTab::OnChangeHexMask() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMask = m_HexMask.GetValue();
}

void COutputTab::OnFlightData() 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	m_VarListDlg.m_ListTypeVar = VARTYPE_VAR_ONLY;

	m_VarListDlg.m_VarTokenName = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName;
	m_VarListDlg.m_VarArrayIndex = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex;
	int nResponse = m_VarListDlg.DoModal();

	if (nResponse == IDOK) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName = m_VarListDlg.m_VarTokenName;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenIndex = m_VarListDlg.m_ArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex = m_VarListDlg.m_VarArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_SimType = m_VarListDlg.m_SimType;

		CString str;
		switch (m_VarListDlg.m_SimType) {
			case SIM_XPLANE:
				if (XPDataArray[m_VarListDlg.m_ArrayIndex].VarArraySize > 0) {
					str.Format("X-Plane/%s[%d]", m_VarListDlg.m_VarTokenName, m_VarListDlg.m_VarArrayIndex + 1);
				} else {
					str.Format("X-Plane/%s", m_VarListDlg.m_VarTokenName);
				}
				break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				str = m_VarListDlg.m_VarTokenName;
				break;

			case SIM_F4USIM:
				str = m_VarListDlg.m_VarTokenName;
				break;

			case SIM_SIMBIN:
				str = m_VarListDlg.m_VarTokenName;
				break;

			case SIM_RF:
				str = m_VarListDlg.m_VarTokenName;
				break;

			case SIM_GPX:
				str = m_VarListDlg.m_VarTokenName;
				break;

			case SIM_LFS:
				str = m_VarListDlg.m_VarTokenName;
				break;

			case USIM_INPUT_FLAGS:
				str = m_VarListDlg.m_VarTokenName;
				break;
		}
		m_Data.SetWindowText(str);
	} else if (nResponse == IDC_CLEARDATA) {
		OnClearData();
	}
}

void COutputTab::OnClearData() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenName.Empty();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_SimType = SIM_NONE;
	m_Data.SetWindowText("");
}

void COutputTab::OnClearMask() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMaskName.Empty();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMask = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitType = 0;
	m_Mask.SetWindowText("");
	m_HexMask.SetValue(0, true);
}

void COutputTab::OnSelchangeOpCombo() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Condition1 = (UCHAR)m_OPCombo1.GetCurSel();

	if (m_OPCombo1.GetCurSel() < 6) {
		m_OVGroup1.SetEnable(true);
		OnSelchangeOpCombo2();

		m_OBGroup2.SetEnable(false);
	} else {
		m_OVGroup1.SetEnable(false);

		m_OBGroup2.SetEnable(true);
	}
}

void COutputTab::OnSelchangeOpCombo2() 
{
	if (m_OPCombo2.GetCurSel() > 0) {
		m_OPCombo3.EnableWindow(true);
		m_OPValue2.EnableWindow(true);
	} else {
		m_OPCombo3.EnableWindow(false);
		m_OPValue2.EnableWindow(false);
	}
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Condition2 = (UCHAR)m_OPCombo2.GetCurSel();
}

void COutputTab::OnSelchangeOpCombo3() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Condition3 = (UCHAR)m_OPCombo3.GetCurSel();
}

void COutputTab::OnChangeOpValue() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_OPValue1 = (float)m_OPValue1.GetDouble();	
}

void COutputTab::OnChangeOpValue2() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_OPValue2 = (float)m_OPValue2.GetDouble();	
}

void COutputTab::OnMaskdata() 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	m_VarListDlg.m_ListTypeVar = VARTYPE_BITS;
	int nResponse = m_VarListDlg.DoModal();

	if (nResponse == IDOK) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMaskName = m_VarListDlg.m_VarTokenName;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitMask = m_VarListDlg.m_BitMask;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_BitType = m_VarListDlg.m_BitType;

		m_Mask.SetWindowText(m_VarListDlg.m_VarTokenName);
		m_HexMask.SetValue(m_VarListDlg.m_BitMask, true);
	} else if (nResponse == IDC_CLEARDATA) {
		OnClearMask();
	}
}

void COutputTab::OnLampTest() 
{
	if (m_LampTest.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_LampTest = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_LampTest = 0;
	}
}

void COutputTab::OnInvert() 
{
	if (m_Invert.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Invert = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Invert = 0;
	}
}

void COutputTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_Notes);
}

void COutputTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_OVGroup1.SetEnable(true);
	m_OBGroup2.SetEnable(false);

	m_OPCombo1.SetCurSel(0);
	m_OPValue1.SetDouble(0.0, true);
	m_OPCombo2.SetCurSel(0);
	m_OPCombo3.SetCurSel(0);
	m_OPValue2.SetDouble(0.0, true);
	m_HexMask.SetValue(0, true);

	m_Data.SetWindowText(_T(""));
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].Clear();

	OnSelchangeOpCombo();

	m_Notes.SetWindowText(_T(""));
	OnChangeNotes();

	m_Invert.SetCheck(0);
	OnInvert();
}


BOOL COutputTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
