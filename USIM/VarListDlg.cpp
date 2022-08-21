// VarListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "VarListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_BT_CLICKED (WM_USER+1)

/////////////////////////////////////////////////////////////////////////////
// CVarListDlg dialog


CVarListDlg::CVarListDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CVarListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVarListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_OldIndex = 0;
	m_VarTokenName.Empty();
}


void CVarListDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVarListDlg)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_VALUETEXT, m_ValueText);
	DDX_Control(pDX, IDC_ED_SEARCH_ITEM, m_SearchText);
	DDX_Control(pDX, IDC_CLEARDATA, m_ClearData);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
	DDX_Control(pDX, IDC_BT_SEARCH_ITEM, m_Search);
	DDX_Control(pDX, IDC_BT_RESET_SEARCH, m_Reset);
	DDX_Control(pDX, IDC_EMINMAX, m_EVarArrayIndex);
	DDX_Control(pDX, IDC_SMINMAX, m_IndexMinMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVarListDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CVarListDlg)
	ON_BN_CLICKED(IDC_CLEARDATA, OnClearData)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_BT_SEARCH_ITEM, OnBtSearchItem)
	ON_BN_CLICKED(IDC_BT_RESET_SEARCH, OnBtResetSearch)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EMINMAX, OnEnChangeMinMax)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarListDlg message handlers

