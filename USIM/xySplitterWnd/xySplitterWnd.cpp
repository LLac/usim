// xySplitterWnd.cpp : implementation file
//

#include "stdafx.h"

#include "xySplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxSplitterWnd

CxSplitterWnd::CxSplitterWnd()
{
}

CxSplitterWnd::~CxSplitterWnd()
{
}

BOOL CxSplitterWnd::BindWithControl(CWnd *parent, DWORD ctrlId)
{
    m_Parent = parent;
    m_minLeft = m_minRight = 10;

    SubclassWindow(m_Parent->GetDlgItem(ctrlId)->GetSafeHwnd());

    // Make sure to get mouse message from the dialog window
    DWORD style = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, style | SS_NOTIFY);

    return TRUE;
}

void CxSplitterWnd::Unbind(void)
{
    DetachAllPanes();
    UnsubclassWindow();
}

void CxSplitterWnd::SetMinWidth(int left, int right)
{
    m_minLeft = left;
    m_minRight = right;
}

BOOL CxSplitterWnd::AttachAsLeftPane(DWORD ctrlId)
{
    m_leftIds.Add(ctrlId);
    return TRUE;
}

BOOL CxSplitterWnd::AttachAsRightPane(DWORD ctrlId)
{
    m_rightIds.Add(ctrlId);
    return TRUE;
}

BOOL CxSplitterWnd::DetachAllPanes(void)
{
    m_leftIds.RemoveAll();
    m_rightIds.RemoveAll();
    return TRUE;
}

void CxSplitterWnd::RecalcLayout(void)
{
    CWnd *pane;
    RECT rcBar, rcPane;

    GetWindowRect(&rcBar);
    m_Parent->ScreenToClient(&rcBar);

    int i;
    DWORD id;

    for (i=0; i<m_leftIds.GetSize(); i++) {
        id = m_leftIds.GetAt(i);
	    pane = m_Parent->GetDlgItem(id);
        pane->GetWindowRect(&rcPane);
        m_Parent->ScreenToClient(&rcPane);
        rcPane.right = rcBar.left - 1;
        pane->MoveWindow(&rcPane, FALSE);
    }

    for (i=0; i<m_rightIds.GetSize(); i++) {
        id = m_rightIds.GetAt(i);
	    pane = m_Parent->GetDlgItem(id);
        pane->GetWindowRect(&rcPane);
        m_Parent->ScreenToClient(&rcPane);
        rcPane.left = rcBar.right + 1;
        pane->MoveWindow(&rcPane, FALSE);
    }

    m_Parent->Invalidate();
}

BOOL CxSplitterWnd::GetMouseClipRect(LPRECT rectClip, CPoint point)
{
    RECT rectOrg, rectTarget, rectParent, rectPane;
    int i;
    DWORD id;

    GetWindowRect(&rectOrg);
    m_Parent->GetClientRect(&rectParent);
    m_Parent->ClientToScreen(&rectParent);

    rectTarget = rectOrg;
    rectTarget.left = rectParent.left + m_minLeft;
    for (i=0; i<m_leftIds.GetSize(); i++) {
        id = m_leftIds.GetAt(i);
        m_Parent->GetDlgItem(id)->GetWindowRect(&rectPane);
        if (rectTarget.left < rectPane.left + m_minLeft) {
            rectTarget.left = rectPane.left + m_minLeft;
        }
    }

    rectTarget.right = rectParent.right - m_minRight;
    for (i=0; i<m_rightIds.GetSize(); i++) {
        id = m_rightIds.GetAt(i);
        m_Parent->GetDlgItem(id)->GetWindowRect(&rectPane);
        if (rectTarget.right > rectPane.right - m_minRight) {
            rectTarget.right = rectPane.right - m_minRight;
        }
    }

    if (rectTarget.left >= rectTarget.right) {
        TRACE("No room to drag the x-splitter bar");
        return FALSE;
    }

    rectClip->left = rectTarget.left + point.x;
    rectClip->right = rectTarget.right - (rectOrg.right - rectOrg.left - point.x) + 1;
    rectClip->top = rectOrg.top;
    rectClip->bottom = rectOrg.bottom;

    return TRUE;
}

BEGIN_MESSAGE_MAP(CxSplitterWnd, CWnd)
	//{{AFX_MSG_MAP(CxSplitterWnd)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxSplitterWnd message handlers

