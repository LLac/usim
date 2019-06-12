// TreeThread.cpp: implementation of the CTreeThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include <math.h>
#include "TreeThread.h"
#include "hid_dll\Biusb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
CTreeThread::CTreeThread()
{
	m_delta_T = 0;
	m_hItemPrev = TVI_LAST;
}

CTreeThread::~CTreeThread()
{
}

void CTreeThread::TreeWorker()
{
	int dIndex, iIndex, pIndex, maxcnt;

	if (::IsIconic(theApp.m_pMainWnd->GetSafeHwnd()) )
		return;
	if ( (timeGetTime() - m_delta_T) < 10) 
		return;

	// 10 ms elapsed
#ifdef _DEBUG
//	TRACE("Tree Delta = %d\n", (timeGetTime() - m_delta_T));
#endif
	m_delta_T = timeGetTime();
	
	// set tree input icons
	CTreeDevices *cTree = theApp.m_pTreeDevices;

	if (theApp.m_HideItems) {
		HTREEITEM hItem = cTree->GetFirstVisibleItem();
		while (hItem != NULL) {
			switch (cTree->GetItemType(hItem)) {
				case ID_INPUTS:
					dIndex = cTree->GetItemDevIndex(hItem);
					maxcnt = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices;
					for (iIndex = 0; iIndex < maxcnt; iIndex++) {
						if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1 &&
							theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex] == 0) {							
							SetInputItemIcon(cTree, dIndex, iIndex);
						}
					}
				break;
			}
			hItem = cTree->GetNextVisibleItem(hItem);
		}
	}

	HTREEITEM hItem = cTree->GetFirstVisibleItem();
	while (hItem != NULL) {
		dIndex = cTree->GetItemDevIndex(hItem);
		iIndex = cTree->GetItemIndex(hItem);
		pIndex = cTree->GetItemPortIndex(hItem);

		if (cTree->GetItemType(hItem) == ID_INPUT) {
			SetInputTabIcon(dIndex, iIndex);
			SetInputItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		if (cTree->GetItemType(hItem) == ID_OUTPUT) {
			SetOutputTabIcon(dIndex, iIndex);
			SetOutputItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		if (cTree->GetItemType(hItem) == ID_MUX) {
			SetMuxTabIcon(dIndex, iIndex);
			SetMuxItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		if (cTree->GetItemType(hItem) == ID_MULTIPLEXED) {
			SetDispTabIcon(dIndex, iIndex);
			SetDisplayItemIcon((LPVOID)cTree, dIndex, pIndex);
		}

		if (cTree->GetItemType(hItem) == ID_RWR) {
			SetRWRTabIcon(dIndex, pIndex);
			SetCrtRwrItemIcon((LPVOID)cTree, dIndex, pIndex);
		}

		if (cTree->GetItemType(hItem) == ID_CLCD) {
			SetCLCDTabIcon(dIndex, pIndex);
			SetCLCDItemIcon((LPVOID)cTree, dIndex, pIndex);
		}

		if (cTree->GetItemType(hItem) == ID_GLCD) {
			SetGLCDTabIcon(dIndex, pIndex);
			SetGLCDItemIcon((LPVOID)cTree, dIndex, pIndex);
		}

		if (cTree->GetItemType(hItem) == ID_LO) {
			SetLatchedTabIcon(dIndex, iIndex);
			SetLatchedItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		if (cTree->GetItemType(hItem) == ID_SPI) {
			SetSPITabIcon(dIndex, iIndex);
			SetSPIItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		if (cTree->GetItemType(hItem) == ID_STEP) {
			SetSTEPTabIcon(dIndex, iIndex);
			SetSTEPItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		if (cTree->GetItemType(hItem) == ID_DMX) {
			SetDotMatrixTabIcon(dIndex, iIndex);
			SetDotMatrixItemIcon((LPVOID)cTree, dIndex, iIndex);
		}

		hItem = cTree->GetNextVisibleItem(hItem);
	}

#ifdef _DEBUG
//	TRACE("Tree Delta = %d\n", (timeGetTime() - m_delta_T));
#endif

}

void CTreeThread::SetInputTabIcon(int dIndex, int iIndex)
{
	int nSet, Idx;
	TCITEM item;

	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_NotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pInputsTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pInputsTabCtrl->m_NOTES;
		}

		theApp.m_pInputsTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);	// 0 = Tab Index
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pInputsTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item); // 0 = Tab Index
		}

		// set Flags Tab Icon
		Idx++;
		if (theApp.m_UserVarsArray.IsSet()) {
			// add _P
			nSet = theApp.m_pInputsTabCtrl->m_IOFLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pInputsTabCtrl->m_IOFLAGS;
		}

		theApp.m_pInputsTabCtrl->m_InputFlagTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pInputsTabCtrl->m_InputFlagTab.m_pTab->SetItem(Idx, &item);
		}

		// set Send Keys Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pInputsTabCtrl->m_SENDKEYS_P;
		} else {
			// item is clear
			nSet = theApp.m_pInputsTabCtrl->m_SENDKEYS;
		}

		theApp.m_pInputsTabCtrl->m_SendKeysTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pInputsTabCtrl->m_SendKeysTab.m_pTab->SetItem(Idx, &item);
		}

		// set Variables Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_VariablesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pInputsTabCtrl->m_VARS_P;
		} else {
			// item is clear
			nSet = theApp.m_pInputsTabCtrl->m_VARS;
		}

		theApp.m_pInputsTabCtrl->m_VariablesTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pInputsTabCtrl->m_VariablesTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetInputItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_NotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_SendKeysArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_VariablesArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	// set INPUT Icon
	if (!theApp.m_pDevArray[dIndex]->m_InputDisabled[iIndex] && theApp.m_pDevArray[dIndex]->m_InputActive[iIndex]) {
		// set to _GR
		if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				if (theApp.m_pDevArray[dIndex]->m_InputActive[iIndex])
					nSet = nTree->m_GR_ON_P;
				else
					nSet = nTree->m_DIS_ON_P;
			} else {
				// item is clear
				if (theApp.m_pDevArray[dIndex]->m_InputActive[iIndex])
					nSet = nTree->m_GR_ON;
				else
					nSet = nTree->m_DIS_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				if (theApp.m_pDevArray[dIndex]->m_InputActive[iIndex])
					nSet = nTree->m_GR_OFF_P;
				else
					nSet = nTree->m_DIS_OFF_P;
			} else {
				if (theApp.m_pDevArray[dIndex]->m_InputActive[iIndex])
					nSet = nTree->m_GR_OFF;
				else
					nSet = nTree->m_DIS_OFF;
				}
		}
	} else {
		// set to _DIS
		if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_ON_P;
			} else {
				nSet = nTree->m_DIS_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_OFF_P;
			} else {
				nSet = nTree->m_DIS_OFF;
			}
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex], nSet, nSet );
		// ensure item visible if turned ON
		if (theApp.m_pDevArray[dIndex]->m_Inputs[iIndex] == 1) {
			if (theApp.m_HideItems && theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex] == 0) {
				// create item
				CString ClassName;
				UINT nItem = iIndex;
				if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0 && theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID != PIDC_GAMMATRON) {
					nItem = theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberInputIndices/theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices;
					nItem = (UINT) fmod ((double)iIndex, (double)nItem);
				}

				// Kludge for GRV3 with Scan Matrix on port JP8
				if (theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices > 0 && theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_GAMMARAY_V3) {
					if (iIndex < 224) {
						nItem = 32;
						nItem = (UINT) fmod ((double)iIndex, (double)nItem);
					} else {
						nItem = iIndex - 224;
					}
				}

				ClassName.Format ("%d", (nItem+1));

				// find m_hItemPrev to insert after
				HTREEITEM hItem = nTree->GetNextItem(theApp.m_pDevArray[dIndex]->m_InputhPItem[iIndex], TVGN_CHILD);
				m_hItemPrev = TVI_FIRST;
				while (hItem != NULL) {
					if (iIndex > (int)nTree->GetItemIndex(hItem)) {
						m_hItemPrev = hItem;
					} else {
						break;
					}

					hItem = nTree->GetNextItem(hItem, TVGN_NEXT);
				}

				hItem = nTree->InsertItem( ClassName, 
											nSet, 
											nSet,
											theApp.m_pDevArray[dIndex]->m_InputhPItem[iIndex],
											m_hItemPrev);
				theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex] = hItem;
				nTree->SetItemType(hItem, ID_INPUT);
				nTree->SetItemDevIndex(hItem, dIndex);	// Device index
				nTree->SetItemPortIndex(hItem, nTree->GetItemPortIndex(theApp.m_pDevArray[dIndex]->m_InputhPItem[iIndex]));// Port index
				nTree->SetItemIndex(hItem, iIndex);	// Item index
				nTree->SetItemLabel(hItem, ClassName);
				nTree->SetItemParent(hItem, theApp.m_pDevArray[dIndex]->m_InputhPItem[iIndex]);
			}

			nTree->EnsureVisible(theApp.m_pDevArray[dIndex]->m_InputhItem[iIndex]);
		}
	}
}

