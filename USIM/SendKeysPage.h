// SendKeysPage.h: interface for the CSendKeysPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDKEYSPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_SENDKEYSPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagKeyMacro
{
	BYTE iVKCode;			// The virtual key code produced by the captured key-stroke.
	USHORT iSKCode;			// The scan code produced by the captured key-stroke.
	BYTE iAlt;				// alt vkode when the key event was captured.
	BYTE iCtrl;				// ctrl vkode when the key event was captured.
	BYTE iShift;			// shift vkode when the key event was captured.
	BYTE iCaps;				// On/off states of keyboard indicator caps-lock
	BYTE iNum;				// On/off states of keyboard indicator num-lock
	BYTE iScroll;			// On/off states of keyboard indicator scroll-lock
	UINT iHoldDelay;		// time to keep key macro pressed
	UINT iMacroDelay;		// time between key macros
	CString iMacroString;	// key macro string
	BOOL iExtended;			// Extended Key flag
} TAGKEYMACRO;

#define MAX_MACROS	8

class CSendKeysPage
{
public:
	CSendKeysPage();
	virtual ~CSendKeysPage();
	void Serialize(CArchive& ar);
	void CopyData(CSendKeysPage *pArray);
	void PasteData(CSendKeysPage *pArray);
	void Clear();
	void OPClear();
	void ORClear();
	BOOL IsSet();

public:
	// SENDKEYS PAGE
	TAGKEYMACRO	m_OPKeyMacros[MAX_MACROS], m_ORKeyMacros[MAX_MACROS];
	BOOL		m_OPRepeat, m_OPHoldKey;
	CString		m_Notes;

protected:

};

#endif // !defined(AFX_SENDKEYSPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
