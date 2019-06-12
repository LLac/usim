// SendKeysTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "SendKeysTab.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendKeysTab dialog

CSendKeysTab::CSendKeysTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSendKeysTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendKeysTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSendKeysTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendKeysTab)
	DDX_Control(pDX, IDC_HOLDKEY, m_HoldKey);
	DDX_Control(pDX, IDC_IONAME, m_IOName);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_P_ADD, m_P_Add);
	DDX_Control(pDX, IDC_P_REMOVE, m_P_Remove);
	DDX_Control(pDX, IDC_P_CLEAR, m_P_Clear);
	DDX_Control(pDX, IDC_R_ADD, m_R_Add);
	DDX_Control(pDX, IDC_R_REMOVE, m_R_Remove);
	DDX_Control(pDX, IDC_R_CLEAR, m_R_Clear);
	DDX_Control(pDX, IDC_ONPRESS, m_P_Frame);
	DDX_Control(pDX, IDC_ONRELEASE, m_R_Frame);
	DDX_Control(pDX, IDC_REPEAT, m_Repeat);
	DDX_Control(pDX, IDC_CLEAR, m_Clear);
	DDX_Control(pDX, IDC_PRESS_REC, m_PressRec);
	DDX_Control(pDX, IDC_RELEASE_REC, m_ReleaseRec);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_GROUP1, m_SendKeysGroup);
	DDX_Control(pDX, IDC_P_GRID, m_PressGrid);
	DDX_Control(pDX, IDC_R_GRID, m_ReleaseGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSendKeysTab, CResizableDialog)
	//{{AFX_MSG_MAP(CSendKeysTab)
	ON_BN_CLICKED(IDC_HOLDKEY, OnHoldKey)
	ON_BN_CLICKED(IDC_P_ADD, OnPAdd)
	ON_BN_CLICKED(IDC_P_REMOVE, OnPRemove)
	ON_BN_CLICKED(IDC_P_CLEAR, OnPClear)
	ON_BN_CLICKED(IDC_R_ADD, OnRAdd)
	ON_BN_CLICKED(IDC_R_REMOVE, OnRRemove)
	ON_BN_CLICKED(IDC_R_CLEAR, OnRClear)
	ON_BN_CLICKED(IDC_REPEAT, OnRepeat)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_PRESS_REC, OnRecord)
	ON_BN_CLICKED(IDC_RELEASE_REC, OnRecord)
	ON_NOTIFY(NM_CLICK, IDC_P_GRID, OnPGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_P_GRID, OnPGridEndEdit)
	ON_NOTIFY(NM_CLICK, IDC_R_GRID, OnRGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_R_GRID, OnRGridEndEdit)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendKeysTab message handlers

void CSendKeysTab::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CResizableDialog::OnShowWindow(bShow, nStatus);
	
	KeyMacroRecord(false);
}