BOOL CVarListDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	AddAnchor(IDOK, BOTTOM_CENTER);
	AddAnchor(IDC_CLEARDATA, BOTTOM_CENTER);
	AddAnchor(IDC_STATIC1, BOTTOM_CENTER);
	AddAnchor(IDC_VALUETEXT, BOTTOM_CENTER);
	AddAnchor(IDC_RADIO1, BOTTOM_CENTER);
	AddAnchor(IDC_RADIO2, BOTTOM_CENTER);
	AddAnchor(IDC_RADIO3, BOTTOM_CENTER);
	AddAnchor(IDCANCEL, BOTTOM_CENTER);
	AddAnchor(IDC_ED_SEARCH_ITEM, TOP_CENTER);
	AddAnchor(IDC_BT_SEARCH_ITEM, TOP_CENTER);
	AddAnchor(IDC_BT_RESET_SEARCH, TOP_CENTER);
	AddAnchor(IDC_STATIC2, TOP_CENTER);
	AddAnchor(IDC_STATIC13, BOTTOM_CENTER);
	AddAnchor(IDC_SMINMAX, BOTTOM_CENTER);
	AddAnchor(IDC_EMINMAX, BOTTOM_CENTER);

    // Declare variables
    CRect rcClient;
    DWORD dwStyle, dwOptions;
    
    // Get the clients rectangle
    GetClientRect(rcClient);
    rcClient.SetRect(rcClient.left + 15, rcClient.top + 45, rcClient.right - 15, rcClient.bottom - 80);
	
    // Setup the window style

    //dwStyle = WS_CHILD | WS_VISIBLE;
	dwStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    // Setup the tree options 
    dwOptions =  OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS | OT_OPTIONS_SHOWINFOWINDOW;

    // Create tree options
    if (m_otTree.Create(dwStyle, rcClient, this, dwOptions, IDC_OPTIONSTREE_OPTIONS) == FALSE) {
        TRACE0("Failed to create options control.\r\n");
        return FALSE;
    }

	m_otTree.SetAutoSizeColumn(true);
	m_otTree.SetNotify(TRUE, this,WM_BT_CLICKED,false);
	m_otTree.SetAttributeOffset(4);
	AddAnchor(m_otTree.GetDlgCtrlID(), TOP_LEFT, BOTTOM_RIGHT);

	m_Radio1.SetCheck(false);
	m_Radio2.SetCheck(false);
	m_Radio3.SetCheck(false);
	m_Radio1.EnableWindow(false);
	m_Radio2.EnableWindow(false);
	m_Radio3.EnableWindow(false);
	switch (m_ListTypeVar) {
		case VARTYPE_VAR_ONLY:
			SetWindowText("Simulation Variable List");
			FillVarList();
			break;
		case VARTYPE_VAR:
			m_Radio1.EnableWindow(true);
			m_Radio2.EnableWindow(true);
			m_Radio1.SetCheck(true);
			SetWindowText("Simulation Variable List");
			FillVarList();
			break;
		case VARTYPE_CMD:
			m_Radio1.EnableWindow(true);
			m_Radio2.EnableWindow(true);
			m_Radio2.SetCheck(true);
			SetWindowText("Simulation Command List");
			FillCmdList();
			break;
		case VARTYPE_BITS:
			m_Radio3.EnableWindow(true);
			m_Radio3.SetCheck(true);
			SetWindowText("Simulation Bitmask List");
			FillBitsList();
			break;
		case VARTYPE_REALTIME:
			SetWindowText("Real-Time Simulation Data");
			m_OK.ShowWindow(false);
			m_Cancel.ShowWindow(false);
			m_ClearData.SetWindowText("OK");
			FillVarList();
			break;
	}

	m_pSearchOffset = NULL;
	m_SearchText.SetWindowText("");

	LoadWindowRect("VarDlg Settings", true);

	m_ValueText.SetWindowText("");
	m_EVarArrayIndex.SetWindowText("");

	if (!m_VarTokenName.IsEmpty()) {
		SearchItem(m_VarTokenName);
		m_OldIndex = 0;
	}

	m_nTimer = SetTimer(1, 500, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVarListDlg::FillVarList()
{
	int i, j;
	CString strSubMS, strSubMSprev, str;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	m_otTree.LockAll();
	m_otTree.DeleteAllItems();

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;
	COptionTreeItemStatic *otiStatic = NULL;
	COptionTreeItemStatic *otiStaticParent[	ArraySize(XPDataArray) + 
											ArraySize(IL2DataArray) + 
											ArraySize(F4DataArray) + 
											ArraySize(SimBinDataArray) +
											ArraySize(RFDataArray) +
											ArraySize(GPXDataArray) +
											ArraySize(LFSDataArray) +
											MAX_INPUT_FLAGS +
											1];

	// -- X-Plane root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("X-Plane Variables"));
	otiRoot->SetInfoText(_T("X-Plane variables."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(XPDataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, XPDataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(XPDataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, XPDataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 255));
					otiStatic->SetBackgroundColor(RGB(245, 245, 255));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", XPDataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(XPDataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(XPDataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_XPLANE);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- IL2 root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("IL2 / Pacific Fighters Variables"));
	otiRoot->SetInfoText(_T("IL2 / Pacific Fighters variables."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(IL2DataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, IL2DataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(IL2DataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, IL2DataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 255));
					otiStatic->SetBackgroundColor(RGB(245, 245, 255));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", IL2DataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(IL2DataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(IL2DataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_IL2);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	if (m_ListTypeVar == VARTYPE_VAR) {
		m_otTree.UnLockAll();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return;
	}

	// -- SimBin root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("SimBin Racing (READ ONLY)"));
	otiRoot->SetInfoText(_T("SimBin Racing Sim variables. (READ ONLY)"));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(SimBinDataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, SimBinDataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(SimBinDataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, SimBinDataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 255));
					otiStatic->SetBackgroundColor(RGB(245, 245, 255));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", SimBinDataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(SimBinDataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(SimBinDataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_SIMBIN);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- RF root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("rFactor Racing (READ ONLY)"));
	otiRoot->SetInfoText(_T("rFactor Racing Sim variables. (READ ONLY)"));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(RFDataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, RFDataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(RFDataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, RFDataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 255));
					otiStatic->SetBackgroundColor(RGB(245, 245, 255));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", RFDataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(RFDataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(RFDataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_RF);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}


	// -- GPX root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("GPx Racing (READ ONLY)"));
	otiRoot->SetInfoText(_T("GPx Racing Sim variables. (READ ONLY)"));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(GPXDataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, GPXDataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(GPXDataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, GPXDataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 255));
					otiStatic->SetBackgroundColor(RGB(245, 245, 255));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", GPXDataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(GPXDataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(GPXDataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_GPX);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- LFS root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("LFS Racing (READ ONLY)"));
	otiRoot->SetInfoText(_T("LFS Racing Sim variables. (READ ONLY)"));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(LFSDataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, LFSDataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(LFSDataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, LFSDataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 255));
					otiStatic->SetBackgroundColor(RGB(245, 245, 255));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", LFSDataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(LFSDataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(LFSDataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_LFS);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- Falcon 4 root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("Falcon 4 (READ ONLY)"));
	otiRoot->SetInfoText(_T("Falcon 4 variables. READ ONLY."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(F4DataArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, F4DataArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(F4DataArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, F4DataArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 250));
					otiStatic->SetBackgroundColor(RGB(245, 245, 250));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("Units: %s", F4DataArray[j+1].Units);
				otiStaticParent[j]->SetAttributeText(_T(F4DataArray[j+1].VarTokenName));
				otiStaticParent[j]->SetInfoText(_T(str));
				otiStaticParent[j]->SetStaticText(_T(F4DataArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_F4USIM);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- Input Flags root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("Input Flags (READ ONLY)"));
	otiRoot->SetInfoText(_T("Input Flags. READ ONLY."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < MAX_INPUT_FLAGS; j++) {	
		strSubMS = theApp.m_UserVarsArray.m_IOFlags[j].VarTokenString;
		otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
		otiStatic->SetAttributeText(_T("Input_Flags/" + strSubMS));
		otiStatic->SetLabelText(_T(strSubMS));
		otiStatic->SetInfoText(_T("BOOL"));
		otiStatic->SetStaticText(_T(theApp.m_UserVarsArray.m_IOFlags[j].ItemPathString));
		otiStatic->SetStaticListType(USIM_INPUT_FLAGS);
		otiStatic->SetStaticDataIndex(j);
		otiStatic->CreateStaticItem(0);
		otiStatic->SetLabelBackgroundColor(RGB(255, 255, 255));
		otiStatic->SetBackgroundColor(RGB(255, 255, 255));
		otiStaticParent[j] = otiStatic;
	}

	m_otTree.UnLockAll();

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CVarListDlg::FillCmdList()
{
	int i, j;
	CString strSubMS, strSubMSprev, str;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	m_otTree.LockAll();
	m_otTree.DeleteAllItems();

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;
	COptionTreeItemStatic *otiStatic = NULL;
	COptionTreeItemStatic *otiStaticParent[ArraySize(XPCmdArray) + ArraySize(IL2CmdArray) + 1];

	// -- X-Plane root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("X-Plane Commands"));
	otiRoot->SetInfoText(_T("X-Plane commands."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(XPCmdArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, XPCmdArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(XPCmdArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, XPCmdArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 250));
					otiStatic->SetBackgroundColor(RGB(245, 245, 250));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				otiStaticParent[j]->SetAttributeText(_T(XPCmdArray[j+1].VarTokenName));
				otiStaticParent[j]->SetStaticText(_T(XPCmdArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_XPLANE);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- IL2 root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("IL2 / Pacific Fighters Commands"));
	otiRoot->SetInfoText(_T("IL2 / Pacific Fighters commands."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(IL2CmdArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, IL2CmdArray[j+1].VarTokenName, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(IL2CmdArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, IL2CmdArray[j+1].VarTokenName, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 250));
					otiStatic->SetBackgroundColor(RGB(245, 245, 250));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				otiStaticParent[j]->SetAttributeText(_T(IL2CmdArray[j+1].VarTokenName));
				otiStaticParent[j]->SetStaticText(_T(IL2CmdArray[j+1].Description));
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_IL2);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	m_otTree.UnLockAll();

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CVarListDlg::FillBitsList()
{
	int i, j;
	CString strSubMS, strSubMSprev, str;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	m_otTree.LockAll();
	m_otTree.DeleteAllItems();

	// Setup options tree
	COptionTreeItem *otiRoot = NULL;
	COptionTreeItemStatic *otiStatic = NULL;
	COptionTreeItemStatic *otiStaticParent[ArraySize(F4BitsArray) + ArraySize(LFSBitsArray) + ArraySize(GPXBitsArray) + 1];

	// -- Falcon 4 root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("Falcon 4 Bitmasks"));
	otiRoot->SetInfoText(_T("Falcon 4 Bitmasks."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(F4BitsArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, F4BitsArray[j+1].TokenString, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(F4BitsArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, F4BitsArray[j+1].TokenString, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 250));
					otiStatic->SetBackgroundColor(RGB(245, 245, 250));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("0x%.4x", F4BitsArray[j+1].BitMask);
				otiStaticParent[j]->SetAttributeText(_T(F4BitsArray[j+1].TokenString));
				otiStaticParent[j]->SetStaticText(str);
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_F4USIM);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- LFS root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("LFS Bitmasks"));
	otiRoot->SetInfoText(_T("LFS Bitmasks."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(LFSBitsArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, LFSBitsArray[j+1].TokenString, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(LFSBitsArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, LFSBitsArray[j+1].TokenString, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 250));
					otiStatic->SetBackgroundColor(RGB(245, 245, 250));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("0x%.4x", LFSBitsArray[j+1].BitMask);
				otiStaticParent[j]->SetAttributeText(_T(LFSBitsArray[j+1].TokenString));
				otiStaticParent[j]->SetStaticText(str);
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_LFS);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	// -- GPX root item
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetAttributeText(_T(""));
	otiRoot->SetLabelText(_T("GPX Bitmasks"));
	otiRoot->SetInfoText(_T("GPX Bitmasks."));
	otiRoot->SetListType(SIM_NONE);

	i = 1;
	for (j = 0; j < ArraySize(GPXBitsArray)-1; j++) {	
		if ( AfxExtractSubString (strSubMS, GPXBitsArray[j+1].TokenString, i, '/') ) {
			if ( strSubMS != strSubMSprev ) {
				strSubMSprev = strSubMS;
				otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
				otiStatic->SetAttributeText(_T(""));
				otiStatic->SetLabelText(_T(strSubMS));
				otiStatic->SetInfoText(_T(""));
				otiStatic->SetStaticText(_T(""));
				otiStatic->SetStaticListType(SIM_NONE);
				otiStatic->CreateStaticItem(0);
				otiStatic->SetLabelBackgroundColor(RGB(240, 240, 245));
				otiStatic->SetBackgroundColor(RGB(240, 240, 245));
			}
			otiStaticParent[j] = otiStatic;
		}
	}

	for (i = 2; i < TREEDEPTH; i++) {
		otiStatic = NULL;
		strSubMSprev.Empty();
		for (j = 0; j < ArraySize(GPXBitsArray)-1; j++) {	
			if ( AfxExtractSubString (strSubMS, GPXBitsArray[j+1].TokenString, i, '/') ) {
				if ( strSubMS != strSubMSprev ) {
					strSubMSprev = strSubMS;
					otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStaticParent[j]);
					otiStatic->SetAttributeText(_T(""));
					otiStatic->SetLabelText(_T(strSubMS));
					otiStatic->SetInfoText(_T(""));
					otiStatic->SetStaticText(_T(""));
					otiStatic->SetStaticListType(SIM_NONE);
					otiStatic->CreateStaticItem(0);
					otiStatic->SetLabelBackgroundColor(RGB(245, 245, 250));
					otiStatic->SetBackgroundColor(RGB(245, 245, 250));
				}
				if (otiStaticParent[j] != otiStaticParent[j+1]) {
					strSubMSprev.Empty();
				}
				otiStaticParent[j] = otiStatic;
			} else {
				str.Format("0x%.4x", GPXBitsArray[j+1].BitMask);
				otiStaticParent[j]->SetAttributeText(_T(GPXBitsArray[j+1].TokenString));
				otiStaticParent[j]->SetStaticText(str);
				otiStaticParent[j]->SetStaticDataIndex(j+1);
				otiStaticParent[j]->SetStaticListType(SIM_GPX);
				otiStaticParent[j]->SetBackgroundColor(RGB(255, 255, 255));
				otiStaticParent[j]->SetLabelBackgroundColor(RGB(255, 255, 255));
			}
		}
	}

	m_otTree.UnLockAll();

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CVarListDlg::OnOK() 
{
//	UINT idx = 0;
	COptionTreeItemStatic *otiStatic = NULL;

	KillTimer(m_nTimer);
	
	otiStatic = (COptionTreeItemStatic *)m_otTree.GetFocusedItem();
	m_VarTokenName.Empty();

	if (otiStatic != NULL) {
		if (otiStatic->GetItemType() != SIM_NONE) {
			switch (otiStatic->GetStaticListType()) {
				case SIM_XPLANE:
					if (m_ListTypeVar == VARTYPE_CMD)
						m_VarTokenName = XPCmdArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					else
						m_VarTokenName = XPDataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = XPDataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case SIM_IL2:
					if (m_ListTypeVar == VARTYPE_CMD)
						m_VarTokenName = IL2CmdArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					else
						m_VarTokenName = IL2DataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = IL2DataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case SIM_F4USIM:
					if (m_ListTypeVar == VARTYPE_BITS) {
						m_VarTokenName = F4BitsArray[otiStatic->GetStaticDataIndex()].TokenString;
						m_BitMask = F4BitsArray[otiStatic->GetStaticDataIndex()].BitMask;
						m_BitSimType = F4BitsArray[otiStatic->GetStaticDataIndex()].BitSimType;
					} else {
						m_VarTokenName = F4DataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					}

					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = F4DataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case SIM_SIMBIN:
					m_VarTokenName = SimBinDataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = SimBinDataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case SIM_RF:
					m_VarTokenName = RFDataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = RFDataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case SIM_GPX:
					if (m_ListTypeVar == VARTYPE_BITS) {
						m_VarTokenName = GPXBitsArray[otiStatic->GetStaticDataIndex()].TokenString;
						m_BitMask = GPXBitsArray[otiStatic->GetStaticDataIndex()].BitMask;
						m_BitSimType = GPXBitsArray[otiStatic->GetStaticDataIndex()].BitSimType;
					} else {
						m_VarTokenName = GPXDataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					}
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = GPXDataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case SIM_LFS:
					if (m_ListTypeVar == VARTYPE_BITS) {
						m_VarTokenName = LFSBitsArray[otiStatic->GetStaticDataIndex()].TokenString;
						m_BitMask = LFSBitsArray[otiStatic->GetStaticDataIndex()].BitMask;
						m_BitSimType = LFSBitsArray[otiStatic->GetStaticDataIndex()].BitSimType;
					} else {
						m_VarTokenName = LFSDataArray[otiStatic->GetStaticDataIndex()].VarTokenName;
					}
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = LFSDataArray[otiStatic->GetStaticDataIndex()].VarType;
					break;

				case USIM_INPUT_FLAGS:
					if (m_ListTypeVar != VARTYPE_BITS) {
						m_VarTokenName = "Input_Flags/" + theApp.m_UserVarsArray.m_IOFlags[otiStatic->GetStaticDataIndex()].VarTokenString;
					}
					m_ArrayIndex = otiStatic->GetStaticDataIndex();
					m_SimType = otiStatic->GetStaticListType();
					m_DataType = (UINT)xplmType_Byte;
					break;
			}
		}
	}

	CResizableDialog::OnOK();
}


void CVarListDlg::OnEnChangeMinMax()
{
	COptionTreeItemStatic *otiStatic = NULL;
	CString str;
	int CurIndex, MaxVarArraySize, VarArrayIndex;

	try {
		otiStatic = (COptionTreeItemStatic *)m_otTree.GetFocusedItem();
		if (otiStatic != NULL) {
			if (otiStatic->GetItemType() != SIM_NONE) {
				CurIndex = otiStatic->GetStaticDataIndex();
				MaxVarArraySize = XPDataArray[CurIndex].VarArraySize;

				m_EVarArrayIndex.GetWindowText(str);
				VarArrayIndex = atoi(str);

				if (VarArrayIndex < MaxVarArraySize && VarArrayIndex > 0) {
					m_VarArrayIndex = VarArrayIndex - 1;
				} else {
					VarArrayIndex = 0;
					m_OldIndex = 0;
				}
			}
		}
	} catch (CThreadException* pe) {
		// write to log file
		TRACE("VarListDlg - OnEnChangeMinMax error\n");
		str.Format("%susim.log", theApp.m_AppPath);
		FILE* stream = fopen(str, "a");
		fprintf(stream, "VarListDlg - OnEnChangeMinMax error\n");
		fclose(stream);

		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CVarListDlg::OnClearData() 
{
	KillTimer(m_nTimer);
	m_VarTokenName.Empty();
	m_ArrayIndex = 0;
	m_SimType = SIM_NONE;

	UpdateData(true);
	EndDialog(IDC_CLEARDATA);
}

void CVarListDlg::OnCancel() 
{
	KillTimer(m_nTimer);
	CResizableDialog::OnCancel();
}

void CVarListDlg::OnRadio1() 
{
	if (m_Radio1.GetCheck()) {
		m_ListTypeVar = VARTYPE_VAR;
		SetWindowText("Simulation Variable List");
		FillVarList();
	}	
}

void CVarListDlg::OnRadio2() 
{
	if (m_Radio2.GetCheck()) {
		m_ListTypeVar = VARTYPE_CMD;
		SetWindowText("Simulation Command List");
		FillCmdList();
	}		
}

void CVarListDlg::OnRadio3() 
{
	if (m_Radio3.GetCheck()) {
		m_ListTypeVar = VARTYPE_BITS;
		SetWindowText("Simulation Bitmask List");
		FillBitsList();
	}		
}

void CVarListDlg::OnBtSearchItem() 
{
	CString SearchText;
	m_SearchText.GetWindowText(SearchText);

	SearchItem(SearchText);
}

void CVarListDlg::SearchItem(CString SearchText) 
{
	UpdateData(TRUE);

	// unselect previous matches
	m_otTree.SelectItems(NULL, FALSE);

	COptionTreeItem *pNewMatch = m_otTree.FindItem(SearchText, OT_FI_ATTRIBUTE, m_pSearchOffset);
	if(pNewMatch == NULL || pNewMatch == m_pSearchOffset) {
		m_SearchText.SetBackgroundColor(0x008888FF);
		m_pSearchOffset = NULL;
		UpdateData(FALSE);
	} else {
		m_SearchText.SetBackgroundColor(0x00FFFFFF);
		m_pSearchOffset = pNewMatch;
		m_otTree.EnsureVisible(pNewMatch);
		m_otTree.SelectItems(pNewMatch, TRUE);
		m_otTree.SetFocusedItem(pNewMatch);

		UpdateData(FALSE);
	}
}

void CVarListDlg::OnBtResetSearch() 
{
	m_pSearchOffset = NULL;
}

void CVarListDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CResizableDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_nTimer = SetTimer(1, 500, 0);
}

void CVarListDlg::OnTimer(UINT nIDEvent) 
{
	COptionTreeItemStatic *otiStatic = NULL;
	double tmpData;
	CString str;
	int CurIndex, MaxVarArraySize;

	try {
		otiStatic = (COptionTreeItemStatic *)m_otTree.GetFocusedItem();
		if (otiStatic != NULL) {
			if (otiStatic->GetItemType() != SIM_NONE) {
				CurIndex = otiStatic->GetStaticDataIndex();
				switch (otiStatic->GetStaticListType()) {

					case SIM_XPLANE:
						if (m_ListTypeVar != VARTYPE_CMD && theApp.m_pXPFlightData != NULL) {

							MaxVarArraySize = XPDataArray[CurIndex].VarArraySize;
							if (MaxVarArraySize > 0) {
								m_EVarArrayIndex.EnableWindow(TRUE);
								str.Format("(1 - %d)", MaxVarArraySize);
								m_IndexMinMax.SetWindowText(str);
								theApp.m_pXPFlightData->Data[CurIndex].inOffset = m_VarArrayIndex;
								str.Format("%d", m_VarArrayIndex);
								m_EVarArrayIndex.SetWindowText(str);
							} else {
								m_EVarArrayIndex.EnableWindow(FALSE);
								m_EVarArrayIndex.SetWindowText("");
								m_IndexMinMax.SetWindowText("");
								theApp.m_pXPFlightData->Data[CurIndex].inOffset = 0;
							}

							theApp.m_pXPFlightData->Data[CurIndex].Active = true;
							if (m_OldIndex != CurIndex) {
								theApp.m_pXPFlightData->Data[m_OldIndex].Active = false;
								m_OldIndex = CurIndex;
							}
							tmpData = VarTypeConversion(XPDataArray[CurIndex].VarType,
														&theApp.m_pXPFlightData->Data[CurIndex].dDataRead);
							str.Format("%5.4f", tmpData);
							m_ValueText.SetWindowText(str);
							TRACE("\n%5.4f\n", tmpData);
						} else {
							if (m_OldIndex != CurIndex) {
								m_ValueText.SetWindowText("");
								MaxVarArraySize = XPDataArray[CurIndex].VarArraySize;
								if (MaxVarArraySize > 0) {
									m_EVarArrayIndex.EnableWindow(TRUE);
									str.Format("(1 - %d)", MaxVarArraySize);
									m_IndexMinMax.SetWindowText(str);
									str.Format("%d", m_VarArrayIndex + 1);
									m_EVarArrayIndex.SetWindowText(str);
								} else {
									m_EVarArrayIndex.EnableWindow(FALSE);
									m_EVarArrayIndex.SetWindowText("");
									m_IndexMinMax.SetWindowText("");
									m_VarArrayIndex = 0;
								}
								m_OldIndex = CurIndex;
							}
						}
						break;

					case SIM_IL2:
						if (m_ListTypeVar != VARTYPE_CMD) {
							theApp.m_IL2FlightData.Data[CurIndex].Active = true;
							if (m_OldIndex != CurIndex) {
								theApp.m_IL2FlightData.Data[m_OldIndex].Active = false;
								m_OldIndex = CurIndex;
							}
							tmpData = VarTypeConversion(IL2DataArray[CurIndex].VarType,
														&theApp.m_IL2FlightData.Data[CurIndex].dDataRead);

//							tmpData = theApp.m_IL2FlightData.Data[CurIndex].dDataRead;

							if (IL2DataArray[CurIndex].VarType == xplmType_Char) 
								str.Format("%s", (char *)&theApp.m_IL2FlightData.Data[CurIndex].dDataRead);
							else
								str.Format("%5.4f", tmpData);

							m_ValueText.SetWindowText(str);
							TRACE("\n%5.4f\n", tmpData);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;

					case SIM_F4USIM:
						if (m_ListTypeVar != VARTYPE_BITS && theApp.m_pF4VarToken != NULL) {
							tmpData = VarTypeConversion(F4DataArray[CurIndex].VarType,
														theApp.m_pF4VarToken[CurIndex]);

							if (F4DataArray[CurIndex].VarType == xplmType_Char) 
								str.Format("%s", (char *)theApp.m_pF4VarToken[CurIndex]);
							else if (F4DataArray[CurIndex].VarType == xplmType_IntHex)
								str.Format("0x%00000000x", (UINT)tmpData);
							else
								str.Format("%5.4f", tmpData);

							m_ValueText.SetWindowText(str);
							TRACE("\nIndex %d : %5.4f\n", CurIndex, tmpData);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;

					case SIM_SIMBIN:
						if (theApp.m_pSimBinSimData != NULL) {
							tmpData = VarTypeConversion(SimBinDataArray[CurIndex].VarType,
														theApp.m_pSimBinVarToken[CurIndex]);

							if (SimBinDataArray[CurIndex].VarType == xplmType_Char) 
								str.Format("%s", (char *)theApp.m_pSimBinVarToken[CurIndex]);
							else
								str.Format("%5.4f", tmpData);

							m_ValueText.SetWindowText(str);
							TRACE("\n%5.4f\n", tmpData);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;

					case SIM_RF:
						if (theApp.m_pRFSimData != NULL) {
							tmpData = VarTypeConversion(RFDataArray[CurIndex].VarType,
														theApp.m_pRFVarToken[CurIndex]);

							if (RFDataArray[CurIndex].VarType == xplmType_Char) 
								str.Format("%s", (char *)theApp.m_pRFVarToken[CurIndex]);
							else
								str.Format("%5.4f", tmpData);

							m_ValueText.SetWindowText(str);
							TRACE("\n%5.4f\n", tmpData);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;


					case SIM_GPX:
						if (m_ListTypeVar != VARTYPE_BITS) {
							tmpData = VarTypeConversion(GPXDataArray[CurIndex].VarType,
														theApp.m_pGPXVarToken[CurIndex]);

							if (GPXDataArray[CurIndex].VarType == xplmType_Char) 
								str.Format("%s", (char *)theApp.m_pGPXVarToken[CurIndex]);
							else if (GPXDataArray[CurIndex].VarType == xplmType_ByteHex)
								str.Format("0x%00x", (char)tmpData);
							else
								str.Format("%5.4f", tmpData);

							m_ValueText.SetWindowText(str);
							TRACE("\n%5.4f\n", tmpData);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;

					case SIM_LFS:
						if (m_ListTypeVar != VARTYPE_BITS) {
							tmpData = VarTypeConversion(LFSDataArray[CurIndex].VarType,
														theApp.m_pLFSVarToken[CurIndex]);

							if (LFSDataArray[CurIndex].VarType == xplmType_Char) 
								str.Format("%s", (char *)theApp.m_pLFSVarToken[CurIndex]);
							else if (LFSDataArray[CurIndex].VarType == xplmType_IntHex)
								str.Format("0x%00000000x", (UINT)tmpData);
							else
								str.Format("%5.4f", tmpData);

							m_ValueText.SetWindowText(str);
							TRACE("\n%5.4f\n", tmpData);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;

					case USIM_INPUT_FLAGS:
						if (m_ListTypeVar != VARTYPE_BITS) {
							str.Format("%d", theApp.m_UserVarsArray.m_IOFlags[CurIndex].FlagValue);

							m_ValueText.SetWindowText(str);
							TRACE("\n%d\n", theApp.m_UserVarsArray.m_IOFlags[CurIndex].FlagValue);
						} else {
							m_ValueText.SetWindowText("");
						}
						break;
				}
			}
		} else {
			m_ValueText.SetWindowText("");
		}		
	} catch (CThreadException* pe) {
		// write to log file
		CString str;
		TRACE("VarListDlg - OnTimer error\n");
		str.Format("%susim.log", theApp.m_AppPath);
		FILE* stream = fopen(str, "a");
		fprintf(stream, "VarListDlg - OnTimer error\n");
		fclose(stream);

		if (!pe->GetErrorMsg().IsEmpty())
			pe->ReportError();
		pe->Delete();
	}
}

void CVarListDlg::OnClose() 
{
	SaveWindowRect("VarDlg Settings", true);

	CResizableDialog::OnClose();
}

void CVarListDlg::OnDestroy() 
{
	CResizableDialog::OnDestroy();
	
	SaveWindowRect("VarDlg Settings", true);
}

BOOL CVarListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// Don't exit when press Enter of Esc
	if(pMsg->message == WM_KEYDOWN)	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
	} else if(pMsg->message == WM_BT_CLICKED) {
		return TRUE;
	}
	return CResizableDialog::PreTranslateMessage(pMsg);
}
