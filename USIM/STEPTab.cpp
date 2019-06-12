// STEPTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "STEPTab.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTEPTab dialog


CSTEPTab::CSTEPTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSTEPTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSTEPTab)
	//}}AFX_DATA_INIT
}

void CSTEPTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTEPTab)
	DDX_Control(pDX, IDC_FLIGHTDATA, m_FlightData);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_OUTPUTNAME, m_IOName);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_TEXT1, m_Data);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_LAMPTEST, m_STEPTest);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_STATIC0, m_Static0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTEPTab, CResizableDialog)
	//{{AFX_MSG_MAP(CSTEPTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_LAMPTEST, OnSTEPTest)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	ON_BN_CLICKED(IDC_FLIGHTDATA, OnFlightData)
	ON_BN_CLICKED(IDC_CLEARDATA, OnClearData)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEPTab message handlers

BOOL CSTEPTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	CRect r;
	m_STEPTest.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_STEPTest.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_STEPTest.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_STEPTest.GetWindowRect(r);
	ScreenToClient(r);
	m_STEPTest.MoveWindow(r.left, r.top, 15, 15);

	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_ImageList.Add(theApp.LoadIcon(IDI_OFF));		// 0
	m_ImageList.Add(theApp.LoadIcon(IDI_ON));		// 1
	m_ImageList.Add(theApp.LoadIcon(IDI_DOWN));		// 2
	m_ImageList.Add(theApp.LoadIcon(IDI_ON_RED));	// 3

    m_Grid.SetImageList(&m_ImageList);

	m_Grid.SetColumnCount(STEP_NUM_COLS);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(1);
	m_Grid.SetFixedRowCount(1);

	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetListMode(FALSE);
	//m_Grid.SetEditable(true);
	m_Grid.EnableSelection(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetColumnResize(FALSE);
	m_Grid.EnableColumnHide(TRUE);
	m_Grid.EnableHiddenColUnhide(FALSE);
	m_Grid.EnableRowHide(FALSE);

	// Set Fixed Row 0
	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER;
	Item.row = 0;
	CString str[] = {"Section", "Formula", "Min Value", "Max Value", "STEP Min", "STEP Max", "STEP Offset", "Cal"};
	for (int i=0; i < STEP_NUM_COLS; i++) {
		Item.col = i;
		Item.strText = str[i];
		m_Grid.SetItem(&Item);
		m_Grid.SetItemBkColour(0, i, RGB(220, 210, 245));
	}

	m_CurrentRow = NULL;
	m_Add.SetIcon(AfxGetApp()->LoadIcon(IDI_PLUS));
	m_Add.EnableWindow(true);
	m_Remove.SetIcon(AfxGetApp()->LoadIcon(IDI_MINUS));
	m_Remove.EnableWindow(false);

	m_Grid.SetTextBkColor(RGB(240, 240, 240));
	m_Grid.AutoSize();

	InitSTEPTab();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_OUTPUTNAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_FLIGHTDATA, TOP_LEFT);
	AddAnchor(IDC_TEXT1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CLEARDATA, TOP_RIGHT);
	AddAnchor(IDC_GRID, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ADD, TOP_RIGHT);
	AddAnchor(IDC_REMOVE, TOP_RIGHT);

	AddAnchor(IDC_STATIC2, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LAMPTEST, BOTTOM_LEFT);
	AddAnchor(IDC_STATIC6, BOTTOM_LEFT);

	AddAnchor(IDC_STATIC3, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSTEPTab::InitSTEPTab() 
{
	int j;

	m_Static0.SetWindowText("Stepper Output"); 
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("STEP : "));
		m_Enable.SetWindowText(_T(""));
		m_Data.SetWindowText(_T(""));

		OnClearButton();
	} else {
		CString str, sPort, sID;
		str.Format("%s : %s",
			theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPhItem[theApp.m_CurItemIndex]),
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPNotesArray[theApp.m_CurItemIndex].m_Desc);

		theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPhItem[theApp.m_CurItemIndex], str);

		// KLUDGE for PLV2
		sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
		sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem)));
		if (sPort.Find(sID) != -1) {
			str.Format(_T("%s %s : STEP %s"), sID,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
				theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		} else {
			str.Format(_T("%s %s : %s - STEP %s"), sID,
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
				sPort,
				theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		}
		m_IOName.SetWindowText(str);

		str.Empty();
		switch (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_SimType) {
			case SIM_XPLANE:
				if (XPDataArray[theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenIndex].VarArraySize > 0) {
					str.Format("X-Plane/%s[%d]", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex + 1);
				} else {
					str.Format("X-Plane/%s", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName);
				}

				break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_F4USIM:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_SIMBIN:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_RF:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_GPX:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case SIM_LFS:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;

			case USIM_INPUT_FLAGS:
				str = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
				break;
		}
		m_Data.SetWindowText(str);

		// Fill in Row data
		UINT nRow, nCol;
		OnClear();
		for (j = 0; j < theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Sections; j++) {
			// call function to set grid combo
			nRow = AddRow();
			nCol = 1;
			m_Grid.SetItemText(nRow, nCol, theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Formula[j]);
			nCol = 2;
			str.Format("%6.3f", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MinValue[j]);
			m_Grid.SetItemText(nRow, nCol, str);
			nCol = 3;
			str.Format("%6.3f", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MaxValue[j]);
			m_Grid.SetItemText(nRow, nCol, str);
			nCol = 4;
			str.Format("%d", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMin[j]);
			m_Grid.SetItemText(nRow, nCol, str);
			nCol = 5;
			str.Format("%d", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[j]);
			m_Grid.SetItemText(nRow, nCol, str);
			nCol = 6;
			str.Format("%d", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[j]);
			m_Grid.SetItemText(nRow, nCol, str);
		}

		m_CurrentRow = NULL;
		m_STEPTest.SetCheck(0);

		m_Notes.SetWindowText(_T(""));
		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Notes);

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPActive[theApp.m_CurItemIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPDisabled[theApp.m_CurItemIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Grid.AutoSize();
	}
}

void CSTEPTab::OnFlightData()
{
	if (theApp.m_CurrenthItem == NULL) return;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	m_VarListDlg.m_ListTypeVar = VARTYPE_VAR_ONLY;

	m_VarListDlg.m_VarTokenName = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName;
	m_VarListDlg.m_VarArrayIndex = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex;

	int nResponse = m_VarListDlg.DoModal();

	if (nResponse == IDOK) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName = m_VarListDlg.m_VarTokenName;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenIndex = m_VarListDlg.m_ArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex = m_VarListDlg.m_VarArrayIndex;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_SimType = m_VarListDlg.m_SimType;

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

void CSTEPTab::OnClearData()
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenName.Empty();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_VarArrayTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_SimType = SIM_NONE;
	m_Data.SetWindowText("");
}

void CSTEPTab::OnSTEPTest() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	if (m_STEPTest.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPTest = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPTest = 0;
	}
}

void CSTEPTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Notes);
}

void CSTEPTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].Clear();

	OnClear();

	m_Data.SetWindowText(_T(""));

	m_Notes.SetWindowText(_T(""));
	OnChangeNotes();

	m_STEPTest.SetCheck(0);
	OnSTEPTest();

	m_Grid.Refresh();
}

void CSTEPTab::OnClear() 
{
	m_Grid.DeleteNonFixedRows();
	m_Grid.Refresh();

	m_Add.EnableWindow(true);
	m_Remove.EnableWindow(false);

	m_Notes.SetWindowText(_T(""));
}

BOOL CSTEPTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

UINT CSTEPTab::AddRow() 
{
	if (m_Grid.GetRowCount() >= STEP_NUM_ROWS) {
		m_Add.EnableWindow(false);
		return 0;
	}

	UINT nRow = m_Grid.InsertRow("");
	CString str;
	str.Format(_T("%i"), nRow);
	m_Grid.SetItemText(nRow, 0, str); 
	m_Grid.SetItemBkColour(nRow, 0, RGB(230, 230, 245));

	// Column FORMULA
	int nCol = 1;
//	m_Grid.SetItemText(nRow, 0, "x");

	// Column MINIMUM
	nCol = 2;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Real | CGridCellNumeric::Negative | CGridCellNumeric::Silent);
	theApp.m_nFormat.NumDigits = 3;
	theApp.m_nFormat.LeadingZero = 1;
	theApp.m_nFormat.Grouping = 6;
	theApp.m_nFormat.lpDecimalSep = ".";
	theApp.m_nFormat.lpThousandSep = "";
	theApp.m_nFormat.NegativeOrder = 1;
	pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);

	// Column MAXIMUM
	nCol = 3;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Real | CGridCellNumeric::Negative | CGridCellNumeric::Silent);
	pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);

	// Column STEP MIN
	nCol = 4;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Integer | CGridCellNumeric::Negative | CGridCellNumeric::Silent);

	// Column STEP MAX
	nCol = 5;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Integer | CGridCellNumeric::Negative | CGridCellNumeric::Silent);

	// Column STEP OFFSET
	nCol = 6;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Integer | CGridCellNumeric::Negative | CGridCellNumeric::Silent);

	// Column STEP CAL
	nCol = 7;
	GV_ITEM Item;
	Item.mask = GVIF_IMAGE | GVIF_FORMAT | GVIF_PARAM;
	Item.lParam = 0;
	Item.nFormat = DT_CENTER | DT_VCENTER;
	Item.iImage = 0;
	Item.col = nCol;
	Item.row = nRow;
	m_Grid.SetItem(&Item);

	m_Remove.EnableWindow(true);

	m_Grid.AutoSize();

	return nRow;
}

