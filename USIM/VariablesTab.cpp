// VariablesTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "VariablesTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVariablesTab dialog


CVariablesTab::CVariablesTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CVariablesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVariablesTab)
	//}}AFX_DATA_INIT
}

void CVariablesTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVariablesTab)
	DDX_Control(pDX, IDC_OR_INC_SPIN, m_ORIncSpin);
	DDX_Control(pDX, IDC_OR_DEC_SPIN, m_ORDecSpin);
	DDX_Control(pDX, IDC_OP_INC_SPIN, m_OPIncSpin);
	DDX_Control(pDX, IDC_OP_DEC_SPIN, m_OPDecSpin);
	DDX_Control(pDX, IDC_OR_VALUE, m_ORValue);
	DDX_Control(pDX, IDC_OR_MASK, m_ORMask);
	DDX_Control(pDX, IDC_OR_INC, m_ORInc);
	DDX_Control(pDX, IDC_OR_DEC, m_ORDec);
	DDX_Control(pDX, IDC_OP_VALUE, m_OPValue);
	DDX_Control(pDX, IDC_OP_MASK, m_OPMask);
	DDX_Control(pDX, IDC_OP_INC, m_OPInc);
	DDX_Control(pDX, IDC_OP_DEC, m_OPDec);
	DDX_Control(pDX, IDC_OR_COMBO1, m_OROperator);
	DDX_Control(pDX, IDC_OP_COMBO1, m_OPOperator);
	DDX_Control(pDX, IDC_OP_RADIO1, m_OPRadio1);
	DDX_Control(pDX, IDC_OP_RADIO2, m_OPRadio2);
	DDX_Control(pDX, IDC_OP_RADIO3, m_OPRadio3);
	DDX_Control(pDX, IDC_OP_RADIO4, m_OPRadio4);
	DDX_Control(pDX, IDC_OR_RADIO1, m_ORRadio1);
	DDX_Control(pDX, IDC_OR_RADIO2, m_ORRadio2);
	DDX_Control(pDX, IDC_OR_RADIO3, m_ORRadio3);
	DDX_Control(pDX, IDC_OR_RADIO4, m_ORRadio4);
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_OP_FLIGHTDATA, m_OPFlightData);
	DDX_Control(pDX, IDC_OP_TEXT, m_OPText);
	DDX_Control(pDX, IDC_OP_CLEARDATA, m_OPClearData);
	DDX_Control(pDX, IDC_OR_FLIGHTDATA, m_ORFlightData);
	DDX_Control(pDX, IDC_OR_CLEARDATA, m_ORClearData);
	DDX_Control(pDX, IDC_OR_TEXT, m_ORText);
	DDX_Control(pDX, IDC_GROUP1, m_OPGroup);
	DDX_Control(pDX, IDC_GROUP2, m_ORGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVariablesTab, CResizableDialog)
	//{{AFX_MSG_MAP(CVariablesTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	ON_BN_CLICKED(IDC_OP_FLIGHTDATA, OnOPFlightData)
	ON_BN_CLICKED(IDC_OP_CLEARDATA, OnOPClearData)
	ON_BN_CLICKED(IDC_OR_FLIGHTDATA, OnORFlightData)
	ON_BN_CLICKED(IDC_OR_CLEARDATA, OnORClearData)
	ON_BN_CLICKED(IDC_OP_RADIO1, OnOPRadio1)
	ON_BN_CLICKED(IDC_OP_RADIO2, OnOPRadio2)
	ON_BN_CLICKED(IDC_OP_RADIO3, OnOPRadio3)
	ON_BN_CLICKED(IDC_OP_RADIO4, OnOPRadio4)
	ON_BN_CLICKED(IDC_OR_RADIO1, OnORRadio1)
	ON_BN_CLICKED(IDC_OR_RADIO2, OnORRadio2)
	ON_BN_CLICKED(IDC_OR_RADIO3, OnORRadio3)
	ON_BN_CLICKED(IDC_OR_RADIO4, OnORRadio4)
	ON_EN_CHANGE(IDC_OP_MASK, OnChangeOPMask)
	ON_EN_CHANGE(IDC_OR_MASK, OnChangeORMask)
	ON_CBN_SELCHANGE(IDC_OP_COMBO1, OnSelchangeOPCombo1)
	ON_CBN_SELCHANGE(IDC_OR_COMBO1, OnSelchangeORCombo1)
	ON_EN_CHANGE(IDC_OP_VALUE, OnChangeOPValue)
	ON_EN_CHANGE(IDC_OR_VALUE, OnChangeORValue)
	ON_EN_CHANGE(IDC_OP_INC, OnChangeOPInc)
	ON_EN_CHANGE(IDC_OP_DEC, OnChangeOPDec)
	ON_EN_CHANGE(IDC_OR_INC, OnChangeORInc)
	ON_EN_CHANGE(IDC_OR_DEC, OnChangeORDec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVariablesTab message handlers

BOOL CVariablesTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_OPGroup.AddControlToList (IDC_OP_COMBO1);
	m_OPGroup.AddControlToList (IDC_OP_MASK);
	m_OPGroup.AddControlToList (IDC_OP_VALUE);
	m_OPGroup.AddControlToList (IDC_OP_INC);
	m_OPGroup.AddControlToList (IDC_OP_DEC);
	m_OPGroup.SetCheck(false);
	m_OPMask.SetHexColor(RGB(255,255,255),RGB(0,0,0));

	m_ORGroup.AddControlToList (IDC_OR_COMBO1);
	m_ORGroup.AddControlToList (IDC_OR_MASK);
	m_ORGroup.AddControlToList (IDC_OR_VALUE);
	m_ORGroup.AddControlToList (IDC_OR_INC);
	m_ORGroup.AddControlToList (IDC_OR_DEC);
	m_ORGroup.SetCheck(false);
	m_ORMask.SetHexColor(RGB(255,255,255),RGB(0,0,0));

	m_OPIncSpin.SetRangeAndDelta(0, 65535, 1);
	m_OPDecSpin.SetRangeAndDelta(0, 65535, 1);
	m_ORIncSpin.SetRangeAndDelta(0, 65535, 1);
	m_ORDecSpin.SetRangeAndDelta(0, 65535, 1);

	InitVariablesTab();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_OP_TEXT, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_OP_CLEARDATA, TOP_RIGHT);
	AddAnchor(IDC_OP_RADIO2, TOP_RIGHT);
	AddAnchor(IDC_OP_VALUE, TOP_RIGHT);
	AddAnchor(IDC_OP_RADIO4, TOP_RIGHT);
	AddAnchor(IDC_OP_DEC, TOP_RIGHT);
	AddAnchor(IDC_OP_DEC_SPIN, TOP_RIGHT);

	AddAnchor(IDC_STATIC2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_OR_TEXT, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_OR_CLEARDATA, TOP_RIGHT);
	AddAnchor(IDC_OR_RADIO2, TOP_RIGHT);
	AddAnchor(IDC_OR_VALUE, TOP_RIGHT);
	AddAnchor(IDC_OR_RADIO4, TOP_RIGHT);
	AddAnchor(IDC_OR_DEC, TOP_RIGHT);
	AddAnchor(IDC_OR_DEC_SPIN, TOP_RIGHT);

	AddAnchor(IDC_STATIC8, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVariablesTab::InitVariablesTab() 
{
//	const UCHAR Size_Index[] = {0, 0, 1, 0, 2, 0, 0, 0, 3};

	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText("Input: ");
		m_Enable.SetWindowText("");

		m_OPGroup.SetCheck(false);
		m_OPRadio1.EnableWindow(false);
		m_OPRadio2.EnableWindow(false);
		m_OPRadio3.EnableWindow(false);
		m_OPRadio4.EnableWindow(false);

		m_ORGroup.SetCheck(false);
		m_ORRadio1.EnableWindow(false);
		m_ORRadio2.EnableWindow(false);
		m_ORRadio3.EnableWindow(false);
		m_ORRadio4.EnableWindow(false);

		OnClear();

	} else {
		CString str;

		str.Format(_T("%s %s : Input %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		m_IOName.SetWindowText(str);

		SetIOStatus();

		m_OPGroup.SetCheck(false);
		m_ORGroup.SetCheck(false);

		str.Empty();
		switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPSimType) {
			case SIM_XPLANE:
				if (XPDataArray[theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenIndex].VarArraySize > 0) {
					str.Format("X-Plane/%s[%d]", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName,
								theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarArrayTokenIndex + 1);
				} else {
					str.Format("X-Plane/%s", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName);
				}
				break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
				break;

			case SIM_F4USIM:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
				break;

			case SIM_SIMBIN:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
				break;

			case SIM_RF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
				break;

			case SIM_GPX:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
				break;

			case SIM_LFS:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
				break;
		}
		m_OPText.SetWindowText (str);

		m_OPRadio1.EnableWindow(false);
		m_OPRadio2.EnableWindow(false);
		m_OPRadio3.EnableWindow(false);
		m_OPRadio4.EnableWindow(false);
		m_OPRadio1.SetCheck(false);
		m_OPRadio2.SetCheck(false);
		m_OPRadio3.SetCheck(false);
		m_OPRadio4.SetCheck(false);
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarType != xplmType_Cmd) {
			switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPRadio) {
				case 0:
					m_OPRadio1.SetCheck(true);
					OnOPRadio1();
					break;
				case 1:
					m_OPRadio2.SetCheck(true);
					OnOPRadio2();
					break;
				case 2:
					m_OPRadio3.SetCheck(true);
					OnOPRadio3();
					break;
				case 3:
					m_OPRadio4.SetCheck(true);
					OnOPRadio4();
					break;
			}
			m_OPRadio1.EnableWindow(true);
			m_OPRadio2.EnableWindow(true);
			m_OPRadio3.EnableWindow(true);
			m_OPRadio4.EnableWindow(true);
		}

		m_OPOperator.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPOperator);
		m_OPMask.SetValue(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPMask, true);
		m_OPValue.SetDouble(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPValue, true);
		m_OPIncSpin.SetPos (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPInc);
		m_OPDecSpin.SetPos (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPDec);

		str.Empty(); 
		switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORSimType) {
			case SIM_XPLANE:
				if (XPDataArray[theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenIndex].VarArraySize > 0) {
					str.Format("X-Plane/%s[%d]", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName,
								theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarArrayTokenIndex + 1);
				} else {
					str.Format("X-Plane/%s", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName);
				}
				break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
				break;

			case SIM_F4USIM:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
				break;

			case SIM_SIMBIN:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
				break;

			case SIM_RF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
				break;

			case SIM_GPX:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
				break;

			case SIM_LFS:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
				break;
		}
		m_ORText.SetWindowText (str);

		m_ORRadio1.EnableWindow(false);
		m_ORRadio2.EnableWindow(false);
		m_ORRadio3.EnableWindow(false);
		m_ORRadio4.EnableWindow(false);
		m_ORRadio1.SetCheck(false);
		m_ORRadio2.SetCheck(false);
		m_ORRadio3.SetCheck(false);
		m_ORRadio4.SetCheck(false);

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarType != xplmType_Cmd) {
			switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORRadio) {
				case 0:
					m_ORRadio1.SetCheck(true);
					OnORRadio1();
					break;
				case 1:
					m_ORRadio2.SetCheck(true);
					OnORRadio2();
					break;
				case 2:
					m_ORRadio3.SetCheck(true);
					OnORRadio3();
					break;
				case 3:
					m_ORRadio4.SetCheck(true);
					OnORRadio4();
					break;
			}
			m_ORRadio1.EnableWindow(true);
			m_ORRadio2.EnableWindow(true);
			m_ORRadio3.EnableWindow(true);
			m_ORRadio4.EnableWindow(true);
		}

		m_OROperator.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OROperator);
		m_ORMask.SetValue(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORMask, true);
		m_ORValue.SetDouble(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORValue, true);
		m_ORIncSpin.SetPos (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORInc);
		m_ORDecSpin.SetPos (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORDec);

		m_Notes.SetWindowText(_T(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_Notes));
	}
}

