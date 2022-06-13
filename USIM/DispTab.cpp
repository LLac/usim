// DispTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "DispTab.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispTab dialog


CDispTab::CDispTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDispTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDispTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDispTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispTab)
	DDX_Control(pDX, IDC_OUTPUTNAME, m_IOName);
	DDX_Control(pDX, IDC_OB_LEVEL_SLIDER, m_OBLevel);
	DDX_Control(pDX, IDC_CLEAR, m_OBClear);
	DDX_Control(pDX, IDC_OB_TEXT3, m_Text3);
	DDX_Control(pDX, IDC_OB_TEXT4, m_Text4);
	DDX_Control(pDX, IDC_NOTES, m_Notes);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_LAMPTEST, m_LampTest);
	DDX_Control(pDX, IDC_INVERT, m_Override);
	DDX_Control(pDX, IDC_OB_LEVEL, m_OBLevelText);
	DDX_Control(pDX, IDC_GROUP1, m_OBGroup);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDC_CLEARDATA, m_ClearData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDispTab, CResizableDialog)
	//{{AFX_MSG_MAP(CDispTab)
	ON_BN_CLICKED(IDC_CLEAR, OnClearButton)
	ON_BN_CLICKED(IDC_LAMPTEST, OnLampTest)
	ON_BN_CLICKED(IDC_INVERT, OnOverride)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_CLEARDATA, OnClearData)
	ON_EN_CHANGE(IDC_OB_LEVEL, OnChangeOBLevel)
	ON_EN_CHANGE(IDC_NOTES, OnChangeNotes)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispTab message handlers

