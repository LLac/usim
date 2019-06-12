// CLCDLinesTab.cpp : implementation file
//

#include "stdafx.h"
#include "usim.h"
#include "CLCDLinesTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLCDLinesTab dialog

CCLCDLinesTab::CCLCDLinesTab(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CCLCDLinesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLCDLinesTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCLCDLinesTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLCDLinesTab)
	DDX_Control(pDX, IDC_BUTTON5, m_Button5);
	DDX_Control(pDX, IDC_BUTTON4, m_Button4);
	DDX_Control(pDX, IDC_BUTTON3, m_Button3);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_EDIT5, m_LCDLine5);
	DDX_Control(pDX, IDC_EDIT4, m_LCDLine4);
	DDX_Control(pDX, IDC_EDIT3, m_LCDLine3);
	DDX_Control(pDX, IDC_EDIT2, m_LCDLine2);
	DDX_Control(pDX, IDC_EDIT1, m_LCDLine1);
	DDX_Control(pDX, IDC_CLEAR, m_Clear);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
	DDX_Control(pDX, IDC_COMBO_PREV, m_ComboPrev);
	DDX_Control(pDX, IDC_COMBO_NEXT, m_ComboNext);
	DDX_Control(pDX, IDC_COMBO_DISPLAY, m_ComboDisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLCDLinesTab, CResizableDialog)
	//{{AFX_MSG_MAP(CCLCDLinesTab)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO_PREV, OnSelchangeComboPrev)
	ON_CBN_SELCHANGE(IDC_COMBO_NEXT, OnSelchangeComboNext)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY, OnSelchangeComboDisplay)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLCDLinesTab message handlers

BOOL CCLCDLinesTab::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	m_CurPageSel = 0;

	m_ImageList.DeleteImageList();
	if ( m_ImageList.GetSafeHandle() == NULL )
		m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 4);

	m_PAGE = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE));
	m_PAGE_P = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE_P));
	m_PAGE_DIS = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE_DIS));
	m_PAGE_DIS_P = m_ImageList.Add(theApp.LoadIcon(IDI_LCD_PAGE_DIS_P));

	AddAnchor(IDC_STATIC1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT3, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT4, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT5, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON1, TOP_RIGHT);
	AddAnchor(IDC_BUTTON2, TOP_RIGHT);
	AddAnchor(IDC_BUTTON3, TOP_RIGHT);
	AddAnchor(IDC_BUTTON4, TOP_RIGHT);
	AddAnchor(IDC_BUTTON5, TOP_RIGHT);
	AddAnchor(IDC_CLEAR, TOP_CENTER);

	m_Enable.SetCheck(false);

	m_ComboPrev.SetCurSel(0);
	m_ComboNext.SetCurSel(0);
	m_ComboDisplay.SetCurSel(0);

	m_Radio1.SetCheck(true);
	m_Radio2.SetCheck(false);

	m_Button5.EnableWindow(false);
	m_LCDLine5.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCLCDLinesTab::InitLCDLinesTab() 
{
	PopulateCombos();

	if (theApp.m_CurrenthItem == NULL || theApp.m_pDevArray[theApp.m_CurDevIndex] == NULL) {
		theApp.m_CurrenthItem = NULL;
		m_ComboPrev.SetCurSel(0);
		m_ComboNext.SetCurSel(0);
		m_ComboDisplay.SetCurSel(0);
		m_CurPageSel = 0;
		return;
	}
	
	m_CurPageSel = m_pTab->GetCurSel();
	if (m_CurPageSel < 0 || m_CurPageSel > LCD_MAX_PAGES) m_CurPageSel = 0;

	m_Enable.SetCheck(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageEnable[m_CurPageSel]);

	m_ComboPrev.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPagePrev);
	m_ComboNext.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageNext);
	m_ComboDisplay.SetCurSel(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageDisplay[m_CurPageSel]);

	m_Radio1.SetCheck(false);
	m_Radio2.SetCheck(false);
	if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDRadio == 0)
		m_Radio1.SetCheck(true);
	else
		m_Radio2.SetCheck(true);

	m_LCDLine1.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][0]);
	m_LCDLine2.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][1]);
	m_LCDLine3.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][2]);
	m_LCDLine4.SetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][3]);
	m_LCDLine5.SetWindowText("");
 
	m_Button5.EnableWindow(false);
	m_LCDLine5.EnableWindow(false);

	SetTabIcons();
}

void CCLCDLinesTab::PopulateCombos() 
{
	// Populate the pulldown lists
	m_ComboPrev.ResetContent();
	m_ComboNext.ResetContent();
	m_ComboDisplay.ResetContent();
	m_ComboPrev.AddString("NONE");
	m_ComboNext.AddString("NONE");
	m_ComboDisplay.AddString("NONE");
	for (int i=1; i<MAX_INPUT_FLAGS + 1; i++) {
		m_ComboPrev.AddString(theApp.m_UserVarsArray.m_IOFlags[i-1].VarTokenString);
		m_ComboNext.AddString(theApp.m_UserVarsArray.m_IOFlags[i-1].VarTokenString);
		m_ComboDisplay.AddString(theApp.m_UserVarsArray.m_IOFlags[i-1].VarTokenString);
	}
}

