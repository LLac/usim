// UpdateCheck.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATECHECK_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_)
#define AFX_UPDATECHECK_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define UPDATECHECK_BROWSER_STRING _T("Update search")
class CUpdateCheck  
{
public:
	virtual void Check(UINT uiURL, BOOL msgFLAG = true);
	virtual void Check(const CString& strURL, BOOL msgFLAG = true);

	CUpdateCheck();
	virtual ~CUpdateCheck();

	static HINSTANCE GotoURL(LPCTSTR url, int showcmd);
	static BOOL GetFileVersion(DWORD &dwMS, DWORD &dwLS);
	static LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);

protected:
	virtual void MsgUpdateNotAvailable();
	virtual void MsgUpdateNoCheck();
};

#endif // !defined(AFX_UPDATECHECK_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_)