BOOL CDispTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_LampTest.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_LampTest.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_LampTest.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	CRect r;
	m_LampTest.GetWindowRect(r);
	ScreenToClient(r);
	m_LampTest.MoveWindow(r.left, r.top, 15, 15);

	m_Override.SetSSLButtonStyle(SSL_BS_AUTOSIZE);
	m_Override.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR, RGB(0, 255, 0));
	m_Override.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR, RGB(150, 150, 150));
	m_Override.GetWindowRect(r);
	ScreenToClient(r);
	m_Override.MoveWindow(r.left, r.top, 15, 15);

	m_OBLevelText.SetSlideLink( this, IDC_OB_LEVEL_SLIDER );
	m_OBLevelText.SetParams( 0, 10, 10);
	m_OBLevelText.SetValue(10);

	m_OBGroup.AddControlToList (IDC_CLEAR);
	m_OBGroup.AddControlToList (IDC_OB_LEVEL);
	m_OBGroup.AddControlToList (IDC_OB_LEVEL_SLIDER);
	m_OBGroup.AddControlToList (IDC_OB_TEXT3);
	m_OBGroup.AddControlToList (IDC_OB_TEXT4);
	m_OBGroup.AddControlToList (IDC_NOTES);
	m_OBGroup.AddControlToList (IDC_LAMPTEST);
	m_OBGroup.AddControlToList (IDC_INVERT);

	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_ImageList.Add(theApp.LoadIcon(IDI_OFF));		// 0
	m_ImageList.Add(theApp.LoadIcon(IDI_ON));		// 1
	m_ImageList.Add(theApp.LoadIcon(IDI_DOWN));		// 2
	m_ImageList.Add(theApp.LoadIcon(IDI_ON_RED));	// 3

    m_Grid.SetImageList(&m_ImageList);

	m_Grid.SetColumnCount(SEG_NUM_COLS);
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
	CString str[] = {"  Display  ", "           Variable           ", "User Value", " Factor ", " Constant ", "Leading Zero", "Grouping", "Digits", "   Minimum   ", "   Maximum   ", "Raw Value"};
	for (int i=0; i < SEG_NUM_COLS; i++) {
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
	m_ClearData.SetIcon(AfxGetApp()->LoadIcon(IDI_CLEAR));
	m_ClearData.EnableWindow(false);

	m_Grid.SetTextBkColor(RGB(240, 240, 240));
	m_Grid.AutoSize();

	AddAnchor(IDC_STATIC0, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ENABLE, TOP_RIGHT);
	AddAnchor(IDC_OUTPUTNAME, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC1, TOP_RIGHT);
	AddAnchor(IDC_GRID, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ADD, TOP_RIGHT);
	AddAnchor(IDC_REMOVE, TOP_RIGHT);
	AddAnchor(IDC_CLEARDATA, TOP_RIGHT);

	AddAnchor(IDC_STATIC2, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LAMPTEST, BOTTOM_LEFT);

	AddAnchor(IDC_STATIC3, BOTTOM_LEFT);
	AddAnchor(IDC_STATIC4, BOTTOM_LEFT);
	AddAnchor(IDC_OB_LEVEL, BOTTOM_LEFT);
	AddAnchor(IDC_OB_LEVEL_SLIDER, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_OB_TEXT3, BOTTOM_LEFT);
	AddAnchor(IDC_OB_TEXT4, BOTTOM_RIGHT);

	AddAnchor(IDC_STATIC5, BOTTOM_LEFT);
	AddAnchor(IDC_NOTES, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEAR, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDispTab::InitDispTab() 
{
	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_IOName.SetWindowText(_T("7-Segment Display : "));
		m_Enable.SetWindowText(_T(""));

		OnClearButton();

		m_OBGroup.SetEnable(false);
	} else {
		CString str, sPort, sID;
		int i, j;
		CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

		// KLUDGE for PLV2
		str.Format("%s : %s",
					theApp.m_pTreeDevices->GetItemLabel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[theApp.m_CurPortIndex]),
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);

		theApp.m_pTreeDevices->SetItemText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[theApp.m_CurPortIndex], str);

		sID = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.DeviceName;
		sPort = theApp.m_pTreeDevices->GetItemText(theApp.m_pTreeDevices->GetParentItem(theApp.m_CurrenthItem));
		if (sPort.Find(sID) != -1) {
			str.Format(_T("%s %s : 7-Segment Display : %s"), 
						sID,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);
		} else {
			str.Format(	_T("%s %s : Port %s - 7-Segment Display : %s"), 
						sID,
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceParam.SerialNum, 
						Alpha[theApp.m_CurPortIndex],
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispNotesArray[theApp.m_CurPortIndex].m_Desc);
		}

		m_IOName.SetWindowText(str);
		m_Notes.SetWindowText(_T(""));
		
		OnClear();

		// Fill in Row data
		CGridCellCheck *pGridCellCheck;
		for (j = 0; j < 7; j++) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Grouping != 0 ||
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Digits != 0) {

				i = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].VarTokenIndex;

				AddItem(false);

				str.Empty();
				switch(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].SimType) {
					case SIM_XPLANE:
						if (XPDataArray[theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].VarTokenIndex].VarArraySize > 0) {
							str.Format("X-Plane/%s[%d]", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].VarTokenName,
								theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].VarArrayTokenIndex + 1);
						} else {
							str.Format("X-Plane/%s", theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].VarTokenName);
						}
						break;

					case SIM_IL2:
					case SIM_IL2FB:
					case SIM_IL2PF:
						str = IL2DataArray[i].VarTokenName;
						break;

					case SIM_F4BMS:
					case SIM_F4USIM:
						str = F4DataArray[i].VarTokenName;
						break;

					case SIM_GTR:
					case SIM_GTR2:
					case SIM_EVO:
					case SIM_GTL:
					case SIM_RACE:
					case SIM_RACE07:
					case SIM_RACEON:
					case SIM_VOLVO:
					case SIM_SIMBIN:
						str = SimBinDataArray[i].VarTokenName;
						break;

					case SIM_RF:
						str = RFDataArray[i].VarTokenName;
						break;

					case SIM_GPX:
						str = GPXDataArray[i].VarTokenName;
						break;

					case SIM_LFS:
						str = LFSDataArray[i].VarTokenName;
						break;

					case USIM_INPUT_FLAGS:
						str = theApp.m_UserVarsArray.m_IOFlags[i].VarTokenString;
						break;
				}
				m_Grid.SetItemText(m_CurrentRow, 1, str);
				m_Grid.SetItemData (m_CurrentRow, 1, i);
				m_Grid.SetItemData (m_CurrentRow, 2, theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].SimType);

				m_Grid.SetItemText(m_CurrentRow, 2, theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].UserValue);
				str.Format(_T("%.7f"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Factor);
				m_Grid.SetItemText(m_CurrentRow, 3, str);
				str.Format(_T("%.7f"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Constant);
				m_Grid.SetItemText(m_CurrentRow, 4, str);
				pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(m_CurrentRow, 5);
				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].LeadingZero) {
					pGridCellCheck->SetCheck(true);
				}
				str.Format(_T("%i"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Grouping);
				m_Grid.SetItemText(m_CurrentRow, 6, str);
				str.Format(_T("%i"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Digits);
				m_Grid.SetItemText(m_CurrentRow, 7, str);
				str.Format(_T("%.7f"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Minimum);
				m_Grid.SetItemText(m_CurrentRow, 8, str);
				str.Format(_T("%.7f"), theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Maximum);
				m_Grid.SetItemText(m_CurrentRow, 9, str);
				pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(m_CurrentRow, 10);
				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[j].Raw) {
					pGridCellCheck->SetCheck(true);
				}
			}
		}

		m_LampTest.SetCheck(0);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_LampTest = 0;

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Override == 1)
			m_Override.SetCheck(1);
		else 
			m_Override.SetCheck(0);

		m_OBLevelText.SetValue((cint)theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_OBLevel);

		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispActive[theApp.m_CurPortIndex]) {
			if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispDisabled[theApp.m_CurPortIndex]) {
				m_Enable.SetWindowText(_T("DISABLED"));
			} else {
				m_Enable.SetWindowText(_T("ENABLED"));
			}
		} else {
			m_Enable.SetWindowText(_T("HARDWARE DISABLED"));
		}

		m_Notes.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Notes);

		m_OBGroup.SetEnable(true);

		m_Grid.AutoSize();
	}
}

