// TreeDevices.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "TreeDevices.h"
#include "USIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString Alpha[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

/////////////////////////////////////////////////////////////////////////////
// CTreeDevices

CTreeDevices::CTreeDevices()
{
	theApp.m_CopyItemClass = ID_NONE;
	m_ReEntryFlag = false;
}

CTreeDevices::~CTreeDevices()
{
}

BEGIN_MESSAGE_MAP(CTreeDevices, CTreeCtrlEx)
	//{{AFX_MSG_MAP(CTreeDevices)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclickTree)
	ON_NOTIFY_REFLECT(NM_CLICK, OnLclickTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_DEVICESTREE, OnEndLabelEdit)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EXPAND, OnExpand)
	ON_COMMAND(ID_COLLAPSE, OnCollapse)
	ON_COMMAND(ID_REFRESH, OnRefreshList)
	ON_COMMAND(ID_CLEAR, OnClearDevice)
	ON_COMMAND(ID_CLEAR_ITEM, OnClearItem)
	ON_COMMAND(ID_COPY_ITEM, OnCopyItem)
	ON_COMMAND(ID_PASTE_ITEM, OnPasteItem)
	ON_COMMAND(ID_EXPORT_ITEM, OnExportItem)
	ON_COMMAND(ID_IMPORT_ITEM, OnImportItem)
	ON_COMMAND(ID_DISABLE_ITEM, OnDisable)
	ON_COMMAND(ID_HIDE_ITEMS, OnHideItems)
	ON_COMMAND(ID_HIDE_PORTS, OnHidePorts)
	ON_COMMAND(ID_HIDE_MODES, OnHideModes)
	ON_COMMAND(ID_SWAP, OnReplace)
	ON_COMMAND(ID_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeDevices message handlers

void CTreeDevices::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM & item = pTVDispInfo->item;

	*pResult = 1;
	if (GetItemType(item.hItem) == ID_PORT)
		*pResult = 0;
}

void CTreeDevices::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM & item = pTVDispInfo->item;
	*pResult = 1;

	if(item.pszText) {
		UINT dIndex = GetItemDevIndex(item.hItem);
		UINT pIndex = GetItemPortIndex(item.hItem);
		CString ClassName = GetItemLabel(item.hItem);
		CString str = item.pszText;
		
		str.Replace(ClassName, "");
		str.Replace(" : ", "");
		str.Remove(':');
		theApp.m_pDevArray[dIndex]->m_PortLabel[pIndex] = str;
		if (str.IsEmpty())
			sprintf(item.pszText, ClassName);
		else
			sprintf(item.pszText, ClassName + " : " + str);
		SetItem(&item);
		*pResult = 0;
	}
}

void CTreeDevices::ExpandItem(HTREEITEM hti, UINT nCode)
{
	bool bOk = true;

	bOk = Expand(hti, nCode) != 0;

	if (bOk)
	{
		HTREEITEM htiChild = GetChildItem(hti);

		while (htiChild != NULL)
		{
			ExpandItem(htiChild, nCode);
			htiChild = GetNextSiblingItem(htiChild);
			GetItemText(htiChild);
		}
	}
}

void CTreeDevices::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LONG pResult;
	UINT uFlags;

	HTREEITEM hItem = HitTest(point, &uFlags);
	SelectItem(hItem);
	
	if (GetItemType(hItem) != ID_MULTIPLEXED)
		OnDisable();

	OnLclickTree(NULL, &pResult);

	CTreeCtrlEx::OnLButtonDblClk(nFlags, point);
}

void CTreeDevices::OnSelect(HTREEITEM hItem) 
{
	// Turn OFF lamp test if last item is ON
	switch (GetItemType(theApp.m_CurrenthItem)) {
		case ID_OUTPUT:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputArray[theApp.m_CurItemIndex].m_LampTest = 0;
			break;
		case ID_MUX:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxArray[theApp.m_CurItemIndex].m_LampTest = 0;
			break;
		case ID_MULTIPLEXED:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisplayArray[theApp.m_CurPortIndex].m_LampTest = 0;
			break;
		case ID_RWR:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrArray[theApp.m_CurPortIndex].m_RWRTest = 0;
			break;
		case ID_CLCD:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDArray[theApp.m_CurPortIndex].m_LCDTest = 0;
			break;
		case ID_GLCD:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDArray[theApp.m_CurPortIndex].m_LCDTest = 0;
			break;
		case ID_LO:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOArray[theApp.m_CurItemIndex].m_LampTest = 0;
			break;
		case ID_SPI:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIArray[theApp.m_CurItemIndex].m_SPITest = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIArray[theApp.m_CurItemIndex].m_SPICalibrate = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIArray[theApp.m_CurItemIndex].m_SPITemp = 0;
			break;
		case ID_STEP:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPTest = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPCalibrate = 0;
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPArray[theApp.m_CurItemIndex].m_STEPTemp = 0;
			break;
		case ID_DMX:
			theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXArray[theApp.m_CurItemIndex].m_LampTest = 0;
			break;
	}

	switch (GetItemType(hItem)) {
		case ID_PORT:
			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				theApp.m_CurrenthItem = hItem;
			}
			break;

		case ID_DEFAULT:
			// update default setting page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pDefTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pDefTabCtrl->UpdateWindow();
			}
			break;
/*
		case ID_UVARS:
			// update User Variables page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));
			}

			theApp.m_CurrenthItem = hItem;

			HideAllTabs();
			theApp.m_pUserVarsTabCtrl->ShowWindow(SW_SHOW);
			theApp.m_pUserVarsTabCtrl->InitAllTabs();
			break;
*/
		case ID_INPUT:
			// update digital input page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pInputsTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pInputsTabCtrl->InitAllTabs();
			}
			break;

		case ID_OUTPUT:
			// update digital output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pOutputTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pOutputTabCtrl->InitAllTabs();
			}
			break;

		case ID_MUX:
			// update mux output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pMuxTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pMuxTabCtrl->InitAllTabs();
			}
			break;

		case ID_MULTIPLEXED:
			// update 7-segment display output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);
			
			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pDispTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pDispTabCtrl->InitAllTabs();
			}
			break;

		case ID_RWR:
			// update RWR output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pRWRTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pRWRTabCtrl->InitAllTabs();
			}
			break;

		case ID_CLCD:
			// update CLCD output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pCLCDTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pCLCDTabCtrl->InitAllTabs();
			}
			break;

		case ID_GLCD:
			// update GLCD output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pGLCDTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pGLCDTabCtrl->InitAllTabs();
			}
			break;

		case ID_LO:
			// update Latched output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pLatchedTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pLatchedTabCtrl->InitAllTabs();
			}
			break;

		case ID_SPI:
			// update SPI output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pSPITabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pSPITabCtrl->InitAllTabs();
			}
			break;

		case ID_STEP:
			// update STEP output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);

			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pSTEPTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pSTEPTabCtrl->InitAllTabs();
			}
			break;

		case ID_DMX:
			// update dot matrix display output page
			theApp.m_CurDevIndex = GetItemDevIndex(hItem);
			theApp.m_CurItemIndex = GetItemIndex(hItem);
			theApp.m_CurPortIndex = GetItemPortIndex(hItem);
			
			SetItemBold(hItem, TRUE);

			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				SetItemBold(theApp.m_CurrenthItem, FALSE);
				SetItemColor(theApp.m_CurrenthItem, RGB(0,0,0));

				theApp.m_CurrenthItem = hItem;

				HideAllTabs();
				theApp.m_pDotMXTabCtrl->ShowWindow(SW_SHOW);
				theApp.m_pDotMXTabCtrl->InitAllTabs();
			}
			break;
	}
}

void CTreeDevices::HideAllTabs()
{
	theApp.m_pDefTabCtrl->ShowWindow(SW_HIDE);
//	theApp.m_pUserVarsTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pOutputTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pMuxTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pDispTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pRWRTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pCLCDTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pGLCDTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pInputsTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pLatchedTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pSPITabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pSTEPTabCtrl->ShowWindow(SW_HIDE);
	theApp.m_pDotMXTabCtrl->ShowWindow(SW_HIDE);
}

void CTreeDevices::OnLclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR;

	CString str;
	CPoint pt;
	UINT uFlags;
	HTREEITEM hItem;

	StopTests();

    GetCursorPos(&pt);
	ScreenToClient(&pt);

	hItem = HitTest(pt, &uFlags);
	SelectItem(hItem);

	if ((hItem != NULL) && ((TVHT_ONITEM | TVHT_ONITEMRIGHT | TVHT_ONITEMINDENT) & uFlags)) {		
		OnSelect(hItem);
	}

	*pResult = 0;
}

void CTreeDevices::StopTests()
{
	theApp.m_pInputsTabCtrl->m_SendKeysTab.KeyMacroRecord(false);

	theApp.m_pDispTabCtrl->m_DispTab.m_LampTest.SetCheck(false);
	theApp.m_pCLCDTabCtrl->m_CLCDTab.m_CLCDTest.SetCheck(false);
	theApp.m_pGLCDTabCtrl->m_GLCDTab.m_GLCDTest.SetCheck(false);
	theApp.m_pMuxTabCtrl->m_MuxTab.m_LampTest.SetCheck(false);
	theApp.m_pOutputTabCtrl->m_OutputTab.m_LampTest.SetCheck(false);
	theApp.m_pRWRTabCtrl->m_RWRTab.m_RWRTest.SetCheck(false);
	theApp.m_pLatchedTabCtrl->m_LatchedTab.m_LampTest.SetCheck(false);
	theApp.m_pSPITabCtrl->m_SPITab.m_SPITest.SetCheck(false);
	theApp.m_pSTEPTabCtrl->m_STEPTab.m_STEPTest.SetCheck(false);
	theApp.m_pDotMXTabCtrl->m_DotMXTab.m_LampTest.SetCheck(false);
}