void CTreeThread::SetRWRTabIcon(int dIndex, int pIndex)
{
	int nSet;
	TCITEM item;
	int Idx;

	// set CRT RWR Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_CrtRwrNotesArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pRWRTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pRWRTabCtrl->m_NOTES;
		}

		theApp.m_pRWRTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pRWRTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_CrtRwrFlagsArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pRWRTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pRWRTabCtrl->m_FLAGS;
		}

		theApp.m_pRWRTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pRWRTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set RWR Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_CrtRwrArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pRWRTabCtrl->m_RWR_P;
		} else {
			// item is clear
			nSet = theApp.m_pRWRTabCtrl->m_RWR;
		}

		theApp.m_pRWRTabCtrl->m_RWRTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pRWRTabCtrl->m_RWRTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetCrtRwrItemIcon(LPVOID npTree, int dIndex, int pIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_CrtRwrNotesArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_CrtRwrFlagsArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_CrtRwrArray[pIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_CrtRwrActive[pIndex] &&
		!theApp.m_pDevArray[dIndex]->m_CrtRwrDisabled[pIndex]) {
		if (bTab_Set) {
			nSet = nTree->m_RWR_P;
		} else {
			nSet = nTree->m_RWR;
		}
	} else {
		if (bTab_Set) {
			nSet = nTree->m_RWR_DIS_P;
		} else {
			nSet = nTree->m_RWR_DIS;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_CrtRwrhItem[pIndex], nSet, nSet );
	}
}