void CDispTab::OnLampTest() 
{
	if (m_LampTest.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_LampTest = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_LampTest = 0;
	}
}

void CDispTab::OnOverride() 
{
	if (m_Override.GetCheck()) {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Override = 1;
	} else {
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Override = 0;
	}
}

void CDispTab::OnChangeOBLevel() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_OBLevel = (UCHAR)m_OBLevel.GetPos();
}

void CDispTab::OnChangeNotes()
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_Notes.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Notes);
}

void CDispTab::OnClearButton() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].Clear();

	OnClear();
	OnChangeNotes();
	m_OBLevelText.SetValue(10);
	m_Override.SetCheck(0);
	OnOverride();
}

void CDispTab::OnClear() 
{
	while (m_Grid.GetRowCount() > 1) {
		m_Grid.DeleteRow(m_Grid.GetRowCount() - 1); 
	}
	m_Grid.Refresh();

	m_Add.EnableWindow(true);
	m_Remove.EnableWindow(false);
	m_ClearData.EnableWindow(false);

	m_Notes.SetWindowText(_T(""));
}

void CDispTab::OnAdd() 
{
	AddItem(true);
}

void CDispTab::AddItem(BOOL defFlag) 
{
	int i;

	if (m_Grid.GetRowCount() >= SEG_NUM_ROWS) {
		m_Add.EnableWindow(false);
		return;
	}

	int last_row = (m_Grid.GetRowCount() - 1);
	if (last_row != 0) {
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].Grouping == 0 &&
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].Digits == 0) {

			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].VarTokenName.Empty();
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].UserValue.Empty();
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].VarTokenIndex = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].VarArrayTokenIndex = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[last_row - 1].SimType = SIM_NONE;
			return;
		}
	}

	int grouping_cnt = 0;
	for (i = 0; i < last_row; i++) {
		grouping_cnt += (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[i].Grouping +
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[i].Digits);
		if (grouping_cnt >= 7) return;
	}
	
	int nRow = m_Grid.InsertRow("");
	CString str;
	str.Format(_T("Display %i"), nRow);
	m_Grid.SetItemText(nRow, 0, str); 
	m_Grid.SetItemBkColour(nRow, 0, RGB(230, 230, 245));

	// VARIABLE
	int nCol = 1;
	m_Grid.SetItemText(nRow, nCol, "");
	m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);

	// USER VALUE
	nCol = 2;
	CGridCellBase* pGridCellBase = (CGridCellBase*)m_Grid.GetCell(nRow, nCol);	
	pGridCellBase->SetText("");
	if (defFlag)
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].UserValue.Empty();

	// FACTOR
	nCol = 3;
	CGridCellNumeric* pGridCellNumeric;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Real | CGridCellNumeric::Negative | CGridCellNumeric::Silent);
	theApp.m_nFormat.NumDigits = 3;
	theApp.m_nFormat.LeadingZero = 1;
	theApp.m_nFormat.Grouping = 6;
	theApp.m_nFormat.lpDecimalSep = ".";
	theApp.m_nFormat.lpThousandSep = "";
	theApp.m_nFormat.NegativeOrder = 1;
	pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);
	pGridCellNumeric->SetText("1");
	if (defFlag)
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Factor = 1;

	// CONSTANT
	nCol = 4;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Real | CGridCellNumeric::Negative | CGridCellNumeric::Silent);
	pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);
	pGridCellNumeric->SetText("0");
	if (defFlag)
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Constant = 0;

	// LEADING ZERO
	nCol = 5;
	GV_ITEM Item;
	Item.mask = GVIF_FORMAT;
	Item.nFormat = DT_CENTER;
	Item.col = nCol;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck));
	m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);
	m_Grid.SetItem(&Item);

	// GROUPING
	nCol = 6;
	CStringArray strAryCombo;
	strAryCombo.RemoveAll();
	for (i = 0; i< 8; i++) {
		str.Format(_T("%i"), i); 
		strAryCombo.Add(str);
	}

	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo* pGridCellCombo = (CGridCellCombo*)m_Grid.GetCell(nRow, nCol);
	pGridCellCombo->SetOptions(strAryCombo);
	pGridCellCombo->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
	m_Grid.SetItemText(nRow, nCol, "0");

	// DIGITS
	nCol = 7;
	strAryCombo.RemoveAll();
	for (i = 0; i < 7; i++) {
		str.Format(_T("%i"), i); 
		strAryCombo.Add(str);
	}

	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCombo));
	pGridCellCombo = (CGridCellCombo*)m_Grid.GetCell(nRow, nCol);
	pGridCellCombo->SetOptions(strAryCombo);
	pGridCellCombo->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
	m_Grid.SetItemText(nRow, nCol, "0");

	// MINIMUM
	nCol = 8;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Real | CGridCellNumeric::Negative | CGridCellNumeric::Silent);
	pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);
	pGridCellNumeric->SetText("0");
	if (defFlag)
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Minimum = 0;

	// MAXIMUM
	nCol = 9;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellNumeric));
	pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);
	pGridCellNumeric->SetFlags(CGridCellNumeric::Real | CGridCellNumeric::Negative | CGridCellNumeric::Silent);
	pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);
	pGridCellNumeric->SetText("9999999");
	if (defFlag)
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Maximum = 9999999;

	// RAW
	nCol = 10;
	m_Grid.SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck));
	m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);

	m_Remove.EnableWindow(true);
	m_ClearData.EnableWindow(true);

	m_Grid.AutoSize();

	m_CurrentRow = nRow;
}