void CSendKeysTab::KeyMacroRecord(BOOL RecFlag) 
{
	if (RecFlag) {
		m_KeyHook.HookProc();
		m_PressGrid.SetEditable(false);
		m_ReleaseGrid.SetEditable(false);

		// disable menu bar
		CWnd *pMain = AfxGetMainWnd();
		CMenu *pMenu = pMain->GetMenu();
		int max = pMenu->GetMenuItemCount();
		for (int i = 0; i < max; i++)
			pMenu->EnableMenuItem(i, MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
		pMain->DrawMenuBar();
	} else {
		m_KeyHook.UnhookProc();
		m_PressGrid.SetEditable(true);
		m_ReleaseGrid.SetEditable(true);
		m_PressRec.SetCheck(false);
		m_ReleaseRec.SetCheck(false);

		// enable menu bar
		CWnd *pMain = AfxGetMainWnd();
		CMenu *pMenu = pMain->GetMenu();
		int max = pMenu->GetMenuItemCount();
		for (int i = 0; i < max; i++)
			pMenu->EnableMenuItem(i, MF_BYPOSITION|MF_ENABLED);
		pMain->DrawMenuBar();
	}
}

CSendKeysTab::~CSendKeysTab()
{

}

BOOL CSendKeysTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	CRect rect;

	m_SendKeysGroup.AddControlToList (IDC_REPEAT);
	m_SendKeysGroup.AddControlToList (IDC_CLEAR);
	m_SendKeysGroup.AddControlToList (IDC_NOTES);

	m_PressRec.SetSSLButtonStyle(SSL_BS_AUTOSIZE);	
	m_PressRec.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(255, 0, 0));
	m_PressRec.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_PressRec.GetWindowRect(rect);
	ScreenToClient(rect);
	m_PressRec.MoveWindow(rect.left, rect.top, 20, 20);

	m_ReleaseRec.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_ReleaseRec.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(255, 0, 0));
	m_ReleaseRec.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_ReleaseRec.GetWindowRect(rect);
	ScreenToClient(rect);
	m_ReleaseRec.MoveWindow(rect.left, rect.top, 20, 20);

	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_ImageList.Add(theApp.LoadIcon(IDI_OFF));		// 0
	m_ImageList.Add(theApp.LoadIcon(IDI_ON));		// 1
	m_ImageList.Add(theApp.LoadIcon(IDI_DOWN));		// 2
	m_ImageList.Add(theApp.LoadIcon(IDI_ON_RED));	// 3

	m_P_Add.SetIcon(AfxGetApp()->LoadIcon(IDI_PLUS));
	m_P_Remove.SetIcon(AfxGetApp()->LoadIcon(IDI_MINUS));
	m_R_Add.SetIcon(AfxGetApp()->LoadIcon(IDI_PLUS));
	m_R_Remove.SetIcon(AfxGetApp()->LoadIcon(IDI_MINUS));

	CreateGrid(&m_PressGrid);
	CreateGrid(&m_ReleaseGrid);

	InitSendKeysTab();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);

	AddAnchor(IDC_ONPRESS, TOP_LEFT, MIDDLE_RIGHT);
	AddAnchor(IDC_P_GRID, TOP_LEFT, MIDDLE_RIGHT);
	AddAnchor(IDC_PRESS_REC, TOP_RIGHT);
	AddAnchor(IDC_P_CLEAR, TOP_RIGHT);
	AddAnchor(IDC_P_ADD, TOP_RIGHT);
	AddAnchor(IDC_P_REMOVE, TOP_RIGHT);

	AddAnchor(IDC_ONRELEASE, MIDDLE_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_R_GRID, MIDDLE_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_RELEASE_REC, MIDDLE_RIGHT);
	AddAnchor(IDC_R_CLEAR, MIDDLE_RIGHT);
	AddAnchor(IDC_R_ADD, MIDDLE_RIGHT);
	AddAnchor(IDC_R_REMOVE, MIDDLE_RIGHT);

	AddAnchor(IDC_REPEAT, MIDDLE_LEFT);
	AddAnchor(IDC_HOLDKEY, MIDDLE_RIGHT);

	AddAnchor(IDC_STATIC1, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSendKeysTab::InitSendKeysTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText("Input: ");
		m_Enable.SetWindowText(_T(""));

		m_PressRec.EnableWindow(false);
		m_P_Add.EnableWindow(false);
		m_P_Remove.EnableWindow(false);
		m_P_Clear.EnableWindow(false);
		m_ReleaseRec.EnableWindow(false);
		m_R_Add.EnableWindow(false);
		m_R_Remove.EnableWindow(false);
		m_R_Clear.EnableWindow(false);

		OnClear();

		m_SendKeysGroup.SetEnable(false);
	} else {
		CString str;

		str.Format(_T("%s %s : Input %s"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName,
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
			theApp.m_pTreeDevices->GetItemText(theApp.m_CurrenthItem));
		m_IOName.SetWindowText(str);
		
		SetIOStatus();

		m_SendKeysGroup.SetEnable(true);
		m_Repeat.EnableWindow(true);
		m_PressRec.EnableWindow(true);
		m_P_Add.EnableWindow(true);
		m_P_Remove.EnableWindow(true);
		m_P_Clear.EnableWindow(true);
		m_ReleaseRec.EnableWindow(true);
		m_R_Add.EnableWindow(true);
		m_R_Remove.EnableWindow(true);
		m_R_Clear.EnableWindow(true);

		OnClear();

		// Set all data from DevArray
		/****************************/

		CSendKeysPage *pSendArray = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
		
		LoadData(&m_PressGrid, pSendArray->m_OPKeyMacros, PRESSGRID);
		LoadData(&m_ReleaseGrid, pSendArray->m_ORKeyMacros, RELEASEGRID);

		m_HoldKey.SetCheck(pSendArray->m_OPHoldKey);
		m_Repeat.SetCheck(pSendArray->m_OPRepeat);
		if (pSendArray->m_OPHoldKey) m_Repeat.EnableWindow(false);
		m_Notes.SetWindowText(pSendArray->m_Notes);

		/****************************/
	}
}

