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
#include "OptionTreeInfo.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeInfo

COptionTreeInfo::COptionTreeInfo()
{
	// Initialize variables
	m_otOption = NULL;
}

COptionTreeInfo::~COptionTreeInfo()
{
}


BEGIN_MESSAGE_MAP(COptionTreeInfo, CEdit)
	//{{AFX_MSG_MAP(COptionTreeInfo)
	//ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void COptionTreeInfo::SetOptionsOwner(COptionTree *otOption)
{
	// Save pointer
	m_otOption = otOption;
}
/*
BOOL COptionTreeInfo::OnEraseBkgnd(CDC* pDC) 
{
	// Ha, Ha
	return FALSE;
}
*/
void  COptionTreeInfo::SetText(COptionTreeItem* pItem,const bool label)
{
  if(::IsWindow(this->GetSafeHwnd()) == TRUE && pItem != NULL)
  {
    CString sText("");
    if(label == true)
      this->SetWindowText(pItem->GetLabelText());
    else
      this->SetWindowText(pItem->GetInfoText());
    /*
    this->SetWindowText(sText);
    this->Invalidate();
    this->UpdateWindow();
    */
  }
}