void CDispTab::OnRemove() 
{
	if (m_Grid.IsValid(m_CurrentRow, 0) && m_CurrentRow != 0) {
		m_Grid.DeleteRow(m_CurrentRow);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].VarTokenName.Empty();
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].VarTokenIndex = 0;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].VarArrayTokenIndex = 0;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].SimType = SIM_NONE;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].UserValue.Empty();
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Factor = 1;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Constant = 0;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].LeadingZero = 0;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Grouping = 0;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Digits = 0;
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Raw = 0;
		m_CurrentRow = NULL;
		CString str;
		for (int nRow = 1; nRow < m_Grid.GetRowCount(); nRow++) {
			str.Format(_T("Display %i"), nRow);
			m_Grid.SetItemText(nRow, 0, str);
		}
		m_Add.EnableWindow(true);

		if (m_Grid.GetRowCount() == 1) {
			m_Remove.EnableWindow(false);
			m_ClearData.EnableWindow(false);
		}

		// Fill in data
		NM_GRIDVIEW pItem;
		LRESULT pResult;
		for (pItem.iRow = 1; pItem.iRow < 8; pItem.iRow++) { // rows
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].VarTokenName.Empty();
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].VarTokenIndex = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].VarArrayTokenIndex = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].SimType = SIM_NONE;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].UserValue.Empty();
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Factor = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Constant = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].LeadingZero = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Grouping = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Digits = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Minimum = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Maximum = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].Raw = 0;

			if (pItem.iRow >= m_Grid.GetRowCount()) 
				continue;

			CGridCellCheck *pGridCellCheck;
			CString str;
			for (pItem.iColumn = 1; pItem.iColumn < DMX_NUM_COLS; pItem.iColumn++) {	// cols
				OnGridEndEdit((tagNMHDR *)&pItem, &pResult);

				if (pItem.iColumn == 1) {			// VARIABLE
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].VarTokenName = m_Grid.GetItemText (pItem.iRow, pItem.iColumn);
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].VarTokenIndex = m_Grid.GetItemData (pItem.iRow, pItem.iColumn);
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].SimType = m_Grid.GetItemData (pItem.iRow, pItem.iColumn + 1);
				} else if (pItem.iColumn == 5) {	// LEADING ZERO
					pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(pItem.iRow, pItem.iColumn);
					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[pItem.iRow - 1].LeadingZero = (UCHAR)pGridCellCheck->GetCheck();
				}
			}
		}
	}

	m_Grid.Refresh();
}