void CCLCDLinesTab::SetTabIcon(UINT nPage)
{
	int nSet;
	TCITEM item;

	item.mask = TCIF_IMAGE;

	if (theApp.m_CurrenthItem == NULL) return;
	if (nPage > LCD_MAX_PAGES) return;

	// set LCD Pages Tab Icon
	if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageEnable[nPage]) {
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].IsPageSet(nPage)) {
			// add _P
			nSet = m_PAGE_P;
		} else {
			// item is clear
			nSet = m_PAGE;
		}
	} else {
		if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].IsPageSet(nPage)) {
			// add _P
			nSet = m_PAGE_DIS_P;
		} else {
			// item is clear
			nSet = m_PAGE_DIS;
		}
	}

	m_pTab->GetItem(nPage, &item);
	if(item.iImage != nSet) {
		item.iImage = nSet;
		m_pTab->SetItem(nPage, &item);
	}
}

void CCLCDLinesTab::SetTabIcons()
{
	if (theApp.m_CurrenthItem == NULL) return;

	for (UINT nPage=0; nPage<LCD_MAX_PAGES; nPage++) {
		SetTabIcon(nPage);
	}
}

void CCLCDLinesTab::OnEnable() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageEnable[m_CurPageSel] = m_Enable.GetCheck();
	SetTabIcon(m_CurPageSel);
}

void CCLCDLinesTab::OnRadio1() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDRadio = 0;
	SetTabIcons();
}

void CCLCDLinesTab::OnRadio2() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDRadio = 1;
	SetTabIcons();
}

void CCLCDLinesTab::OnSelchangeComboPrev() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPagePrev = m_ComboPrev.GetCurSel();
	SetTabIcons();
}

void CCLCDLinesTab::OnSelchangeComboNext() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageNext = m_ComboNext.GetCurSel();
	SetTabIcons();
}

void CCLCDLinesTab::OnSelchangeComboDisplay() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPageDisplay[m_CurPageSel] = m_ComboDisplay.GetCurSel();
	SetTabIcons();
}

void CCLCDLinesTab::OnClear() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].ClearLCDPage(m_CurPageSel);

	InitLCDLinesTab();
}

void CCLCDLinesTab::OnChangeEdit1() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_LCDLine1.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][0]);
	SetTabIcon(m_CurPageSel);
}

void CCLCDLinesTab::OnChangeEdit2() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_LCDLine2.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][1]);
	SetTabIcon(m_CurPageSel);
}

void CCLCDLinesTab::OnChangeEdit3() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_LCDLine3.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][2]);
	SetTabIcon(m_CurPageSel);
}

void CCLCDLinesTab::OnChangeEdit4() 
{
	if (theApp.m_CurrenthItem == NULL) return;

	m_LCDLine4.GetWindowText(theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][3]);
	SetTabIcon(m_CurPageSel);
}

BOOL CCLCDLinesTab::SetClipboardText(LPCTSTR pszText)
{
   BOOL ok = FALSE;
   if(OpenClipboard()) {
      // the text should be placed in "global" memory
      HGLOBAL hMem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, 
         (lstrlen(pszText)+1)*sizeof(pszText[0]) );
      LPTSTR ptxt = (LPTSTR)GlobalLock(hMem);
      lstrcpy(ptxt, pszText);
      GlobalUnlock(hMem);
      // set data in clipboard; we are no longer responsible for hMem
      ok = (BOOL)SetClipboardData(CF_TEXT, hMem);

      CloseClipboard(); // relinquish it for other windows
   }
   return ok;
}

void CCLCDLinesTab::AddLineText(CEdit *pLCDLine, UINT nLine)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	m_VarListDlg.m_ListTypeVar = VARTYPE_VAR_ONLY;
	int nResponse = m_VarListDlg.DoModal();

	CString str, sFmt;
	if (nResponse == IDOK) {
		switch (m_VarListDlg.m_DataType) {
			case xplmType_Int:
				sFmt = "$%6i,";
				break;
			case xplmType_Float:
				sFmt = "$%6.4f,";
				break;
			case xplmType_Double:
				sFmt = "$%6.4f,";
				break;
			case xplmType_Char:
				sFmt = "$%s,";
				break;
			case xplmType_Byte:
				sFmt = "$%3d,";
				break;
			case xplmType_Word:
				sFmt = "$%6d,";
				break;
			case xplmType_DWord:
				sFmt = "$%6d,";
				break;
			case xplmType_IntHex:
				sFmt = "$%x,";
				break;
			case xplmType_ByteHex:
				sFmt = "$%x,";
				break;
		}

		str = sFmt + m_VarListDlg.m_VarTokenName + "$";
		SetClipboardText(LPCTSTR(str));

		pLCDLine->Paste();
		pLCDLine->GetWindowText(str);
		theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDPages[m_CurPageSel][nLine] = str;
	}	
}

void CCLCDLinesTab::OnButton1() 
{
	AddLineText(&m_LCDLine1, 0);
}

void CCLCDLinesTab::OnButton2() 
{
	AddLineText(&m_LCDLine2, 1);
}

void CCLCDLinesTab::OnButton3() 
{
	AddLineText(&m_LCDLine3, 2);
}

void CCLCDLinesTab::OnButton4() 
{
	AddLineText(&m_LCDLine4, 3);
}