void CTreeThread::SetCLCDTabIcon(int dIndex, int pIndex)
{
	int nSet;
	TCITEM item;
	int Idx;

	// set CLCD Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex]) {
		item.mask = TCIF_IMAGE;

		// set NOTES Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_CLCDNotesArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pCLCDTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pCLCDTabCtrl->m_NOTES;
		}

		theApp.m_pCLCDTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pCLCDTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_CLCDFlagsArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pCLCDTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pCLCDTabCtrl->m_FLAGS;
		}

		theApp.m_pCLCDTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pCLCDTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set GLCD Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_CLCDArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pCLCDTabCtrl->m_CLCD_P;
		} else {
			// item is clear
			nSet = theApp.m_pCLCDTabCtrl->m_CLCD;
		}

//		theApp.m_pCLCDTabCtrl->m_CLCDTab.m_CLCDLinesTabCtrl.m_LCDLinesTab.SetTabIcons();

		theApp.m_pCLCDTabCtrl->m_CLCDTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pCLCDTabCtrl->m_CLCDTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetCLCDItemIcon(LPVOID npTree, int dIndex, int pIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_CLCDNotesArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_CLCDFlagsArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_CLCDArray[pIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_CLCDActive[pIndex] &&
		!theApp.m_pDevArray[dIndex]->m_CLCDDisabled[pIndex]) {
		if (bTab_Set) {
			nSet = nTree->m_CLCD_P;
		} else {
			nSet = nTree->m_CLCD;
		}
	} else {
		if (bTab_Set) {
			nSet = nTree->m_CLCD_DIS_P;
		} else {
			nSet = nTree->m_CLCD_DIS;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_CLCDhItem[pIndex], nSet, nSet );
	}
}

void CTreeThread::SetGLCDTabIcon(int dIndex, int pIndex)
{
	int nSet;
	TCITEM item;
	int Idx;

	// set GLCD Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex]) {
		item.mask = TCIF_IMAGE;

		// set NOTES Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_GLCDNotesArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pGLCDTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pGLCDTabCtrl->m_NOTES;
		}

		theApp.m_pGLCDTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pGLCDTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_GLCDFlagsArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pGLCDTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pGLCDTabCtrl->m_FLAGS;
		}

		theApp.m_pGLCDTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pGLCDTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set GLCD Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_GLCDArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pGLCDTabCtrl->m_GLCD_P;
		} else {
			// item is clear
			nSet = theApp.m_pGLCDTabCtrl->m_GLCD;
		}

		theApp.m_pGLCDTabCtrl->m_GLCDTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pGLCDTabCtrl->m_GLCDTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetGLCDItemIcon(LPVOID npTree, int dIndex, int pIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_GLCDNotesArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_GLCDFlagsArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_GLCDArray[pIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_GLCDActive[pIndex] &&
		!theApp.m_pDevArray[dIndex]->m_GLCDDisabled[pIndex]) {
		if (bTab_Set) {
			nSet = nTree->m_GLCD_P;
		} else {
			nSet = nTree->m_GLCD;
		}
	} else {
		if (bTab_Set) {
			nSet = nTree->m_GLCD_DIS_P;
		} else {
			nSet = nTree->m_GLCD_DIS;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_GLCDhItem[pIndex], nSet, nSet );
	}
}

