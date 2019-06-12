// USIMDlg.h : header file
//

#if !defined(AFX_USIMDLG_H__C15189E8_E22D_46B2_9D5F_DCFD27FF847D__INCLUDED_)
#define AFX_USIMDLG_H__C15189E8_E22D_46B2_9D5F_DCFD27FF847D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputTabCtrl.h"
#include "MuxTabCtrl.h"
#include "DispTabCtrl.h"
#include "OutputTab.h"
#include "TreeDevices.h"
#include "DefTabCtrl.h"
#include "RWRTabCtrl.h"
#include "CLCDTabCtrl.h"
#include "GLCDTabCtrl.h"
#include "InputsTabCtrl.h"
#include "LatchedTabCtrl.h"
#include "SPITabCtrl.h"
#include "STEPTabCtrl.h"
#include "DotMXTabCtrl.h"
#include "F4DataOut.h"
//#include "ResizableLib\ResizableDialog.h"
#include "xySplitterWnd\xySplitterWnd.h"
#include "TrayDialog\TrayDialog.h"
#include "MyStatusBar.h"

static UINT BASED_CODE indicators[] =
{
    ID_INDICATOR_1,
    ID_INDICATOR_2
};

#define WM_TRAY_MESSAGE (WM_APP + 1)

#define RIGHTPANE_MINX	500
#define DIALOG_MINX		800
#define DIALOG_MINY		600

/////////////////////////////////////////////////////////////////////////////
// CUSIMDlg dialog

class CUSIMDlg : public CTrayDialog //CTrayDialog
{
// Construction
public:
	CUSIMDlg(CWnd* pParent = NULL);	// standard constructor
	void OnLoadFile();
	void OnFileSave();
	long OnSplitterMoved(UINT, long);
	void SetupMinimizeToTray();
	void StartTreeThread() {m_nTimer = SetTimer(1, 100, 0);};
	void StopTreeThread() {KillTimer(m_nTimer);};

	NOTIFYICONDATA	m_TrayData;
	CNewMenu		m_menu;
	CxSplitterWnd	m_xSplitter;
	CTreeDevices	m_TreeDevices;
	CMyStatusBar	m_bar;

// Dialog Data
	//{{AFX_DATA(CUSIMDlg)
	enum { IDD = IDD_USIM_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSIMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CDefTabCtrl		m_DefTabCtrl;
	COutputTabCtrl	m_OutputTabCtrl;
	CMuxTabCtrl		m_MuxTabCtrl;
	CDispTabCtrl	m_DispTabCtrl;
	CRWRTabCtrl		m_RWRTabCtrl;
	CCLCDTabCtrl	m_CLCDTabCtrl;
	CGLCDTabCtrl	m_GLCDTabCtrl;
	CInputsTabCtrl	m_InputsTabCtrl;
	CLatchedTabCtrl	m_LatchedTabCtrl;
	CSPITabCtrl		m_SPITabCtrl;
	CSTEPTabCtrl	m_STEPTabCtrl;
	CDotMXTabCtrl	m_DotMXTabCtrl;

	CF4DataOut		m_F4ShowData;
	CVarListDlg		m_ShowData;

	int				m_nTimer;
	CTreeThread		m_TreeThread;

	// Generated message map functions
	//{{AFX_MSG(CUSIMDlg)
    virtual void OnOK();
    virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnAboutbox();
	afx_msg void OnAppExit();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUpdate();
	afx_msg void OnUpdateUpdate(CCmdUI* pCmdUI);
	afx_msg void OnLoad();
	afx_msg void OnSave();
	afx_msg void OnSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileLoad(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnF4Showdata();
	afx_msg void OnF4UpdateShowdata(CCmdUI* pCmdUI);
	afx_msg void OnShowdata();
	afx_msg void OnUpdateShowdata(CCmdUI* pCmdUI);
	afx_msg void OnAutoconnect();
	afx_msg void OnUpdateAutoconnect(CCmdUI* pCmdUI);
	afx_msg void OnDisable();
	afx_msg void OnUpdateDisable(CCmdUI* pCmdUI);
	afx_msg void OnRestore();
	afx_msg void OnUpdateKBSconnect(CCmdUI* pCmdUI);
	afx_msg void OnHelpFinder();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

static const GUID GUID_DEVINTERFACE_LIST[] = 
{
	// GUID_DEVINTERFACE_USB_DEVICE
	{ 0x4d1e55b2, 0xf16f, 0x11cf, { 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } } // Human Interface Device (HID)
//	{ 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } }	// USB Raw Device
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USIMDLG_H__C15189E8_E22D_46B2_9D5F_DCFD27FF847D__INCLUDED_)
