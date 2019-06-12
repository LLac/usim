// RwrPage.h: interface for the CrtRwrPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_RWRPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRwrPage
{
public:
	CRwrPage();
	virtual ~CRwrPage();
	void Serialize(CArchive& ar);
	void CopyData(CRwrPage *pArray);
	void PasteData(CRwrPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	// CRT RWR PAGE
	CString		m_Notes;
	UCHAR		m_RWRTest;
	UCHAR		m_RWREnable;

protected:

};

#endif // !defined(AFX_RWRPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