void CDispTab::OnClearData() 
{
	if (m_CurrentRow == 0) return;

	// VARIABLE
	int nCol = 1;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].VarTokenName.Empty();
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].VarTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].VarArrayTokenIndex = 0;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].SimType = SIM_NONE;
	m_Grid.SetItemText(m_CurrentRow, nCol, "");

	// USER VALUE
	nCol = 2;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].UserValue.Empty();
	m_Grid.SetItemText(m_CurrentRow, nCol, "");

	// FACTOR
	nCol = 3;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Factor = 1;
	m_Grid.SetItemText(m_CurrentRow, nCol, "1");

	// CONSTANT
	nCol = 4;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Constant = 0;
	m_Grid.SetItemText(m_CurrentRow, nCol, "0");

	// LEADING ZERO
	nCol = 5;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].LeadingZero = 0;
	CGridCellCheck *pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(m_CurrentRow, nCol);
	pGridCellCheck->SetCheck(false);

	// GROUPING
	nCol = 6;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Grouping = 0;
	m_Grid.SetItemText(m_CurrentRow, nCol, "0");

	// DIGITS
	nCol = 7;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Digits = 0;
	m_Grid.SetItemText(m_CurrentRow, nCol, "0");

	// MINIMUM
	nCol = 8;
	m_Grid.SetItemText(m_CurrentRow, nCol, "0");
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Minimum = 0;

	// MAXIMUM
	nCol = 9;
	m_Grid.SetItemText(m_CurrentRow, nCol, "9999999");
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Maximum = 9999999;

	// RAW
	nCol = 10;
	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[m_CurrentRow - 1].Raw = 0;
	pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(m_CurrentRow, nCol);
	pGridCellCheck->SetCheck(false);

	m_Grid.AutoSize();
}

