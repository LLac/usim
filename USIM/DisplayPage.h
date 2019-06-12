// DisplayPage.h: interface for the DisplayPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_DISPLAYPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _DISP_LIST
{
	CString		VarTokenName;
//	UINT		VarToken;
	UINT		VarTokenIndex;
	UINT		VarArrayTokenIndex;
	UINT		SimType;
	BOOL		Read;
	BOOL		Write;
	CString		UserValue;
	double		Factor;
	double		Constant;
	UCHAR		LeadingZero;
	UCHAR		Grouping;
	UCHAR		Digits;
	double		Minimum, Maximum;
	UCHAR		Raw;
};

class CDisplayPage
{
public:
	CDisplayPage();
	virtual ~CDisplayPage();
	void Serialize(CArchive& ar, UINT uiFileVersion);
	void CopyData(CDisplayPage *pArray);
	void PasteData(CDisplayPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	// OUTPUT PAGE
	CString		m_Notes;
	UCHAR		m_OBLevel;
	UCHAR		m_LampTest;
	UCHAR		m_Override, m_OverrideOld;
	_DISP_LIST	m_Displays[7];

protected:

};

#endif // !defined(AFX_DISPLAYPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