void CTreeDevices::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR;

	OnLclickTree(pNMHDR, pResult);

	if( m_menuDevices.m_hMenu == NULL ) {
		if( m_menuDevices.CreatePopupMenu() ) {
			CNewMenu::SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
			CNewMenu::SetXpBlending(TRUE);
			CNewMenu::SetSelectDisableMode(FALSE);

			m_menuDevices.SetMenuTitle(_T("Device Tools"), MFT_TOP_TITLE);
			m_menuDevices.AppendMenu( MF_STRING, ID_REFRESH, _T("Refresh device list"));
			m_menuDevices.AppendMenu( MF_STRING, ID_CLEAR, _T("Clear device settings"));
			m_menuDevices.AppendMenu( MF_STRING, ID_SWAP, _T("Transfer settings..."));
			m_menuDevices.AppendMenu( MF_STRING, ID_REMOVE, _T("Remove device"));
			
			m_menuDevices.AppendMenu( MF_SEPARATOR );

			m_menuDevices.AppendMenu( MF_STRING | MF_CHECKED, ID_DISABLE_ITEM, _T("Enable Item"));
			m_menuDevices.AppendMenu( MF_STRING, ID_CLEAR_ITEM, _T("Clear item settings"));
			m_menuDevices.AppendMenu( MF_STRING, ID_COPY_ITEM, _T("Copy item settings"));
			m_menuDevices.AppendMenu( MF_STRING, ID_PASTE_ITEM, _T("Paste item settings"));
			m_menuDevices.AppendMenu( MF_STRING, ID_EXPORT_ITEM, _T("Export item settings..."));
			m_menuDevices.AppendMenu( MF_STRING, ID_IMPORT_ITEM, _T("Import item settings..."));

			m_menuDevices.AppendMenu( MF_SEPARATOR );

			m_menuDevices.AppendMenu( MF_STRING | MF_CHECKED, ID_HIDE_ITEMS, _T("Hide unused items"));
			m_menuDevices.AppendMenu( MF_STRING | MF_CHECKED, ID_HIDE_PORTS, _T("Hide inactive ports"));
			m_menuDevices.AppendMenu( MF_STRING | MF_CHECKED, ID_HIDE_MODES, _T("Hide inactive modes"));

			m_menuDevices.AppendMenu( MF_SEPARATOR );

			m_menuDevices.AppendMenu( MF_STRING, ID_EXPAND, _T( "Expand all" ) );
			m_menuDevices.AppendMenu( MF_STRING, ID_COLLAPSE, _T( "Collapse all" ) );
		}
	}

	m_menuDevices.LoadToolBar(IDR_POPMENU);
	UINT refresh = MF_ENABLED;
	UINT swap = MF_DISABLED | MF_GRAYED;
	UINT clear = MF_DISABLED | MF_GRAYED;
	UINT remove = MF_DISABLED | MF_GRAYED;
	UINT expand = MF_ENABLED;
	UINT item = MF_DISABLED | MF_GRAYED | MF_UNCHECKED;

	UINT clearitem = MF_DISABLED | MF_GRAYED;
	UINT copyitem = MF_DISABLED | MF_GRAYED;
	UINT pasteitem = MF_DISABLED | MF_GRAYED;
	UINT exportitem = MF_DISABLED | MF_GRAYED;
	UINT importitem = MF_DISABLED | MF_GRAYED;

	UINT hide = MF_ENABLED;
	UINT hideports = MF_ENABLED;
	UINT hidemodes = MF_ENABLED;

	CPoint pt;
    GetCursorPos(&pt);
	ScreenToClient(&pt);

	UINT uFlags;
	HTREEITEM hItem = HitTest(pt, &uFlags);
	SelectItem(hItem);
	int ioIndex, pIndex;
	CString sLabel;

	m_ItemTypeString.Empty();

	if (hItem != NULL) {
		switch (GetItemType(hItem)) {
			case ID_DEVICE:
//				swap = MF_ENABLED;
				clear = MF_ENABLED;
				break;
			case ID_VIRTUAL:
				swap = MF_ENABLED;
				remove = MF_ENABLED;
				break;
			default:
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);
				sLabel = GetItemLabel(hItem);
				switch (GetItemType(hItem)) {
					case ID_INPUT:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_INPUT)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputActive[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_INPUT_%s_%s", Alpha[pIndex], sLabel);
						break;

					case ID_OUTPUT:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_OUTPUT)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_OUTPUT_%s_%s", Alpha[pIndex], sLabel);
						break;

					case ID_MULTIPLEXED:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_MULTIPLEXED)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_MULTIPLEXED_%s", Alpha[ioIndex]);
						break;
					
					case ID_MUX:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_MUX)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxDisabled[ioIndex])
							item |= MF_CHECKED;
						
						m_ItemTypeString.Format("_MUX_OUTPUT_%s_%s", Alpha[pIndex], sLabel);
						break;

					case ID_RWR:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_RWR)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_RWR_%s", Alpha[ioIndex]);
						break;

					case ID_CLCD:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_CLCD)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_CLCD_%s", Alpha[ioIndex]);
						break;

					case ID_GLCD:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_GLCD)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_GLCD_%s", Alpha[ioIndex]);
						break;

					case ID_LO:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_LO)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LODisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_LO_%s_%s", Alpha[pIndex], sLabel);
						break;

					case ID_SPI:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_SPI)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_SPI_%s_%s", Alpha[pIndex], sLabel);
						break;

					case ID_STEP:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_STEP)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_STEPPER_%s_%s", Alpha[pIndex], sLabel);
						break;

					case ID_DMX:
						item = MF_ENABLED;
						clearitem = MF_ENABLED;
						copyitem = MF_ENABLED;
						if (theApp.m_CopyItemClass == ID_DMX)
							pasteitem = MF_ENABLED;
						exportitem = MF_ENABLED;
						importitem = MF_ENABLED;

						if (!theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXDisabled[ioIndex])
							item |= MF_CHECKED;

						m_ItemTypeString.Format("_DOTMATRIX_%s_%s", Alpha[pIndex], sLabel);
						break;
				}
				break;
		}
	}

	if (theApp.m_HideItems)
		hide = MF_CHECKED;
	else
		hide = MF_UNCHECKED;

	if (theApp.m_HidePorts)
		hideports = MF_CHECKED;
	else
		hideports = MF_UNCHECKED;

	if (theApp.m_HideModes)
		hidemodes = MF_CHECKED;
	else
		hidemodes = MF_UNCHECKED;

	m_menuDevices.EnableMenuItem( ID_REFRESH, MF_BYCOMMAND | refresh );
	m_menuDevices.EnableMenuItem( ID_SWAP, MF_BYCOMMAND | swap );
	m_menuDevices.EnableMenuItem( ID_CLEAR, MF_BYCOMMAND | clear );
	m_menuDevices.EnableMenuItem( ID_CLEAR_ITEM, MF_BYCOMMAND | clearitem );

	m_menuDevices.EnableMenuItem( ID_COPY_ITEM, MF_BYCOMMAND | copyitem );
	m_menuDevices.EnableMenuItem( ID_PASTE_ITEM, MF_BYCOMMAND | pasteitem );
	m_menuDevices.EnableMenuItem( ID_EXPORT_ITEM, MF_BYCOMMAND | exportitem );
	m_menuDevices.EnableMenuItem( ID_IMPORT_ITEM, MF_BYCOMMAND | importitem );

	m_menuDevices.EnableMenuItem( ID_REMOVE, MF_BYCOMMAND | remove );
	m_menuDevices.EnableMenuItem( ID_DISABLE_ITEM, MF_BYCOMMAND | item );
	m_menuDevices.EnableMenuItem( ID_HIDE_ITEMS, MF_BYCOMMAND | hide );
	m_menuDevices.EnableMenuItem( ID_EXPAND, MF_BYCOMMAND | expand );
	m_menuDevices.EnableMenuItem( ID_COLLAPSE, MF_BYCOMMAND | expand );

	m_menuDevices.CheckMenuItem( ID_DISABLE_ITEM, MF_BYCOMMAND | item );
	m_menuDevices.CheckMenuItem( ID_HIDE_ITEMS, MF_BYCOMMAND | hide );
	m_menuDevices.CheckMenuItem( ID_HIDE_MODES, MF_BYCOMMAND | hidemodes );
	m_menuDevices.CheckMenuItem( ID_HIDE_PORTS, MF_BYCOMMAND | hideports );

    GetCursorPos(&pt);
	m_menuDevices.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CTreeDevices::OnDisable() 
{
	HTREEITEM hItem;
	int ioIndex, pIndex;

	hItem = GetSelectedItem();

	if (hItem != NULL) {
		switch (GetItemType(hItem)) {
			case ID_INPUT:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_INPUT) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputDisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_InputDisabled[ioIndex];
					theApp.m_pInputsTabCtrl->InitAllTabs();
				}
				break;

			case ID_OUTPUT:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DIRECT &&
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputDisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_OutputDisabled[ioIndex];
					theApp.m_pOutputTabCtrl->InitAllTabs();
				}
				break;

			case ID_MULTIPLEXED:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DisphItem[pIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MUX &&
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispDisabled[pIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DispDisabled[pIndex];
					theApp.m_pDispTabCtrl->InitAllTabs();
				}
				break;

			case ID_MUX:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MUX) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxDisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_MuxDisabled[ioIndex];
					theApp.m_pMuxTabCtrl->InitAllTabs();
				}
				break;

			case ID_RWR:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrhItem[pIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_RWR) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrDisabled[pIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CrtRwrDisabled[pIndex];
					theApp.m_pRWRTabCtrl->InitAllTabs();
				}
				break;

			case ID_CLCD:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDhItem[pIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_CLCD &&
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDDisabled[pIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_CLCDDisabled[pIndex];
					theApp.m_pCLCDTabCtrl->InitAllTabs();
				}
				break;

			case ID_GLCD:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDhItem[pIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_GLCD) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDDisabled[pIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_GLCDDisabled[pIndex];
					theApp.m_pGLCDTabCtrl->InitAllTabs();
				}
				break;

			case ID_LO:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LOhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_LATCHED &&
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LODisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_LODisabled[ioIndex];
					theApp.m_pLatchedTabCtrl->InitAllTabs();
				}
				break;

			case ID_SPI:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_SPI &&
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DAC &&
						theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIDisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_SPIDisabled[ioIndex];
					theApp.m_pSPITabCtrl->InitAllTabs();
				}
				break;

			case ID_STEP:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_STEPPER) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPDisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_STEPDisabled[ioIndex];
					theApp.m_pSTEPTabCtrl->InitAllTabs();
				}
				break;

			case ID_DMX:
				// Toggle item disable
				// find device in tree list
				ioIndex = GetItemIndex(hItem);
				pIndex = GetItemPortIndex(hItem);

				if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXhItem[ioIndex] == hItem) {
					//found item
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortActive[pIndex] == 0) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) return;
					if (theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DOTMATRIX) return;

					theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXDisabled[ioIndex] = !theApp.m_pDevArray[theApp.m_CurDevIndex]->m_DotMXDisabled[ioIndex];
					theApp.m_pDotMXTabCtrl->InitAllTabs();
				}
				break;
		}
	}
}