void CSendKeysTab::SetIOStatus()
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

void CSendKeysTab::LoadData(CGridCtrl *pGrid, TAGKEYMACRO *KeyMacros, UINT GridType)
{
	int nRow;
	CGridCellCheck *pGridCellCheck;
	CGridCellNumeric *pCell;
	//CString str;

	for (UINT i = 0; i<MAX_MACROS; i++) {
		if (KeyMacros[i].iVKCode == 0) continue;

		if (GridType == PRESSGRID)
			OnPAdd();
		else
			OnRAdd();

		nRow = pGrid->GetRowCount() - 1;

		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, 1);
		if (KeyMacros[i].iNum != 0)
			pGridCellCheck->SetCheck(true);
		else
			pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(KeyMacros[i].iAlt);

		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, 2);
		if (KeyMacros[i].iCaps != 0)
			pGridCellCheck->SetCheck(true);
		else
			pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(KeyMacros[i].iShift);

		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, 3);
		if (KeyMacros[i].iScroll != 0)
			pGridCellCheck->SetCheck(true);
		else
			pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(KeyMacros[i].iCtrl);

		pGrid->SetItemText(nRow, 4, KeyMacros[i].iMacroString);
		pGrid->SetItemData(nRow, 4, KeyMacros[i].iVKCode);

		pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, 5);
		pGrid->SetItemData (nRow, 5, KeyMacros[i].iHoldDelay);
		//str.Format("%d", KeyMacros[i].iHoldDelay);
		//pGrid->SetItemText(nRow, 5, str);
		pCell->SetNumber(KeyMacros[i].iHoldDelay);

		pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, 6);
		pGrid->SetItemData (nRow, 6, KeyMacros[i].iMacroDelay);
		//str.Format("%d", KeyMacros[i].iMacroDelay);
		//pGrid->SetItemText(nRow, 6, str);
		pCell->SetNumber(KeyMacros[i].iMacroDelay);

		pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, 7);
		pGrid->SetItemData (nRow, 7, KeyMacros[i].iSKCode);
	}

	pGrid->AutoSize();
}

void CSendKeysTab::OnRecord() 
{
	if (!m_PressRec.GetCheck() && !m_ReleaseRec.GetCheck()) {
		KeyMacroRecord(false);
	} else {
		KeyMacroRecord(true);
	}
}

void CSendKeysTab::RecordKey(tagKeyResultA* lpKeyResult, CString ComboKeys)
{
	// Called from KeyHook Callback

	CString	RecordKeys;

	if (lpKeyResult->iVKCode == 0) return;

	if (ComboKeys != "") {
		RecordKeys.Format(_T("%s + %s"), ComboKeys, lpKeyResult->szKeyName);
	} else {
		RecordKeys.Format(_T("%s"), lpKeyResult->szKeyName);
	}

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	if (m_PressRec.GetCheck()) {
		RecordData(&m_PressGrid, pSendKeys->m_OPKeyMacros, lpKeyResult, RecordKeys);
	}
	
	if (m_ReleaseRec.GetCheck()) {
		RecordData(&m_ReleaseGrid, pSendKeys->m_ORKeyMacros, lpKeyResult, RecordKeys);
	}

	lpKeyResult->iVKCode = 0;
	lpKeyResult->iSKCode = 0;
	lpKeyResult->szKeyName[0] = '\0';
}

