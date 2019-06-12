#if !defined(AFX_SIMPATHTAB_H__83C66988_66ED_4A09_B06C_ADF3A7C7E584__INCLUDED_)
#define AFX_SIMPATHTAB_H__83C66988_66ED_4A09_B06C_ADF3A7C7E584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimPathTab.h : header file
//

#include "GridCtrl_src\GridCtrl.h"
#include "ResizableLib\ResizableDialog.h"

enum SIMTYPES
{
	SIM_NONE,	// 0
	SIM_TEST,	// 1
	SIM_KBS,	// 2
	SIM_XPLANE,	// 3
	SIM_FS9,	// 4
	SIM_FSX,	// 5
	SIM_IL2,	// 6
	SIM_IL2FB,	// 7
	SIM_IL2PF,	// 8
	SIM_F4BMS,	// 9
	SIM_F4AF,	// 10
	SIM_F4FF,	// 11
	SIM_F4OF,	// 12
	SIM_F4USIM,	// 13
	SIM_GTR,	// 14
	SIM_GTR2,	// 15
	SIM_EVO,	// 16
	SIM_GTL,	// 17
	SIM_RACE,	// 18
	SIM_RACE07,	// 19
	SIM_SIMBIN,	// 20
	SIM_RF,		// 21
	SIM_LFS,	// 22
	SIM_GPX,	// 23
	SIM_RACEON,	// 24
	SIM_VOLVO,	// 25
	USIM_INPUT_FLAGS	// 26
};

struct _SIM_LIST
{
	BOOL		Enable;
	CString		SimName;
	int			SimType;
	CString		SimPath;
	CString		SimFileName;
	CString		USCFile;
	UINT		Port;
};

struct _CSIM_LIST
{
	LPTSTR		SimName;
	LPTSTR		SimExe;
	int			SimType;
	HKEY		RegSection;
	LPTSTR		RegKey;
	LPTSTR		RegValue;
};

_CSIM_LIST const SimTypes[] = {
	{"X-Plane",			"X-Plane.exe",	SIM_XPLANE, NULL, "", ""},
	{"MS FS9",			"fs9.exe",		SIM_FS9,	NULL, "", ""},
	{"MS FSX",			"fsx.exe",		SIM_FSX,	NULL, "", ""},
	{"Falcon 4 BMS",	"F4-BMS.exe",	SIM_F4BMS,	HKEY_CURRENT_USER, "SOFTWARE\\F4Patch\\Settings", "F4Exe"},
	{"Falcon 4 AF",		"FalconAF.exe",	SIM_F4AF,	HKEY_LOCAL_MACHINE, "SOFTWARE\\Lead Pursuit\\Battlefield Operations\\Falcon", "baseDir"},
	{"Falcon 4 OF",		"F4-BMS.exe",	SIM_F4OF,	HKEY_LOCAL_MACHINE, "SOFTWARE\\MicroProse\\Falcon\\4.0", "baseDir"},
	{"Falcon 4 FF",		"",				SIM_F4FF,	NULL, "", ""},
	{"IL-2",			"il2.exe",		SIM_IL2,	HKEY_LOCAL_MACHINE, "SOFTWARE\\Ubi Soft\\IL-2 Sturmovik 1946", "Directory"},
	{"IL-2 FB",			"il2fb.exe",	SIM_IL2FB,	HKEY_LOCAL_MACHINE, "SOFTWARE\\Ubi Soft\\IL-2 Sturmovik Forgotten Battles", "directory"},
	{"Pacific Fighters","pf.exe",		SIM_IL2PF,	HKEY_LOCAL_MACHINE, "SOFTWARE\\Ubi Soft\\Pacific Fighters", "directory"},
	{"GTR",				"gtr.exe",		SIM_GTR,	NULL, "", ""},
	{"GTR2",			"gtr2.exe",		SIM_GTR2,	NULL, "", ""},
	{"GTR EVO",			"gtrevo.exe",	SIM_EVO,	NULL, "", ""},
	{"GTL",				"gtl.exe",		SIM_GTL,	NULL, "", ""},
	{"RACE",			"race.exe",		SIM_RACE,	NULL, "", ""},
	{"RACE07",			"race07.exe",	SIM_RACE07,	NULL, "", ""},
	{"RACEON",			"raceon.exe",	SIM_RACEON,	NULL, "", ""},
	{"VOLVO",			"volvo.exe",	SIM_VOLVO,	NULL, "", ""},
	{"rFactor",			"rFactor.exe",	SIM_RF,		HKEY_LOCAL_MACHINE, "SOFTWARE\\rFactor", "InstallPath"},
	{"LFS",				"lfs.exe",		SIM_LFS,	NULL, "", ""},
	{"GPX",				"gp4.exe",		SIM_GPX,	NULL, "", ""},
	{"User Application 1",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 2",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 3",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 4",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 5",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 6",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 7",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 8",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 9",			"",				SIM_KBS,	NULL, "", ""},
	{"User Application 10",			"",				SIM_KBS,	NULL, "", ""}
};

#define SIM_NUM_ROWS	ArraySize(SimTypes) + 1
#define SIM_NUM_COLS	7

/////////////////////////////////////////////////////////////////////////////
// CSimPathTab dialog
class CXTabCtrl;

class CSimPathTab : public CResizableDialog
{
	friend class CTreeDevices;

// Construction
public:
	CSimPathTab(CWnd* pParent = NULL);   // standard constructor

	void GridDefaults();

	CDefTabCtrl*	m_pTab;

// Global Variables
public:

// Dialog Data
	//{{AFX_DATA(CSimPathTab)
	enum { IDD = IDD_SIMPATH };
	//}}AFX_DATA

private:
	CGridCtrl	m_Grid;
	CImageList	m_ImageList;
	int			m_CurrentRow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimPathTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CSimPathTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnClearitem();
	afx_msg void OnClearall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPATHTAB_H__83C66988_66ED_4A09_B06C_ADF3A7C7E584__INCLUDED_)

