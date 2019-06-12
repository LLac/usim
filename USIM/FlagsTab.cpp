// FlagsTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "FlagsTab.h"
#include "NewCellTypes/GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlagsTab dialog


CFlagsTab::CFlagsTab(UINT id, CWnd* pParent /*=NULL*/)
	: CResizableDialog(id, pParent)
{
	//{{AFX_DATA_INIT(CFlagsTab)
	//}}AFX_DATA_INIT
}

void CFlagsTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlagsTab)
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_POWERFLAG, m_PowerFlag);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlagsTab, CResizableDialog)
	//{{AFX_MSG_MAP(CFlagsTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_POWERFLAG, OnPowerFlag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlagsTab message handlers

BOOL CFlagsTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	CRect r;
	m_PowerFlag.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_PowerFlag.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_PowerFlag.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_PowerFlag.GetWindowRect(r);
	ScreenToClient(r);
	m_PowerFlag.MoveWindow(r.left, r.top, 15, 15);

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
	m_Grid.SetEditable(false);
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
	CString str[] = {"Flag", "Monitor", " Flag Name ", " Input ID "};
	int i;
	for (i=0; i < FLAG_NUM_COLS; i++) {
		Item.col = i;
		Item.strText = str[i];
		m_Grid.SetItem(&Item);
		m_Grid.SetItemBkColour(0, i, RGB(220, 210, 245));
	}

	for (i = 0; i < MAX_INPUT_FLAGS; i++)
		m_Grid.InsertRow("");

	m_Grid.SetTextBkColor(RGB(240, 240, 240));
	m_Grid.AutoSize();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_IONAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_GRID, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_STATIC2, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_POWERFLAG, BOTTOM_LEFT);
	AddAnchor(IDC_STATIC3, BOTTOM_LEFT);

	AddAnchor(IDC_STATIC4, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlagsTab::InitFlagsTab() 
{
	int i;
	CString str;

	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		str.Format(_T("%s : "), m_StrIO);
		m_IOName.SetWindowText(str);
		m_Enable.SetWindowText(_T(""));

		OnClearButton();
	} else {
		// Fill in Row data
		CGridCellCheck *pGridCellCheck;
		int nRow, nCol;
		for (i = 0; i < MAX_INPUT_FLAGS; i++) {
			nRow = i+1;

			nCol = 0;
			str.Format(_T("Flag %i"), nRow);
			m_Grid.SetItemText(nRow, nCol, str); 
			m_Grid.SetItemBkColour(nRow, nCol, RGB(230, 230, 245));

			nCol = 1;	// Active
			m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck));
			pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(nRow, nCol);
			pGridCellCheck->SetCheck(m_pIOArray->Active[i]);
			
			nCol = 2;	// Assign to Flag
			m_Grid.SetItemText(nRow, nCol, theApp.m_UserVarsArray.m_IOFlags[i].VarTokenString);
			
			nCol = 3;	// Item Path
			m_Grid.SetItemText(nRow, nCol, theApp.m_UserVarsArray.m_IOFlags[i].ItemPathString);
		}

		m_PowerFlag.SetCheck(m_pIOArray->m_PowerFlag);

		m_Notes.SetWindowText(_T(""));
		m_Notes.SetWindowText(m_pIOArray->m_Notes);

		if (*m_pIOActive) {
			if (*m_pIODisabled) {
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

void CFlagsTab::OnPowerFlag() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	if (m_PowerFlag.GetCheck()) {
		m_pIOArray->m_PowerFlag = 1;
	} else {
		m_pIOArray->m_PowerFlag = 0;
	}
}

void CFlagsTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(m_pIOArray->m_Notes);
}

void CFlagsTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_pIOArray->Clear();

	m_Notes.SetWindowText(_T(""));
	OnChangeNotes();

	m_PowerFlag.SetCheck(0);
	OnPowerFlag();

	InitFlagsTab();
}

BOOL CFlagsTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

void CFlagsTab::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
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
			if (!m_pIOArray->Active[nRow - 1]) {
				pGridCellCheck->SetCheck(true);
				m_pIOArray->Active[nRow - 1] = true;
			} else {
				pGridCellCheck->SetCheck(false);
				m_pIOArray->Active[nRow - 1] = false;
			}
			break;
	} // switch (nCol)

	m_Grid.Refresh();
}