void CTreeThread::SetOutputTabIcon(int dIndex, int iIndex)
{
	int nSet, Idx;
	TCITEM item;

	// set DirectOutputs Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_OutputhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_OutputNotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pOutputTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pOutputTabCtrl->m_NOTES;
		}

		theApp.m_pOutputTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pOutputTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_OutputFlagsArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pOutputTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pOutputTabCtrl->m_FLAGS;
		}

		theApp.m_pOutputTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pOutputTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set OUTPUT Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_OutputArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pOutputTabCtrl->m_OUTPUT_P;
		} else {
			// item is clear
			nSet = theApp.m_pOutputTabCtrl->m_OUTPUT;
		}

		theApp.m_pOutputTabCtrl->m_OutputTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pOutputTabCtrl->m_OutputTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetOutputItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_OutputNotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_OutputFlagsArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_OutputArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_OutputDisabled[iIndex] || !theApp.m_pDevArray[dIndex]->m_OutputActive[iIndex]) {
		// set to _DIS
		if (theApp.m_pDevArray[dIndex]->m_Outputs[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_ON_P;
			} else {
				// item is clear
				nSet = nTree->m_DIS_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_OFF_P;
			} else {
				nSet = nTree->m_DIS_OFF;
			}
		}
	} else {
		// set to _RD
		if (theApp.m_pDevArray[dIndex]->m_Outputs[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_RD_ON_P;
			} else {
				nSet = nTree->m_RD_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_RD_OFF_P;
			} else {
				nSet = nTree->m_RD_OFF;
			}
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_OutputhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_OutputhItem[iIndex], nSet, nSet );
	}
}

void CTreeThread::SetDispTabIcon(int dIndex, int pIndex)
{
	int nSet;
	TCITEM item;
	int Idx;

	// set DirectOutputs Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_DispNotesArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pDispTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pDispTabCtrl->m_NOTES;
		}

		theApp.m_pDispTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pDispTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set DISP Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_DisplayFlagsArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pDispTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pDispTabCtrl->m_FLAGS;
		}

		theApp.m_pDispTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pDispTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set DISP Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_DisplayArray[pIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pDispTabCtrl->m_DISP_P;
		} else {
			// item is clear
			nSet = theApp.m_pDispTabCtrl->m_DISP;
		}

		theApp.m_pDispTabCtrl->m_DispTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pDispTabCtrl->m_DispTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetDisplayItemIcon(LPVOID npTree, int dIndex, int pIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_DispNotesArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_DisplayFlagsArray[pIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_DisplayArray[pIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_DispActive[pIndex] &&
		!theApp.m_pDevArray[dIndex]->m_DispDisabled[pIndex]) {
		if (bTab_Set) {
			nSet = nTree->m_MUX_P;
		} else {
			nSet = nTree->m_MUX;
		}
	} else {
		if (bTab_Set) {
			nSet = nTree->m_MUX_DIS_P;
		} else {
			nSet = nTree->m_MUX_DIS;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_DisphItem[pIndex], nSet, nSet );
	}
}

void CTreeThread::SetMuxTabIcon(int dIndex, int iIndex)
{
	int nSet;
	TCITEM item;
	int Idx;

	// set Mux Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_MuxhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_MuxNotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pMuxTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pMuxTabCtrl->m_NOTES;
		}

		theApp.m_pMuxTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pMuxTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_MuxFlagsArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pMuxTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pMuxTabCtrl->m_FLAGS;
		}

		theApp.m_pMuxTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pMuxTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set MUX Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_MuxArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pMuxTabCtrl->m_MUX_P;
		} else {
			// item is clear
			nSet = theApp.m_pMuxTabCtrl->m_MUX;
		}

		theApp.m_pMuxTabCtrl->m_MuxTab.m_pTab->GetItem(Idx, &item);
		if(item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pMuxTabCtrl->m_MuxTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetMuxItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_MuxNotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_MuxFlagsArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_MuxArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_MuxDisabled[iIndex] || !theApp.m_pDevArray[dIndex]->m_MuxActive[iIndex]) {
		// set to _DIS
		if (theApp.m_pDevArray[dIndex]->m_Mux[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_ON_P;
			} else {
				// item is clear
				nSet = nTree->m_DIS_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_OFF_P;
			} else {
				nSet = nTree->m_DIS_OFF;
			}
		}
	} else {
		// set to _RD
		if (theApp.m_pDevArray[dIndex]->m_Mux[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_RD_ON_P;
			} else {
				nSet = nTree->m_RD_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_RD_OFF_P;
			} else {
				nSet = nTree->m_RD_OFF;
			}
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_MuxhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_MuxhItem[iIndex], nSet, nSet );
	}
}

