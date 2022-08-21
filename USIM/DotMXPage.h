// DotMXPage.h: interface for the DotMXPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTMXPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_DOTMXPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _DMX_LIST
{
	CString		VarTokenName, VarBlinkTokenName, BlinkMaskName;
	UINT		VarTokenIndex, VarBlinkTokenIndex;
	UINT		VarArrayTokenIndex, VarArrayBlinkTokenIndex;
	UINT		SimType, BlinkSimType;
	BOOL		Read;
	BOOL		Write;
	CString		UserValue;
	double		Factor;
	double		Constant;
	UCHAR		LeadingZero;
	UCHAR		Grouping;
	UCHAR		Digits;
	double		Minimum, Maximum;
	int			BlinkMask;
	UINT		BlinkOnTime, BlinkOffTime;
	UINT		BlinkOnTimeCnt, BlinkOffTimeCnt;
	BOOL		BlinkOnFlag;
};

class CDotMXPage
{
public:
	CDotMXPage();
	virtual ~CDotMXPage();
	void Serialize(CArchive& ar, UINT uiFileVersion);
	void CopyData(CDotMXPage *pArray);
	void PasteData(CDotMXPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	// OUTPUT PAGE
	CString		m_Notes;
	UINT		m_OBLevel;
	UCHAR		m_LampTest;
	_DMX_LIST	m_Displays[4];
protected:

};

#endif // !defined(AFX_DOTMXPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