void CDispTab::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CGridCellCheck *pGridCellCheck;
 
	if (pItem->iRow < 0) return;

	m_CurrentRow = pItem->iRow;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
	int i, j;
	int nResponse;
	CString str;

    *pResult = 0;

	if (nRow <= 0 || nCol <= 0) return;

	for (i = 1; i < m_Grid.GetRowCount(); i++) {
		for (j = 1; j < SEG_NUM_COLS; j++) {
			if (i == nRow)
				m_Grid.SetItemBkColour(i, j, RGB(191, 212, 227));
			else
				m_Grid.SetItemBkColour(i, j, RGB(240, 240, 240));
		}
	}

	switch (nCol) {
		case 1:	// VARIABLE
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
			m_VarListDlg.m_ListTypeVar = VARTYPE_VAR_ONLY;
			m_VarListDlg.m_VarTokenName = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].VarTokenName;
			m_VarListDlg.m_VarArrayIndex = theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].VarArrayTokenIndex;
			nResponse = m_VarListDlg.DoModal();

			if (nResponse == IDOK) {
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].VarTokenName = m_VarListDlg.m_VarTokenName;
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].VarTokenIndex = m_VarListDlg.m_ArrayIndex;
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].VarArrayTokenIndex = m_VarListDlg.m_VarArrayIndex;
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].SimType = m_VarListDlg.m_SimType;

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
				m_Grid.SetItemText (nRow, nCol, str);
				m_Grid.SetItemData (nRow, nCol, m_VarListDlg.m_ArrayIndex);
				m_Grid.SetItemData (nRow, nCol+1, m_VarListDlg.m_SimType);
			} else if (nResponse == IDC_CLEARDATA) {
				OnClearData();
			}
			break;

		case 5:	// LEADING ZERO
			pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(nRow, nCol);
			if (pGridCellCheck->GetCheck()) {
				pGridCellCheck->SetCheck(false);
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].LeadingZero = 0;
			} else {
				pGridCellCheck->SetCheck(true);
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].LeadingZero = 1;
			}
			break;

		case 10:	// RAW
			pGridCellCheck = (CGridCellCheck*)m_Grid.GetCell(nRow, nCol);
			if (pGridCellCheck->GetCheck()) {
				pGridCellCheck->SetCheck(false);
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Raw = 0;
			} else {
				pGridCellCheck->SetCheck(true);
				theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Raw = 1;
			}
			break;
	} // switch (nCol)

	m_Grid.AutoSize();
}


void CDispTab::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	switch (nCol) {
		case 1:	// VARIABLE

			break;

		case 2:	// USER VALUE
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].UserValue = m_Grid.GetItemText(nRow, nCol);
			break;

		case 3:	// FACTOR
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Factor = atof(m_Grid.GetItemText(nRow, nCol));
			break;

		case 4:	// CONSTANT
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Constant = atof(m_Grid.GetItemText(nRow, nCol));
			break;
		
		case 5:	// LEADING ZERO
			break;

		case 6:	// GROUP ComboBox
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Grouping = (UCHAR)atoi(m_Grid.GetItemText(nRow, nCol));
			break;

		case 7:	// DIGITS ComboBox
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Digits = (UCHAR)atoi(m_Grid.GetItemText(nRow, nCol));
			break;

		case 8:	// MINIMUM
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Minimum = atof(m_Grid.GetItemText(nRow, nCol));
			break;

		case 9:	// MAXIMUM
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_Displays[nRow - 1].Maximum = atof(m_Grid.GetItemText(nRow, nCol));
			break;

		case 10:	// RAW
			break;
	} // switch (nCol)

	m_Grid.Refresh();
	m_Grid.AutoSize();
    *pResult = 0;
}

BOOL CDispTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