void CSTEPTab::OnAdd() 
{
	UINT nRow = AddRow();
	if (nRow == 0) return;

	CGridCellNumeric* pGridCellNumeric;
	CString str;

	// Column FORMULA
	int nCol = 1;
	m_Grid.SetItemText(nRow, nCol, "(x)");
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Formula[nRow-1] = "(x)";

	// Column MINIMUM
	nCol = 2;
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	if (nRow > 1) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MinValue[nRow-1] = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MaxValue[nRow-2];
		// fix for double conversion
		str.Format("%6.3f", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MaxValue[nRow-2]);
		pGridCellNumeric->SetText(str);
	} else {
		str.Format("%6.3f", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MaxValue[nRow-2]);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MinValue[nRow-1] = 0;
		pGridCellNumeric->SetNumber(0);
	}

	// Column MAXIMUM
	nCol = 3;
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetNumber (999999);
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MaxValue[nRow-1] = 999999;

	// Column STEP MIN
	nCol = 4;
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	if (nRow > 1) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMin[nRow-1] = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[nRow-2];
		pGridCellNumeric->SetNumber(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[nRow-2]);
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMin[nRow-1] = 0;
		pGridCellNumeric->SetNumber(0);
	}

	// Column STEP MAX
	nCol = 5;
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetNumber (0);
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[nRow-1] = 0;

	// Column STEP OFFSET
	nCol = 6;
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	
	if (nRow > 1) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-1] = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-2];
		pGridCellNumeric->SetNumber (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-1]);
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-1] = 0;
		pGridCellNumeric->SetNumber (0);
	}
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Sections = (UCHAR)nRow;
	m_CurrentRow = nRow;

	m_Grid.AutoSize();
}

void CSTEPTab::OnRemove() 
{
	if (m_Grid.IsValid(m_CurrentRow, 0) && m_CurrentRow != 0) {
		m_Grid.DeleteRow(m_CurrentRow);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Sections = (UCHAR)(m_Grid.GetRowCount() - 1);

		m_CurrentRow = NULL;
		CString str;
		for (int nRow = 1; nRow < m_Grid.GetRowCount(); nRow++) {
			str.Format(_T("%i"), nRow);
			m_Grid.SetItemText(nRow, 0, str);
		}
		m_Add.EnableWindow(true);

		if (m_Grid.GetRowCount() == 1) {
			m_Remove.EnableWindow(false);
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Sections = 0;
		} else {
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Sections = (UCHAR)(m_Grid.GetRowCount() - 1);
			// Fill in data
			NM_GRIDVIEW pItem;
			LRESULT pResult;
			for (pItem.iRow = 1; pItem.iRow < m_Grid.GetRowCount(); pItem.iRow++) { // rows
				for (pItem.iColumn = 1; pItem.iColumn < 7; pItem.iColumn++) {	// cols
					OnGridEndEdit((tagNMHDR *)&pItem, &pResult);
				}
			}
		}
	}

	m_Grid.Refresh(); 	
}