void CTreeDevices::OnHideItems() 
{
	theApp.m_HideItems = !theApp.m_HideItems;
	theApp.WriteProfileInt("Options", "HideItems", theApp.m_HideItems);

	OnCollapse();
	EnumDevices();

	theApp.m_CurrenthItem = nLocal;
	OnSelect(nDefault);
}

void CTreeDevices::OnHidePorts() 
{
	theApp.m_HidePorts = !theApp.m_HidePorts;
	theApp.WriteProfileInt("Options", "HidePorts", theApp.m_HidePorts);

	OnCollapse();
	EnumDevices();

	theApp.m_CurrenthItem = nLocal;
	OnSelect(nDefault);
}

void CTreeDevices::OnHideModes() 
{
	theApp.m_HideModes = !theApp.m_HideModes;
	theApp.WriteProfileInt("Options", "HideModes", theApp.m_HideModes);

	OnCollapse();
	EnumDevices();

	theApp.m_CurrenthItem = nLocal;
	OnSelect(nDefault);
}

void CTreeDevices::OnClearDevice() 
{
	HTREEITEM hItem, tmphItem;
	DWORD i;

	hItem = GetSelectedItem();
	if (hItem != NULL) {
		if (GetItemType(hItem) == ID_DEVICE) {
			// clear all data items from list for this device
			i = GetItemDevIndex(hItem); // Device Index index;

			theApp.m_pDevArray[i]->ClearDeviceData();
			if (theApp.m_CurrenthItem != NULL && theApp.m_CurrenthItem != hItem) {
				tmphItem = theApp.m_CurrenthItem;
				theApp.m_CurrenthItem = hItem;
				OnSelect(tmphItem);	// call this to clear currently displayed page
			}
		}
	}
}

void CTreeDevices::OnClearItem() 
{
	HTREEITEM hItem;
	DWORD dIndex, pIndex, iIndex;

	hItem = GetSelectedItem();
	if (hItem != NULL) {
		dIndex = GetItemDevIndex(hItem);
		iIndex = GetItemIndex(hItem);
		pIndex = GetItemPortIndex(hItem);
		switch (GetItemType(hItem)) {
			case ID_INPUT:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_INPUT, iIndex);
				theApp.m_pInputsTabCtrl->InitAllTabs();
				break;

			case ID_OUTPUT:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_OUTPUT, iIndex);
				theApp.m_pOutputTabCtrl->InitAllTabs();
				break;

			case ID_MULTIPLEXED:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_MULTIPLEXED, pIndex);
				theApp.m_pDispTabCtrl->InitAllTabs();
				break;

			case ID_MUX:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_MUX, iIndex);
				theApp.m_pMuxTabCtrl->InitAllTabs();
				break;

			case ID_RWR:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_RWR, pIndex);
				theApp.m_pRWRTabCtrl->InitAllTabs();
				break;

			case ID_CLCD:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_CLCD, pIndex);
				theApp.m_pCLCDTabCtrl->InitAllTabs();
				break;

			case ID_GLCD:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_GLCD, pIndex);
				theApp.m_pGLCDTabCtrl->InitAllTabs();
				break;

			case ID_LO:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_LO, iIndex);
				theApp.m_pLatchedTabCtrl->InitAllTabs();
				break;

			case ID_SPI:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_SPI, iIndex);
				theApp.m_pSPITabCtrl->InitAllTabs();
				break;

			case ID_STEP:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_STEP, iIndex);
				theApp.m_pSTEPTabCtrl->InitAllTabs();
				break;

			case ID_DMX:
				theApp.m_pDevArray[dIndex]->ClearItemData(ID_DMX, iIndex);
				theApp.m_pDotMXTabCtrl->InitAllTabs();
				break;
		}
	}
}

void CTreeDevices::OnCopyItem() 
{
	DWORD iItemType = 0;
	DWORD dIndex, pIndex, iIndex;

	if (theApp.m_CurrenthItem != NULL) {
		dIndex = GetItemDevIndex(theApp.m_CurrenthItem);
		iIndex = GetItemIndex(theApp.m_CurrenthItem);
		pIndex = GetItemPortIndex(theApp.m_CurrenthItem);
		iItemType = GetItemType(theApp.m_CurrenthItem);

		switch (iItemType) {
			case ID_INPUT:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_INPUT, iIndex);
				break;

			case ID_OUTPUT:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_OUTPUT, iIndex);
				break;

			case ID_MULTIPLEXED:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_MULTIPLEXED, pIndex);
				break;

			case ID_MUX:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_MUX, iIndex);
				break;

			case ID_RWR:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_RWR, pIndex);
				break;

			case ID_CLCD:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_CLCD, pIndex);
				break;

			case ID_GLCD:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_GLCD, pIndex);
				break;

			case ID_LO:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_LO, iIndex);
				break;

			case ID_SPI:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_SPI, iIndex);
				break;

			case ID_STEP:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_STEP, iIndex);
				break;

			case ID_DMX:
				theApp.m_CopyItemClass = iItemType;
				theApp.m_DevTemp.CopyItemData(theApp.m_pDevArray[dIndex], ID_DMX, iIndex);
				break;
		}
	}
}

void CTreeDevices::OnPasteItem() 
{
	DWORD iItemType = 0;
	DWORD dIndex, pIndex, iIndex;

	if (theApp.m_CurrenthItem != NULL) {
		dIndex = GetItemDevIndex(theApp.m_CurrenthItem);
		iIndex = GetItemIndex(theApp.m_CurrenthItem);
		pIndex = GetItemPortIndex(theApp.m_CurrenthItem);
		iItemType = GetItemType(theApp.m_CurrenthItem);

		switch (theApp.m_CopyItemClass) {
			case ID_INPUT:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_INPUT, iIndex);
				theApp.m_pInputsTabCtrl->InitAllTabs();
				break;

			case ID_OUTPUT:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_OUTPUT, iIndex);
				theApp.m_pOutputTabCtrl->InitAllTabs();
				break;

			case ID_MULTIPLEXED:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_MULTIPLEXED, pIndex);
				theApp.m_pDispTabCtrl->InitAllTabs();
				break;

			case ID_MUX:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_MUX, iIndex);
				theApp.m_pMuxTabCtrl->InitAllTabs();
				break;

			case ID_RWR:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_RWR, pIndex);
				theApp.m_pRWRTabCtrl->InitAllTabs();
				break;

			case ID_CLCD:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_CLCD, pIndex);
				theApp.m_pCLCDTabCtrl->InitAllTabs();
				break;

			case ID_GLCD:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_GLCD, pIndex);
				theApp.m_pGLCDTabCtrl->InitAllTabs();
				break;

			case ID_LO:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_LO, iIndex);
				theApp.m_pLatchedTabCtrl->InitAllTabs();
				break;

			case ID_SPI:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_SPI, iIndex);
				theApp.m_pSPITabCtrl->InitAllTabs();
				break;

			case ID_DMX:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_DMX, iIndex);
				theApp.m_pDotMXTabCtrl->InitAllTabs();
				break;

			case ID_STEP:
				theApp.m_DevTemp.PasteItemData(theApp.m_pDevArray[dIndex], ID_STEP, iIndex);
				theApp.m_pSTEPTabCtrl->InitAllTabs();
				break;
		}
	}
}

