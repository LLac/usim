#if !defined(AFX_TREEDEVICES_H__89BADF3A_E28F_4AF1_BC6C_208106E5BD3D__INCLUDED_)
#define AFX_TREEDEVICES_H__89BADF3A_E28F_4AF1_BC6C_208106E5BD3D__INCLUDED_

#include "F4VarList.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeDevices.h : header file
//
#include "ColorTreeCtrl\TreeCtrlEx.h"
#include "ReplaceDevice.h"
#include "TreeThread.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeDevices window

class CTreeDevices : public CTreeCtrlEx
{
// Construction
public:
	CTreeDevices();

// Dialog Data
	//{{AFX_DATA(CTreeDevices)
	enum { IDD = IDC_DEVICESTREE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CImageList  m_ImageListDevices;
	HTREEITEM	nLocal, nNetwork, nDevice, nDefault, nUserVars;
	HTREEITEM	m_CurrenthItem;
	int			m_CurrentIndex;
	int			m_CARD, m_NOCARD; 
	int			m_RD_ON, m_RD_OFF, m_RD_ON_P, m_RD_OFF_P;
	int			m_GR_ON, m_GR_OFF, m_GR_OFF_P, m_GR_ON_P;
	int			m_DIS_ON, m_DIS_ON_P, m_DIS_OFF, m_DIS_OFF_P;
	int			m_MUX_DIS, m_MUX_DIS_P, m_MUX, m_MUX_P;
	int			m_RWR_DIS, m_RWR_DIS_P, m_RWR, m_RWR_P;
	int			m_GLCD_DIS, m_GLCD_DIS_P, m_GLCD, m_GLCD_P;
	int			m_CLCD_DIS, m_CLCD_DIS_P, m_CLCD, m_CLCD_P;
	int			m_DACS_DIS, m_DACS_DIS_P, m_DACS, m_DACS_P;
	int			m_DAC_DIS, m_DAC_DIS_P, m_DAC, m_DAC_P;
	int			m_SPIS, m_SPIS_DIS, m_SPI_DIS, m_SPI_DIS_P, m_SPI, m_SPI_P;
	int			m_LO, m_LO_DIS, m_LO_ON, m_LO_OFF, m_LO_ON_P, m_LO_OFF_P;
	int			m_DMXS, m_DMXS_DIS, m_DMX_DIS, m_DMX_DIS_P, m_DMX, m_DMX_P;
	int			m_STEPS, m_STEPS_DIS, m_STEP_DIS, m_STEP_DIS_P, m_STEP, m_STEP_P;

	POSITION	m_ReplacePos;

protected:
	CNewMenu	m_menuDevices;
	UINT		m_ItemState;
	CReplaceDevice	m_ReplaceDevice;
	BOOL		m_ReEntryFlag;
// Attributes
public:

// Operations
public:
	void ExpandItem(HTREEITEM hti = TVI_ROOT, UINT nCode = TVE_EXPAND);
	void DeactivateItem(HTREEITEM hti, UINT nCode);
	void OnRefreshList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeDevices)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeDevices();
	void	OnSelect(HTREEITEM);
	void	EnumDevices();
	void	DetectDevices(BOOL);
	void	OnInit();
	void    SetNewStyle(long lStyleMask, BOOL bSetBits);
	BOOL    TransferItem(HTREEITEM hitem, HTREEITEM hNewParent);
	BOOL    IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);
	void	HideAllTabs();
	void	StopTests();
	void	OnRefresh();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeDevices)
	afx_msg void OnBeginLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnExpand();
	afx_msg void OnCollapse();
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClearDevice();
	afx_msg void OnClearItem();
	afx_msg void OnCopyItem();
	afx_msg void OnPasteItem();
	afx_msg void OnExportItem();
	afx_msg void OnImportItem();
	afx_msg void OnDisable();
	afx_msg void OnHideItems();
	afx_msg void OnHideModes();
	afx_msg void OnHidePorts();
	afx_msg void OnReplace();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetDeviceTree(HTREEITEM, int, int);
	CString	m_ItemTypeString;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEDEVICES_H__89BADF3A_E28F_4AF1_BC6C_208106E5BD3D__INCLUDED_)
