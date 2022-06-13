// USIM.h : main header file for the USIM application
//

#if !defined(AFX_USIM_H__064A8B71_FFA8_4667_A613_AA9B7B857B13__INCLUDED_)
#define AFX_USIM_H__064A8B71_FFA8_4667_A613_AA9B7B857B13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "TreeDevices.h"
#include "SaveDevice.h"
#include "SaveUserVars.h"

#include "DefTabCtrl.h"
#include "OutputTabCtrl.h"
#include "MuxTabCtrl.h"
#include "DispTabCtrl.h"
#include "RWRTabCtrl.h"
#include "CLCDTabCtrl.h"
#include "GLCDTabCtrl.h"
#include "InputsTabCtrl.h"
#include "LatchedTabCtrl.h"
#include "SPITabCtrl.h"
#include "STEPTabCtrl.h"
#include "DotMXTabCtrl.h"
#include "F4SharedMem.h"
#include "XPSharedMem.h"
#include "SimBinSharedMem.h"
#include "RFSharedMem.h"
#include "GPXSharedMem.h"
#include "OutputThread.h"
#include "InputThread.h"
#include "SendKeysThread.h"
#include "SimConnectThread.h"
#include "IL2_UDPThread.h"
#include "LFS_UDPThread.h"
#include "KeyCheckThread.h"

#define NUMLINES 2

#define ID_FILECLASS			0xFA08	// Unique ID for this app
#define ID_ITEMFILECLASS		0xAC01	// Unique ID for this app items files
#define ID_SAVEFILEVERSION		0x000D	// current file version
#define ID_SAVEFILEVERSION_2	0x000C	// older-er file version
#define ID_SAVEFILEVERSION_1	0x000B	// older-ist file version

/////////////////////////////////////////////////////////////////////////////
// CUSIMApp:
// See USIM.cpp for the implementation of this class
//

class CUSIMApp : public CWinApp
{
	friend class CDefaultTab;
public:
	CUSIMApp();
	void StartSimConnectThread();
	void StopSimConnectThread();
	void StartOutputThread();
	void StopOutputThread();
	void StartInputThread();
	void StopInputThread();
	void StartSendKeysThread();
	void StopSendKeysThread();
	void StartUDPThread();
	void StopUDPThread();
	void StartKeyCheckThread();
	void StopKeyCheckThread();
	void StopAllThreads();
	void StartAllThreads();

// Global Variables
public:
	DeviceParam		m_DeviceList[MAX_DEVICES];
	DeviceStatus	m_DeviceStatus[MAX_DEVICES];
	UINT			m_CurDevIndex, m_CurItemIndex, m_CurPortIndex, m_CopyItemClass;
	CDefTabCtrl		*m_pDefTabCtrl;
	COutputTabCtrl	*m_pOutputTabCtrl;
	CMuxTabCtrl		*m_pMuxTabCtrl;
	CDispTabCtrl	*m_pDispTabCtrl;
	CRWRTabCtrl		*m_pRWRTabCtrl;
	CCLCDTabCtrl	*m_pCLCDTabCtrl;
	CGLCDTabCtrl	*m_pGLCDTabCtrl;
	CInputsTabCtrl	*m_pInputsTabCtrl;
	CLatchedTabCtrl	*m_pLatchedTabCtrl;
	CSPITabCtrl		*m_pSPITabCtrl;
	CDotMXTabCtrl	*m_pDotMXTabCtrl;
	CSTEPTabCtrl	*m_pSTEPTabCtrl;

	CTreeDevices	*m_pTreeDevices;
	CSaveDevice		*m_pDevArray[MAX_DEVICES];
	CSaveDevice		m_DevTemp;
	CList <int,int>	m_DevArrayList;
	CSaveUserVars	m_UserVarsArray;

	UINT			m_DevCnt;
	HTREEITEM		m_CurrenthItem;
	UINT			m_DeviceType[MAX_DEVICES];
	BOOL			m_HideItems, m_HidePorts, m_HideModes;
	CString			m_AppPath, m_AppFilePath, m_FileName, m_FilePath;

	CF4SharedMem	m_F4SharedMem;
	F4FlightData	m_F4FlightData;
	double			*m_pF4VarToken[ArraySize(F4DataArray)];
	BMSFlightData	*m_pBMSFlightData;
	BMSFlightData2	*m_pBMSFlightData2;

	CXPSharedMem	m_XPSharedMem;
	XPFlightData	*m_pXPFlightData;

	CSimBinSharedMem	m_SimBinSharedMem;
	SimBinSimData	*m_pSimBinSimData;
	SimBinSimData	m_SimBinSimData;
	double			*m_pSimBinVarToken[ArraySize(SimBinDataArray)];

	CRFSharedMem	m_RFSharedMem;
	RFSimData		*m_pRFSimData;
	RFSimData		m_RFSimData;
	double			*m_pRFVarToken[ArraySize(RFDataArray)];

	CGPXSharedMem	m_GPXSharedMem;
	GPXSimData		*m_pGPXSimData;
	GPXSimData		m_GPXSimData;
	double			*m_pGPXVarToken[ArraySize(GPXDataArray)];

	IL2FlightData	m_IL2FlightData;
	CIL2_UDPThread	m_IL2_UDPThread;

	LFSSimData		m_LFSSimData;
	double			*m_pLFSVarToken[ArraySize(LFSDataArray)];
	CLFS_UDPThread	m_LFS_UDPThread;
	
	BOOL			m_WarningFlag;
	UCHAR			m_SimulationModel;
	BOOL			m_AutoConnect, m_Visible, m_Disable;
	NUMBERFMT		m_nFormat;
	COutputThread		m_OutputThread;
	CInputThread		m_InputThread;
	CSendKeysThread		m_SendKeysThread;
	CSimConnectThread	m_SimConnectThread;
	
	CKeyCheckThread		m_KeyCheckThread;

	_SIM_LIST		SimList[ArraySize(SimTypes)];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSIMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUSIMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

UINT SearchIndex(CString, UINT, UINT);
double VarTypeConversion(int, double *);

extern CUSIMApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USIM_H__064A8B71_FFA8_4667_A613_AA9B7B857B13__INCLUDED_)