void CTreeDevices::OnExportItem() 
{
	CString str, m_ItemFileName, m_ItemFilePath;

	char strFilter[] = { "USIM Item Settings Configuration Files (*.isc)|*.isc|All Files (*.*)|*.*||" };

	if (m_ItemFileName.GetLength() == 0) {
		m_ItemFileName = "usim" + m_ItemTypeString + ".isc";
	}

	CFileDialog FileDlg(FALSE, ".isc", m_ItemFileName, OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, strFilter);

	if( FileDlg.DoModal() != IDOK )	return;
	
	m_ItemFileName = FileDlg.GetFileName();
	m_ItemFilePath = FileDlg.GetPathName();
	
	str.Format ("%susim.log", theApp.m_AppPath);
	FILE* stream = fopen(str, "a");

	// Save all changes to file
	CFile *file = new CFile();
	CFileException exception;

	// attempt opening a file
	str.Format ("%s", m_ItemFilePath);
	
	if ( !file->Open( str, CFile::modeCreate | CFile::modeWrite, &exception ) ) {
		// an error occured opening file
		str.Format("\nERROR : %d - ISC save failed!\n", exception.m_cause);
		TRACE(str);
		fprintf(stream, str);
		fclose(stream);
		return;
	}

	DWORD iItemType, dIndex, pIndex, iIndex;

	if (theApp.m_CurrenthItem != NULL) {
		dIndex = GetItemDevIndex(theApp.m_CurrenthItem);
		iIndex = GetItemIndex(theApp.m_CurrenthItem);
		pIndex = GetItemPortIndex(theApp.m_CurrenthItem);
		iItemType = GetItemType(theApp.m_CurrenthItem);

		// now create a CArchive object
		CArchive archive( file, CArchive::store );

		try {
			theApp.m_pDevArray[dIndex]->SerializeItem(archive, iItemType, iIndex, pIndex);
		} catch ( CException *err ) {
			// cleanup
			err->Delete();
		}

		// everything went fine, so close
		// and return success
		archive.Close();
	}

	file->Close();
	delete file;
	if (stream != NULL) fclose(stream);

	TRACE(m_ItemFilePath);
}

void CTreeDevices::OnImportItem() 
{
	CString str, m_ItemFileName, m_ItemFilePath;

	char strFilter[] = { "USIM Item Settings Configuration Files (*.isc)|*.isc|All Files (*.*)|*.*||" };
	
	CFileDialog FileDlg(TRUE, ".isc", NULL, OFN_HIDEREADONLY | OFN_READONLY | OFN_NOCHANGEDIR, strFilter);

	if (FileDlg.DoModal() != IDOK) return;

	m_ItemFileName = FileDlg.GetFileName();
	m_ItemFilePath = FileDlg.GetPathName();
	if (m_ItemFilePath.IsEmpty()) return;

	str.Format("%susim.log", theApp.m_AppPath);
	FILE* stream = fopen(str, "w");
	if (stream != NULL)
		fprintf(stream, "Log File Started.\n");

	CFile *file = new CFile();
	CFileException exception;

	// attempt opening a file
	str.Format("%s", m_ItemFilePath);
	if ( !file->Open( str, CFile::modeRead, &exception ) ) {
		// an error occured opening file
//		MessageBox (_T("Cannot open ") + theApp.m_FileName, _T("File Load Error"), MB_ICONEXCLAMATION);
		fprintf(stream, "\nError opening file : Exception %d\n", exception.m_cause);
		TRACE("\nError opening file : Exception %d\n", exception.m_cause);
		delete file;
		if (stream != NULL) fclose(stream);

		return;
	}

	DWORD iItemType, dIndex, pIndex, iIndex;

	if (theApp.m_CurrenthItem != NULL) {
		dIndex = GetItemDevIndex(theApp.m_CurrenthItem);
		iIndex = GetItemIndex(theApp.m_CurrenthItem);
		pIndex = GetItemPortIndex(theApp.m_CurrenthItem);
		iItemType = GetItemType(theApp.m_CurrenthItem);

		// now create a CArchive object
		CArchive archive( file, CArchive::load );

		try {
			theApp.m_pDevArray[dIndex]->SerializeItem(archive, iItemType, iIndex, pIndex);
		} catch ( CException *err ) {
			// cleanup
			err->Delete();
		}

		// everything went good, so close
		// and return success
		archive.Close();
	}

	file->Close();
	delete file;
	if (stream != NULL) fclose(stream);

	TRACE(m_ItemFilePath);
}

void CTreeDevices::OnReplace()
{
	// if virtual item, swap it with real device
	HTREEITEM hItem;
	DWORD i;

	hItem = GetSelectedItem();
	if (hItem != NULL) {
		if (GetItemType(hItem) == ID_VIRTUAL) {
			// clear all data items from list for this device
			// find device in tree list
			m_CurrentIndex = GetItemDevIndex(hItem); // Device Index index;
			m_CurrenthItem = hItem;

			m_ReplaceDevice.DoModal();

			if (m_ReplacePos != NULL) {
				// replace with returned position.
				i = theApp.m_DevArrayList.GetAt(m_ReplacePos);

				theApp.m_pDevArray[i]->CopyArrayData(theApp.m_pDevArray[m_CurrentIndex]);
			}
		}
	}
}

void CTreeDevices::OnRemove() 
{
	// if virtual item, remove from array list
	HTREEITEM hItem;
	DWORD i;

	hItem = GetSelectedItem();
	if (hItem != NULL) {
		if (GetItemType(hItem) == ID_VIRTUAL) {
			//Clear the array data

			theApp.StopSimConnectThread();
			theApp.StopAllThreads();

			i = GetItemDevIndex(hItem);	// Device Index index
			delete theApp.m_pDevArray[i];
			theApp.m_pDevArray[i] = NULL;
			//poss = theApp.m_DevArrayList.Find(i);
			theApp.m_DevArrayList.RemoveAt(theApp.m_DevArrayList.Find(i));

			// remove tree item.
			DeleteItem(hItem);

			theApp.m_CurrenthItem = nLocal;
			OnSelect(nDefault);

			theApp.StartAllThreads();
			theApp.StartSimConnectThread();
		}
	}
}

void CTreeDevices::OnExpand() 
{
	SetRedraw(FALSE);
	ExpandItem();
	SetRedraw(TRUE);
}

void CTreeDevices::OnCollapse() 
{
	SetRedraw(FALSE);
	ExpandItem(TVI_ROOT, TVE_COLLAPSE);
	SetRedraw(TRUE);
}

void CTreeDevices::OnRefreshList() 
{
	if (m_ReEntryFlag)
		return;

	m_ReEntryFlag = true;

	// save settings to temp file
	CString FileTemp = theApp.m_FilePath;
	theApp.m_FilePath = "temp.imc";
	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd; 
	pUSIMDlg->OnFileSave();

	// load settings from temp file
	theApp.m_SimConnectThread.Pause();
	theApp.StopAllThreads();

	pUSIMDlg->OnLoadFile();
	theApp.m_FilePath = FileTemp;

	OnRefresh();
	theApp.m_SimConnectThread.Continue();
	
	m_ReEntryFlag = false;
}

void CTreeDevices::OnRefresh()
{
	theApp.m_CurrenthItem = NULL;

	HideAllTabs();
	theApp.m_pDefTabCtrl->ShowWindow(SW_SHOW);
	theApp.m_pDefTabCtrl->UpdateWindow();

	// Close all HID devices
	if (!CloseDevices(theApp.m_DevCnt, theApp.m_DeviceList))
		MessageBox ("An error occured while attempting to close device!", _T("Close Device Failed"), MB_ICONEXCLAMATION);

	theApp.m_DevCnt = 0;

	DetectDevices(TRUE);
	OnInit();

	theApp.m_HideItems = theApp.GetProfileInt("Options", "HideItems", false);

	theApp.StartAllThreads();

	Invalidate(true);
}