void CTreeThread::SetLatchedTabIcon(int dIndex, int iIndex)
{
	int nSet, Idx;
	TCITEM item;

	// set DirectLOs Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_LOhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_LONotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pLatchedTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pLatchedTabCtrl->m_NOTES;
		}

		theApp.m_pLatchedTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pLatchedTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_LOFlagsArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pLatchedTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pLatchedTabCtrl->m_FLAGS;
		}

		theApp.m_pLatchedTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pLatchedTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set OUTPUT Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_LOArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pLatchedTabCtrl->m_LATCHED_P;
		} else {
			// item is clear
			nSet = theApp.m_pLatchedTabCtrl->m_LATCHED;
		}

		theApp.m_pLatchedTabCtrl->m_LatchedTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pLatchedTabCtrl->m_LatchedTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetLatchedItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_LONotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_LOFlagsArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_LOArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_LODisabled[iIndex] || !theApp.m_pDevArray[dIndex]->m_LOActive[iIndex]) {
		// set to _DIS
		if (theApp.m_pDevArray[dIndex]->m_LO[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_ON_P;
			} else {
				// item is clear
				nSet = nTree->m_DIS_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_DIS_OFF_P;
			} else {
				nSet = nTree->m_DIS_OFF;
			}
		}
	} else {
		// set to _RD
		if (theApp.m_pDevArray[dIndex]->m_LO[iIndex] == 1) {
			// set to _ON
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_RD_ON_P;
			} else {
				nSet = nTree->m_RD_ON;
			}
		} else {
			// set to _OFF
			if (bTab_Set) {
				// add _P
				nSet = nTree->m_RD_OFF_P;
			} else {
				nSet = nTree->m_RD_OFF;
			}
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_LOhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_LOhItem[iIndex], nSet, nSet );
	}
}

void CTreeThread::SetSPITabIcon(int dIndex, int iIndex)
{
	int nSet, Idx;
	TCITEM item;

	// set DirectSPIs Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_SPIhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_SPINotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pSPITabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pSPITabCtrl->m_NOTES;
		}

		theApp.m_pSPITabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pSPITabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_SPIFlagsArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pSPITabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pSPITabCtrl->m_FLAGS;
		}

		theApp.m_pSPITabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pSPITabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set SPI Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_SPIArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pSPITabCtrl->m_SPI_P;
		} else {
			// item is clear
			nSet = theApp.m_pSPITabCtrl->m_SPI;
		}

		theApp.m_pSPITabCtrl->m_SPITab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pSPITabCtrl->m_SPITab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetSPIItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_SPINotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_SPIFlagsArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_SPIArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_SPIDisabled[iIndex] || !theApp.m_pDevArray[dIndex]->m_SPIActive[iIndex]) {
		if (bTab_Set) {
			// add _P
			nSet = nTree->m_SPI_DIS_P;
		} else {
			// item is clear
			nSet = nTree->m_SPI_DIS;
		}
	} else {
		if (bTab_Set) {
			// add _P
			nSet = nTree->m_SPI_P;
		} else {
			nSet = nTree->m_SPI;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_SPIhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_SPIhItem[iIndex], nSet, nSet );
	}
}

