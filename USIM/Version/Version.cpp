/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 1999
//
// based on a sample of
// John McTainsh <john.mctainsh@printrak.com>
//
/////////////////////////////////////////////////////////////////////////////
// Version.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Version.h"
#include "..\Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// Extract version information from a file.
// Note: only the first language data is returned.
//
// PARAMS:
// szFilePath File name to extract version info from. "C:\\WINNT\\Explorer.exe"
//		For the current app use
//		GetModuleFileName(AfxGetInstanceHandle(), szFullPath, sizeof(szFullPath));
// szSection Version section data. "ProductVersion"
//		See also <Resourcefile> -> Version.
// psData  Data read, OR error message.
//
// RETURN:
// True if all went well.

BOOL GetVersionInfo(LPTSTR szFilePath, LPCTSTR szSection, CString* const psData)
{
	// Inital parameters
	DWORD dwHandle;				// Set to Zero (Unused)
	DWORD dwInfoSize;			// Size of the Info Structure.
	UINT uiLanguageSize;		// Size of language buffer
	UINT uiSize;				// Size of Data recieved with version information
	DWORD const* pdwLanguages;

	// Retrieve the File information
	dwInfoSize = ::GetFileVersionInfoSize(szFilePath, &dwHandle);
	if (dwInfoSize)
	{
		// Allocate the memory
		BYTE *const pbData = new BYTE[dwInfoSize];
		if (!pbData)
		{
//			psData->LoadString(IDS_ERROR_VERSION_INSUF_MEM);
			return FALSE;
		}

		// Get the Version data
		if (::GetFileVersionInfo(szFilePath, dwHandle, dwInfoSize, pbData))
		{
			psData->Empty();
			LPCTSTR szData = NULL;	 // Pointer to where data will be located

			// Get the translation information. (Language)
			if (::VerQueryValue(pbData, _T("\\VarFileInfo\\Translation"), (void**)&pdwLanguages, &uiLanguageSize))
			{
				if (uiLanguageSize)
				{
					TCHAR szSubBlock[512];   // Version language and section definition
					_stprintf(szSubBlock, _T("\\StringFileInfo\\%04hX%04hX\\%s"), 
						LOWORD(*pdwLanguages), HIWORD(*pdwLanguages), szSection);
					::VerQueryValue(pbData, szSubBlock, (void**)&szData, &uiSize);
					
					CString verText = szData; 
					verText.Replace(",", ".");
					*psData = verText;

					// This is a happy ending
					delete [] pbData;
					return TRUE;
				}
			}
		}

//		psData->LoadString(IDS_ERROR_VERSION_EXTRACT);
		delete [] pbData;
		return FALSE;
	}

	// Failed to locate file or read version data
//	psData->LoadString(IDS_ERROR_VERSION_NO_INFO);
	return FALSE;
}

BOOL GetVersionInfo(LPTSTR szFilePath, VS_FIXEDFILEINFO* const pInfo)
{
	// Inital parameters
	DWORD dwHandle;				// Set to Zero (Unused)
	DWORD dwInfoSize;			// Size of the Info Structure.
	UINT uiSize;				// Size of Data recieved with version information
	CString str;

	// Retrieve the File information
	dwInfoSize = ::GetFileVersionInfoSize(szFilePath, &dwHandle);
	if (dwInfoSize)
	{
		// Allocate the memory
		BYTE *const pbData = new BYTE[dwInfoSize];
		if (!pbData)
		{
//			str.LoadString(IDS_ERROR_VERSION_INSUF_MEM);
			TRACE0(str+"\n");
			return FALSE;
		}

		VS_FIXEDFILEINFO* pData = NULL;	 // Pointer to where data will be located
		// Get the Version data
		if (::GetFileVersionInfo(szFilePath, dwHandle, dwInfoSize, pbData) && 
			::VerQueryValue(pbData, _T("\\"), (void**)&pData, &uiSize))
		{
			// This is a happy ending
			memcpy(pInfo, pData, sizeof(VS_FIXEDFILEINFO));
			delete [] pbData;
			return TRUE;
		}

//		str.LoadString(IDS_ERROR_VERSION_EXTRACT);
		TRACE0(str+"\n");
		delete [] pbData;
		return FALSE;
	}

	// Failed to locate file or read version data
//	str.LoadString(IDS_ERROR_VERSION_NO_INFO);
	TRACE0(str+"\n");
	return FALSE;
}

BOOL GetVersionInfo(LPTSTR szFilePath, DWORD& dwLanguageAndCodepage)
{
	// Inital parameters
	DWORD dwHandle;				// Set to Zero (Unused)
	DWORD dwInfoSize;			// Size of the Info Structure.
	UINT uiLanguageSize;		// Size of language buffer
	DWORD const* pdwLanguages;
	CString str;

	// Retrieve the File information
	dwInfoSize = ::GetFileVersionInfoSize(szFilePath, &dwHandle);
	if (dwInfoSize)
	{
		// Allocate the memory
		BYTE *const pbData = new BYTE[dwInfoSize];
		if (!pbData)
		{
//			str.LoadString(IDS_ERROR_VERSION_INSUF_MEM);
			TRACE0(str+"\n");
			return FALSE;
		}

		// Get the Version data
		if (::GetFileVersionInfo(szFilePath, dwHandle, dwInfoSize, pbData))
		{
			// Get the translation information. (Language)
			if (::VerQueryValue(pbData, _T("\\VarFileInfo\\Translation"), (void**)&pdwLanguages, &uiLanguageSize))
			{
				if (uiLanguageSize)
				{
					dwLanguageAndCodepage = *pdwLanguages;

					// This is a happy ending
					delete [] pbData;
					return TRUE;
				}
			}
		}

//		str.LoadString(IDS_ERROR_VERSION_EXTRACT);
		TRACE0(str+"\n");
		delete [] pbData;
		return FALSE;
	}

	// Failed to locate file or read version data
//	str.LoadString(IDS_ERROR_VERSION_NO_INFO);
	TRACE0(str+"\n");
	return FALSE;
}