void CTreeDevices::DetectDevices(BOOL DetectFlag)
{
	UINT i, j;
	BOOL FreeAgent;
	UINT DevCntTemp = 0;

	// Stop all worker threads
	// Stop TreeIcon thread
	CUSIMDlg *pUSIMDlg = (CUSIMDlg *)theApp.m_pMainWnd;
	pUSIMDlg->StopTreeThread();

	// look for and reset any FreeAgent items
	POSITION pos = theApp.m_DevArrayList.GetHeadPosition();
	while (pos != NULL) {
		i = theApp.m_DevArrayList.GetNext(pos);
		theApp.m_pDevArray[i]->m_Connected = FALSE;

		if (theApp.m_pDevArray[i]->m_FreeAgent) {
			theApp.m_DevArrayList.RemoveAt(theApp.m_DevArrayList.Find(i));
			theApp.m_pDevArray[i]->m_FreeAgent = FALSE;
			if (theApp.m_pDevArray[i] != NULL)
				delete theApp.m_pDevArray[i];
			theApp.m_pDevArray[i] = NULL;
		}
	}

	int listcnt = theApp.m_DevArrayList.GetCount();

    // The DetectDevice function returns the number "m_DevCnt" of devices detected and
    // the path / name string length in chars.
	if (DetectFlag) {
		if (!DetectDevice(&theApp.m_DevCnt, theApp.m_DeviceList, 0, DT_STANDARD)) {
			theApp.m_DevCnt = 0;
		}

		// Special case for PLV2 CFG for outputs
		if (DetectDevice(&DevCntTemp, &theApp.m_DeviceList[theApp.m_DevCnt], PIDC_PLASMA_LITE_V2, DT_DEVICES)) {
			theApp.m_DevCnt += DevCntTemp;
		}

		// get all port status data
		for (i = 0; i < theApp.m_DevCnt; i++) {
			RetrieveStatus(&theApp.m_DeviceList[i], &theApp.m_DeviceStatus[i]);
		}
	}

	for (i = 0; i < theApp.m_DevCnt; i++) {
		FreeAgent = TRUE;

		pos = theApp.m_DevArrayList.GetHeadPosition();
		while (pos != NULL) {
			j = theApp.m_DevArrayList.GetNext(pos);
			if (theApp.m_pDevArray[j]->m_DeviceParam.VendorID == theApp.m_DeviceList[i].VendorID &&
				theApp.m_pDevArray[j]->m_DeviceParam.ProductID == theApp.m_DeviceList[i].ProductID &&
				theApp.m_pDevArray[j]->m_DeviceParam.ConfigFlag == theApp.m_DeviceList[i].ConfigFlag) {

				if (strncmp(theApp.m_pDevArray[j]->m_DeviceParam.SerialNum, theApp.m_DeviceList[i].SerialNum, 3) == 0
					&& strncmp(theApp.m_DeviceList[i].SerialNum, "N/A", 3) != 0) { // compare serials if any
					// same device found
					theApp.m_pDevArray[j]->m_Local = TRUE;
					theApp.m_pDevArray[j]->m_Connected = TRUE;
					theApp.m_pDevArray[j]->m_FreeAgent = FALSE;
					theApp.m_pDevArray[j]->CopyDeviceParam(&theApp.m_DeviceList[i]);
					theApp.m_pDevArray[j]->CopyDeviceStatus(&theApp.m_DeviceStatus[i]);
					FreeAgent = FALSE;
					break;
				} else {
					// find similar class device
					// no serial found, compare paths
					if ( strcmp(theApp.m_pDevArray[j]->m_DeviceParam.DevicePath, theApp.m_DeviceList[i].DevicePath) == 0) {
						// same device found
						theApp.m_pDevArray[j]->m_Local = TRUE;
						theApp.m_pDevArray[j]->m_Connected = TRUE;
						theApp.m_pDevArray[j]->m_FreeAgent = FALSE;
						theApp.m_pDevArray[j]->CopyDeviceParam(&theApp.m_DeviceList[i]);
						theApp.m_pDevArray[j]->CopyDeviceStatus(&theApp.m_DeviceStatus[i]);
						FreeAgent = FALSE;
						break;
					} else {
						// same device paths not found
						// automatically assign 1st matching device
						// assume same device found
						if (!theApp.m_pDevArray[j]->m_Connected) {
							// virtual device found
							theApp.m_pDevArray[j]->m_Local = TRUE;
							theApp.m_pDevArray[j]->m_Connected = TRUE;
							theApp.m_pDevArray[j]->m_FreeAgent = FALSE;
							theApp.m_pDevArray[j]->CopyDeviceParam(&theApp.m_DeviceList[i]);
							theApp.m_pDevArray[j]->CopyDeviceStatus(&theApp.m_DeviceStatus[i]);
							FreeAgent = FALSE;
							break;
						}
					}
				}
			}
		} // while (pos != NULL)

		if (FreeAgent && listcnt < MAX_DEVICES) {
			// device not listed anywhere
			// assign to new array item
			// find a free spot in DevArray
			for (j=0; j<MAX_DEVICES; j++) {
				if (theApp.m_pDevArray[j] == NULL) {
					// free spot found
					theApp.m_pDevArray[j] = new CSaveDevice();
					theApp.m_pDevArray[j]->m_Local = TRUE;
					theApp.m_pDevArray[j]->m_Connected = TRUE;
					theApp.m_pDevArray[j]->m_FreeAgent = TRUE;
					theApp.m_pDevArray[j]->CopyDeviceParam(&theApp.m_DeviceList[i]);
					theApp.m_pDevArray[j]->CopyDeviceStatus(&theApp.m_DeviceStatus[i]);
					theApp.m_DevArrayList.AddTail(j);
					break;
				}
			}
		} // if (FreeAgent && listcnt < MAX_DEVICES)
    } // for (i = 0; i < theApp.m_HIDDevCnt; i++)

	// Restart all worker threads
	pUSIMDlg->StartTreeThread();
}

void CTreeDevices::OnInit()
{
	//Setup the Image list for HID Device Icons
	m_ImageListDevices.DeleteImageList();
	if ( m_ImageListDevices.GetSafeHandle() == NULL )
		m_ImageListDevices.Create(16, 16, ILC_MASK | ILC_COLOR8, 0, 1);

	m_ImageListDevices.Add(theApp.LoadIcon(IDI_NA));			// 0
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_QUE_YELLOW));	// 1
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_PC3));			// 2
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_NETHOOD));		// 3
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_USB));			// 4
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_CARD));			// 5
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_JOY));			// 6
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_INPUT));			// 7
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_OUTPUT));		// 8
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_JP));			// 9
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_ROTARY));		// 10
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_ALPHA));			// 11
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_JP_DIS));		// 12
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_INPUT_DIS));		// 13
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_ROTARY_DIS));	// 14
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_OUTPUT_DIS));	// 15
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_MUX_DIS));		// 16
	m_ImageListDevices.Add(theApp.LoadIcon(IDI_USIM_OFF));		// 17
	m_CARD =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_CARD));
	m_NOCARD =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_CARD3));
	m_RD_ON =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_ON));
	m_RD_OFF =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_OFF));
	m_RD_ON_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_ON_P));
	m_RD_OFF_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_OFF_P));
	m_GR_ON =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_GR_ON));
	m_GR_OFF =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_GR_OFF));
	m_GR_OFF_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_GR_OFF_P));
	m_GR_ON_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_GR_ON_P));
	m_DIS_ON =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DIS_ON));
	m_DIS_ON_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_DIS_ON_P));
	m_DIS_OFF =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DIS_OFF));
	m_DIS_OFF_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_DIS_OFF_P));
	m_CLCD_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_CLCD_DIS));
	m_CLCD_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_CLCD_DIS_P));
	m_CLCD =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_CLCD));
	m_CLCD_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_CLCD_P));
	m_MUX_DIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_MUX_DIS));
	m_MUX_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_MUX_DIS_P));
	m_MUX =			m_ImageListDevices.Add(theApp.LoadIcon(IDI_MUX));
	m_MUX_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_MUX_P));
	m_RWR_DIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RWR_DIS));
	m_RWR_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_RWR_DIS_P));
	m_RWR =			m_ImageListDevices.Add(theApp.LoadIcon(IDI_RWR));
	m_RWR_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RWR_P));
	m_GLCD_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_GLCD_DIS));
	m_GLCD_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_GLCD_DIS_P));
	m_GLCD =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_GLCD));
	m_GLCD_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_GLCD_P));
	m_DAC_DIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DAC_DIS));
	m_DAC_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_DAC_DIS_P));
	m_DAC =			m_ImageListDevices.Add(theApp.LoadIcon(IDI_DAC));
	m_DAC_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DAC_P));
	m_DACS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DACS));
	m_DACS_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_DACS_DIS));
	m_SPI_DIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_SPI_DIS));
	m_SPI_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_SPI_DIS_P));
	m_SPI =			m_ImageListDevices.Add(theApp.LoadIcon(IDI_SPI));
	m_SPI_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_SPI_P));
	m_SPIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_SPIS));
	m_SPIS_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_SPIS_DIS));
	m_LO =			m_ImageListDevices.Add(theApp.LoadIcon(IDI_LO));
	m_LO_DIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_LO_DIS));
	m_LO_ON =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_ON));
	m_LO_OFF =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_OFF));
	m_LO_ON_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_ON_P));
	m_LO_OFF_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_RED_OFF_P));
	m_DMXS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DMXS));
	m_DMXS_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_DMXS_DIS));
	m_DMX_DIS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DMX_DIS));
	m_DMX_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_DMX_DIS_P));
	m_DMX =			m_ImageListDevices.Add(theApp.LoadIcon(IDI_DMX));
	m_DMX_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_DMX_P));
	m_STEPS =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_STEPS));
	m_STEPS_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_STEPS_DIS));
	m_STEP_DIS =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_STEP_DIS));
	m_STEP_DIS_P =	m_ImageListDevices.Add(theApp.LoadIcon(IDI_STEP_DIS_P));
	m_STEP =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_STEP));
	m_STEP_P =		m_ImageListDevices.Add(theApp.LoadIcon(IDI_STEP_P));

	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, WS_EX_CLIENTEDGE);
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
											| TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT
											| LVS_SHAREIMAGELISTS | TVS_DISABLEDRAGDROP | TVS_EDITLABELS); 

	SetImageList( &m_ImageListDevices, TVSIL_NORMAL );

	EnumDevices();
}

