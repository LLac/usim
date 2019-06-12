// USIMx64.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "USIMx64.h"
#include <psapi.h>
#include <atlstr.h>

#define MAX_LOADSTRING 100
#define FIND_PROCESS 0x01
#define BUFSIZE 4096 

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL SearchForProcess(CString sPath, UINT ModuleType, BOOL ScopePath)
{
	HANDLE hProcess;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	CStringA sProcessName;
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	unsigned int i;

	sPath.MakeUpper();

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return false;
	}

	cProcesses = cbNeeded / sizeof(DWORD);	// Calculate how many process identifiers were returned.
	// Search for process
	for (i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			// Get a handle to the process.
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

			// Get the process name.
			if (hProcess != NULL) {
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, ModuleType)) {
					if (ScopePath)
						GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
					else
						GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
					sProcessName.Format("%S", szProcessName);
					sProcessName.MakeUpper();
					if ((sProcessName == (CStringA)sPath) && !sProcessName.IsEmpty()) {
						CloseHandle(hProcess);
						hProcess = NULL;
						return true;
					}
				}
			}
			if (hProcess) {
				CloseHandle(hProcess);
				hProcess = NULL;
			}
		}
	}

	return false;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	CHAR chBuf[BUFSIZE];
	CString cSimPath;
	CHAR cbuf;
	DWORD dwRead, dwWritten;
	HANDLE hStdin, hStdout;
	BOOL bSuccess;

	dwRead = NULL;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_USIMX64, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_USIMX64));
    MSG msg;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if ((hStdout == INVALID_HANDLE_VALUE) || (hStdin == INVALID_HANDLE_VALUE))
		ExitProcess(1);

    // Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		cSimPath = _T("USIM.exe");
		if (!SearchForProcess(cSimPath, LIST_MODULES_32BIT, true)) {
			PostQuitMessage(0);	// USIM not runnning so quit.
			return 0;
		}

		// Read from standard input and stop on error or no data.
		bSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);
		if (bSuccess && (dwRead > 0)) {
			cSimPath = CString(chBuf, dwRead);

			// Search for process
			cbuf = 0;
			if (SearchForProcess((LPTSTR)(LPCTSTR)cSimPath, LIST_MODULES_64BIT, false)) {
				cbuf = 1;
			}
			
			bSuccess = WriteFile(hStdout, &cbuf, 1, &dwWritten, NULL);
		}
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_USIMX64));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_USIMX64);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

//   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

