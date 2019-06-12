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

// Added Headers
#include "OptionTree.h"
#include ".\optiontreeitemdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemDate

COptionTreeItemDate::COptionTreeItemDate()
{
	// Initialize variables
	m_bFocus = FALSE;

	// Set item type
	SetItemType(OT_ITEM_DATE);
  _changed = 0;
}

COptionTreeItemDate::~COptionTreeItemDate()
{
}


BEGIN_MESSAGE_MAP(COptionTreeItemDate, CDateTimeCtrl)
	//{{AFX_MSG_MAP(COptionTreeItemDate)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(DTN_DATETIMECHANGE, OnDtnDatetimechange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemDate message handlers

void COptionTreeItemDate::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	// If we don't have focus, text is drawn.
	if (m_bFocus == TRUE)
	{
		return;
	}

	// Make sure options aren't NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Make sure there is a window
	if (!IsWindow(GetSafeHwnd()))
	{
		return;
	}

	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}

	// Declare variables
	HGDIOBJ hOld;
	COLORREF crOld;
	int nOldBack;
	CRect rcText;
	CString strText;
	COLORREF crOldBack;

	// Select font
	hOld = pDC->SelectObject(m_otOption->GetNormalFont());
	
	// Set text color
	if (IsReadOnly() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
	{
		crOld = pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	else
	{
		crOld = pDC->SetTextColor(GetTextColor());
	}

	// Set background mode
	nOldBack = pDC->SetBkMode(TRANSPARENT);

	// Set background color
	crOldBack = pDC->SetBkColor(GetBackgroundColor());	

	// Get rectangle
	rcText = rcRect;

	// Get text
	GetWindowText(strText);

	// Draw text
	pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER);

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
	pDC->SetBkColor(crOldBack);

}

void COptionTreeItemDate::OnCommit()
{
	// Hide edit control
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
    m_bActivated = FALSE;
	}
}

void COptionTreeItemDate::OnRefresh()
{
	// Set the window positiion
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}
}

void COptionTreeItemDate::OnMove()
{
	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
  }

	// Hide window
	if (m_bFocus == FALSE && IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
    m_bActivated = FALSE;
	}
}

void COptionTreeItemDate::OnActivate()
{
	// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);
    m_bActivated = TRUE;

		// -- Set window position
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());

		// -- Set focus
		SetFocus();
	}
}

void COptionTreeItemDate::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}

void COptionTreeItemDate::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;	

	CDateTimeCtrl::OnSetFocus(pOldWnd);	
}

void COptionTreeItemDate::OnKillFocus(CWnd* pNewWnd) 
{
	// Declare variables
	CMonthCalCtrl *mCal;

	// Validate
	if (m_otOption == NULL)
	{
		CDateTimeCtrl::OnKillFocus(pNewWnd);
		return;
	}

	// See if new window is tree of list
	if (m_otOption->IsChild(pNewWnd) == FALSE)
	{
		CDateTimeCtrl::OnKillFocus(pNewWnd);
		return;
	}

	// Get calendar
	mCal = GetMonthCalCtrl();

	// If new focus is a the calendar, we are not really loosing focucs
	if (mCal != NULL)
	{
		if (pNewWnd->GetSafeHwnd() != mCal->GetSafeHwnd())
		{
			// -- Mark focus
			m_bFocus = FALSE;

			// -- Commit changes
			CommitChanges(false);//_changed > 0 ? true : false);
		}
	}
	else
	{
		// -- Mark focus
		m_bFocus = FALSE;

		// -- Commit changes
		CommitChanges(false);//_changed > 0 ? true : false);
	}

	CDateTimeCtrl::OnKillFocus(pNewWnd);	
}

BOOL COptionTreeItemDate::CreateDateItem(CString strFormat, DWORD dwDateStyle)
{
	// Declare variables
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	BOOL bRet = FALSE;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Add styles
	if (dwStyle != 0)
	{
		dwStyle |= dwDateStyle;
	}
  m_dwOptions = dwDateStyle;

	// Create edit control
	if (!IsWindow(GetSafeHwnd()))
	{
		// -- Create the edit view
		bRet = Create(dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup window
		if (bRet == TRUE)
		{
			// -- -- Set font
			SetFont(m_otOption->GetNormalFont(), TRUE);

			// -- Set format
      if(strFormat.GetLength() > 0)
			  SetFormat(strFormat);
      else
        SetFormat(NULL);

			// -- -- Set window position
			MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());

			// -- -- Hide window
			ShowWindow(SW_HIDE);
      m_bActivated = FALSE;
		}
	}

	return bRet;
}

void COptionTreeItemDate::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
    m_bActivated = FALSE;
	}
}

void COptionTreeItemDate::OnSelect()
{
	// Do nothing here
}


void COptionTreeItemDate::OnDtnDatetimechange(NMHDR *pNMHDR, LRESULT *pResult)
{
//  LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
  SetChanged(1);

  if(_changed > 0)
  {
	  m_otOption->SendNotify(OT_NOTIFY_ITEMCHANGED, this);
    _changed = 0;
  }
}
void COptionTreeItemDate::OnShow(BOOL bShow)
{
  if(::IsWindow(this->GetSafeHwnd()) == TRUE)
  {
    if(bShow == TRUE && m_bActivated == TRUE)
      this->ShowWindow(SW_SHOW);
    else
      this->ShowWindow(SW_HIDE);
  }
}
CString COptionTreeItemDate::GetAttributeText(void)
{
  CString sText("");
  if(::IsWindow(this->GetSafeHwnd()) == TRUE)    
    this->GetWindowText(sText);
  return sText;
}
