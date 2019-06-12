// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AE73E488_9E2A_43C7_B4E8_EA5E9DFAED7A__INCLUDED_)
#define AFX_STDAFX_H__AE73E488_9E2A_43C7_B4E8_EA5E9DFAED7A__INCLUDED_

#define WINVER 0x0501	// XP

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _DEBUG
// custom functions declaration (ATL + BETA version problems)
extern "C"
{
void * __cdecl _alloca(size_t);
#define alloca _alloca
}

#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#include <oaidl.h>
#include <setupapi.h>

extern "C"
{
#pragma warning (disable : 4201) // C4201 nonstandard extension used : nameless struct/union
#include <hidsdi.h>
#pragma warning (default : 4201) // C4201 nonstandard extension used : nameless struct/union
}

#include "hid_dll\bicfg.h"
#include "hid_dll\biapi.h"
#include "hid_dll\biusb.h"
#include "hid_dll\biusb_dev.h"
#pragma warning (disable : 4100)
#pragma warning (disable : 4505)
#include "NewMenu\NewMenu.h"
//#pragma warning (default : 4100)
//#pragma warning (default : 4505)

#pragma warning(disable:4201)
#include <MMSystem.h>

#define NO_XLISTCTRL_TOOL_TIPS
#define DO_NOT_INCLUDE_XCOMBOLIST

#define APL	0
#define IBM 1
#define XPLM200

#include "XPLMDataAccess.h"

#include "F4FlightData.h"
#include "BMSflightData.h"
#include "AFflightData.h"
#include "F4VarList.h"

#define ArraySize(X) (sizeof X / sizeof *X)

#include "XPdatarefs.h"
#include "XPFlightData.h"
#include "IL2datarefs.h"
#include "IL2FlightData.h"
#include "SimBindatarefs.h"
#include "SimBinSimData.h"
#include "RFdatarefs.h"
#include "RFSimData.h"
#include "GPXdatarefs.h"
#include "GPXSimData.h"
#include "LFSdatarefs.h"
#include "LFSSimData.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AE73E488_9E2A_43C7_B4E8_EA5E9DFAED7A__INCLUDED_)
