// SaveDevice.h: interface for the CSaveDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEDEVICE_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_)
#define AFX_SAVEDEVICE_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputPage.h"
#include "DisplayPage.h"
#include "RwrPage.h"
#include "CLCDPage.h"
#include "SPIPage.h"
#include "STEPPage.h"
#include "DotMXPage.h"
#include "FlagsPage.h"
#include "NotesPage.h"
#include "SendKeysPage.h"
#include "VariablesPage.h"
//#include "InputFlagPage.h"

class CSaveDevice : public CObject
{
public:
	CSaveDevice();
	virtual ~CSaveDevice();

	int m_nData;
	int m_nIndex;

	virtual void Serialize(CArchive& ar);
	virtual void SerializeItem(CArchive &ar, UINT ItemType, UINT iIndex, UINT pIndex);
	void CopyDeviceParam(DeviceParam *DevParam);
	void CopyDeviceStatus(DeviceStatus *DevStatus);
	void CopyDeviceData(CSaveDevice *m_pDevArray);
	void CopyArrayData(CSaveDevice *m_pDevArray);
	void ClearDeviceData();
	void ClearItemData(UINT, UINT);
	BOOL IsSet();

	void CopyItemData(CSaveDevice *pDevArray, UINT ItemType, UINT ItemIndex);
	void PasteItemData(CSaveDevice *pDevArray, UINT ItemType, UINT ItemIndex);

public:
	static const UINT m_uiItemFileClass;
	static const UINT m_uiFileVersion;

	BOOL			m_Local;
	BOOL			m_Connected;
	BOOL			m_FreeAgent;
	HTREEITEM		m_hItem;
	HTREEITEM		m_hPort;
	DeviceParam		m_DeviceParam;
	DeviceStatus	m_DeviceStatus;	

	BOOL			m_PortChange[MAX_PORTS];
	CString			m_PortLabel[MAX_PORTS];

	BYTE			m_Inputs[MAX_INPUTS];
	BYTE			*m_pUserVar[MAX_INPUTS];
	BYTE			m_InputsOld[MAX_INPUTS];
	BYTE			m_InputsSKOld[MAX_INPUTS];
	BYTE			m_InputToggled[MAX_INPUTS];
	HTREEITEM		m_InputhItem[MAX_INPUTS];
	HTREEITEM		m_InputhPItem[MAX_INPUTS];
	BOOL			m_InputActive[MAX_INPUTS];		// hardware state
	BOOL			m_InputDisabled[MAX_INPUTS];	// software state
	CNotesPage		m_NotesArray[MAX_INPUTS];
	CSendKeysPage	m_SendKeysArray[MAX_INPUTS];
	CVariablesPage	m_VariablesArray[MAX_INPUTS];
	CNotesPage		m_Notes;
	CSendKeysPage	m_SendKeys;
	CVariablesPage	m_Variables;
//	CCondPage		m_CondArray[MAX_INPUTS];

	HTREEITEM		m_hOutput[MAX_PORTS];
	DIRECT_OUTPUT	m_DirectOutputs[MAX_OUTPUTS];
	BYTE			m_Outputs[MAX_OUTPUTS];
	BYTE			m_OutputsOld[MAX_OUTPUTS];
	HTREEITEM		m_OutputhItem[MAX_OUTPUTS];
	BOOL			m_OutputActive[MAX_OUTPUTS];	// hardware state
	BOOL			m_OutputDisabled[MAX_OUTPUTS];	// software state
	CNotesPage		m_OutputNotesArray[MAX_OUTPUTS];
	COutputPage		m_OutputArray[MAX_OUTPUTS];
	CFlagsPage		m_OutputFlagsArray[MAX_OUTPUTS];
	CNotesPage		m_OutputNotes;
	COutputPage		m_Output;
	CFlagsPage		m_OutputFlags;

	DIRECT_OUTPUT	m_MuxOutputs[MAX_MUXDISPLAYS];
	BYTE			m_Mux[MAX_MUXDISPLAYS];
	BYTE			m_MuxOld[MAX_MUXDISPLAYS];
	HTREEITEM		m_MuxhItem[MAX_MUXDISPLAYS];
	BOOL			m_MuxActive[MAX_MUXDISPLAYS];	// hardware state
	BOOL			m_MuxDisabled[MAX_MUXDISPLAYS];	// software state
	CNotesPage		m_MuxNotesArray[MAX_MUXDISPLAYS];
	COutputPage		m_MuxArray[MAX_MUXDISPLAYS];
	CFlagsPage		m_MuxFlagsArray[MAX_MUXDISPLAYS];
	CNotesPage		m_MuxNotes;
	COutputPage		m_MuxItem;
	CFlagsPage		m_MuxFlags;

	BYTE			m_DispLevel[MAX_PORTS];
	CString			m_StrDispOld[MAX_PORTS];
	CString			m_StrDisp[MAX_PORTS];
	HTREEITEM		m_DisphItem[MAX_PORTS];
	BOOL			m_DispActive[MAX_PORTS];		// hardware state
	BOOL			m_DispDisabled[MAX_PORTS];		// software state
	CNotesPage		m_DispNotesArray[MAX_PORTS];
	CDisplayPage	m_DisplayArray[MAX_PORTS];
	CFlagsPage		m_DisplayFlagsArray[MAX_PORTS];
	CNotesPage		m_DispNotes;
	CDisplayPage	m_Display;
	CFlagsPage		m_DisplayFlags;

