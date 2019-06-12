// SimPathTab.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "SimPathTab.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellCheck.h"
#include "PathSplit/CPathSplit.h"
#include "FileOperations\FileOperations.h"
#include "FileEdit.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimPathTab dialog


CSimPathTab::CSimPathTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSimPathTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimPathTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSimPathTab::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimPathTab)
	DDX_Control(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimPathTab, CResizableDialog)
	//{{AFX_MSG_MAP(CSimPathTab)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID, OnGridDblClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_BN_CLICKED(IDC_CLEARITEM, OnClearitem)
	ON_BN_CLICKED(IDC_CLEARALL, OnClearall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimPathTab message handlers

BOOL CSimPathTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	AddAnchor(IDC_STATIC1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_GRID, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CLEARITEM, BOTTOM_CENTER);
	AddAnchor(IDC_CLEARALL, BOTTOM_CENTER);

	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_ImageList.Add(theApp.LoadIcon(IDI_OFF));		// 0
	m_ImageList.Add(theApp.LoadIcon(IDI_ON));		// 1
	m_ImageList.Add(theApp.LoadIcon(IDI_DOWN));		// 2
	m_ImageList.Add(theApp.LoadIcon(IDI_ON_RED));	// 3
	m_ImageList.Add(theApp.LoadIcon(IDI_BROWSE));	// 4

	m_Grid.SetImageList(&m_ImageList);

	m_Grid.SetColumnCount(SIM_NUM_COLS);
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetRowCount(SIM_NUM_ROWS);
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
	CString str[] = {"Enable", "Simulator", "Auto launch on startup", "Simulator Path (Left Dbl Click to change)", "IMC Configuration File (Left Dbl Click to change)", "Server Port", "Install Plugin"};
	int i;
	for (i=0; i < SIM_NUM_COLS; i++) {
		Item.col = i;
		Item.strText = str[i];
		m_Grid.SetItem(&Item);
		m_Grid.SetItemBkColour(0, i, RGB(220, 210, 245));
	}

	// Column 0 ON/OFF
	Item.mask = GVIF_IMAGE | GVIF_FORMAT | GVIF_PARAM;
	Item.lParam = 0;
	Item.nFormat = DT_CENTER;
	Item.iImage = 0;
	Item.col = 0;
	for (i=1; i < SIM_NUM_ROWS; i++) {
		Item.row = i;
		m_Grid.SetItem(&Item);
		m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
	}

	// Column 1 Sim Name
	Item.col = 1;
	for (Item.row = 1; Item.row < SIM_NUM_ROWS; Item.row++) {
		m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
	}

	// Column 2 ON/OFF
	Item.mask = GVIF_IMAGE | GVIF_FORMAT | GVIF_PARAM;
	Item.lParam = 0;
	Item.nFormat = DT_CENTER;
	Item.iImage = 0;
	Item.col = 2;
	for (i=1; i < SIM_NUM_ROWS; i++) {
		Item.row = i;
		m_Grid.SetItem(&Item);
		m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
	}

	// Column 3 Path
	Item.col = 3;
	for (Item.row = 1; Item.row < SIM_NUM_ROWS; Item.row++) {
		m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
	}

	// Column 4 USC PATH
	Item.col = 4;
	for (Item.row = 1; Item.row < SIM_NUM_ROWS; Item.row++) {
		m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
	}

	// Col 5 SERVER PORT
	Item.col = 5;
	CGridCellNumeric* pGridCellNumeric;
	for (Item.row = 1; Item.row < SIM_NUM_ROWS; Item.row++) {
		switch (SimTypes[Item.row-1].SimType) {
			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
			case SIM_LFS:
				m_Grid.SetCellType(Item.row, Item.col, RUNTIME_CLASS(CGridCellNumeric));
				pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(Item.row, Item.col);	
				pGridCellNumeric->SetFlags(CGridCellNumeric::Integer | CGridCellNumeric::Silent);
				theApp.m_nFormat.NumDigits = 0;
				theApp.m_nFormat.LeadingZero = 1;
				theApp.m_nFormat.Grouping = 5;
				theApp.m_nFormat.lpDecimalSep = ".";
				theApp.m_nFormat.lpThousandSep = "";
				theApp.m_nFormat.NegativeOrder = 1;
				pGridCellNumeric->SetNumberFmt(&theApp.m_nFormat);
				pGridCellNumeric->SetText("0");
				break;
			default:
				m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
		}
	}

	// Column 6 ON/OFF
	Item.mask = GVIF_IMAGE | GVIF_FORMAT | GVIF_PARAM;
	Item.lParam = 0;
	Item.nFormat = DT_CENTER;
	Item.iImage = 0;
	Item.col = 6;
	for (i=1; i < SIM_NUM_ROWS; i++) {
		Item.row = i;
		if (SimTypes[Item.row-1].SimType == SIM_XPLANE ||
			SimTypes[Item.row-1].SimType == SIM_RF ||
			SimTypes[Item.row-1].SimType == SIM_LFS) {
			m_Grid.SetItem(&Item);
			m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
		} else {
			m_Grid.SetItemState(Item.row, Item.col, m_Grid.GetItemState(Item.row, Item.col) | GVIS_READONLY);
		}
	}

	m_Grid.SetTextBkColor(RGB(240, 240, 240));
	GridDefaults();
	m_Grid.AutoSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSimPathTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nID = LOWORD(wParam);

	// Prevent this ID to close the list when user press Enter key
	if( nID == 0x01 || nID == 0x02 ) //blocks esc button
		return FALSE;
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

void CSimPathTab::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;
	int i, j;
	CString str;

	*pResult = 0;

	m_CurrentRow = pItem->iRow;

	if (nRow <= 0 || nCol < 0) return;

//	CGridCellCheck* pGridCellCheck;

	for (i = 1; i < m_Grid.GetRowCount(); i++) {
		for (j = 0; j < SIM_NUM_COLS; j++) {
			if (i == m_CurrentRow)
				m_Grid.SetItemBkColour(i, j, RGB(191, 212, 227));
			else
				m_Grid.SetItemBkColour(i, j, RGB(240, 240, 240));
		}
	}

	switch (nCol) {
		case 0:		// ON/OFF toggle
			if ( m_Grid.GetItemData(nRow, nCol) == 0 ) {
				m_Grid.SetItemData(nRow, nCol, 1);
				m_Grid.SetItemImage(nRow, nCol, 1); // Green ON icon
				str.Format("Enable-%d", nRow-1);
				theApp.WriteProfileInt("Sim Paths", str, true);
				theApp.SimList[nRow-1].Enable = true;
			} else {
				m_Grid.SetItemData(nRow, nCol, 0);
				m_Grid.SetItemImage(nRow, nCol, 0);
				str.Format("Enable-%d", nRow-1);
				theApp.WriteProfileInt("Sim Paths", str, false);
				theApp.SimList[nRow-1].Enable = false;
			}
			break;

		case 2:		// ON/OFF toggle
			if ( m_Grid.GetItemData(nRow, nCol) == 0 ) {
				for (i=1; i<SIM_NUM_ROWS;i++) {
					m_Grid.SetItemData(i, nCol, 0);
					m_Grid.SetItemImage(i, nCol, 0); // OFF icon
				}

				m_Grid.SetItemData(nRow, nCol, 1);
				m_Grid.SetItemImage(nRow, nCol, 3);		// ON icon
				theApp.WriteProfileInt("Sim Paths", "Auto Launch", nRow);
			} else {
				m_Grid.SetItemData(nRow, nCol, 0);
				m_Grid.SetItemImage(nRow, nCol, 0); // OFF icon
				theApp.WriteProfileInt("Sim Paths", "Auto Launch", 0);
			}
			break;

		case 6:		// ON/OFF toggle
			if (SimTypes[nRow-1].SimType == SIM_XPLANE ||
				SimTypes[nRow-1].SimType == SIM_RF ||
				SimTypes[nRow-1].SimType == SIM_LFS) {
				m_Grid.SetItemData(nRow, nCol, 1);
				m_Grid.SetItemImage(nRow, nCol, 1);		// ON icon
				m_Grid.AutoSize();

				CFileOperation fo;      // create object
				TCHAR szFullPath[_MAX_PATH];
				CString SourcePath, DestinationPath;
				HINSTANCE hInstance = AfxGetInstanceHandle();

				GetModuleFileName(hInstance, szFullPath, sizeof(szFullPath));
				CPathSplit l_oPathSplit(szFullPath);


				if (SimTypes[nRow-1].SimType == SIM_XPLANE) {
					SourcePath = l_oPathSplit.GetDrive() + l_oPathSplit.GetDirectory() + "xpip.xpl";

					l_oPathSplit.Split(theApp.SimList[nRow-1].SimPath);
					DestinationPath = l_oPathSplit.GetDrive() + l_oPathSplit.GetDirectory() + "\\Resources\\plugins";
					
					fo.SetOverwriteMode(true);
					if (!fo.Copy(SourcePath, DestinationPath)) { // do Copy
						fo.ShowError(); // if copy fails show error message
					} else {
						MessageBox("Plugin successfully installed.", "XPIP.XPL Plugin", MB_OK | MB_ICONINFORMATION);
					}
				} else if (SimTypes[nRow-1].SimType == SIM_RF) {
					SourcePath = l_oPathSplit.GetDrive() + l_oPathSplit.GetDirectory() + "rfip.dll";

					l_oPathSplit.Split(theApp.SimList[nRow-1].SimPath);
					DestinationPath = l_oPathSplit.GetDrive() + l_oPathSplit.GetDirectory() + "\\plugins";
					
					fo.SetOverwriteMode(true);
					if (!fo.Copy(SourcePath, DestinationPath)) { // do Copy
						fo.ShowError(); // if copy fails show error message
					} else {
						MessageBox("Plugin successfully installed.", "RFIP.DLL Plugin", MB_OK | MB_ICONINFORMATION);
					}
				} else if (SimTypes[nRow-1].SimType == SIM_LFS) {
					l_oPathSplit.Split(theApp.SimList[nRow-1].SimPath);
					DestinationPath = l_oPathSplit.GetDrive() + l_oPathSplit.GetDirectory() + "cfg.txt";

					CString strSearch[5], strWrite[5];

					// OutGauge Mode 2
					// OutGauge Delay 1
					// OutGauge IP 127.0.0.1
					// OutGauge Port 5033
					// OutGauge ID 0

					strSearch[0] = "OutGauge Mode";
					strSearch[1] = "OutGauge Delay";
					strSearch[2] = "OutGauge IP";
					strSearch[3] = "OutGauge Port";
					strSearch[4] = "OutGauge ID";

					strWrite[0] = "2";
					strWrite[1] = "1";
					strWrite[2] = "127.0.0.1";
					strWrite[3].Format("%d", theApp.SimList[nRow-1].Port);
					strWrite[4] = "0";

					if (::PathFileExists(DestinationPath)) {
						std::vector<FileValues> values;
						ReadFileValues(DestinationPath, values);
						for (int i=0; i<5; i++)
							ReplaceValueWith(values, strSearch[i], strSearch[i], strWrite[i]);
						SaveFileValues(DestinationPath, values);

#ifndef _DEBUG
						MessageBox("cfg.txt file successfully modified.", "LFS CFG", MB_OK | MB_ICONINFORMATION);
#endif
					} else {
#ifndef _DEBUG
						MessageBox("cfg.txt file not found!", "LFS CFG", MB_OK | MB_ICONEXCLAMATION);
#endif
					}
				}

				m_Grid.SetItemData(nRow, nCol, 0);
				m_Grid.SetItemImage(nRow, nCol, 0); // OFF icon
				theApp.WriteProfileInt("Sim Paths", "Install Plugin", 0);
			}
			break;
	} // switch (nCol)

	m_Grid.AutoSize();
}

