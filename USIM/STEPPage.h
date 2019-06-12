// STEPPage.h: interface for the F4STEPPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEPPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_STEPPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSTEPPage
{
public:
	CSTEPPage();
	virtual ~CSTEPPage();
	void Serialize(CArchive& ar, UINT uiFileVersion);
	void CopyData(CSTEPPage *pArray);
	void PasteData(CSTEPPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	CString		m_Notes;
	CString		m_VarTokenName;
	UINT		m_VarTokenIndex;
	UINT		m_VarArrayTokenIndex;
	UINT		m_SimType;
	BOOL		m_Read;
	BOOL		m_Write;
	long		m_STEPMin[8], m_STEPMax[8], m_STEPOffset[8];
	float		m_MinValue[8], m_MaxValue[8];
	CString		m_Formula[8];
	UCHAR		m_STEPTest;
	long		m_STEPTemp;
	UCHAR		m_STEPCalibrate;
	UCHAR		m_Sections;

protected:

};

#endif // !defined(AFX_STEPPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
