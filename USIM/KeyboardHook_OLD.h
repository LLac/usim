#if !defined(AFX_KEYBOARDHOOK_H__90B79ACC_6C8A_485B_8AD5_C21BE77C07AB__INCLUDED_)
#define AFX_KEYBOARDHOOK_H__90B79ACC_6C8A_485B_8AD5_C21BE77C07AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardHook.h : header file
//
////////////////////////////////////////////////////////////////////////////////
//	The KEYRESULT Struct -- To be Passed to the "GetKeyEventResult" and Receives
//							Detailed Information of the Captured Key Events.
////////////////////////////////////////////////////////////////////////////////
typedef struct tagKeyResultA
{
	BYTE iKeyEvent;       // The captured key event type(key-down, key-up, or key-repeat).		
	BYTE iVKCode;         // The virtual key code produced by the captured key-stroke.
	USHORT iSKCode;         // The scan code produced by the captured key-stroke.
	BYTE iExtended;         // Extended key flag
	BYTE iAlt;				// alt vkode when the key event was captured.
	BYTE iCtrl;				// ctrl vkode when the key event was captured.
	BYTE iShift;			// shift vkode when the key event was captured.
	BYTE iCaps;				// On/off states of keyboard indicator caps-lock
	BYTE iNum;				// On/off states of keyboard indicator num-lock
	BYTE iScroll;			// On/off states of keyboard indicator scroll-lock
	char chPrintableChar; // The printable character produced by the captured key-stroke, 0 if not printable.
	char szKeyName[32];   // Name of the key. e.g. "Shift".
} KEYRESULTA, *LPKEYRESULTA;

typedef KEYRESULTA				KEYRESULT;
typedef LPKEYRESULTA			LPKEYRESULT;
typedef const LPKEYRESULTA		LPCKEYRESULT; 

//////////////////////////////////////////////////////////////////////////////////
//	Key Event Types -- Key-Down, Key-Up, Key-Repeat.
//////////////////////////////////////////////////////////////////////////////////
#define KH_KEY_DOWN				0x01 // Key-down event
#define KH_KEY_UP				0x02 // Key-up event
#define KH_KEY_REPEAT			0x04 // Key-repeat event, the key is held down for long enough

/////////////////////////////////////////////////////////////////////////////
// CKeyboardHook window

class CKeyboardHook
{
// Construction
public:
	CKeyboardHook();
	static void		GetKeyEventResult(WPARAM, LPARAM, tagKeyResultA*);
	static CString	GetKeyCombStr(WPARAM, LPARAM, tagKeyResultA*);
	static BYTE		GetKeyEventType(LPARAM);
	LRESULT	UnhookProc();
	LRESULT	HookProc();

	static LRESULT CALLBACK KeyboardProc (int nCode, WPARAM wParam, LPARAM lParam);

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CKeyboardHook();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDHOOK_H__90B79ACC_6C8A_485B_8AD5_C21BE77C07AB__INCLUDED_)
