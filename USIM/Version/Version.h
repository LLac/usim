/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 1999
//
// based on a sample of
// John McTainsh <john.mctainsh@printrak.com>
//
/////////////////////////////////////////////////////////////////////////////
// Version.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERSION_H__C1260221_EBFA_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_VERSION_H__C1260221_EBFA_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

BOOL GetVersionInfo(LPTSTR szFilePath, LPCTSTR szSection, CString* const psData);
BOOL GetVersionInfo(LPTSTR szFilePath, VS_FIXEDFILEINFO* const pInfo);
BOOL GetVersionInfo(LPTSTR szFilePath, DWORD& dwLanguageAndCodepage);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_VERSION_H__C1260221_EBFA_11D1_B619_0000C0F55FDF__INCLUDED_)
