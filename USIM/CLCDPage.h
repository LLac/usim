// CLCDPage.h: interface for the CLCDPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLCDPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_CLCDPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LCD_MAX_PAGES	10
#define LCD_MAX_LINES	4
#define LCD_MAX_CHARS	40
#define GLCD_MAX_LINES	5

class CCLCDPage
{
public:
	CCLCDPage();
	virtual ~CCLCDPage();
	void Serialize(CArchive& ar);
	void CopyData(CCLCDPage *pArray);
	void PasteData(CCLCDPage *pArray);
	void Clear();
	void ClearLCDPage(UINT);
	BOOL IsSet();
	BOOL IsPageSet(UINT);

public:
	// CRT RWR PAGE
	CString		m_Notes;
	UCHAR		m_LCDTest, m_LCDTestOld;
	UCHAR		m_LCDInvert, m_LCDInvertOld;
	UCHAR		m_DEDEnable;
	UCHAR		m_LCDRadio;
	UINT		m_LCDPageNext, m_LCDPagePrev;
	UINT		m_LCDPageDisplay[LCD_MAX_PAGES];
	int			m_LCDPageEnable[LCD_MAX_PAGES];
	CString		m_LCDPages[LCD_MAX_PAGES][GLCD_MAX_LINES];

	char		m_LCDLines[GLCD_MAX_LINES][LCD_MAX_CHARS];
	char		m_LCDLinesOld[GLCD_MAX_LINES][LCD_MAX_CHARS];
	char		m_LCDCurrentPage;
	UCHAR		m_FlagNextOld, m_FlagPrevOld;
	UCHAR		m_FlagDisplayOld;
protected:

};

#endif // !defined(AFX_CLCDPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
