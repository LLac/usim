// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code, this includes 
// commercial libraries and anything else for profit.
//
// I would appreciate a notification of any bugs or bug fixes to help the control grow.
//
// History:
// --------
//	See License.txt for full history information.
//
//
// Copyright (c) 1999-2002 
// ComputerSmarts.net 
// mattrmiller@computersmarts.net


#include "stdafx.h"
#include "OptionTreeItemFont.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _D_DEFAULT_SAMPLE_TEXT "AaBbIiJjSsKkWwYyZz";
static size_t szLogFont = sizeof(LOGFONT);

//////////////////////////////////////////////////////////////////////
// CDefFont
//////////////////////////////////////////////////////////////////////
CDefFont::CDefFont(const CString& sFaceName,const LONG height)
{
  memset((void*) (LOGFONT*)this,0,szLogFont);
  lfHeight = -11;//height;
  strcpy(lfFaceName,sFaceName);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDefFont COptionTreeItemFont::__lfDefault;;
COptionTreeItemFont::COptionTreeItemFont()
{
	// Initialize variables
	m_crDefTextColor = RGB(0, 0, 0);
	m_crTextColor = RGB(0, 0, 0);
	m_dwOptions = NULL;
	m_strCustomSample = _T("");
	m_pApplyWindow = NULL;
	m_lDefaultHeight = 0;

	// Set item type
	SetItemType(OT_ITEM_FONT);
}

COptionTreeItemFont::~COptionTreeItemFont()
{

}

void COptionTreeItemFont::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	// Make sure options aren't NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Declare variables
	CFont *pOldFont=NULL;
	COLORREF crOld;
	int nOldBack;
	CRect rcText, rcClient;
	COLORREF crOldBack;

	// Get window rect
	rcClient = rcRect;

	// Select font
	pOldFont = pDC->SelectObject(&_Font);
	
	// Set text color
	if (IsReadOnly() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
	{
		crOld = pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	else
	{
		crOld = pDC->SetTextColor(m_crTextColor);
	}

	// Set background mode
	nOldBack = pDC->SetBkMode(TRANSPARENT);

	// Set background color
	crOldBack = pDC->SetBkColor(GetBackgroundColor());	

  // ce
  if(m_strCustomSample.GetLength() == 0)
    m_strCustomSample = _D_DEFAULT_SAMPLE_TEXT;
  /*
  if (GetOption(OT_FS_FONTNAMESAMPLE) == TRUE)
	{
		fFont.GetLogFont(&lf);
	}
  */
	// Get text rectangle
	rcText.left  = rcRect.left + 1;
	rcText.right = rcRect.right;
	rcText.top = rcRect.top;
	rcText.bottom = rcRect.bottom;

	// Draw text
	pDC->DrawText(m_strCustomSample, rcText, DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
  pDC->DrawText(m_strCustomSample, rcText, DT_SINGLELINE | DT_VCENTER);
	
	// Restore GDI ojects
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
	pDC->SetBkColor(crOldBack);
}

void COptionTreeItemFont::OnCommit()
{
}

void COptionTreeItemFont::OnRefresh()
{
}

void COptionTreeItemFont::OnMove()
{
}

void COptionTreeItemFont::OnActivate()
{
	// Declare variables
	CWnd *pWnd;

	// Set apply window
	if (m_pApplyWindow != NULL)
	{
		pWnd = m_pApplyWindow;
	}
	else
	{
		pWnd = m_otOption;
	}

	// Show dialog
	COptionTreeFontSel fDialog(m_dwOptions, m_lfFont, m_lfFont, pWnd);
  fDialog.SetCustomSample(m_strCustomSample);

	// -- Default Font Color
	fDialog.SetDefaultTextColor(m_crDefTextColor);
	// -- Font Color
	fDialog.SetTextColor(m_crTextColor);
	// -- Control ID
	fDialog.SetControlID(GetCtrlID());

	// Show dialog
	if (fDialog.DoModal() == ID_FONTSEL_OK)	
	{
		// -- Get font
		fDialog.GetLogFont(m_lfFont);
    this->SetCurFont(&m_lfFont);

		// -- Get font color
		m_crTextColor = fDialog.GetTextColor();

		// -- Set item height
		if ((abs(m_lfFont.lfHeight) + (ATTRIB_OFFSET * 2)) > m_lDefaultHeight)
		{
			SetItemHeight(abs(m_lfFont.lfHeight) + (ATTRIB_OFFSET * 2));
			SetDrawMultiline(TRUE);
		}
		else
		{
			SetItemHeight(m_lDefaultHeight);
			SetDrawMultiline(FALSE);
		}
    CommitChanges();
	}

	// Update items
	if (m_otOption != NULL)
	{
		m_otOption->UpdatedItems();
	}
}

void COptionTreeItemFont::CleanDestroyWindow()
{
}

void COptionTreeItemFont::OnDeSelect()
{
}

void COptionTreeItemFont::OnSelect()
{
}

BOOL COptionTreeItemFont::CreateFontItem(LOGFONT* plfFont, COLORREF crFontColor, DWORD dwOptions)
{
	// Declare variables
	CRect rcButton;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Save options
	m_dwOptions = dwOptions;

	// Set font
	SetCurFont(plfFont);

	// Set color
	SetTextColor(crFontColor);

	// Get default height
	m_lDefaultHeight = GetHeight();

	return TRUE;
}


void COptionTreeItemFont::SetCurFont(LOGFONT* plf)
{
  LOGFONT* lplf = (plf != NULL) ? plf : &COptionTreeItemFont::__lfDefault;
  memcpy((void*)&m_lfFont,(const void*)lplf,szLogFont);
  if(_Font.GetSafeHandle() != NULL)
    _Font.DeleteObject();
//  BOOL bres = _Font.CreateFontIndirect(lplf);
}


void COptionTreeItemFont::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor;
}

COLORREF COptionTreeItemFont::GetTextColor()
{
	return m_crTextColor;
}

COLORREF COptionTreeItemFont::GetDefaultTextColor()
{
	return m_crDefTextColor;
}

void COptionTreeItemFont::SetCustomSample(const CString& strSample)
{
	m_strCustomSample = strSample;
}

void COptionTreeItemFont::SetApplyWindow(CWnd *pWnd)
{
	m_pApplyWindow = pWnd;
}

void  COptionTreeItemFont::GetCurFont(LOGFONT& lf)
{
  memcpy((void*)&lf,(const void*)&m_lfFont,szLogFont);
}