void CVariablesTab::SetIOStatus()
{
	if (theApp.m_CurrenthItem == NULL) return;

	// Check IO Status
	if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputActive[theApp.m_CurItemIndex]) {
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputDisabled[theApp.m_CurItemIndex]) {
			m_Enable.SetWindowText(_T("DISABLED"));
		} else {
			m_Enable.SetWindowText(_T("ENABLED"));
		}
	} else {
		m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
	}
}

void CVariablesTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_Notes);
}

void CVariablesTab::OnOPFlightData() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	CString str;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	m_VarListDlg.m_ListTypeVar = VARTYPE_VAR;
	if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarType == xplmType_Cmd)
		m_VarListDlg.m_ListTypeVar = VARTYPE_CMD;

	m_VarListDlg.m_VarTokenName = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName;
	m_VarListDlg.m_VarArrayIndex = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarArrayTokenIndex;

	int nResponse = m_VarListDlg.DoModal();

	if (nResponse == IDOK) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName = m_VarListDlg.m_VarTokenName;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenIndex = m_VarListDlg.m_ArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarArrayTokenIndex = m_VarListDlg.m_VarArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPSimType = m_VarListDlg.m_SimType;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPWrite = true;
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
		}
		m_OPText.SetWindowText(str);

		if (m_VarListDlg.m_ListTypeVar == VARTYPE_CMD) {
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarType = xplmType_Cmd;
			m_OPGroup.SetCheck (false);
			m_OPRadio1.EnableWindow(false);
			m_OPRadio2.EnableWindow(false);
			m_OPRadio3.EnableWindow(false);
			m_OPRadio4.EnableWindow(false);
		} else {
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarType = XPDataArray[m_VarListDlg.m_ArrayIndex].VarType;
			m_OPGroup.SetCheck (true);
			m_OPRadio1.EnableWindow(true);
			m_OPRadio2.EnableWindow(true);
			m_OPRadio3.EnableWindow(true);
			m_OPRadio4.EnableWindow(true);
			m_OPRadio1.SetCheck(true);
			OnOPRadio1();
		}
	} else if (nResponse == IDC_CLEARDATA) {
		OnOPClearData();
	}
}