BOOL CxSplitterWnd::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	// TODO: Add your message handler code here and/or call default
    ::SetCursor(AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR));
    return TRUE;
	
	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CxSplitterWnd::OnLButtonDown(UINT /*nFlags*/, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// don't handle if capture already set
	if (::GetCapture() != NULL) return;

    // don't handle if no room to drag
    RECT rectMouseClip;
    if (!GetMouseClipRect(&rectMouseClip, point)) return;
    ::ClipCursor(&rectMouseClip);

	// set capture to the window which received this message
	SetCapture();
	ASSERT(this == CWnd::GetCapture());

    // get DC for drawing
	CDC* pDrawDC;
    pDrawDC = m_Parent->GetDC();
	ASSERT_VALID(pDrawDC);

    int     curX, curY;
	int     xDiff, yDiff;
	CRect   rectOrg, rectCur, rectOld;
    CSize   sizeBar;

    GetWindowRect(rectOrg);
    sizeBar = CSize(rectOrg.Width(), rectOrg.Height());

    m_Parent->ScreenToClient(rectOrg);
    pDrawDC->DrawDragRect(&rectOrg, sizeBar, NULL, sizeBar);
    rectOld = rectCur = rectOrg;
    xDiff = yDiff = 0;

	// get messages until capture lost or cancelled/accepted
	for (;;) {
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			break;

		switch (msg.message) {
		// handle movement/accept messages
		case WM_MOUSEMOVE:
			// handle resize cases (and part of move)
            curX = (int)(short)LOWORD(msg.lParam);
            curY = (int)(short)HIWORD(msg.lParam);

            xDiff = curX - point.x;
            yDiff = curY - point.y;

            rectCur = rectOrg;
            rectCur.left += xDiff;
            rectCur.right += xDiff;
            pDrawDC->DrawDragRect(&rectCur, sizeBar, &rectOld, sizeBar);
            rectOld = rectCur;

            break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
            goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
    pDrawDC->DrawDragRect(&rectCur, sizeBar, NULL, sizeBar);

    m_Parent->ReleaseDC(pDrawDC);
    ReleaseCapture();
    ::ClipCursor(NULL);

    if (xDiff == 0) return;

    // move the splitter bar & re-position the attached panes if necessary
    MoveWindow(rectCur, FALSE);
    RecalcLayout();

	m_Parent->SendMessage(WM_SPLITTER_MOVED, xDiff, GetDlgCtrlID());

    //CWnd::OnLButtonDown(nFlags, point);
}

//#define PLACEMENT_FMT 	_T("%d,%d,%d,%d,%d,%d")

BOOL CxSplitterWnd::SaveSplitter(LPCTSTR pszSection)
{
	CRect myRect;
	CString data;

	GetWindowRect(&myRect);
	GetParent()->ScreenToClient(&myRect);
	data.Format(_T("%d,%d"), myRect.left, myRect.right);

	return AfxGetApp()->WriteProfileString(pszSection, "SplitterX", data);
}

BOOL CxSplitterWnd::RestoreSplitter(LPCTSTR pszSection)
{
	CRect myRect;
	CString data;

	data = AfxGetApp()->GetProfileString(pszSection, "SplitterX");

	GetWindowRect(&myRect);
	GetParent()->ScreenToClient(&myRect);
	if (_stscanf(data, _T("%d,%d"), &myRect.left, &myRect.right) == 2) {
		MoveWindow(&myRect, false);
		RecalcLayout();
		m_Parent->SendMessage(WM_SPLITTER_MOVED, 0, 0);
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CySplitterWnd

CySplitterWnd::CySplitterWnd()
{
}

CySplitterWnd::~CySplitterWnd()
{
}

BOOL CySplitterWnd::BindWithControl(CWnd *parent, DWORD ctrlId)
{
    m_Parent = parent;

    SubclassWindow(m_Parent->GetDlgItem(ctrlId)->GetSafeHwnd());
    
    // Make sure to get mouse message from the dialog window
    DWORD style = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, style | SS_NOTIFY);

    return TRUE;
}

void CySplitterWnd::Unbind(void)
{
    DetachAllPanes();
    UnsubclassWindow();
}

void CySplitterWnd::SetMinHeight(int above, int below)
{
    m_minAbove = above;
    m_minBelow = below;
}

BOOL CySplitterWnd::AttachAsAbovePane(DWORD ctrlId)
{
    m_aboveIds.Add(ctrlId);
    return TRUE;
}

BOOL CySplitterWnd::AttachAsBelowPane(DWORD ctrlId)
{
    m_belowIds.Add(ctrlId);
    return TRUE;
}

BOOL CySplitterWnd::DetachAllPanes(void)
{
    m_aboveIds.RemoveAll();
    m_belowIds.RemoveAll();
    return TRUE;
}

void CySplitterWnd::RecalcLayout(void)
{
    CWnd *pane;
    RECT rcBar, rcPane;

    GetWindowRect(&rcBar);
    m_Parent->ScreenToClient(&rcBar);

    int i;
    DWORD id;

    for (i=0; i<m_aboveIds.GetSize(); i++) {
        id = m_aboveIds.GetAt(i);
	    pane = m_Parent->GetDlgItem(id);
        pane->GetWindowRect(&rcPane);
        m_Parent->ScreenToClient(&rcPane);
        rcPane.bottom = rcBar.top - 1;
        pane->MoveWindow(&rcPane, FALSE);
    }

    for (i=0; i<m_belowIds.GetSize(); i++) {
        id = m_belowIds.GetAt(i);
	    pane = m_Parent->GetDlgItem(id);
        pane->GetWindowRect(&rcPane);
        m_Parent->ScreenToClient(&rcPane);
        rcPane.top = rcBar.bottom + 1;
        pane->MoveWindow(&rcPane, FALSE);
    }

    m_Parent->Invalidate();
}

BOOL CySplitterWnd::GetMouseClipRect(LPRECT rectClip, CPoint point)
{
    RECT rectOrg, rectTarget, rectParent, rectPane;
    int i;
    DWORD id;

    GetWindowRect(&rectOrg);
    m_Parent->GetClientRect(&rectParent);
    m_Parent->ClientToScreen(&rectParent);

    rectTarget = rectOrg;
    rectTarget.top = rectParent.top + m_minAbove;
    for (i=0; i<m_aboveIds.GetSize(); i++) {
        id = m_aboveIds.GetAt(i);
        m_Parent->GetDlgItem(id)->GetWindowRect(&rectPane);
        if (rectTarget.top < rectPane.top + m_minAbove) {
            rectTarget.top = rectPane.top + m_minAbove;
        }
    }

    rectTarget.bottom = rectParent.bottom - m_minBelow;
    for (i=0; i<m_belowIds.GetSize(); i++) {
        id = m_belowIds.GetAt(i);
        m_Parent->GetDlgItem(id)->GetWindowRect(&rectPane);
        if (rectTarget.bottom > rectPane.bottom - m_minBelow) {
            rectTarget.bottom = rectPane.bottom - m_minBelow;
        }
    }

    if (rectTarget.top >= rectTarget.bottom) {
        TRACE("No room to drag the y-splitter bar");
        return FALSE;
    }

    rectClip->left = rectOrg.left;
    rectClip->right = rectOrg.right;
    rectClip->top = rectTarget.top + point.y;
    rectClip->bottom = rectTarget.bottom - (rectOrg.bottom - rectOrg.top - point.y) + 1;

    return TRUE;
}


BEGIN_MESSAGE_MAP(CySplitterWnd, CWnd)
	//{{AFX_MSG_MAP(CySplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CySplitterWnd message handlers

BOOL CySplitterWnd::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	// TODO: Add your message handler code here and/or call default
    ::SetCursor(AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR));
    return TRUE;
	
	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CySplitterWnd::OnLButtonDown(UINT /*nFlags*/, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// don't handle if capture already set
	if (::GetCapture() != NULL) return;

    // don't handle if no room to drag
    RECT rectMouseClip;
    if (!GetMouseClipRect(&rectMouseClip, point)) return;
    ::ClipCursor(&rectMouseClip);

	// set capture to the window which received this message
	SetCapture();
	ASSERT(this == CWnd::GetCapture());

    // get DC for drawing
	CDC* pDrawDC;
    pDrawDC = m_Parent->GetDC();
	ASSERT_VALID(pDrawDC);

    int     curX, curY;
	int     xDiff, yDiff;
	CRect   rectOrg, rectCur, rectOld;
    CSize   sizeBar;

    GetWindowRect(rectOrg);
    sizeBar = CSize(rectOrg.Width(), rectOrg.Height());

    m_Parent->ScreenToClient(rectOrg);
    pDrawDC->DrawDragRect(&rectOrg, sizeBar, NULL, sizeBar);
    rectOld = rectCur = rectOrg;
    xDiff = yDiff = 0;

	// get messages until capture lost or cancelled/accepted
	for (;;) {
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			break;

		switch (msg.message) {
		// handle movement/accept messages
		case WM_MOUSEMOVE:
			// handle resize cases (and part of move)
            curX = (int)(short)LOWORD(msg.lParam);
            curY = (int)(short)HIWORD(msg.lParam);

            xDiff = curX - point.x;
            yDiff = curY - point.y;

            rectCur = rectOrg;
            rectCur.top += yDiff;
            rectCur.bottom += yDiff;
            pDrawDC->DrawDragRect(&rectCur, sizeBar, &rectOld, sizeBar);
            rectOld = rectCur;

            break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
            goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
    pDrawDC->DrawDragRect(&rectCur, sizeBar, NULL, sizeBar);

    m_Parent->ReleaseDC(pDrawDC);
    ReleaseCapture();
    ::ClipCursor(NULL);

    if (yDiff == 0) return;

    // move the splitter bar & re-position the attached panes if necessary
    MoveWindow(rectCur, FALSE);
    RecalcLayout();

	m_Parent->SendMessage(WM_SPLITTER_MOVED, yDiff, GetDlgCtrlID());

    //CWnd::OnLButtonDown(nFlags, point);
}
