// KeyboardHook.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "KeyboardHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BYTE		g_iCombKeys;  // Combo key flags
HHOOK		g_hookKeyboard; // the hook handle
KEYRESULT	g_KeyStates;

/////////////////////////////////////////////////////////////////////////////
// CKeyboardHook

CKeyboardHook::CKeyboardHook()
{
	g_iCombKeys = 0;  // Combo key flags
	g_hookKeyboard = NULL; // the hook handle
}

CKeyboardHook::~CKeyboardHook()
{
}

/////////////////////////////////////////////////////////////////////////////
// CKeyboardHook message handlers

LRESULT CKeyboardHook::HookProc ()
{
	if (g_hookKeyboard) 
		return 0;

	HINSTANCE hExe = GetModuleHandle(NULL);
	g_hookKeyboard = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, hExe, AfxGetThread()->m_nThreadID);
	TRACE("Current Thread ID = 0x%X\n", AfxGetThread()->m_nThreadID);

	if (g_hookKeyboard) {
		TRACE ("Keyboard hook OK.\n");
		return 1;
	} else {
		TRACE ("Keyboard hook FAILED!\n");
		return 0;
	}
}


LRESULT CALLBACK CKeyboardHook::KeyboardProc (int nCode, WPARAM wParam, LPARAM lParam)
{
	CString sKey;

    if (nCode != HC_ACTION)
		return CallNextHookEx (g_hookKeyboard, nCode, wParam, lParam);

	if (GetKeyEventType(lParam) == KH_KEY_UP) {
		// update key states
		GetKeyCombStr(wParam, lParam, &g_KeyStates);
		return 0;
	}

	TRACE ("nCode 0x%x wParam 0x%x lParam 0x%x\n", nCode, wParam, lParam);
	
	GetKeyEventResult(wParam, lParam, &g_KeyStates);
	
	// store in focused cell
	theApp.m_pInputsTabCtrl->m_SendKeysTab.RecordKey( &g_KeyStates, GetKeyCombStr(wParam, lParam, &g_KeyStates) );

	return 1;
}

LRESULT CKeyboardHook::UnhookProc ()
{
	
	if(::UnhookWindowsHookEx(g_hookKeyboard)) {
		g_hookKeyboard = 0;
		TRACE ("Keyboard unhook OK.\n");
		return 1;
	} else {
		g_hookKeyboard = 0;
//		TRACE ("Keyboard unhook FAILED!\n");
		return 0;
	}
}

CString CKeyboardHook::GetKeyEventStr(BYTE iKeyEvent)
{
	CString sKeyEvent;
	if (iKeyEvent & KH_KEY_DOWN)
		sKeyEvent = _T("Key Down");

	if (iKeyEvent & KH_KEY_UP)
	{
		if (!sKeyEvent.IsEmpty())
			sKeyEvent += _T(",");
		sKeyEvent += _T("Key Up");
	}

	if (iKeyEvent & KH_KEY_REPEAT)
	{
		if (!sKeyEvent.IsEmpty())
			sKeyEvent += _T(",");
		sKeyEvent += _T("Key Repeat");
	}

	return sKeyEvent;
}

