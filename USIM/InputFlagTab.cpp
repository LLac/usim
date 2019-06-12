// InputFlagTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "InputFlagTab.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputFlagTab dialog


CInputFlagTab::CInputFlagTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CInputFlagTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputFlagTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CInputFlagTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputFlagTab)
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_GROUP1, m_OBGroup);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInputFlagTab, CResizableDialog)
	//{{AFX_MSG_MAP(CInputFlagTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
//	ON_BN_CLICKED(IDC_ADD, OnAdd)
//	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputFlagTab message handlers

BOOL CInputFlagTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	CRect r;

	m_OBGroup.AddControlToList (IDC_CLEAR);
//	m_OBGroup.AddControlToList (IDC_ADD);
//	m_OBGroup.AddControlToList (IDC_REMOVE);

	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_ImageList.Add(theApp.LoadIcon(IDI_OFF));		// 0
	m_ImageList.Add(theApp.LoadIcon(IDI_ON));		// 1
	m_ImageList.Add(theApp.LoadIcon(IDI_DOWN));		// 2
	m_ImageList.Add(theApp.LoadIcon(IDI_ON_RED));	// 3

    m_Grid.SetImageList(&m_ImageList);

	m_Grid.SetColumnCount(FLAG_NUM_COLS);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(1);
	m_Grid.SetFixedRowCount(1);

	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetListMode(FALSE);
	m_Grid.SetEditable(true);
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
	CString str[] = {"Flag", "Assign", " Flag Name ", " Input ID "};
	for (int i=0; i < FLAG_NUM_COLS; i++) {
		Item.col = i;
		Item.strText = str[i];
		m_Grid.SetItem(&Item);
		m_Grid.SetItemBkColour(0, i, RGB(220, 210, 245));
	}

	m_Grid.SetTextBkColor(RGB(240, 240, 240));
	m_Grid.AutoSize();

	m_Add.SetIcon(AfxGetApp()->LoadIcon(IDI_PLUS));
	m_Add.EnableWindow(false);
	m_Remove.SetIcon(AfxGetApp()->LoadIcon(IDI_MINUS));
	m_Remove.EnableWindow(false);

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_IONAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_GRID, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ADD, TOP_RIGHT);
	AddAnchor(IDC_REMOVE, TOP_RIGHT);

	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputFlagTab::InitInputFlagTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText("Input: ");
		m_Enable.SetWindowText(_T(""));

		OnClearButton();

		m_OBGroup.SetEnable(false);
	} else {
		CString str;
		int i;

		str.Format(_T("%s %s : Input %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		m_IOName.SetWindowText(str);
		
		OnClear();

		// Fill in Row data
		CGridCellCheck *pGridCellCheck;
		int nRow, nCol;
		for (i = 0; i < MAX_INPUT_FLAGS; i++) {
			nRow = m_Grid.InsertRow("");

			nCol = 0;
			str.Format(_T("Flag %i"), nRow);
			m_Grid.SetItemText(nRow, nCol, str); 
			m_Grid.SetItemBkColour(nRow, nCol, RGB(230, 230, 245));

			nCol = 1;	// Active
			m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck));
			pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(nRow, nCol);
			pGridCellCheck->SetCheck(theApp.m_UserVarsArray.m_IOFlags[i].Active);
			m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);

			nCol = 2;	// Assign to Flag
			m_Grid.SetItemText(nRow, nCol, theApp.m_UserVarsArray.m_IOFlags[i].VarTokenString);

			nCol = 3;	// Item Path
			m_Grid.SetItemText(nRow, nCol, theApp.m_UserVarsArray.m_IOFlags[i].ItemPathString);
			m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);
		}

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputActive[theApp.m_CurItemIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputDisabled[theApp.m_CurItemIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_OBGroup.SetEnable(true);

		m_Grid.AutoSize();
	}
}

void CInputFlagTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_UserVarsArray.ClearUserVarsData();

	OnClear();
	InitInputFlagTab();
}

void CInputFlagTab::OnClear() 
{
	m_Grid.DeleteNonFixedRows();
	m_Grid.Refresh();

	m_Add.EnableWindow(false);
	m_Remove.EnableWindow(false);
}

void CInputFlagTab::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CGridCellCheck *pGridCellCheck;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
	int i, j;

    *pResult = 0;

	if (nRow <= 0 || nCol <= 0) return;

	for (i = 1; i < m_Grid.GetRowCount(); i++) {
		for (j = 1; j < FLAG_NUM_COLS; j++) {
			if (i == nRow)
				m_Grid.SetItemBkColour(i, j, RGB(191, 212, 227));
			else
				m_Grid.SetItemBkColour(i, j, RGB(240, 240, 240));
		}
	}

	switch (nCol) {
		case 1:	// Assign
			pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(nRow, nCol);
			if (!theApp.m_UserVarsArray.m_IOFlags[nRow - 1].Active) {
				pGridCellCheck->SetCheck(true);
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].Active = true;
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].VarTokenString = m_Grid.GetItemText (nRow, 2);
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].ItemPathString.Format(	_T("%s %s : Input %i"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
																		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, theApp.m_CurItemIndex + 1);
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].ProductID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.ProductID;
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].IOIndex = (USHORT)theApp.m_CurItemIndex;
			} else {
				pGridCellCheck->SetCheck(false);
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].Active = false;
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].FlagValue = 0;
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].ItemPathString.Empty();
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].ProductID = 0x0000;
				theApp.m_UserVarsArray.m_IOFlags[nRow - 1].IOIndex = 0;
				m_Grid.SetItemText (nRow, 2, theApp.m_UserVarsArray.m_IOFlags[nRow - 1].VarTokenString);
			}
			
			m_Grid.SetItemText (nRow, 3, theApp.m_UserVarsArray.m_IOFlags[nRow - 1].ItemPathString);
			break;
	} // switch (nCol)

	m_Grid.AutoSize();
}


void CInputFlagTab::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	switch (nCol) {
		case 1:	// Assign
			break;

		case 2:	// Flag Name
			theApp.m_UserVarsArray.m_IOFlags[nRow - 1].VarTokenString = m_Grid.GetItemText (nRow, nCol);
			break;
	} // switch (nCol)

	m_Grid.AutoSize();
    *pResult = 0;
}

BOOL CInputFlagTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