void CTreeDevices::EnumDevices()
{
	HTREEITEM n, nClass;
	CString ClassName;
	int ClassImage, i, j;

	DeleteAllItems();

	HTREEITEM hItem;
	nDefault = InsertItem("Application Settings", 17, 17);
		SetItemBold(nDefault, true);
		SetItemColor(nDefault, RGB(0,0,0));
		SetItemType(nDefault, ID_DEFAULT);	// Specify type

		theApp.m_CurrenthItem = nDefault;
/*
	nUserVars = InsertItem("User Defined Variables", 5, 5);
		SetItemBold(nUserVars, false);
		SetItemColor(nUserVars, RGB(0,0,0));
		SetItemType(nUserVars, ID_UVARS);	// Specify type
*/
	nLocal = InsertItem("Local Devices", 2, 2);
		SetItemBold(nLocal, true);
		SetItemColor(nLocal, RGB(0,0,0));
		SetItemType(nLocal, ID_LOCAL);
		hItem = InsertItem("Non Detected", 0, 0, nLocal);
		SetItemType(hItem, ID_NONE);	// Specify no device
/*
	nNetwork = InsertItem("Network Devices", 3, 3);
		SetItemBold(nNetwork, true);
		SetItemColor(nNetwork, RGB(0,0,0));
		SetItemType(nNetwork, ID_NETWORK);	// Specify type
		hItem = InsertItem("Non Detected", 0, 0, nNetwork);
		SetItemType(hItem, ID_NONE);	// Specify no device
*/
	OnExpand();

	// Add all devices in DevArray
	POSITION pos = theApp.m_DevArrayList.GetHeadPosition();
	while (pos != NULL) {
		i = theApp.m_DevArrayList.GetNext(pos);

		nClass = nLocal;
		ClassName.Format("%s : %s", theApp.m_pDevArray[i]->m_DeviceParam.DeviceName,
									theApp.m_pDevArray[i]->m_DeviceParam.SerialNum);
		if (theApp.m_pDevArray[i]->m_Connected)
			ClassImage = m_CARD;
		else
			ClassImage = m_NOCARD;

		n = GetChildItem(nClass);
		// check if "non" icon is present
		if (GetItemType(n) == ID_NONE)
			DeleteItem(n);		// delete non-icon

		nDevice = InsertItem( ClassName, 
					ClassImage, 
					ClassImage, 
					nClass);

		theApp.m_pDevArray[i]->m_hItem = nDevice;
		SetItemDevIndex(nDevice, i);	// Device Index index

		if (!theApp.m_pDevArray[i]->m_Connected) {
			SetItemColor(nDevice, RGB(162,162,162));
			//SetItemColor(nDevice, RGB(255,119,9));
			SetItemBold(nDevice, TRUE);
			SetItemType(nDevice, ID_VIRTUAL);	// Specify Device object
		} else {
			SetItemColor(nDevice, RGB(0,0,0));
			SetItemBold(nDevice, TRUE);
			SetItemType(nDevice, ID_DEVICE);	// Specify Device object
		}

		if (theApp.m_pDevArray[i]->m_DeviceParam.NumberPortIndices == 0 || 
			theApp.m_pDevArray[i]->m_DeviceParam.ProductID == PIDC_GAMMATRON ||
			theApp.m_pDevArray[i]->m_DeviceParam.ProductID == PIDC_PLASMA_LITE_V2 ||
			theApp.m_pDevArray[i]->m_DeviceParam.ProductID == PIDC_NITRO_XG ||
			theApp.m_pDevArray[i]->m_DeviceParam.ProductID == PIDC_NITRO_SLG) {
			SetDeviceTree(theApp.m_pDevArray[i]->m_hItem, i, 0);
		} else {
			for (j = 0; j < theApp.m_pDevArray[i]->m_DeviceParam.NumberPortIndices; j++) {
				ClassImage = 9;
				nClass = nDevice;
				if (theApp.m_pDevArray[i]->m_DeviceParam.ProductID == PIDC_ELECTRONFLUX) 
					ClassName.Format (_T("Port %s"), Alpha[j]);
				else if (theApp.m_pDevArray[i]->m_DeviceParam.ProductID == PIDC_GAMMARAY_V3 && j == 7)
					ClassName.Format (_T("Scan Matrix JP %d"), (j+1));
				else
					ClassName.Format (_T("JP %d"), (j+1));

				if (theApp.m_pDevArray[i]->m_DeviceStatus.PortActive[j] == 0) {
					ClassImage = 12;
					if (theApp.m_HidePorts) {
						// skip this port item if hide inactive ports selected
						continue;
					}
				}

				hItem = InsertItem(	ClassName, 
									ClassImage, 
									ClassImage,
									nClass);

				//theApp.m_pDevArray[i]->m_IOhItem[j] = hItem;
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);
				SetItemType(hItem, ID_PORT);
				SetItemDevIndex(hItem, i);
				SetItemPortIndex(hItem, j);	// Port index
				SetItemLabel(hItem, ClassName);
				if (theApp.m_pDevArray[i]->m_PortLabel[j].IsEmpty())
					SetItemText(hItem, ClassName);
				else
					SetItemText(hItem, ClassName + " : " + theApp.m_pDevArray[i]->m_PortLabel[j]);

				SetDeviceTree(hItem, i, j);
				if (theApp.m_pDevArray[i]->m_DeviceStatus.PortActive[j] == 1) 
					Expand(hItem, TVE_EXPAND);
			} // for (j = 0; j < theApp.m_pDevArray[i]->m_DeviceParam.NumberPortIndices; j++)
		} // if (theApp.m_pDevArray[i]->m_DeviceParam.NumberPortIndices > 0)
//		Expand(nClass, TVE_EXPAND);
	} // while (pos != NULL)
}