void CSendKeysTab::RecordData(CGridCtrl *pGrid, TAGKEYMACRO *KeyMacros, tagKeyResultA* lpKeyResult, CString RecordKeys)
{
	CCellID nCell;
	CGridCellCheck* pGridCellCheck;

	nCell = pGrid->GetFocusCell();
	if (nCell.IsValid()) {
		pGrid->SetItemText(nCell.row, 4, RecordKeys);
		pGrid->RedrawRow(nCell.row);

		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nCell.row, 1);
		if (lpKeyResult->iNum != 0)
			pGridCellCheck->SetCheck(true);
		else
			pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(lpKeyResult->iAlt);

		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nCell.row, 2);
		if (lpKeyResult->iCaps != 0)
			pGridCellCheck->SetCheck(true);
		else
			pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(lpKeyResult->iShift);

		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nCell.row, 3);
		if (lpKeyResult->iScroll != 0)
			pGridCellCheck->SetCheck(true);
		else
			pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(lpKeyResult->iCtrl);

		pGrid->SetItemData(nCell.row, 4, lpKeyResult->iVKCode);

		pGrid->SetItemData(nCell.row, 7, lpKeyResult->iSKCode);

		pGrid->SetItemData(nCell.row, 8, lpKeyResult->iExtended);

		pGrid->AutoSize();

		SaveToSendKeysArray(pGrid, KeyMacros, nCell.row);
	}
}

void CSendKeysTab::SaveToSendKeysArray(CGridCtrl *pGrid, TAGKEYMACRO *KeyMacros, UINT i)
{
	CGridCellCheck *pGridCellCheck;
	UINT startidx, endidx;

	if (i == NULL) {
		// save all rows
		startidx = 1; 
		endidx = MAX_MACROS + 1;
	} else {
		// save single row
		startidx = i; 
		endidx = i + 1;
	}

	if ((int)endidx > pGrid->GetRowCount()) 
		endidx = (UINT)pGrid->GetRowCount();

	for (UINT nRow = startidx; nRow < endidx; nRow++) {	
		// Column 1 NUM LOCK Checkbox
		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, 1);
		if (pGridCellCheck->GetCheck()) {
			KeyMacros[nRow - 1].iNum = VK_NUMLOCK;
		} else {
			KeyMacros[nRow - 1].iNum = 0;
		}
		KeyMacros[nRow - 1].iAlt = (UCHAR)pGridCellCheck->GetData();

		// Column 2 Caps Lock Checkbox
		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, 2);
		if (pGridCellCheck->GetCheck()) {
			KeyMacros[nRow - 1].iCaps = VK_CAPITAL;
		} else {
			KeyMacros[nRow - 1].iCaps= 0;
		}
		KeyMacros[nRow - 1].iShift = (UCHAR)pGridCellCheck->GetData();

		// Column 3 Scroll Lock Checkbox
		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, 3);
		if (pGridCellCheck->GetCheck()) {
			KeyMacros[nRow - 1].iScroll = VK_SCROLL;
		} else {
			KeyMacros[nRow - 1].iScroll = 0;
		}
		KeyMacros[nRow - 1].iCtrl = (UCHAR)pGridCellCheck->GetData();

		// Column 4 Key Macro
		KeyMacros[nRow - 1].iMacroString = pGrid->GetItemText (nRow, 4);
		KeyMacros[nRow - 1].iVKCode = (UCHAR)pGrid->GetItemData (nRow, 4);

		// Column 5 Hold Delay TextBox
		KeyMacros[nRow - 1].iHoldDelay = pGrid->GetItemData (nRow, 5);

		// Column 6 Macro Delay TextBox
		KeyMacros[nRow - 1].iMacroDelay = pGrid->GetItemData (nRow, 6);

		// Column 7 hidden
		KeyMacros[nRow - 1].iSKCode = (USHORT)pGrid->GetItemData (nRow, 7);
		
		// Column 7 hidden
		KeyMacros[nRow - 1].iExtended = (UCHAR)pGrid->GetItemData (nRow, 8);

		TRACE ("SaveToSendKeysArray nCode %d, vCode %d\n", KeyMacros[nRow - 1].iSKCode, KeyMacros[nRow - 1].iVKCode);
	}
}