void CVariablesTab::OnOPClearData() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenName.Empty();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarArrayTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPSimType = SIM_NONE;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPVarType = 0;
	m_OPText.SetWindowText("");

	m_OPGroup.SetCheck (true);
	m_OPRadio1.EnableWindow(true);
	m_OPRadio2.EnableWindow(true);
	m_OPRadio3.EnableWindow(true);
	m_OPRadio4.EnableWindow(true);
	m_OPRadio1.SetCheck(true);
	OnOPRadio1();
}

void CVariablesTab::OnORFlightData() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	CString str;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	
	m_VarListDlg.m_ListTypeVar = VARTYPE_VAR;
	if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarType == xplmType_Cmd)
		m_VarListDlg.m_ListTypeVar = VARTYPE_CMD;

	m_VarListDlg.m_VarTokenName = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName;
	m_VarListDlg.m_VarArrayIndex = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarArrayTokenIndex;

	int nResponse = m_VarListDlg.DoModal();

	if (nResponse == IDOK) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName = m_VarListDlg.m_VarTokenName;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenIndex = m_VarListDlg.m_ArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarArrayTokenIndex = m_VarListDlg.m_VarArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORSimType = m_VarListDlg.m_SimType;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORWrite = true;
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
		}
		m_ORText.SetWindowText(str);

		if (m_VarListDlg.m_ListTypeVar == VARTYPE_CMD) {
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarType = xplmType_Cmd;
			m_ORGroup.SetCheck (false);
			m_ORRadio1.EnableWindow(false);
			m_ORRadio2.EnableWindow(false);
			m_ORRadio3.EnableWindow(false);
			m_ORRadio4.EnableWindow(false);
		} else {
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarType = XPDataArray[m_VarListDlg.m_ArrayIndex].VarType;
			m_ORGroup.SetCheck (true);
			m_ORRadio1.EnableWindow(true);
			m_ORRadio2.EnableWindow(true);
			m_ORRadio3.EnableWindow(true);
			m_ORRadio4.EnableWindow(true);
			m_ORRadio1.SetCheck(true);
			OnORRadio1();
		}
	}else if (nResponse == IDC_CLEARDATA) {
		OnORClearData();
	}
}