void CSimPathTab::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CPathSplit l_oPathSplit;

	char strFilter[] = { "Simulation Executable (*.exe)|*.exe|All Files (*.*)|*.*||" };
	CFileDialog FileDlg (TRUE, ".exe", NULL, OFN_HIDEREADONLY | OFN_READONLY | OFN_NOCHANGEDIR, strFilter);

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;

	*pResult = 0;

	switch (nCol) {
		case 3:		// PATH Browse
			if (FileDlg.DoModal() != IDOK)
				return;

			m_Grid.SetItemText(nRow, nCol, FileDlg.GetPathName());

			theApp.WriteProfileString("Sim Paths", SimTypes[nRow-1].SimName, FileDlg.GetPathName());
			theApp.SimList[nRow-1].SimPath = FileDlg.GetPathName();

			l_oPathSplit.Split (theApp.SimList[nRow-1].SimPath);
			theApp.SimList[nRow-1].SimFileName.Format("%s.exe", l_oPathSplit.GetFileName());
			break;

		case 4:		// USC Browse
			FileDlg.m_ofn.lpstrFilter = "USIM Configuration Files (*.imc)\0*.imc\0All Files (*.*)\0*.*\0\0";
			FileDlg.m_ofn.lpstrDefExt = ".imc";

			if( FileDlg.DoModal() != IDOK )	{
				return;
			}

			theApp.WriteProfileString("USC Files", SimTypes[nRow-1].SimName, FileDlg.GetPathName());
			theApp.SimList[nRow-1].USCFile = FileDlg.GetPathName();

			m_Grid.SetItemText(nRow, nCol, FileDlg.GetPathName());
			break;

	} // switch (nCol)

	m_Grid.AutoSize();
}