void CSendKeysTab::CreateGrid(CGridCtrl *pGrid)
{
	GV_ITEM Item;
	int i;
	//																		   ALT+SHIFT+CTRL+PAGE DOWN
	CString	str[SK_NUM_COLS]={"   ", "Num Lock", "Caps Lock", "Scroll Lock", "Key Macro", "Hold Delay (ms)", "Macro Delay (ms)"};

	// fill it up with stuff
	pGrid->SetImageList(&m_ImageList);

	pGrid->SetColumnCount(SK_NUM_COLS + 2);
	pGrid->SetFixedColumnCount(1);
	pGrid->SetRowCount(1);
	pGrid->SetFixedRowCount(1);

	pGrid->SetGridLines(GVL_BOTH);
	pGrid->SetListMode(FALSE);
	//pGrid->SetEditable(true);
	pGrid->EnableSelection(FALSE);
	pGrid->EnableTitleTips(FALSE);
	pGrid->EnableDragAndDrop(FALSE);
	pGrid->SetRowResize(FALSE);
	pGrid->SetColumnResize(FALSE);
	pGrid->EnableColumnHide(TRUE);
	pGrid->EnableHiddenColUnhide(FALSE);
	pGrid->EnableRowHide(FALSE);

	// Set Fixed Row 0
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER;
	Item.row = 0;
	pGrid->SetItemBkColour(0, 0, RGB(220, 210, 245));
	for (i=0; i < SK_NUM_COLS; i++) {
		Item.col = i;
		Item.strText = str[i];
		pGrid->SetItem(&Item);
		pGrid->SetItemBkColour(0, i, RGB(220, 210, 245));
	}

	pGrid->SetTextBkColor(RGB(240, 240, 240));
	
	pGrid->SetColumnWidth(5, 100);
	pGrid->SetColumnWidth(7, 0);
	pGrid->SetColumnWidth(8, 0);
	pGrid->AutoSize();
}

void CSendKeysTab::OnHoldKey() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex].m_OPHoldKey = 
		m_HoldKey.GetCheck();
	if (m_HoldKey.GetCheck())
		m_Repeat.EnableWindow(false);
	else
		m_Repeat.EnableWindow(true);
}

void CSendKeysTab::OnRepeat() 
{
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex].m_OPRepeat = 
		m_Repeat.GetCheck();
}

void CSendKeysTab::OnPAdd() 
{
	OnAddButton(&m_PressGrid);
}

void CSendKeysTab::OnRAdd() 
{
	OnAddButton(&m_ReleaseGrid);
}

void CSendKeysTab::OnAddButton(CGridCtrl *pGrid) 
{
	GV_ITEM Item;
	CString str;

	if (pGrid->GetRowCount() >= SK_NUM_ROWS) {
		return;
	}

	// Set Fixed Col 0
	int nRow = pGrid->InsertRow("");
	int nCol = 0;
	str.Format("%i", nRow);
	pGrid->SetItemText(nRow, nCol, str); 
	pGrid->SetItemBkColour(nRow, nCol, RGB(230, 230, 255));

	// Column 1 Num Lock
	nCol = 1;
	pGrid->SetCellType (nRow, nCol, RUNTIME_CLASS(CGridCellCheck) );
	pGrid->SetItemState (nRow, nCol, pGrid->GetItemState(nRow, nCol) | GVIS_READONLY);

	// Column 2 Caps Lock
	nCol = 2;
	pGrid->SetCellType (nRow, nCol, RUNTIME_CLASS(CGridCellCheck) );
	pGrid->SetItemState (nRow, nCol, pGrid->GetItemState(nRow, nCol) | GVIS_READONLY);

	// Column 3 Scroll Lock
	nCol = 3;
	pGrid->SetCellType (nRow, nCol, RUNTIME_CLASS(CGridCellCheck) );
	pGrid->SetItemState (nRow, nCol, pGrid->GetItemState(nRow, nCol) | GVIS_READONLY);

	// Column 4 Key Macro
	nCol = 4;
	pGrid->SetItemState(nRow, nCol, pGrid->GetItemState(nRow, nCol) | GVIS_READONLY);

	// Column 5 Hold Delay
	nCol = 5;
	pGrid->SetCellType( nRow, nCol, RUNTIME_CLASS(CGridCellNumeric) );
	CGridCellNumeric *pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, nCol);
	pCell->SetText("0"); 

	// Column 6 Macro Delay
	nCol = 6;
	pGrid->SetCellType( nRow, nCol, RUNTIME_CLASS(CGridCellNumeric) );
	pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, nCol);
	pCell->SetText("0");

	NM_GRIDVIEW nItem;
	nItem.iColumn = 4;
	nItem.iRow = nRow;
	OnGridClick(pGrid, (NMHDR *)&nItem);
	pGrid->EnsureVisible(nRow, 4);
	pGrid->AutoSize();
