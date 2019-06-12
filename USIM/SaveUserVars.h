// SaveUserVars.h: interface for the CSaveUserVars class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEUSERVARS_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_)
#define AFX_SAVEUSERVARS_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_INPUT_FLAGS			256

struct _INPUT_FLAG_LIST
{
	UCHAR		Active;
	CString		VarTokenString;
	CString		ItemPathString;
	USHORT		ProductID;
	USHORT		IOIndex;
	UCHAR		FlagValue;
};

class CSaveUserVars : public CObject
{
public:
	CSaveUserVars();
	virtual ~CSaveUserVars();

	virtual void Serialize(CArchive& ar);
	void ClearUserVarsData();
	BOOL IsSet();

public:
	static const UINT m_uiFileClass;
	static const UINT m_uiFileVersion;
	static const UINT m_uiFileVersion_1;

	_INPUT_FLAG_LIST	m_IOFlags[MAX_INPUT_FLAGS];

protected:
	DECLARE_SERIAL(CSaveUserVars)
};

#endif // !defined(AFX_SAVEUSERVARS_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_)
