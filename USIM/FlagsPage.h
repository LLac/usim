// FlagsPage.h: interface for the CFlagsPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLAGSPAGE_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_)
#define AFX_FLAGSPAGE_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SaveUserVars.h"

//struct _IO_FLAG_LIST
//{
//	UCHAR		Active;
//	USHORT		IOIndex;
//};

class CFlagsPage
{
public:
	CFlagsPage();
	virtual ~CFlagsPage();

	virtual void Serialize(CArchive& ar);
	void CopyData(CFlagsPage *pArray);
	void PasteData(CFlagsPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	CString		m_Notes;
	UCHAR		m_PowerFlag;
	UCHAR		Active[MAX_INPUT_FLAGS];
//	_IO_FLAG_LIST	m_IOFlags[MAX_INPUT_FLAGS];

protected:

};

#endif // !defined(AFX_F4FLAGSPAGE_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_)