//	GridOnSize();
}

void CSendKeysTab::OnPRemove() 
{
	OnRemoveButton(&m_PressGrid);
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex].OPClear();

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_PressGrid, pSendKeys->m_OPKeyMacros);
}

void CSendKeysTab::OnRRemove() 
{
	OnRemoveButton(&m_ReleaseGrid);
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex].ORClear();

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_ReleaseGrid, pSendKeys->m_ORKeyMacros);
}

void CSendKeysTab::OnRemoveButton(CGridCtrl *pGrid)
{
	CString str;
	CCellID nCell = pGrid->GetFocusCell();

	if (nCell.row > 0) {
		pGrid->DeleteRow(nCell.row);

		// reorder row numbers
		for (int nRow = 1; nRow < pGrid->GetRowCount(); nRow++) {
			str.Format(_T("%i"), nRow);
			pGrid->SetItemText(nRow, 0, str);
		}
		pGrid->Refresh();
	}
}

void CSendKeysTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;
	
	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex].m_Notes);
}

void CSendKeysTab::OnPClear() 
{
	OnClearGridRow(&m_PressGrid);
	CCellID nCell = m_PressGrid.GetFocusCell();

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_PressGrid, pSendKeys->m_OPKeyMacros, nCell.row);
}

void CSendKeysTab::OnRClear() 
{
	OnClearGridRow(&m_ReleaseGrid);
	CCellID nCell = m_ReleaseGrid.GetFocusCell();

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_ReleaseGrid, pSendKeys->m_ORKeyMacros, nCell.row);
}

void CSendKeysTab::OnClearGridRow(CGridCtrl *pGrid)
{
	CCellID nCell = pGrid->GetFocusCell();
	CGridCellNumeric *pCell;
	CGridCellCheck* pGridCellCheck;
	
	if (nCell.row > 0) {
		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nCell.row, 1);
		pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(0);
		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nCell.row, 2);
		pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(0);
		pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nCell.row, 3);
		pGridCellCheck->SetCheck(false);
		pGridCellCheck->SetData(0);

		pGrid->SetItemText(nCell.row, 4, "");

		pCell = (CGridCellNumeric *)pGrid->GetCell(nCell.row, 5);
		pCell->SetNumber(0);
		pGrid->SetItemData (nCell.row, 5, 0);
		pCell = (CGridCellNumeric *)pGrid->GetCell(nCell.row, 6);
		pCell->SetNumber(0);
		pGrid->SetItemData (nCell.row, 6, 0);
		pGrid->SetItemData (nCell.row, 7, 0);
		pGrid->SetItemData (nCell.row, 8, 0);

		pGrid->Refresh(); 
	}
}

void CSendKeysTab::OnClear() 
{
	m_PressGrid.DeleteNonFixedRows();
	m_PressGrid.Refresh();
	m_ReleaseGrid.DeleteNonFixedRows();
	m_ReleaseGrid.Refresh();
	m_HoldKey.SetCheck(true);
	m_Repeat.SetCheck(false);
	m_Notes.SetWindowText("");
}

void CSendKeysTab::OnClearButton() 
{
	OnClear();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex].Clear();
}

void CSendKeysTab::OnPGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	OnGridClick(&m_PressGrid, pNotifyStruct);

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_PressGrid, pSendKeys->m_OPKeyMacros, pItem->iRow);

    *pResult = 0;
}