CString CKeyboardHook::GetKeyCombStr(WPARAM wParam, LPARAM lParam, tagKeyResultA* lpKeyResult)
{
	wParam;
	CString sComb;

	// update indicators on key-down only
	if (::GetKeyState(VK_CAPITAL) & 0x0001)
		lpKeyResult->iCaps = VK_CAPITAL;
	else
		lpKeyResult->iCaps = 0;

	if (::GetKeyState(VK_NUMLOCK) & 0x0001)
		lpKeyResult->iNum = VK_NUMLOCK;
	else
		lpKeyResult->iNum = 0;

	if (::GetKeyState(VK_SCROLL) & 0x0001)
		lpKeyResult->iScroll = VK_SCROLL;
	else
		lpKeyResult->iScroll = 0;

	// update combo keys on both key-down and key-up
	if (GetKeyEventType(lParam) == KH_KEY_DOWN || GetKeyEventType(lParam) == KH_KEY_UP) {
		lpKeyResult->iAlt = 0;
		if (::GetKeyState(VK_LMENU) & 0x8000) {
			lpKeyResult->iAlt = VK_LMENU;
			sComb = _T("LALT");
		} 
		if (::GetKeyState(VK_RMENU) & 0x8000) {
			lpKeyResult->iAlt = VK_RMENU;
			sComb = _T("RALT");
		}

		lpKeyResult->iCtrl = 0;
		if (::GetKeyState(VK_LCONTROL) & 0x8000) {
			lpKeyResult->iCtrl = VK_LCONTROL;
			if (!sComb.IsEmpty())
				sComb += _T("+");
			sComb += _T("LCTRL");
		}
		if (::GetKeyState(VK_RCONTROL) & 0x8000) {
			lpKeyResult->iCtrl = VK_RCONTROL;
			if (!sComb.IsEmpty())
				sComb += _T("+");
			sComb += _T("RCTRL");
		}

		lpKeyResult->iShift = 0;
		if (::GetKeyState(VK_LSHIFT) & 0x8000) {
			lpKeyResult->iShift = VK_LSHIFT;
			if (!sComb.IsEmpty())
				sComb += _T("+");
			sComb += _T("LSHIFT");
		}
		if (::GetKeyState(VK_RSHIFT) & 0x8000) {
			lpKeyResult->iShift = VK_RSHIFT;
			if (!sComb.IsEmpty())
				sComb += _T("+");
			sComb += _T("RSHIFT");
		}
	}

	return sComb;
}

BYTE CKeyboardHook::GetKeyEventType(LPARAM lParam)
{
	// Reference: WM_KEYDOWN on MSDN
	if (lParam & 0x80000000) {// check bit 31 for up/down
		return KH_KEY_UP;
	} else {
		if (lParam & 0x40000000) // check bit 30 for previous up/down
			return KH_KEY_REPEAT; // It was pressed down before this key-down event, so it's a key-repeat for sure
		else
			return KH_KEY_DOWN;
	}
}

void CKeyboardHook::GetKeyEventResult(WPARAM wParam, LPARAM lParam, tagKeyResultA* lpKeyResult)
{
	wParam;

	if (lpKeyResult == NULL)
		return;

	BYTE* pKeyboard = NULL; // only needed when the application requires to fill iPrintableChar
	pKeyboard = new BYTE[256];
	
	// Get the printable char
	WORD wCh = 0;
	if (::ToAscii(lpKeyResult->iVKCode, MAKELPARAM(0, HIWORD(lParam)), pKeyboard, &wCh, 0) == 1)
		lpKeyResult->chPrintableChar = (CHAR)wCh;
	else
		lpKeyResult->chPrintableChar = '\0';

	if (lpKeyResult->chPrintableChar < ' ')
		lpKeyResult->chPrintableChar = '\0'; // Only printable

	if (pKeyboard != NULL)
		delete [] pKeyboard;

	// Get the key name text
	if ((BYTE)wParam != VK_MENU &&
		(BYTE)wParam != VK_CONTROL &&
		(BYTE)wParam != VK_SHIFT &&
		(BYTE)wParam != VK_CAPITAL &&
		(BYTE)wParam != VK_NUMLOCK &&
		(BYTE)wParam != VK_SCROLL){

		lpKeyResult->iVKCode = LOBYTE(LOWORD(wParam));
		lpKeyResult->szKeyName[0] = L'\0';
		::GetKeyNameTextA(MAKELPARAM(0, HIWORD(lParam)), lpKeyResult->szKeyName, 31);

		//lpKeyResult->iSKCode = (USHORT)MapVirtualKey(lpKeyResult->iVKCode, 0);
		lpKeyResult->iSKCode = (USHORT)HIWORD(lParam);
		lpKeyResult->iExtended = false;
		if (lpKeyResult->iSKCode&0x100)
			lpKeyResult->iExtended = true; // Add extended bit
		lpKeyResult->iSKCode&=0xFF;

		TRACE ("iSKCode = 0x%x iExtended = %d\n", lpKeyResult->iSKCode, lpKeyResult->iExtended);
	}
}