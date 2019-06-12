// MyStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "usim.h"
#include "MyStatusBar.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStatusBar

CMyStatusBar::CMyStatusBar()
{
}

CMyStatusBar::~CMyStatusBar()
{
}


BEGIN_MESSAGE_MAP(CMyStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CMyStatusBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyStatusBar::SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate)
{
	CDC * pDC = GetDC();
	// Select the correct font into the DC
    CFont * pFontOld = pDC->SelectObject(GetFont());

	CRect rect;
	this->GetClientRect(rect);
	PathCompactPath(pDC->GetSafeHdc(), (char *)lpszNewText, rect.Width());

    // Clean up!
    pDC->SelectObject(pFontOld);
    ReleaseDC(pDC);

	return CStatusBar::SetPaneText(nIndex, lpszNewText, bUpdate);
}

/////////////////////////////////////////////////////////////////////////////
// CMyStatusBar message handlers