void CSimPathTab::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	*pResult = 0;

	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(nRow, nCol);

	UINT dNum = (UINT)pGridCellNumeric->GetNumber();
	switch (nCol) {
		case 5:	// SERVER PORT
			if (dNum > 65535) {
				AfxMessageBox("Enter a value between 0 & 65535", MB_ICONEXCLAMATION);
				pGridCellNumeric->SetNumber(theApp.SimList[nRow-1].Port);
				return;
			}

			theApp.SimList[nRow-1].Port = dNum;
			theApp.WriteProfileInt("Server Ports", SimTypes[nRow-1].SimName, (UINT)dNum);
			break;
	} // switch (nCol)

	m_Grid.AutoSize();

    *pResult = 0;
}

void CSimPathTab::GridDefaults() 
{
	CString str;
	GV_ITEM Item;

	Item.mask = GVIF_TEXT;
	for (Item.row = 1; Item.row < SIM_NUM_ROWS; Item.row++) {
		Item.col = 0;
		str.Format("Enable-%d", Item.row-1);
		if (theApp.GetProfileInt("Sim Paths", str, true)) {
			m_Grid.SetItemData(Item.row, Item.col, 1);
			m_Grid.SetItemImage(Item.row, Item.col, 1); // Green ON icon
			theApp.SimList[Item.row-1].Enable = true;
		} else {
			m_Grid.SetItemData(Item.row, Item.col, 0);
			m_Grid.SetItemImage(Item.row, Item.col, 0);
			theApp.SimList[Item.row-1].Enable = false;
		}

		Item.col = 1;
		Item.strText = SimTypes[Item.row-1].SimName;
		m_Grid.SetItem(&Item);
		theApp.SimList[Item.row-1].SimName = Item.strText;

		Item.col = 2;
		if (Item.row == (int)theApp.GetProfileInt("Sim Paths", "Auto Launch", 0)) {
			m_Grid.SetItemData(Item.row, Item.col, 1);
			m_Grid.SetItemImage(Item.row, Item.col, 3); // Red ON icon
		} else {
			m_Grid.SetItemData(Item.row, Item.col, 0);
			m_Grid.SetItemImage(Item.row, Item.col, 0);
		}

		Item.col = 3;
		Item.strText = theApp.GetProfileString("Sim Paths", SimTypes[Item.row-1].SimName, "");
		if (Item.strText.IsEmpty()) {
			Item.strText = SimTypes[Item.row-1].SimExe;
			theApp.SimList[Item.row-1].SimFileName = SimTypes[Item.row-1].SimExe;
		} else {
			CPathSplit l_oPathSplit(Item.strText);
			theApp.SimList[Item.row-1].SimFileName.Format("%s.exe", l_oPathSplit.GetFileName());
		}
		m_Grid.SetItem(&Item);
		theApp.SimList[Item.row-1].SimPath = Item.strText;
		
		theApp.SimList[Item.row-1].SimType = SimTypes[Item.row-1].SimType;

		Item.col = 4;
		Item.strText = theApp.GetProfileString("USC Files", SimTypes[Item.row-1].SimName, "");
		m_Grid.SetItem(&Item);
		theApp.SimList[Item.row-1].USCFile = Item.strText;

		Item.col = 5;
		int port = 0;
		switch (SimTypes[Item.row-1].SimType) {
			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
			case SIM_LFS:
				port = theApp.GetProfileInt("Server Ports", SimTypes[Item.row-1].SimName, 21000);
				CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(Item.row, Item.col);
				pGridCellNumeric->SetNumber(port);
				theApp.SimList[Item.row-1].Port = port;
				break;
		}

		Item.col = 6;
		if (SimTypes[Item.row-1].SimType == SIM_XPLANE ||
			SimTypes[Item.row-1].SimType == SIM_RF ||
			SimTypes[Item.row-1].SimType == SIM_LFS) {
			m_Grid.SetItemData(Item.row, Item.col, 0);
			m_Grid.SetItemImage(Item.row, Item.col, 0);
		}
	}

	m_Grid.Refresh();
}