void CSendKeysTab::OnPGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	OnGridEndEdit(&m_PressGrid, pNotifyStruct, pResult);

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_PressGrid, pSendKeys->m_OPKeyMacros, pItem->iRow);
}

void CSendKeysTab::OnRGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	OnGridClick(&m_ReleaseGrid, pNotifyStruct);

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_ReleaseGrid, pSendKeys->m_ORKeyMacros, pItem->iRow);

	*pResult = 0;
}

void CSendKeysTab::OnRGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    OnGridEndEdit(&m_ReleaseGrid, pNotifyStruct, pResult);

	CSendKeysPage *pSendKeys = &theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SendKeysArray[theApp.m_CurItemIndex];
	SaveToSendKeysArray(&m_ReleaseGrid, pSendKeys->m_ORKeyMacros, pItem->iRow);
}

void CSendKeysTab::OnGridEndEdit(CGridCtrl *pGrid, NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
    //pItem->iRow, pItem->iColumn;
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	CGridCellNumeric *pCell;

	*pResult = 0;

	switch (nCol) {
		case 5:	// Column 5 Hold Delay TextBox
			pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, nCol);
			if ((int)pCell->GetNumber() > 100000) {
				MessageBox("Enter a value between 0 - 100000", "Invalid Hold Delay", MB_ICONEXCLAMATION);
				pGrid->SetFocusCell(nRow, nCol); 
				*pResult = -1;
				return;
			} else {
				pGrid->SetItemData (nRow, nCol, (int)pCell->GetNumber());
			}
			break;

		case 6:	// Column 6 Macro Delay TextBox
			pCell = (CGridCellNumeric *)pGrid->GetCell(nRow, nCol);
			if ((int)pCell->GetNumber() > 100000) {
				MessageBox("Enter a value between 0 - 100000", "Invalid Macro Delay", MB_ICONEXCLAMATION);
				pGrid->SetFocusCell(nRow, nCol); 
				*pResult = -1;
				return;
			} else {
				pGrid->SetItemData (nRow, nCol, (int)pCell->GetNumber());
			}
			break;
	} // switch (nCol)

	pGrid->Refresh();
}

void CSendKeysTab::OnGridClick(CGridCtrl *pGrid, NMHDR *pNotifyStruct)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
	int i, j;

	if (nRow <= 0 || nCol <= 0) return;

	CGridCellCheck* pGridCellCheck;

	for (i = 1; i < SK_NUM_COLS; i++)
		pGrid->SetItemBkColour(nRow, i, RGB(191, 212, 227));

	for (j = 1; j < pGrid->GetRowCount(); j++) {
		if ( nRow != j ) {
			for (i = 1; i < SK_NUM_COLS; i++)
				pGrid->SetItemBkColour(j, i, RGB(240, 240, 240));
		}
	}

	switch (nCol) {
		case 1:	// Column 1 NUM LOCK Checkbox
			pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, nCol);
			if (pGridCellCheck->GetCheck()) {
				pGridCellCheck->SetCheck(FALSE);
			} else {
				pGridCellCheck->SetCheck(TRUE);
			}
		break;

		case 2:	// Column 2 Caps Lock Checkbox
			pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, nCol);
			if (pGridCellCheck->GetCheck()) {
				pGridCellCheck->SetCheck(FALSE);
			} else {
				pGridCellCheck->SetCheck(TRUE);
			}
		break;

		case 3:	// Column 3 Scroll Lock Checkbox
			pGridCellCheck = (CGridCellCheck*)pGrid->GetCell(nRow, nCol);
			if (pGridCellCheck->GetCheck()) {
				pGridCellCheck->SetCheck(FALSE);
			} else {
				pGridCellCheck->SetCheck(TRUE);
			}
		break;
	}

	pGrid->Refresh();
}

void CSendKeysTab::GridOnSize()
{
	m_PressGrid.ExpandColumnsToFit(true);
//	if (m_PressGrid.GetColumnWidth(1) < 53)
		m_PressGrid.AutoSize();

	m_ReleaseGrid.ExpandColumnsToFit(true);
//	if (m_ReleaseGrid.GetColumnWidth(1) < 53)
		m_ReleaseGrid.AutoSize();
}

BOOL CSendKeysTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}
