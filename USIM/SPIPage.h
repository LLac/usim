// SPIPage.h: interface for the F4SPIPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_SPIPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSPIPage
{
public:
	CSPIPage();
	virtual ~CSPIPage();
	void Serialize(CArchive& ar, UINT uiFileVersion);
	void CopyData(CSPIPage *pArray);
	void PasteData(CSPIPage *pArray);
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
	long		m_SPIMin[8], m_SPIMax[8], m_SPIOffset[8];
	float		m_MinValue[8], m_MaxValue[8];
	CString		m_Formula[8];
	UCHAR		m_SPITest;
	long		m_SPITemp;
	UCHAR		m_SPICalibrate;
	UCHAR		m_Sections;

protected:

};

#endif // !defined(AFX_SPIPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