void CVariablesTab::OnORClearData() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenName.Empty();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarArrayTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORSimType = SIM_NONE;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORVarType = 0;
	m_ORText.SetWindowText("");

	m_ORGroup.SetCheck (true);
	m_ORRadio1.EnableWindow(true);
	m_ORRadio2.EnableWindow(true);
	m_ORRadio3.EnableWindow(true);
	m_ORRadio4.EnableWindow(true);
	m_ORRadio1.SetCheck(true);
	OnORRadio1();
}

void CVariablesTab::OnClear() 
{
	m_OPText.SetWindowText("");
	m_OPOperator.SetCurSel(0);
	m_ORGroup.SetCheck (true);
	m_OPRadio1.EnableWindow(true);
	m_OPRadio2.EnableWindow(true);
	m_OPRadio3.EnableWindow(true);
	m_OPRadio4.EnableWindow(true);
	m_OPRadio1.SetCheck(true);
	OnOPRadio1();
	m_OPRadio2.SetCheck(false);
	m_OPRadio3.SetCheck(false);
	m_OPRadio4.SetCheck(false);
	m_OPMask.SetValue(0,true);
	m_OPValue.SetDouble(0, true);
	m_OPIncSpin.SetPos (0);
	m_OPDecSpin.SetPos (0);

	m_ORText.SetWindowText("");
	m_OROperator.SetCurSel(0);
	m_ORGroup.SetCheck (true);
	m_ORRadio1.EnableWindow(true);
	m_ORRadio2.EnableWindow(true);
	m_ORRadio3.EnableWindow(true);
	m_ORRadio4.EnableWindow(true);
	m_ORRadio1.SetCheck(true);
	OnORRadio1();
	m_ORRadio2.SetCheck(false);
	m_ORRadio3.SetCheck(false);
	m_ORRadio4.SetCheck(false);
	m_ORMask.SetValue(0,true);
	m_ORValue.SetDouble(0, true);
	m_ORIncSpin.SetPos (0);
	m_ORDecSpin.SetPos (0);

	m_Notes.SetWindowText("");
}