void CTreeThread::SetSTEPTabIcon(int dIndex, int iIndex)
{
	int nSet, Idx;
	TCITEM item;

	// set DirectSPIs Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_STEPhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_STEPNotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pSTEPTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pSTEPTabCtrl->m_NOTES;
		}

		theApp.m_pSTEPTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pSTEPTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_STEPFlagsArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pSTEPTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pSTEPTabCtrl->m_FLAGS;
		}

		theApp.m_pSTEPTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pSTEPTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set SPI Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_STEPArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pSTEPTabCtrl->m_STEP_P;
		} else {
			// item is clear
			nSet = theApp.m_pSTEPTabCtrl->m_STEP;
		}

		theApp.m_pSTEPTabCtrl->m_STEPTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pSTEPTabCtrl->m_STEPTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetSTEPItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_STEPNotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_STEPFlagsArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_STEPArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_STEPDisabled[iIndex] || !theApp.m_pDevArray[dIndex]->m_STEPActive[iIndex]) {
		if (bTab_Set) {
			// add _P
			nSet = nTree->m_STEP_DIS_P;
		} else {
			// item is clear
			nSet = nTree->m_STEP_DIS;
		}
	} else {
		if (bTab_Set) {
			// add _P
			nSet = nTree->m_STEP_P;
		} else {
			nSet = nTree->m_STEP;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_STEPhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_STEPhItem[iIndex], nSet, nSet );
	}
}

void CTreeThread::SetDotMatrixTabIcon(int dIndex, int iIndex)
{
	int nSet, Idx;
	TCITEM item;

	// set Dot Matrix Tab Icon
	if (theApp.m_CurrenthItem == theApp.m_pDevArray[dIndex]->m_DotMXhItem[iIndex]) {
		item.mask = TCIF_IMAGE;

		// set Notes Tab Icon
		Idx = 0;
		if (theApp.m_pDevArray[dIndex]->m_DotMXNotesArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pDotMXTabCtrl->m_NOTES_P;
		} else {
			// item is clear
			nSet = theApp.m_pDotMXTabCtrl->m_NOTES;
		}

		theApp.m_pDotMXTabCtrl->m_NotesTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pDotMXTabCtrl->m_NotesTab.m_pTab->SetItem(Idx, &item);
		}

		// set FLAGS Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_DotMXFlagsArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pDotMXTabCtrl->m_FLAGS_P;
		} else {
			// item is clear
			nSet = theApp.m_pDotMXTabCtrl->m_FLAGS;
		}

		theApp.m_pDotMXTabCtrl->m_FlagsTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pDotMXTabCtrl->m_FlagsTab.m_pTab->SetItem(Idx, &item);
		}

		// set DOT MX Tab Icon
		Idx++;
		if (theApp.m_pDevArray[dIndex]->m_DotMXArray[iIndex].IsSet()) {
			// add _P
			nSet = theApp.m_pDotMXTabCtrl->m_DMX_P;
		} else {
			// item is clear
			nSet = theApp.m_pDotMXTabCtrl->m_DMX;
		}

		theApp.m_pDotMXTabCtrl->m_DotMXTab.m_pTab->GetItem(Idx, &item);
		if (item.iImage != nSet) {
			item.iImage = nSet;
			theApp.m_pDotMXTabCtrl->m_DotMXTab.m_pTab->SetItem(Idx, &item);
		}
	}
}

void CTreeThread::SetDotMatrixItemIcon(LPVOID npTree, int dIndex, int iIndex)
{
	int nSet, nImage, nsImage;
	CString ClassName;
	BOOL bTab_Set = false;

	CTreeDevices *nTree = (CTreeDevices *)npTree;

	if (theApp.m_pDevArray[dIndex]->m_DotMXNotesArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_DotMXFlagsArray[iIndex].IsSet() ||
		theApp.m_pDevArray[dIndex]->m_DotMXArray[iIndex].IsSet()) {
		bTab_Set = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_DotMXDisabled[iIndex] || !theApp.m_pDevArray[dIndex]->m_DotMXActive[iIndex]) {
		if (bTab_Set) {
			// add _P
			nSet = nTree->m_DMX_DIS_P;
		} else {
			// item is clear
			nSet = nTree->m_DMX_DIS;
		}
	} else {
		if (bTab_Set) {
			// add _P
			nSet = nTree->m_DMX_P;
		} else {
			nSet = nTree->m_DMX;
		}
	}

	nTree->GetItemImage(theApp.m_pDevArray[dIndex]->m_DotMXhItem[iIndex], nImage, nsImage);
	if (nSet != nImage) {
		nTree->SetItemImage( theApp.m_pDevArray[dIndex]->m_DotMXhItem[iIndex], nSet, nSet );
	}
}