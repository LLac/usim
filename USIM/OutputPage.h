// OutputPage.h: interface for the OutputPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_OUTPUTPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputPage
{
public:
	COutputPage();
	virtual ~COutputPage();
	void Serialize(CArchive& ar, UINT uiFileVersion);
	void CopyData(COutputPage *pArray);
	void PasteData(COutputPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	// OUTPUT PAGE
	int			m_BitMask, m_BlinkMask;
	UINT		m_BitType, m_BlinkType;
	UINT		m_BlinkOnTime, m_BlinkOffTime;
	UINT		m_BlinkOnTimeCnt, m_BlinkOffTimeCnt;
	BOOL		m_BlinkOnFlag;
	CString		m_Notes;
	UINT		m_OBRate;
	UINT		m_OBLevel;
	UCHAR		m_LampTest;
	UCHAR		m_Invert;
	CString		m_VarTokenName, m_BitMaskName, m_VarBlinkTokenName, m_BlinkMaskName;
	UINT		m_VarTokenIndex, m_VarBlinkTokenIndex;
	UINT		m_VarArrayTokenIndex, m_VarArrayBlinkTokenIndex;
	UINT		m_SimType;
	BOOL		m_Read;
	BOOL		m_Write;
	UCHAR		m_Condition1, m_Condition2, m_Condition3;
	float		m_OPValue1, m_OPValue2;

protected:

};

#endif // !defined(AFX_F4OUTPUTPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