void CVariablesTab::OnClearButton() 
{
	OnClear();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].Clear();
}

BOOL CVariablesTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

void CVariablesTab::OnOPRadio1() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_OPRadio1.GetCheck()) {
		m_OPGroup.SetCheck(false);
		m_OPOperator.EnableWindow(true);
		m_OPMask.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPRadio = 0;
	}
}

void CVariablesTab::OnOPRadio2() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_OPRadio2.GetCheck()) {
		m_OPGroup.SetCheck(false);
		m_OPValue.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPRadio = 1;
	}
}

void CVariablesTab::OnOPRadio3() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_OPRadio3.GetCheck()) {
		m_OPGroup.SetCheck(false);
		m_OPInc.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPRadio = 2;
	}	
}

void CVariablesTab::OnOPRadio4() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_OPRadio4.GetCheck()) {
		m_OPGroup.SetCheck(false);
		m_OPDec.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPRadio = 3;
	}	
}

void CVariablesTab::OnORRadio1() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_ORRadio1.GetCheck()) {
		m_ORGroup.SetCheck(false);
		m_OROperator.EnableWindow(true);
		m_ORMask.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORRadio = 0;
	}
}

void CVariablesTab::OnORRadio2() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_ORRadio2.GetCheck()) {
		m_ORGroup.SetCheck(false);
		m_ORValue.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORRadio = 1;
	}	
}

void CVariablesTab::OnORRadio3() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_ORRadio3.GetCheck()) {
		m_ORGroup.SetCheck(false);
		m_ORInc.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORRadio = 2;
	}		
}

void CVariablesTab::OnORRadio4() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	if (m_ORRadio4.GetCheck()) {
		m_ORGroup.SetCheck(false);
		m_ORDec.EnableWindow(true);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORRadio = 3;
	}		
}

void CVariablesTab::OnChangeOPMask() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPMask = m_OPMask.GetValue();
}

void CVariablesTab::OnChangeORMask() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORMask = m_ORMask.GetValue();	
}

void CVariablesTab::OnSelchangeOPCombo1() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPOperator = (UCHAR)m_OPOperator.GetCurSel();
}

void CVariablesTab::OnSelchangeORCombo1() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OROperator = (UCHAR)m_OROperator.GetCurSel();
}

void CVariablesTab::OnChangeOPValue() 
{
	if (theApp.m_CurrenthItem == NULL) return;
	
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPValue =
		m_OPValue.GetDouble();
}

void CVariablesTab::OnChangeORValue() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORValue =
		m_ORValue.GetDouble();	
}

void CVariablesTab::OnChangeOPInc() 
{
	CString str;
	if (theApp.m_CurrenthItem == NULL) return;

	m_OPInc.GetWindowText(str);

	double temp = atoi(str);
	if (temp > 65535) temp = 65535;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPInc = (UINT)temp;
}

void CVariablesTab::OnChangeOPDec() 
{
	CString str;
	if (theApp.m_CurrenthItem == NULL) return;

	m_OPDec.GetWindowText(str);

	double temp = atoi(str);
	if (temp > 65535) temp = 65535;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_OPDec = (UINT)temp;
}

void CVariablesTab::OnChangeORInc() 
{
	CString str;
	if (theApp.m_CurrenthItem == NULL) return;

	m_ORInc.GetWindowText(str);

	double temp = atoi(str);
	if (temp > 65535) temp = 65535;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORInc = (UINT)temp;
}

void CVariablesTab::OnChangeORDec() 
{
	CString str;
	if (theApp.m_CurrenthItem == NULL) return;

	m_ORDec.GetWindowText(str);

	double temp = atoi(str);
	if (temp > 65535) temp = 65535;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_VariablesArray[theApp.m_CurItemIndex].m_ORDec = (UINT)temp;
}