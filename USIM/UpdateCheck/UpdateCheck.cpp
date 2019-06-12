// UpdateCheck.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxinet.h>
#include "UpdateCheck.h"
#include "..\resource.h"
#include "..\USIM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpdateCheck::CUpdateCheck()
{

}

CUpdateCheck::~CUpdateCheck()
{
}

BOOL CUpdateCheck::GetFileVersion(DWORD &dwMS, DWORD &dwLS)
{
	char szModuleFileName[MAX_PATH];

    LPBYTE  lpVersionData; 

	if (GetModuleFileName(AfxGetInstanceHandle(), szModuleFileName, sizeof(szModuleFileName)) == 0) return FALSE;

    DWORD dwHandle;     
    DWORD dwDataSize = ::GetFileVersionInfoSize(szModuleFileName, &dwHandle); 
    if ( dwDataSize == 0 ) 
        return FALSE;

    lpVersionData = new BYTE[dwDataSize]; 
    if (!::GetFileVersionInfo(szModuleFileName, dwHandle, dwDataSize, (void**)lpVersionData) )
    {
		delete [] lpVersionData;
        return FALSE;
    }

    ASSERT(lpVersionData != NULL);

    UINT nQuerySize;
	VS_FIXEDFILEINFO* pVsffi;
    if ( ::VerQueryValue((void **)lpVersionData, _T("\\"),
                         (void**)&pVsffi, &nQuerySize) )
    {
		dwMS = pVsffi->dwFileVersionMS;
		dwLS = pVsffi->dwFileVersionLS;
		delete [] lpVersionData;
        return TRUE;
    }

	delete [] lpVersionData;
    return FALSE;

}

void CUpdateCheck::Check(UINT uiURL, BOOL msgFLAG)
{
	CString strURL(MAKEINTRESOURCE(uiURL));
	Check(strURL, msgFLAG);
}

void CUpdateCheck::Check(const CString& strURL, BOOL msgFLAG)
{
	DWORD dwMS, dwLS;
	if (!GetFileVersion(dwMS, dwLS))
	{
		ASSERT(FALSE);
		return;
	}

	CWaitCursor wait;
	HINTERNET hInet = InternetOpen(UPDATECHECK_BROWSER_STRING, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	HINTERNET hUrl = InternetOpenUrl(hInet,
										strURL,
										NULL,
										(DWORD)(-1L),
										INTERNET_FLAG_RELOAD | 
										INTERNET_FLAG_PRAGMA_NOCACHE |
										INTERNET_FLAG_NO_CACHE_WRITE |
										WININET_API_FLAG_ASYNC,
										NULL);
	if (hUrl)
	{
		char szBuffer[512];
		DWORD dwRead;
		if (InternetReadFile(hUrl, szBuffer, sizeof(szBuffer), &dwRead))
		{
			if (dwRead > 0)
			{
				szBuffer[dwRead] = 0;
				CString strSubMS1;
				CString strSubMS2;
				CString strSubMS3;
				CString strSubMS4;
				CString strSub;
				DWORD dwMSWeb, dwLSWeb;
				AfxExtractSubString(strSubMS1, szBuffer, 0, '|');
				AfxExtractSubString(strSubMS2, szBuffer, 1, '|');
				AfxExtractSubString(strSubMS3, szBuffer, 2, '|');
				AfxExtractSubString(strSubMS4, szBuffer, 3, '|');
				dwMSWeb = MAKELONG((WORD) atol(strSubMS2), (WORD) atol(strSubMS1));
				dwLSWeb = MAKELONG((WORD) atol(strSubMS4), (WORD) atol(strSubMS3));

				if (dwMSWeb > dwMS) {
					AfxExtractSubString(strSub, szBuffer, 4, '|');
					strSubMS1.Format(IDS_UPDATE_AVAILABLE, HIWORD(dwMSWeb), LOWORD(dwMSWeb), HIWORD(dwLSWeb), LOWORD(dwLSWeb));

					if (MessageBox(NULL, strSubMS1, "Update Check", MB_YESNO|MB_ICONINFORMATION) == IDYES)
						GotoURL(strSub, SW_SHOW);
				} else if (dwMSWeb == dwMS && dwLSWeb > dwLS) {
					AfxExtractSubString(strSub, szBuffer, 4, '|');
					strSubMS1.Format(IDS_UPDATE_AVAILABLE, HIWORD(dwMSWeb), LOWORD(dwMSWeb), HIWORD(dwLSWeb), LOWORD(dwLSWeb));

					if (MessageBox(NULL, strSubMS1, "Update Check", MB_YESNO|MB_ICONINFORMATION) == IDYES)
						GotoURL(strSub, SW_SHOW);
				} else {
					if (msgFLAG) MsgUpdateNotAvailable();
				}
			}
			else
				if (msgFLAG) MsgUpdateNoCheck();

		}
		InternetCloseHandle(hUrl);
	}
	else
		if (msgFLAG) MsgUpdateNoCheck();

	InternetCloseHandle(hInet);
}

HINSTANCE CUpdateCheck::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) 
	{

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
		{
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = _tcsstr(key, _T("%1"));      // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);

                result = (HINSTANCE) WinExec(key,showcmd);
            }
        }
    }

    return result;
}

LONG CUpdateCheck::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) 
	{
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

void CUpdateCheck::MsgUpdateNotAvailable()
{
	CString str;
	str.Format(IDS_UPDATE_NO);
	MessageBox(NULL, str, "Version Check", MB_OK|MB_ICONINFORMATION);
}

void CUpdateCheck::MsgUpdateNoCheck()
{
	CString str;
	str.Format(IDS_UPDATE_NOCHECK);
	MessageBox(NULL, str, "Version Check", MB_OK|MB_ICONINFORMATION);
}