	CRTRWR_OUTPUT	m_CrtRwrOutputs[MAX_RWR_CONTACTS];
	CRTRWR_OUTPUT	m_CrtRwrOld[MAX_RWR_CONTACTS];
	UCHAR			m_RWRSymbolCnt;
	UCHAR			m_RWRSymbolCntOld;
	HTREEITEM		m_CrtRwrhItem[MAX_PORTS];
	BOOL			m_CrtRwrActive[MAX_PORTS];		// hardware state
	BOOL			m_CrtRwrDisabled[MAX_PORTS];	// software state
	CNotesPage		m_CrtRwrNotesArray[MAX_PORTS];
	CRwrPage		m_CrtRwrArray[MAX_PORTS];
	CFlagsPage		m_CrtRwrFlagsArray[MAX_PORTS];
	CNotesPage		m_CrtRwrNotes;
	CRwrPage		m_CrtRwr;
	CFlagsPage		m_CrtRwrFlags;

	HTREEITEM		m_CLCDhItem[MAX_PORTS];
	BOOL			m_CLCDActive[MAX_PORTS];		// hardware state
	BOOL			m_CLCDDisabled[MAX_PORTS];		// software state
	CNotesPage		m_CLCDNotesArray[MAX_PORTS];
	CFlagsPage		m_CLCDFlagsArray[MAX_PORTS];
	CCLCDPage		m_CLCDArray[MAX_PORTS];
	CNotesPage		m_CLCDNotes;
	CFlagsPage		m_CLCDFlags;
	CCLCDPage		m_CLCD;

	HTREEITEM		m_GLCDhItem[MAX_PORTS];
	BOOL			m_GLCDActive[MAX_PORTS];		// hardware state
	BOOL			m_GLCDDisabled[MAX_PORTS];		// software state
	CNotesPage		m_GLCDNotesArray[MAX_PORTS];
	CFlagsPage		m_GLCDFlagsArray[MAX_PORTS];
	CCLCDPage		m_GLCDArray[MAX_PORTS];
	CNotesPage		m_GLCDNotes;
	CFlagsPage		m_GLCDFlags;
	CCLCDPage		m_GLCD;

	HTREEITEM		m_hLO[MAX_PORTS];
	BYTE			m_LOLevel[MAX_PORTS];
	BYTE			m_LOLevelOld[MAX_PORTS];
	BYTE			m_LO[MAX_LATCHED];
	BYTE			m_LOOld[MAX_LATCHED];
	HTREEITEM		m_LOhItem[MAX_LATCHED];
	BOOL			m_LOActive[MAX_LATCHED];		// hardware state
	BOOL			m_LODisabled[MAX_LATCHED];		// software state
	CNotesPage		m_LONotesArray[MAX_LATCHED];
	COutputPage		m_LOArray[MAX_LATCHED];
	CFlagsPage		m_LOFlagsArray[MAX_LATCHED];
	CNotesPage		m_LONotes;
	COutputPage		m_LOItem;
	CFlagsPage		m_LOFlags;

	HTREEITEM		m_hSPI[MAX_PORTS];
	BYTE			m_SPILevel[MAX_PORTS];
	BYTE			m_SPILevelOld[MAX_PORTS];
	ULONG			m_SPI[MAX_SPI];
	ULONG			m_SPIOld[MAX_SPI];
	HTREEITEM		m_SPIhItem[MAX_SPI];
	BOOL			m_SPIActive[MAX_SPI];			// hardware state
	BOOL			m_SPIDisabled[MAX_SPI];			// software state
	CNotesPage		m_SPINotesArray[MAX_SPI];
	CSPIPage		m_SPIArray[MAX_SPI];
	CFlagsPage		m_SPIFlagsArray[MAX_SPI];
	CNotesPage		m_SPINotes;
	CSPIPage		m_SPIItem;
	CFlagsPage		m_SPIFlags;

	HTREEITEM		m_hDotMX[MAX_PORTS];
	BYTE			m_DotMXLevel[MAX_PORTS];
	BYTE			m_DotMXLevelOld[MAX_PORTS];
	CString			m_StrDotMXOld[MAX_DOTMATRIX];
	CString			m_StrDotMX[MAX_DOTMATRIX];
	HTREEITEM		m_DotMXhItem[MAX_DOTMATRIX];
	BOOL			m_DotMXActive[MAX_DOTMATRIX];		// hardware state
	BOOL			m_DotMXDisabled[MAX_DOTMATRIX];		// software state
	CNotesPage		m_DotMXNotesArray[MAX_DOTMATRIX];
	CDotMXPage		m_DotMXArray[MAX_DOTMATRIX];
	CFlagsPage		m_DotMXFlagsArray[MAX_DOTMATRIX];
	CNotesPage		m_DotMXNotes;
	CDotMXPage		m_DotMX;
	CFlagsPage		m_DotMXFlags;

	HTREEITEM		m_hSTEP[MAX_PORTS];
	long			m_STEP[MAX_STEPPER];
	long			m_STEPOld[MAX_STEPPER];
	HTREEITEM		m_STEPhItem[MAX_STEPPER];
	BOOL			m_STEPActive[MAX_STEPPER];		// hardware state
	BOOL			m_STEPDisabled[MAX_STEPPER];		// software state
	CNotesPage		m_STEPNotesArray[MAX_STEPPER];
	CSTEPPage		m_STEPArray[MAX_STEPPER];
	CFlagsPage		m_STEPFlagsArray[MAX_STEPPER];
	CNotesPage		m_STEPNotes;
	CSTEPPage		m_STEPItem;
	CFlagsPage		m_STEPFlags;

protected:
	DECLARE_SERIAL(CSaveDevice)
};

#endif // !defined(AFX_SAVEDEVICE_H__E352C60E_991D_460C_8E78_914AA569CC74__INCLUDED_)