void CSimPathTab::OnClearitem() 
{
	CString str;

	CCellID Item =	m_Grid.GetFocusCell();

	if (!Item.IsValid()) return;
	
	m_Grid.SetItemImage(Item.row, 0, 0);
	m_Grid.SetItemData(Item.row, 0, 0);
	theApp.SimList[Item.row-1].Enable = false;
	str.Format("Enable-%d", Item.row-1);
	theApp.WriteProfileInt("Sim Paths", str, false);

	m_Grid.SetItemImage(Item.row, 2, 0);
	m_Grid.SetItemData(Item.row, 2, 0);
	theApp.WriteProfileInt("Sim Paths", "Auto Launch", 0);

	m_Grid.SetItemText(Item.row, 3, SimTypes[Item.row-1].SimExe);
	theApp.SimList[Item.row-1].SimName = SimTypes[Item.row-1].SimExe;
	theApp.SimList[Item.row-1].SimPath.Empty();
	theApp.SimList[Item.row-1].SimFileName = SimTypes[Item.row-1].SimExe;
	theApp.WriteProfileString("Sim Paths", SimTypes[Item.row-1].SimName, "");

	m_Grid.SetItemText(Item.row, 4, "");
	m_Grid.SetItemData(Item.row, 4, 0);
	theApp.SimList[Item.row-1].USCFile.Empty();
	theApp.WriteProfileString("USC Files", SimTypes[Item.row-1].SimName, "");

	CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(Item.row, 5);
	switch (SimTypes[Item.row-1].SimType) {
		case SIM_IL2:
		case SIM_IL2FB:
		case SIM_IL2PF:
		case SIM_LFS:
			pGridCellNumeric->SetNumber(21000);
			
			theApp.SimList[Item.row-1].Port = 21000;
			theApp.WriteProfileInt("Server Ports", SimTypes[Item.row-1].SimName, 21000);
			break;
		default:
			m_Grid.SetItemText(Item.row, 5, "");
			theApp.SimList[Item.row-1].Port = NULL;
			theApp.WriteProfileInt("Server Ports", SimTypes[Item.row-1].SimName, NULL);
	}

	m_Grid.AutoSize();
}