void CSTEPTab::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
	int i, j, retval;

	*pResult = 0;

	m_CurrentRow = pItem->iRow;
	

	if (nRow <= 0 || nCol <= 0) return;

//	CGridCellCheck* pGridCellCheck;

	for (i = 1; i < m_Grid.GetRowCount(); i++) {
		for (j = 1; j < STEP_NUM_COLS; j++) {
			if (i == nRow)
				m_Grid.SetItemBkColour(i, j, RGB(191, 212, 227));
			else
				m_Grid.SetItemBkColour(i, j, RGB(240, 240, 240));
		}
	}

	switch (nCol) {
		case 1:	// Column FORMULA
			break;
		case 2:	// Column MIN
			break;
		case 3:	// Column MAX
			break;
		case 4:	// Column STEP MIN
			break;
		case 5:	// Column STEP MAX
			break;
		case 6:	// Column STEP OFFSET
			break;
		case 7:	// Column STEP CAL
			if ( m_Grid.GetItemData(nRow, nCol) == 0 ) {
				m_Grid.SetItemData(nRow, nCol, 3);
				m_Grid.SetItemImage(nRow, nCol, 3); // RED ON icon

				m_CalSTEPDlg.m_ResMax = LONG_MAX;
				m_CalSTEPDlg.m_ResMin = LONG_MIN;
				m_CalSTEPDlg.m_pTemp = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPTemp;
				m_CalSTEPDlg.m_pCalibrate = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPCalibrate;
				m_CalSTEPDlg.m_pMin = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMin[nRow-1];
				m_CalSTEPDlg.m_pMax = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[nRow-1];
				m_CalSTEPDlg.m_pOffset = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-1];

				retval = m_CalSTEPDlg.DoModal();
				if (retval) {
					// update values
					CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, 4);
					pGridCellNumeric->SetNumber(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMin[nRow-1]);
					pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, 5);
					pGridCellNumeric->SetNumber(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[nRow-1]);
					pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, 6);
					pGridCellNumeric->SetNumber(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-1]);
				}
				m_Grid.SetItemData(nRow, nCol, 0);
				m_Grid.SetItemImage(nRow, nCol, 0);
			}
			break;
	} // switch (nCol)

	m_Grid.AutoSize();
}

void CSTEPTab::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;
	double dNum;
	CString str;

	CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);

	dNum = pGridCellNumeric->GetNumber();
	switch (nCol) {
		case 1:	// Col FORMULA
			str = m_Grid.GetItemText(nRow, nCol);
			if (str.IsEmpty()) str = "x";
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_Formula[nRow-1] = str;
			break;

		case 2:	// Col MIN
			if (dNum > 999999) {pGridCellNumeric->SetNumber (999999); dNum = 999999;}
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MinValue[nRow-1] = (float)dNum;
			break;

		case 3:	// Col MAX
			if (dNum > 999999) {pGridCellNumeric->SetNumber (999999); dNum = 999999;}
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_MaxValue[nRow-1] = (float)dNum;
			break;

		case 4:	// Col STEP MIN
			if (dNum > LONG_MAX ) {pGridCellNumeric->SetNumber (LONG_MAX); dNum = LONG_MAX;}
			else if (dNum < LONG_MIN ) {pGridCellNumeric->SetNumber (LONG_MIN); dNum = LONG_MIN;}
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMin[nRow-1] = (long)dNum;
			break;

		case 5:	// Col STEP MAX
			if (dNum > LONG_MAX) {pGridCellNumeric->SetNumber (LONG_MAX); dNum = LONG_MAX;}
			else if (dNum < LONG_MIN) {pGridCellNumeric->SetNumber (LONG_MIN); dNum = LONG_MIN;}
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPMax[nRow-1] = (long)dNum;
			break;

		case 6:	// Col STEP OFFSET
			if (dNum > LONG_MAX ) {pGridCellNumeric->SetNumber (LONG_MAX); dNum = LONG_MAX;}
			else if (dNum < LONG_MIN ) {pGridCellNumeric->SetNumber (LONG_MIN); dNum = LONG_MIN;}
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPOffset[nRow-1] = (long)dNum;
			break;

		case 7:	// Col STEP CAL
			break;

	} // switch (nCol)

	m_Grid.AutoSize();

    *pResult = 0;
}