void CTreeDevices::SetDeviceTree(HTREEITEM hItem, int dIndex, int pIndex)
{
	HTREEITEM nClass, m_hItem;
	CString ClassName;
	int ClassImage, i, idx_end, ioPerPort, ioIndex, pIndex_cur;
	BOOL CreateItemFlag;

	pIndex_cur = pIndex;
	// INPUTS
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices > 0) {
		ClassImage = 7;
		if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = 15;
		if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] == 0) ClassImage = 15;

		m_hItem = InsertItem("Inputs", ClassImage, ClassImage, hItem);
		SetItemType(m_hItem, ID_INPUTS);		// Specify object type INPUTS
		SetItemDevIndex(m_hItem, dIndex);

		idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices;
		ioPerPort = idx_end;

		if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0 &&
			theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID != PIDC_GAMMATRON) {
			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;	
			ioPerPort = idx_end;
		}

		if (theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_GAMMARAY_V3) {
			ioPerPort = 32;
			if (pIndex == 7) 
				idx_end = 256;
			else
				idx_end = 32;
		}

		for (i = 0; i < idx_end; i++) {
			ioIndex = i + pIndex*ioPerPort;	

			theApp.m_pDevArray[dIndex]->m_InputhPItem[ioIndex] = m_hItem;

			if (theApp.m_HideItems &&
				!theApp.m_pDevArray[dIndex]->m_NotesArray[ioIndex].IsSet() &&
				!theApp.m_pDevArray[dIndex]->m_SendKeysArray[ioIndex].IsSet()) {

				theApp.m_pDevArray[dIndex]->m_InputhItem[ioIndex] = 0;	
				continue;
			}

			nClass = m_hItem;
			ClassName.Format ("%d", (i+1));	
			ClassImage = m_GR_OFF;		// input icon index

			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0 ||
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] == 0 ||
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.InputActive[ioIndex] == 0) {

				theApp.m_pDevArray[dIndex]->m_InputActive[ioIndex] = false;
				if (theApp.m_pDevArray[dIndex]->m_Connected)
					theApp.m_pDevArray[dIndex]->m_InputDisabled[ioIndex] = true;

				ClassImage = m_DIS_OFF;
			} else {
				theApp.m_pDevArray[dIndex]->m_InputActive[ioIndex] = true;
			}

			HTREEITEM hItem = InsertItem(	ClassName, 
											ClassImage, 
											ClassImage,
											nClass);
			SetItemColor(hItem, RGB(0,0,0));
			SetItemBold(hItem, FALSE);

			SetItemType(hItem, ID_INPUT);
			SetItemDevIndex(hItem, dIndex);
			SetItemPortIndex(hItem, pIndex);
			SetItemIndex(hItem, ioIndex);
			SetItemLabel(hItem, ClassName);
			SetItemParent(hItem, m_hItem);

			ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_NotesArray[ioIndex].m_Desc);
			SetItemText(hItem, ClassName);

			theApp.m_pDevArray[dIndex]->m_InputhItem[ioIndex] = hItem;
		}
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices > 0)

	// DIRECT OUTPUTS
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberOutputIndices > 0) {
//		if (theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID != PIDC_ELECTRONFLUX)
//			pIndex = 0;

		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if ((theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DIRECT &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = 8;

			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = 15;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = 15;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DIRECT &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = 15;

			theApp.m_pDevArray[dIndex]->m_hOutput[pIndex] = InsertItem("Direct Outputs", ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_hOutput[pIndex], ID_OUTPUTS);	// Specify object type OUTPUT
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_hOutput[pIndex], dIndex);	
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_hOutput[pIndex], pIndex);

			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberOutputIndices;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0 &&
				theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_ELECTRONFLUX)
				idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberOutputIndices / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;

			for (i = 0; i < idx_end; i++) {
				ioIndex = i + pIndex*idx_end;

				if (theApp.m_HideItems && 
					!theApp.m_pDevArray[dIndex]->m_OutputNotesArray[ioIndex].IsSet() &&
					!theApp.m_pDevArray[dIndex]->m_OutputArray[ioIndex].IsSet()) {
					theApp.m_pDevArray[dIndex]->m_OutputhItem[ioIndex] = 0;
					continue;
				}

				nClass = theApp.m_pDevArray[dIndex]->m_hOutput[pIndex];
				ClassName.Format ("%d", (i+1));
				if (theApp.m_pDevArray[dIndex]->m_OutputArray[ioIndex].IsSet())
					ClassImage = m_RD_OFF_P;	// output icon index
				else
					ClassImage = m_RD_OFF;		// output icon index

				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DIRECT &&
					theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.OutputActive[ioIndex] == 0) ClassImage = m_DIS_OFF;

				if (ClassImage == m_DIS_OFF) {
					theApp.m_pDevArray[dIndex]->m_OutputActive[ioIndex] = false;
				} else {
					theApp.m_pDevArray[dIndex]->m_OutputActive[ioIndex] = true;
				}

				HTREEITEM hItem = InsertItem( ClassName, 
							ClassImage, 
							ClassImage, 
							nClass);
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);

				SetItemType(hItem, ID_OUTPUT);
				SetItemDevIndex(hItem, dIndex);
				SetItemPortIndex(hItem, pIndex);
				SetItemIndex(hItem, ioIndex);
				SetItemLabel(hItem, ClassName);

				ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_OutputNotesArray[ioIndex].m_Desc);
				SetItemText(hItem, ClassName);
				
				theApp.m_pDevArray[dIndex]->m_OutputhItem[ioIndex] = hItem;
			}
		} 
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberOutputIndices > 0)

	// MULTIPLEXED OUTPUTS
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberMuxDisplayIndices > 0) {
		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if ((theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MUX &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_MUX;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_MUX_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_MUX_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MUX &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_MUX_DIS;

			if (ClassImage == m_MUX_DIS)
				theApp.m_pDevArray[dIndex]->m_DispActive[pIndex] = false;

			ClassName = "Multiplexed Outputs";
			theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex] = InsertItem(ClassName, ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], ID_MULTIPLEXED);	// Specify object type
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], dIndex);	
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], pIndex);
			SetItemLabel(theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], ClassName);

			ClassName.Format("%s : %s",	ClassName, theApp.m_pDevArray[dIndex]->m_DispNotesArray[pIndex].m_Desc);
			SetItemText(theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], ClassName);

			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberMuxDisplayIndices;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0)
				idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberMuxDisplayIndices / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;

			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_NITRO_XG ||
				theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_NITRO_SLG ||
				theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_PLASMA_LITE_V2)
				idx_end = 0;

			for (i = 0; i < idx_end; i++) {
				ioIndex = i + pIndex*idx_end;

				if (theApp.m_HideItems &&
					!theApp.m_pDevArray[dIndex]->m_MuxNotesArray[ioIndex].IsSet() &&
					!theApp.m_pDevArray[dIndex]->m_MuxArray[i + pIndex*idx_end].IsSet()) {
					theApp.m_pDevArray[dIndex]->m_MuxhItem[i + pIndex*idx_end] = 0;
					continue;
				}

				nClass = theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex];
				ClassName.Format ("%d", (i+1));
				if (theApp.m_pDevArray[dIndex]->m_MuxArray[ioIndex].IsSet())
					ClassImage = m_RD_OFF_P;	// output icon index
				else
					ClassImage = m_RD_OFF;		// output icon index

				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MUX &&
					theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.MuxDisplayActive[ioIndex] == 0) ClassImage = m_DIS_OFF;
				
				if (ClassImage == m_DIS_OFF) {
					theApp.m_pDevArray[dIndex]->m_MuxActive[ioIndex] = false;
				} else {
					theApp.m_pDevArray[dIndex]->m_MuxActive[ioIndex] = true;
				}

				HTREEITEM hItem = InsertItem( ClassName, 
							ClassImage, 
							ClassImage, 
							nClass);
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);

				SetItemType(hItem, ID_MUX);
				SetItemDevIndex(hItem, dIndex);
				SetItemPortIndex(hItem, pIndex);
				SetItemIndex(hItem, ioIndex);
				SetItemLabel(hItem, ClassName);
				
				ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_MuxNotesArray[ioIndex].m_Desc);
				SetItemText(hItem, ClassName);

				theApp.m_pDevArray[dIndex]->m_MuxhItem[ioIndex] = hItem;
			}
		}
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberMuxDisplayIndices > 0)

	// RWR OUTPUT
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberCRTRWRIndices > 0) {
		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_RWR ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_RWR_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] == IO_OUTPUT &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] == MODE_RWR &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 1) ClassImage = m_RWR;

			if (ClassImage == m_RWR_DIS)
				theApp.m_pDevArray[dIndex]->m_CrtRwrActive[pIndex] = false;

			ClassName = "CRT RWR";
			theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex] = InsertItem(ClassName, ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], ID_RWR);	// Specify object type
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], dIndex);
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], pIndex);
			SetItemLabel(theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], ClassName);

			ClassName.Format("%s : %s",	ClassName, theApp.m_pDevArray[dIndex]->m_CrtRwrNotesArray[pIndex].m_Desc);
			SetItemText(theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], ClassName);
		}
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberCRTRWRIndices > 0)

	// CLCD OUTPUT
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberLCDIndices > 0) {
		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if ((theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_CLCD &&
				 theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_CLCD;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_CLCD_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_CLCD_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_CLCD &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_CLCD_DIS;

			if (ClassImage == m_CLCD_DIS)
				theApp.m_pDevArray[dIndex]->m_CLCDActive[pIndex] = false;

			ClassName = "Character LCD";
			theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex] = InsertItem(ClassName, ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], ID_CLCD);	// Specify object type
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], dIndex);
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], pIndex);
			SetItemLabel(theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], ClassName);

			ClassName.Format("%s : %s",	ClassName, theApp.m_pDevArray[dIndex]->m_CLCDNotesArray[pIndex].m_Desc);
			SetItemText(theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], ClassName);

		}
		pIndex = pIndex_cur;
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberGLCDIndices > 0)

	// GLCD OUTPUT
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberGLCDIndices > 0) {
		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_GLCD ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_GLCD_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] == IO_OUTPUT &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] == MODE_GLCD &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 1) ClassImage = m_GLCD;

			if (ClassImage == m_GLCD_DIS)
				theApp.m_pDevArray[dIndex]->m_GLCDActive[pIndex] = false;

			ClassName = "Graphic LCD";
			theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex] = InsertItem(ClassName, ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], ID_GLCD);	// Specify object type
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], dIndex);
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], pIndex);
			SetItemLabel(theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], ClassName);

			ClassName.Format("%s : %s",	ClassName, theApp.m_pDevArray[dIndex]->m_GLCDNotesArray[pIndex].m_Desc);
			SetItemText(theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], ClassName);

		}
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberGLCDIndices > 0)

	// LATCHED OUTPUTS
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberLatchedIndices > 0) {

		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if ((theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_LATCHED &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_LO;

			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_LO_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_LO_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_LATCHED &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_LO_DIS;

			theApp.m_pDevArray[dIndex]->m_hLO[pIndex] = InsertItem("Latched Outputs", ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_hLO[pIndex], ID_LATCHED);	// Specify object type LATCHED
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_hLO[pIndex], dIndex);	
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_hLO[pIndex], pIndex);

			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberLatchedIndices;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0 &&
				theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_ELECTRONFLUX)
				idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberLatchedIndices / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;

			for (i = 0; i < idx_end; i++) {
				ioIndex = i + pIndex*idx_end;

				if (theApp.m_HideItems && 
					!theApp.m_pDevArray[dIndex]->m_LONotesArray[ioIndex].IsSet() &&
					!theApp.m_pDevArray[dIndex]->m_LOArray[ioIndex].IsSet()) {
					theApp.m_pDevArray[dIndex]->m_LOhItem[ioIndex] = 0;
					continue;
				}

				nClass = theApp.m_pDevArray[dIndex]->m_hLO[pIndex];
				ClassName.Format ("%d", (i+1));
				if (theApp.m_pDevArray[dIndex]->m_LOArray[ioIndex].IsSet())
					ClassImage = m_RD_OFF_P;	// output icon index
				else
					ClassImage = m_RD_OFF;		// output icon index

				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_LATCHED &&
					theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_DIS_OFF;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.LatchedActive[ioIndex] == 0) ClassImage = m_DIS_OFF;

				if (ClassImage == m_DIS_OFF) {
					theApp.m_pDevArray[dIndex]->m_LOActive[ioIndex] = false;
				} else {
					theApp.m_pDevArray[dIndex]->m_LOActive[ioIndex] = true;
				}

				HTREEITEM hItem = InsertItem( ClassName, 
							ClassImage, 
							ClassImage, 
							nClass);
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);

				SetItemType(hItem, ID_LO);
				SetItemDevIndex(hItem, dIndex);
				SetItemPortIndex(hItem, pIndex);
				SetItemIndex(hItem, ioIndex);
				SetItemLabel(hItem, ClassName);

				ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_LONotesArray[ioIndex].m_Desc);
				SetItemText(hItem, ClassName);
				
				theApp.m_pDevArray[dIndex]->m_LOhItem[ioIndex] = hItem;
			}
		} 
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberLatchedIndices > 0)

	// SPI OUTPUT
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberSPIIndices > 0) {
		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if ((theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_SPI &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DAC &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_SPIS;

			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_SPIS_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_SPIS_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID != PIDC_NITRO_XG &&
				theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID != PIDC_NITRO_SLG &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_SPI &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DAC &&
				theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_SPIS_DIS;

			theApp.m_pDevArray[dIndex]->m_hSPI[pIndex] = InsertItem("SPI/DAC Outputs", ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_hSPI[pIndex], ID_SPIS);	// Specify object type SPI
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_hSPI[pIndex], dIndex);	
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_hSPI[pIndex], pIndex);

			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberSPIIndices;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0 &&
				theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_ELECTRONFLUX)
				idx_end = idx_end / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;

			for (i = 0; i < idx_end; i++) {
				ioIndex = i + pIndex*idx_end;

				nClass = theApp.m_pDevArray[dIndex]->m_hSPI[pIndex];
				ClassName.Format ("%d", (i+1));

				if (theApp.m_HideItems && 
					!theApp.m_pDevArray[dIndex]->m_SPINotesArray[ioIndex].IsSet() &&
					!theApp.m_pDevArray[dIndex]->m_SPIArray[ioIndex].IsSet()) {
					theApp.m_pDevArray[dIndex]->m_OutputhItem[ioIndex] = 0;
					continue;
				}

				if (theApp.m_pDevArray[dIndex]->m_SPIArray[ioIndex].IsSet())
					ClassImage = m_SPI_P;	// output icon index
				else
					ClassImage = m_SPI;		// output icon index

				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_SPI_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_SPI_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_SPI &&
					theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DAC &&
					theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_MIXED) ClassImage = m_SPI_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.SPIActive[ioIndex] == 0) ClassImage = m_SPI_DIS;

				if (ClassImage == m_SPI_DIS) {
					theApp.m_pDevArray[dIndex]->m_SPIActive[ioIndex] = false;
				} else {
					theApp.m_pDevArray[dIndex]->m_SPIActive[ioIndex] = true;
				}

				HTREEITEM hItem = InsertItem(	ClassName, 
												ClassImage, 
												ClassImage, 
												nClass);
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);

				SetItemType(hItem, ID_SPI);
				SetItemDevIndex(hItem, dIndex);
				SetItemPortIndex(hItem, pIndex);
				SetItemIndex(hItem, ioIndex);
				SetItemLabel(hItem, ClassName);

				ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_SPINotesArray[ioIndex].m_Desc);
				SetItemText(hItem, ClassName);
				
				theApp.m_pDevArray[dIndex]->m_SPIhItem[ioIndex] = hItem;
			}
		}
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberSPIIndices > 0)

	// STEPPER OUTPUT
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberStepperIndices > 0) {

		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_STEPPER ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_STEPS;

			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_STEPS_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_STEPS_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_STEPPER) ClassImage = m_STEPS_DIS;

			theApp.m_pDevArray[dIndex]->m_hSTEP[pIndex] = InsertItem("Stepper Motors", ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_hSTEP[pIndex], ID_STEPS);		// Specify object type
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_hSTEP[pIndex], dIndex);	
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_hSTEP[pIndex], pIndex);

			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberStepperIndices;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0)
				idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberStepperIndices / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;

			for (i = 0; i < idx_end; i++) {
				ioIndex = i + pIndex*idx_end;

				if (theApp.m_HideItems && 
					!theApp.m_pDevArray[dIndex]->m_STEPNotesArray[ioIndex].IsSet() &&
					!theApp.m_pDevArray[dIndex]->m_STEPArray[ioIndex].IsSet()) {
					theApp.m_pDevArray[dIndex]->m_STEPhItem[ioIndex] = 0;
					continue;
				}

				if (theApp.m_pDevArray[dIndex]->m_STEPArray[ioIndex].IsSet())
					ClassImage = m_STEP_P;	// output icon index
				else
					ClassImage = m_STEP;		// output icon index

				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_STEP_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_STEP_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_STEPPER) ClassImage = m_STEP_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.StepperActive[ioIndex] == 0) ClassImage = m_STEP_DIS;

				nClass = theApp.m_pDevArray[dIndex]->m_hSTEP[pIndex];
				ClassName.Format ("%d", (i+1));
				if (ClassImage == m_STEP_DIS) {
					theApp.m_pDevArray[dIndex]->m_STEPActive[ioIndex] = false;
				} else {
					theApp.m_pDevArray[dIndex]->m_STEPActive[ioIndex] = true;
				}

				HTREEITEM hItem = InsertItem( ClassName, 
							ClassImage, 
							ClassImage, 
							nClass);
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);

				SetItemType(hItem, ID_STEP);
				SetItemDevIndex(hItem, dIndex);
				SetItemPortIndex(hItem, pIndex);
				SetItemIndex(hItem, ioIndex);
				SetItemLabel(hItem, ClassName);

				ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_STEPNotesArray[ioIndex].m_Desc);
				SetItemText(hItem, ClassName);
				
				theApp.m_pDevArray[dIndex]->m_STEPhItem[ioIndex] = hItem;
			}
		} 
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberStepperIndices > 0)

	// DOT MATRIX OUTPUTS
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberDotMatrixIndices > 0) {

		CreateItemFlag = true;
		if (theApp.m_HideModes) {
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DOTMATRIX ||
				!theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex]) {
				// do not create tree items
				CreateItemFlag = false;
			}
		}

		if (CreateItemFlag) {
			ClassImage = m_DMXS;

			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_DMXS_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_DMXS_DIS;
			if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DOTMATRIX) ClassImage = m_DMXS_DIS;

			theApp.m_pDevArray[dIndex]->m_hDotMX[pIndex] = InsertItem("Dot Matrix Displays", ClassImage, ClassImage, hItem);
			SetItemType(theApp.m_pDevArray[dIndex]->m_hDotMX[pIndex], ID_DMXS);		// Specify object type Dot Matrix
			SetItemDevIndex(theApp.m_pDevArray[dIndex]->m_hDotMX[pIndex], dIndex);	
			SetItemPortIndex(theApp.m_pDevArray[dIndex]->m_hDotMX[pIndex], pIndex);

			idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberDotMatrixIndices;
			if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0)
				idx_end = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberDotMatrixIndices / theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;

			for (i = 0; i < idx_end; i++) {
				ioIndex = i + pIndex*idx_end;

				if (theApp.m_HideItems && 
					!theApp.m_pDevArray[dIndex]->m_DotMXNotesArray[ioIndex].IsSet() &&
					!theApp.m_pDevArray[dIndex]->m_DotMXArray[ioIndex].IsSet()) {
					theApp.m_pDevArray[dIndex]->m_DotMXhItem[ioIndex] = 0;
					continue;
				}

				if (theApp.m_pDevArray[dIndex]->m_DotMXArray[ioIndex].IsSet())
					ClassImage = m_DMX_P;	// output icon index
				else
					ClassImage = m_DMX;		// output icon index

				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[pIndex] == 0) ClassImage = m_DMX_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[pIndex] != IO_OUTPUT) ClassImage = m_DMX_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[pIndex] != MODE_DOTMATRIX) ClassImage = m_DMX_DIS;
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.DotMatrixActive[ioIndex] == 0) ClassImage = m_DMX_DIS;

				nClass = theApp.m_pDevArray[dIndex]->m_hDotMX[pIndex];
				ClassName.Format ("%d", (i+1));
				if (ClassImage == m_DMX_DIS) {
					theApp.m_pDevArray[dIndex]->m_DotMXActive[ioIndex] = false;
				} else {
					theApp.m_pDevArray[dIndex]->m_DotMXActive[ioIndex] = true;
				}

				HTREEITEM hItem = InsertItem( ClassName, 
							ClassImage, 
							ClassImage, 
							nClass);
				SetItemColor(hItem, RGB(0,0,0));
				SetItemBold(hItem, FALSE);

				SetItemType(hItem, ID_DMX);
				SetItemDevIndex(hItem, dIndex);
				SetItemPortIndex(hItem, pIndex);
				SetItemIndex(hItem, ioIndex);
				SetItemLabel(hItem, ClassName);

				ClassName.Format("%d : %s",	(i+1), theApp.m_pDevArray[dIndex]->m_DotMXNotesArray[ioIndex].m_Desc);
				SetItemText(hItem, ClassName);
				
				theApp.m_pDevArray[dIndex]->m_DotMXhItem[ioIndex] = hItem;
			}
		} 
	} // if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberDotMatrixIndices > 0)
}