void CSimPathTab::OnClearall() 
{
	CString str;

	for (int i=1; i<SIM_NUM_ROWS; i++) {	
		m_Grid.SetItemImage(i, 0, 1);
		m_Grid.SetItemData(i, 0, 0);
		theApp.SimList[i-1].Enable = true;
		str.Format("Enable-%d", i-1);
		theApp.WriteProfileInt("Sim Paths", str, true);

		m_Grid.SetItemImage(i, 2, 0);
		m_Grid.SetItemData(i, 2, 0);
		theApp.WriteProfileInt("Sim Paths", "Auto Launch", 0);

		m_Grid.SetItemText(i, 3, SimTypes[i-1].SimExe);
		theApp.SimList[i-1].SimName = SimTypes[i-1].SimExe;

/*		Add this section when I'm bored. Detect paths that contain exe name and remove it.
		HKEY hkey = NULL;
		DWORD dwDisposition;
		if(RegOpenKeyEx(SimTypes[i-1].RegSection, TEXT (SimTypes[i-1].RegKey), 0, KEY_READ, &hkey) == ERROR_SUCCESS) {
			DWORD dwType, dwSize;
			dwType = REG_SZ;
			dwSize = MAX_PATH;
	
			RegQueryValueEx(hkey, TEXT (SimTypes[i-1].RegValue), NULL, &dwType, (UCHAR *)(LPTSTR)(LPCTSTR)theApp.SimList[i-1].SimPath, &dwSize);
			theApp.WriteProfileString("Sim Paths", SimTypes[i-1].SimName, theApp.SimList[i-1].SimPath);
		}

		if (hkey != NULL) 
			RegCloseKey(hkey);
*/

		theApp.SimList[i-1].SimFileName = SimTypes[i-1].SimExe;
		theApp.WriteProfileString("Sim Paths", SimTypes[i-1].SimName, "");

		m_Grid.SetItemText(i, 4, "");
		m_Grid.SetItemData(i, 4, 0);
		theApp.SimList[i-1].USCFile.Empty();
		theApp.WriteProfileString("USC Files", SimTypes[i-1].SimName, "");

		CGridCellNumeric* pGridCellNumeric = (CGridCellNumeric*)m_Grid.GetCell(i, 5);
		switch (SimTypes[i-1].SimType) {
			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
			case SIM_LFS:
				pGridCellNumeric->SetNumber(21000);
				
				theApp.SimList[i-1].Port = 21000;
				theApp.WriteProfileInt("Server Ports", SimTypes[i-1].SimName, 21000);
				break;
			default:
				m_Grid.SetItemText(i, 5, "");
				theApp.SimList[i-1].Port = NULL;
				theApp.WriteProfileString("Server Ports", SimTypes[i-1].SimName, NULL);
		}
	}

	m_Grid.AutoSize();
}
