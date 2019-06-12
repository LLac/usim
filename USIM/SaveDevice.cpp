// SaveDevice.cpp: implementation of the CSaveDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "SaveDevice.h"
//#include "XYDataArray\XYDataArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const UINT CSaveDevice::m_uiItemFileClass = ID_ITEMFILECLASS;
const UINT CSaveDevice::m_uiFileVersion = ID_SAVEFILEVERSION;

IMPLEMENT_SERIAL( CSaveDevice, CObject, CSaveDevice::m_uiFileVersion);

CSaveDevice::CSaveDevice()
{
	m_nData = 0;
	m_nIndex = 0;

	m_Local = TRUE;
	m_Connected = FALSE;
	m_FreeAgent = FALSE;

	// init ports
	int i;
	for (i=0; i<MAX_PORTS; i++) {
		m_PortChange[i] = true;		// force device update to sync IOs
		m_PortLabel[i].Empty();
	}

	// init inputs
	memset(m_Inputs, 0, MAX_INPUTS);
	memset(m_InputsOld, 0, MAX_INPUTS);
	memset(m_InputsSKOld, 0, MAX_INPUTS);

	for (i=0; i<MAX_INPUTS; i++) {
		m_NotesArray[i].Clear();
		m_SendKeysArray[i].Clear();
		m_VariablesArray[i].Clear();

		m_InputActive[i] = true;
		m_InputDisabled[i] = false;
		m_InputToggled[i] = false;
	}

	// init direct outputs
	for (i=0; i<MAX_OUTPUTS; i++) {
		m_DirectOutputs[i].state = 0;
		m_DirectOutputs[i].level = 0;
	}

	memset(m_Outputs, 0, MAX_OUTPUTS);
	memset(m_OutputsOld, 0, MAX_OUTPUTS);

	for (i=0; i<MAX_OUTPUTS; i++) {
		m_OutputActive[i] = true;
		m_OutputDisabled[i] = false;
	}

	// init mux displays	
	for (i=0; i<MAX_MUXDISPLAYS; i++) {
		m_MuxOutputs[i].state = 0;
		m_MuxOutputs[i].level = 0;
	}

	memset(m_Mux, 0, MAX_MUXDISPLAYS);
	memset(m_MuxOld, 0, MAX_MUXDISPLAYS);

	for (i=0; i<MAX_MUXDISPLAYS; i++) {
		m_MuxActive[i] = true;
		m_MuxDisabled[i] = false;
	}

	// init 7-seg displays	
	for (i=0; i<MAX_PORTS; i++) {
		m_DispLevel[i] = 0;
		m_StrDisp[i] = "";
		m_StrDispOld[i] = "";
	}

	for (i=0; i<MAX_PORTS; i++) {
		m_DispActive[i] = true;
		m_DispDisabled[i] = false;
	}

	// init crt rwr outputs
	m_RWRSymbolCnt = 0;
	m_RWRSymbolCntOld = 0;
	for (i=0; i<MAX_RWR_CONTACTS; i++) {
		m_CrtRwrOutputs[i].symbol = 0;
		m_CrtRwrOutputs[i].xPos = 0;
		m_CrtRwrOutputs[i].yPos = 0;
		m_CrtRwrOutputs[i].missileActivity = 0;
		m_CrtRwrOutputs[i].missileLaunch = 0;
		m_CrtRwrOutputs[i].newguy = 0;
		m_CrtRwrOutputs[i].selected = 0;

		m_CrtRwrOld[i].symbol = 0;
		m_CrtRwrOld[i].xPos = 0;
		m_CrtRwrOld[i].yPos = 0;
		m_CrtRwrOld[i].missileActivity = 0;
		m_CrtRwrOld[i].missileLaunch = 0;
		m_CrtRwrOld[i].newguy = 0;
		m_CrtRwrOld[i].selected = 0;
	}

	for (i=0; i<MAX_PORTS; i++) {
		m_CrtRwrActive[i] = true;
		m_CrtRwrDisabled[i] = false;
	}

	for (i=0; i<MAX_PORTS; i++) {
		m_CLCDActive[i] = true;
		m_CLCDDisabled[i] = false;
	}

	for (i=0; i<MAX_PORTS; i++) {
		m_GLCDActive[i] = true;
		m_GLCDDisabled[i] = false;
	}

	for (i=0; i<MAX_PORTS; i++)
		m_SPILevel[i] = 7;

	for (i=0; i<MAX_SPI; i++) {
		m_SPI[i] = 0;
		m_SPIOld[i] = 0;
		m_SPIActive[i] = true;
		m_SPIDisabled[i] = false;
	}

	memset(m_LO, 0, MAX_LATCHED);
	memset(m_LOOld, 0, MAX_LATCHED);
	memset(m_LOLevel, 10, MAX_PORTS);
	memset(m_LOLevelOld, 10, MAX_PORTS);

	for (i=0; i<MAX_LATCHED; i++) {
		m_LOActive[i] = true;
		m_LODisabled[i] = false;
	}

	// init Dot Matrix displays	
	for (i=0; i<MAX_PORTS; i++)
		m_DotMXLevel[i] = 10;

	for (i=0; i<MAX_DOTMATRIX; i++) {
		m_StrDotMX[i] = "    ";
		m_StrDotMXOld[i] = "    ";
	}

	for (i=0; i<MAX_DOTMATRIX; i++) {
		m_DotMXActive[i] = true;
		m_DotMXDisabled[i] = false;
	}

	for (i=0; i<MAX_STEPPER; i++) {
		m_STEP[i] = 0;
		m_STEPOld[i] = 0;
		m_STEPActive[i] = true;
		m_STEPDisabled[i] = false;
	}

	m_DeviceParam.ConfigFlag = 0;
	memset(m_DeviceParam.DeviceName, atoi(" "), MAX_CHAR);
	m_DeviceParam.DeviceNameLength = 0;
	memset(m_DeviceParam.DevicePath, atoi(" "), MAX_CHAR);
	m_DeviceParam.PathLength = 0;
	m_DeviceParam.DevIndex = 0;
	memset(m_DeviceParam.ManufName, atoi(" "), MAX_CHAR);
	m_DeviceParam.ManufNameLength = 0;
	m_DeviceParam.NumberInputIndices = 0;
	m_DeviceParam.NumberOutputIndices = 0;
	m_DeviceParam.NumberPortIndices = 0;
	m_DeviceParam.NumberAnalogIndices = 0;
	m_DeviceParam.NumberHATIndices = 0;
	m_DeviceParam.NumberRotaryIndices = 0;
	m_DeviceParam.NumberPWMIndices = 0;
	m_DeviceParam.NumberLCDIndices = 0;
	m_DeviceParam.NumberGLCDIndices = 0;
	m_DeviceParam.NumberCRTRWRIndices = 0;
	m_DeviceParam.NumberMuxDisplayIndices = 0;
	m_DeviceParam.NumberAlphanumericIndices = 0;
	m_DeviceParam.NumberSPIIndices = 0;
	m_DeviceParam.NumberLatchedIndices = 0;
	m_DeviceParam.NumberDotMatrixIndices = 0;
	m_DeviceParam.NumberStepperIndices = 0;
	m_DeviceParam.ProductID = 0;
	m_DeviceParam.DeviceHandle = NULL;
	memset(m_DeviceParam.SerialNum, atoi(" "), 3);
	m_DeviceParam.SerialNum[3] = NULL;
	m_DeviceParam.SerialNumLength = 3;
	m_DeviceParam.VendorID = 0;
	m_DeviceParam.VersionNumber = 0;
	m_DeviceParam.FlashVersion = 0;
	m_DeviceParam.ProgramFlag = 0;
	m_DeviceParam.InputReportByteLength = 0;
	m_DeviceParam.OutputReportByteLength = 0;
	m_DeviceParam.Usage = 0;
	m_DeviceParam.UsagePage = 0;

	memset(m_DeviceStatus.InputActive, 0, MAX_INPUTS);
	memset(m_DeviceStatus.OutputActive, 0, MAX_OUTPUTS);
	memset(m_DeviceStatus.PortActive, 0, MAX_PORTS);
	memset(m_DeviceStatus.PortIOMode, 1,  MAX_PORTS);
	memset(m_DeviceStatus.PortMode, 0, MAX_PORTS);
	memset(m_DeviceStatus.OutputMode, 0, MAX_PORTS);
	memset(m_DeviceStatus.AnalogActive, 0, MAX_ANALOG);
	memset(m_DeviceStatus.HATActive, 0, MAX_HATS);
	memset(m_DeviceStatus.RotaryActive, 0, MAX_ROTARY);
	memset(m_DeviceStatus.CRTRWRActive, 0, MAX_PORTS);
	memset(m_DeviceStatus.PWMActive, 0, MAX_PWM);
	memset(m_DeviceStatus.LCDActive, 0, MAX_LCDS);
	memset(m_DeviceStatus.GLCDActive, 0, MAX_GLCDS);
	memset(m_DeviceStatus.MuxDisplayActive, 0, MAX_MUXDISPLAYS);
	memset(m_DeviceStatus.AlphanumericActive, 0, MAX_ALPHANUMERICS);
	memset(m_DeviceStatus.SPIActive, 0, MAX_SPI);
	memset(m_DeviceStatus.LatchedActive, 0, MAX_LATCHED);
	memset(m_DeviceStatus.DotMatrixActive, 0, MAX_DOTMATRIX);
	memset(m_DeviceStatus.StepperActive, 0, MAX_STEPPER);
}

CSaveDevice::~CSaveDevice()
{

}

void CSaveDevice::Serialize(CArchive &ar)
{	// MSDN says we ne need to call ancestor's serialize
	CObject::Serialize( ar );

	CString str;
	UINT uiFileVersion;
	int i;

	// standard procedure when serializing
	if ( ar.IsStoring() )
	{	// store the rest of our data
		ar << m_uiFileVersion;
		ar << m_Local;
		ar << m_DeviceParam.ConfigFlag;
		
		str = m_DeviceParam.DeviceName;
		ar << str;
		ar << m_DeviceParam.DeviceNameLength;

		str = m_DeviceParam.DevicePath;
		ar << str;
		ar << m_DeviceParam.PathLength;

		str = m_DeviceParam.ManufName;
		ar << str;
		ar << m_DeviceParam.ManufNameLength;

		ar << m_DeviceParam.NumberInputIndices;
		ar << m_DeviceParam.NumberOutputIndices;
		ar << m_DeviceParam.NumberPortIndices;
		ar << m_DeviceParam.NumberAnalogIndices;
		ar << m_DeviceParam.NumberHATIndices;
		ar << m_DeviceParam.NumberRotaryIndices;
		ar << m_DeviceParam.NumberPWMIndices;
		ar << m_DeviceParam.NumberLCDIndices;
		ar << m_DeviceParam.NumberGLCDIndices;
		ar << m_DeviceParam.NumberCRTRWRIndices;
		ar << m_DeviceParam.NumberMuxDisplayIndices;
		ar << m_DeviceParam.NumberAlphanumericIndices;
		ar << m_DeviceParam.NumberDotMatrixIndices;
		ar << m_DeviceParam.NumberLatchedIndices;
		ar << m_DeviceParam.NumberSPIIndices;
		ar << m_DeviceParam.NumberStepperIndices;

		ar << m_DeviceParam.ProductID;
		for (i=0; i < 3; i++) ar << m_DeviceParam.SerialNum[i];
		ar << m_DeviceParam.SerialNumLength;
		ar << m_DeviceParam.VendorID;
		ar << m_DeviceParam.VersionNumber;

		for (i = 0; i < m_DeviceParam.NumberInputIndices; i++) {
			ar << m_InputDisabled[i];
			m_NotesArray[i].Serialize(ar);
			m_SendKeysArray[i].Serialize(ar);
			m_VariablesArray[i].Serialize(ar, NULL);
		}

		for (i = 0; i < m_DeviceParam.NumberOutputIndices; i++) {
			ar << m_OutputDisabled[i];
			m_OutputNotesArray[i].Serialize(ar);
			m_OutputFlagsArray[i].Serialize(ar);
			m_OutputArray[i].Serialize(ar, NULL);
		}

		for (i = 0; i < m_DeviceParam.NumberMuxDisplayIndices; i++) {
			ar << m_MuxDisabled[i];
			m_MuxNotesArray[i].Serialize(ar);
			m_MuxFlagsArray[i].Serialize(ar);
			m_MuxArray[i].Serialize(ar, NULL);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_DispDisabled[i];
			m_DispNotesArray[i].Serialize(ar);
			m_DisplayFlagsArray[i].Serialize(ar);
			m_DisplayArray[i].Serialize(ar, NULL);
		}

		// store RWR data
		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_CrtRwrDisabled[i];
			m_CrtRwrNotesArray[i].Serialize(ar);
			m_CrtRwrFlagsArray[i].Serialize(ar);
			m_CrtRwrArray[i].Serialize(ar);
		}

		// store CLCD data
		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_CLCDDisabled[i];
			m_CLCDNotesArray[i].Serialize(ar);
			m_CLCDFlagsArray[i].Serialize(ar);
			m_CLCDArray[i].Serialize(ar);
		}

		// store GLCD data
		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_GLCDDisabled[i];
			m_GLCDNotesArray[i].Serialize(ar);
			m_GLCDFlagsArray[i].Serialize(ar);
			m_GLCDArray[i].Serialize(ar);
		}

		// store Latched data
		for (i = 0; i < m_DeviceParam.NumberLatchedIndices; i++) {
			ar << m_LODisabled[i];
			m_LONotesArray[i].Serialize(ar);
			m_LOFlagsArray[i].Serialize(ar);
			m_LOArray[i].Serialize(ar, NULL);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_LOLevel[i];
		}

		// store SPI data
		for (i = 0; i < m_DeviceParam.NumberSPIIndices; i++) {
			ar << m_SPIDisabled[i];
			m_SPINotesArray[i].Serialize(ar);
			m_SPIFlagsArray[i].Serialize(ar);
			m_SPIArray[i].Serialize(ar, NULL);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_SPILevel[i];
		}

		// store Dot Matrix data
		for (i = 0; i < m_DeviceParam.NumberDotMatrixIndices; i++) {
			ar << m_DotMXDisabled[i];
			m_DotMXNotesArray[i].Serialize(ar);
			m_DotMXFlagsArray[i].Serialize(ar);
			m_DotMXArray[i].Serialize(ar, NULL);
		}

		// store Stepper data
		for (i = 0; i < m_DeviceParam.NumberStepperIndices; i++) {
			ar << m_STEPDisabled[i];
			m_STEPNotesArray[i].Serialize(ar);
			m_STEPFlagsArray[i].Serialize(ar);
			m_STEPArray[i].Serialize(ar, NULL);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_DotMXLevel[i];
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar << m_PortLabel[i];
		}

	} else {
		// we are loading data, so we first need to check
		// if it's the correct version
		ar >> uiFileVersion;

		ar >> m_Local;
		ar >> m_DeviceParam.ConfigFlag;

		ar >> str;
		memcpy(m_DeviceParam.DeviceName, str, (strlen(str) > MAX_CHAR) ? MAX_CHAR : strlen(str));
		ar >> m_DeviceParam.DeviceNameLength;

		ar >> str;
		memcpy(m_DeviceParam.DevicePath, str, (strlen(str) > MAX_CHAR) ? MAX_CHAR : strlen(str));
		ar >> m_DeviceParam.PathLength;

		ar >> str;
		memcpy(m_DeviceParam.ManufName, str, (strlen(str) > MAX_CHAR) ? MAX_CHAR : strlen(str));
		ar >> m_DeviceParam.ManufNameLength;

		ar >> m_DeviceParam.NumberInputIndices;
		ar >> m_DeviceParam.NumberOutputIndices;
		ar >> m_DeviceParam.NumberPortIndices;
		ar >> m_DeviceParam.NumberAnalogIndices;
		ar >> m_DeviceParam.NumberHATIndices;
		ar >> m_DeviceParam.NumberRotaryIndices;
		ar >> m_DeviceParam.NumberPWMIndices;
		ar >> m_DeviceParam.NumberLCDIndices;
		ar >> m_DeviceParam.NumberGLCDIndices;
		ar >> m_DeviceParam.NumberCRTRWRIndices;
		ar >> m_DeviceParam.NumberMuxDisplayIndices;
		ar >> m_DeviceParam.NumberAlphanumericIndices;
		ar >> m_DeviceParam.NumberDotMatrixIndices;
		ar >> m_DeviceParam.NumberLatchedIndices;
		ar >> m_DeviceParam.NumberSPIIndices;
		ar >> m_DeviceParam.NumberStepperIndices;

		ar >> m_DeviceParam.ProductID;
		for (i=0; i < 3; i++) ar >> m_DeviceParam.SerialNum[i];
		ar >> m_DeviceParam.SerialNumLength;
		ar >> m_DeviceParam.VendorID;
		ar >> m_DeviceParam.VersionNumber;

		for (i = 0; i < m_DeviceParam.NumberInputIndices; i++) {
			ar >> m_InputDisabled[i];
			m_NotesArray[i].Serialize(ar);
			m_SendKeysArray[i].Serialize(ar);
			m_VariablesArray[i].Serialize(ar, uiFileVersion);
		}

		for (i = 0; i < m_DeviceParam.NumberOutputIndices; i++) {
			ar >> m_OutputDisabled[i];
			m_OutputNotesArray[i].Serialize(ar);
			m_OutputFlagsArray[i].Serialize(ar);
			m_OutputArray[i].Serialize(ar, uiFileVersion);
		}

		for (i = 0; i < m_DeviceParam.NumberMuxDisplayIndices; i++) {
			ar >> m_MuxDisabled[i];
			m_MuxNotesArray[i].Serialize(ar);
			m_MuxFlagsArray[i].Serialize(ar);
			m_MuxArray[i].Serialize(ar, uiFileVersion);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_DispDisabled[i];
			m_DispNotesArray[i].Serialize(ar);
			m_DisplayFlagsArray[i].Serialize(ar);
			m_DisplayArray[i].Serialize(ar, uiFileVersion);
		}

		// RWR data
		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_CrtRwrDisabled[i];
			m_CrtRwrNotesArray[i].Serialize(ar);
			m_CrtRwrFlagsArray[i].Serialize(ar);
			m_CrtRwrArray[i].Serialize(ar);
		}

		// CLCD data
		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_CLCDDisabled[i];
			m_CLCDNotesArray[i].Serialize(ar);
			m_CLCDFlagsArray[i].Serialize(ar);
			m_CLCDArray[i].Serialize(ar);
		}

		// GLCD data
		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_GLCDDisabled[i];
			m_GLCDNotesArray[i].Serialize(ar);
			m_GLCDFlagsArray[i].Serialize(ar);
			m_GLCDArray[i].Serialize(ar);
		}

		// Latched data
		for (i = 0; i < m_DeviceParam.NumberLatchedIndices; i++) {
			ar >> m_LODisabled[i];
			m_LONotesArray[i].Serialize(ar);
			m_LOFlagsArray[i].Serialize(ar);
			m_LOArray[i].Serialize(ar, uiFileVersion);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_LOLevel[i];
		}

		// SPI data
		for (i = 0; i < m_DeviceParam.NumberSPIIndices; i++) {
			ar >> m_SPIDisabled[i];
			m_SPINotesArray[i].Serialize(ar);
			m_SPIFlagsArray[i].Serialize(ar);
			m_SPIArray[i].Serialize(ar, uiFileVersion);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_SPILevel[i];
		}

		// Dot Matrix data
		for (i = 0; i < m_DeviceParam.NumberDotMatrixIndices; i++) {
			ar >> m_DotMXDisabled[i];
			m_DotMXNotesArray[i].Serialize(ar);
			m_DotMXFlagsArray[i].Serialize(ar);
			m_DotMXArray[i].Serialize(ar, uiFileVersion);
		}

		// Stepper data
		for (i = 0; i < m_DeviceParam.NumberStepperIndices; i++) {
			ar >> m_STEPDisabled[i];
			m_STEPNotesArray[i].Serialize(ar);
			m_STEPFlagsArray[i].Serialize(ar);
			m_STEPArray[i].Serialize(ar, uiFileVersion);
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_DotMXLevel[i];
		}

		for (i = 0; i < m_DeviceParam.NumberPortIndices; i++) {
			ar >> m_PortLabel[i];
		}

		if (uiFileVersion != ID_SAVEFILEVERSION && !theApp.m_WarningFlag) {
			AfxMessageBox("Obsolete file loaded! Resave this file.", MB_OK );
			theApp.m_WarningFlag = TRUE;	// warn only once
		}
	}
}

void CSaveDevice::SerializeItem(CArchive &ar, UINT ItemType, UINT iIndex, UINT pIndex)
{	// MSDN says we ne need to call ancestor's serialize
	CObject::Serialize( ar );

	CString str;
	UINT uiFileVersion, uiFileClass, tmpItemType;

	// standard procedure when serializing
	if ( ar.IsStoring() )
	{	// store the rest of our data
		ar << m_uiItemFileClass;
		ar << m_uiFileVersion;
		ar << ItemType;

		switch (ItemType) {
			case ID_INPUT:
				ar << m_InputDisabled[iIndex];
				m_NotesArray[iIndex].Serialize(ar);
				m_SendKeysArray[iIndex].Serialize(ar);
				m_VariablesArray[iIndex].Serialize(ar, NULL);
			break;

			case ID_OUTPUT:
				ar << m_OutputDisabled[iIndex];
				m_OutputNotesArray[iIndex].Serialize(ar);
				m_OutputFlagsArray[iIndex].Serialize(ar);
				m_OutputArray[iIndex].Serialize(ar, NULL);
			break;

			case ID_MULTIPLEXED:
				ar << m_DispDisabled[pIndex];
				m_DispNotesArray[pIndex].Serialize(ar);
				m_DisplayFlagsArray[pIndex].Serialize(ar);
				m_DisplayArray[pIndex].Serialize(ar, NULL);
			break;

			case ID_MUX:
				ar << m_MuxDisabled[iIndex];
				m_MuxNotesArray[iIndex].Serialize(ar);
				m_MuxFlagsArray[iIndex].Serialize(ar);
				m_MuxArray[iIndex].Serialize(ar, NULL);
			break;

			case ID_RWR:
				ar << m_CrtRwrDisabled[pIndex];
				m_CrtRwrNotesArray[pIndex].Serialize(ar);
				m_CrtRwrFlagsArray[pIndex].Serialize(ar);
				m_CrtRwrArray[pIndex].Serialize(ar);
			break;

			case ID_CLCD:
				ar << m_CLCDDisabled[pIndex];
				m_CLCDNotesArray[pIndex].Serialize(ar);
				m_CLCDFlagsArray[pIndex].Serialize(ar);
				m_CLCDArray[pIndex].Serialize(ar);
			break;

			case ID_GLCD:
				ar << m_GLCDDisabled[pIndex];
				m_GLCDNotesArray[pIndex].Serialize(ar);
				m_GLCDFlagsArray[pIndex].Serialize(ar);
				m_GLCDArray[pIndex].Serialize(ar);
			break;

			case ID_LO:
				ar << m_LODisabled[iIndex];
				m_LONotesArray[iIndex].Serialize(ar);
				m_LOFlagsArray[iIndex].Serialize(ar);
				m_LOArray[iIndex].Serialize(ar, NULL);
			break;

			case ID_SPI:
				ar << m_SPIDisabled[iIndex];
				m_SPINotesArray[iIndex].Serialize(ar);
				m_SPIFlagsArray[iIndex].Serialize(ar);
				m_SPIArray[iIndex].Serialize(ar, NULL);
			break;

			case ID_DMX:
				ar << m_DotMXDisabled[iIndex];
				m_DotMXNotesArray[iIndex].Serialize(ar);
				m_DotMXFlagsArray[iIndex].Serialize(ar);
				m_DotMXArray[iIndex].Serialize(ar, NULL);
			break;

			case ID_STEP:
				ar << m_STEPDisabled[iIndex];
				m_STEPNotesArray[iIndex].Serialize(ar);
				m_STEPFlagsArray[iIndex].Serialize(ar);
				m_STEPArray[iIndex].Serialize(ar, NULL);
			break;
		}
	} else {
		// we are loading data, so we first need to check
		// if it's the correct version
		ar >> uiFileClass;
		ar >> uiFileVersion;
		ar >> tmpItemType;

		if (uiFileClass != ID_ITEMFILECLASS || tmpItemType != ItemType) {
			AfxMessageBox("Incompatible file!", MB_OK );
			AfxThrowArchiveException(CArchiveException::badClass);
			return;		// wrong file class
		}

		switch (uiFileVersion) {
			case m_uiFileVersion:
				// everything is okay, load data
				// in the EXACT order as saving
				switch (tmpItemType) {
					case ID_INPUT:
						ar >> m_InputDisabled[iIndex];
						m_NotesArray[iIndex].Serialize(ar);
						m_SendKeysArray[iIndex].Serialize(ar);
						m_VariablesArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pInputsTabCtrl->InitAllTabs();
					break;

					case ID_OUTPUT:
						ar >> m_OutputDisabled[iIndex];
						m_OutputNotesArray[iIndex].Serialize(ar);
						m_OutputFlagsArray[iIndex].Serialize(ar);
						m_OutputArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pOutputTabCtrl->InitAllTabs();
					break;

					case ID_MULTIPLEXED:
						ar >> m_DispDisabled[pIndex];
						m_DispNotesArray[pIndex].Serialize(ar);
						m_DisplayFlagsArray[pIndex].Serialize(ar);
						m_DisplayArray[pIndex].Serialize(ar, uiFileVersion);
						theApp.m_pDispTabCtrl->InitAllTabs();
					break;

					case ID_MUX:
						ar >> m_MuxDisabled[iIndex];
						m_MuxNotesArray[iIndex].Serialize(ar);
						m_MuxFlagsArray[iIndex].Serialize(ar);
						m_MuxArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pMuxTabCtrl->InitAllTabs();
					break;

					case ID_RWR:
						ar >> m_CrtRwrDisabled[pIndex];
						m_CrtRwrNotesArray[pIndex].Serialize(ar);
						m_CrtRwrFlagsArray[pIndex].Serialize(ar);
						m_CrtRwrArray[pIndex].Serialize(ar);
						theApp.m_pRWRTabCtrl->InitAllTabs();
					break;

					case ID_CLCD:
						ar >> m_CLCDDisabled[pIndex];
						m_CLCDNotesArray[pIndex].Serialize(ar);
						m_CLCDFlagsArray[pIndex].Serialize(ar);
						m_CLCDArray[pIndex].Serialize(ar);
						theApp.m_pCLCDTabCtrl->InitAllTabs();
					break;

					case ID_GLCD:
						ar >> m_GLCDDisabled[pIndex];
						m_GLCDNotesArray[pIndex].Serialize(ar);
						m_GLCDFlagsArray[pIndex].Serialize(ar);
						m_GLCDArray[pIndex].Serialize(ar);
						theApp.m_pGLCDTabCtrl->InitAllTabs();
					break;

					case ID_LO:
						ar >> m_LODisabled[iIndex];
						m_LONotesArray[iIndex].Serialize(ar);
						m_LOFlagsArray[iIndex].Serialize(ar);
						m_LOArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pLatchedTabCtrl->InitAllTabs();
					break;

					case ID_SPI:
						ar >> m_SPIDisabled[iIndex];
						m_SPINotesArray[iIndex].Serialize(ar);
						m_SPIFlagsArray[iIndex].Serialize(ar);
						m_SPIArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pSPITabCtrl->InitAllTabs();
					break;

					case ID_DMX:
						ar >> m_DotMXDisabled[iIndex];
						m_DotMXNotesArray[iIndex].Serialize(ar);
						m_DotMXFlagsArray[iIndex].Serialize(ar);
						m_DotMXArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pDotMXTabCtrl->InitAllTabs();
					break;

					case ID_STEP:
						ar >> m_STEPDisabled[iIndex];
						m_STEPNotesArray[iIndex].Serialize(ar);
						m_STEPFlagsArray[iIndex].Serialize(ar);
						m_STEPArray[iIndex].Serialize(ar, uiFileVersion);
						theApp.m_pSTEPTabCtrl->InitAllTabs();
					break;
				}
			break;

			default:	// version mismatch, so inform the user
				AfxMessageBox("Incompatible file!", MB_OK );
				AfxThrowArchiveException(CArchiveException::genericException);
		}
	}
}

void CSaveDevice::CopyDeviceData(CSaveDevice *pDevArray)
{
	int i;

	for (i = 0; i < MAX_INPUTS; i++) {
		m_InputDisabled[i] = pDevArray->m_InputDisabled[i];
	}

	for (i = 0; i < MAX_OUTPUTS; i++) {
		m_OutputDisabled[i] = pDevArray->m_OutputDisabled[i];
	}

	for (i = 0; i < MAX_MUXDISPLAYS; i++) {
		m_MuxDisabled[i] = pDevArray->m_MuxDisabled[i];
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_DispDisabled[i] = pDevArray->m_DispDisabled[i];
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_CrtRwrDisabled[i] = pDevArray->m_CrtRwrDisabled[i];
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_CLCDDisabled[i] = pDevArray->m_CLCDDisabled[i];
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_GLCDDisabled[i] = pDevArray->m_GLCDDisabled[i];
	}

	for (i = 0; i < MAX_SPI; i++) {
		m_SPIDisabled[i] = pDevArray->m_SPIDisabled[i];
	}

	for (i = 0; i < MAX_LATCHED; i++) {
		m_LODisabled[i] = pDevArray->m_LODisabled[i];
	}

	for (i = 0; i < MAX_DOTMATRIX; i++) {
		m_DotMXDisabled[i] = pDevArray->m_DotMXDisabled[i];
	}

	for (i = 0; i < MAX_STEPPER; i++) {
		m_STEPDisabled[i] = pDevArray->m_STEPDisabled[i];
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_PortLabel[i] = pDevArray->m_PortLabel[i];
	}
}

void CSaveDevice::CopyArrayData(CSaveDevice *pDevArray)
{
	int i;

	for (i = 0; i < MAX_INPUTS; i++) {
		m_NotesArray[i].CopyData(&(pDevArray->m_NotesArray[i]));
		m_SendKeysArray[i].CopyData(&(pDevArray->m_SendKeysArray[i]));
		m_VariablesArray[i].CopyData(&(pDevArray->m_VariablesArray[i]));
	}

	for (i = 0; i < MAX_OUTPUTS; i++) {
		m_OutputArray[i].CopyData(&(pDevArray->m_OutputArray[i]));
		m_OutputFlagsArray[i].CopyData(&(pDevArray->m_OutputFlagsArray[i]));
		m_OutputNotesArray[i].CopyData(&(pDevArray->m_OutputNotesArray[i]));
	}

	for (i = 0; i < MAX_MUXDISPLAYS; i++) {
		m_MuxArray[i].CopyData(&(pDevArray->m_MuxArray[i]));
		m_MuxFlagsArray[i].CopyData(&(pDevArray->m_MuxFlagsArray[i]));
		m_MuxNotesArray[i].CopyData(&(pDevArray->m_MuxNotesArray[i]));
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_DisplayArray[i].CopyData(&(pDevArray->m_DisplayArray[i]));
		m_DisplayFlagsArray[i].CopyData(&(pDevArray->m_DisplayFlagsArray[i]));
		m_DispNotesArray[i].CopyData(&(pDevArray->m_DispNotesArray[i]));
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_CrtRwrArray[i].CopyData(&(pDevArray->m_CrtRwrArray[i]));
		m_CrtRwrFlagsArray[i].CopyData(&(pDevArray->m_CrtRwrFlagsArray[i]));
		m_CrtRwrNotesArray[i].CopyData(&(pDevArray->m_CrtRwrNotesArray[i]));
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_CLCDArray[i].CopyData(&(pDevArray->m_CLCDArray[i]));
		m_CLCDFlagsArray[i].CopyData(&(pDevArray->m_CLCDFlagsArray[i]));
		m_CLCDNotesArray[i].CopyData(&(pDevArray->m_CLCDNotesArray[i]));
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_GLCDArray[i].CopyData(&(pDevArray->m_GLCDArray[i]));
		m_GLCDFlagsArray[i].CopyData(&(pDevArray->m_GLCDFlagsArray[i]));
		m_GLCDNotesArray[i].CopyData(&(pDevArray->m_GLCDNotesArray[i]));
	}

	for (i = 0; i < MAX_SPI; i++) {
		m_SPIArray[i].CopyData(&(pDevArray->m_SPIArray[i]));
		m_SPIFlagsArray[i].CopyData(&(pDevArray->m_SPIFlagsArray[i]));
		m_SPINotesArray[i].CopyData(&(pDevArray->m_SPINotesArray[i]));
	}

	for (i = 0; i < MAX_LATCHED; i++) {
		m_LOArray[i].CopyData(&(pDevArray->m_LOArray[i]));
		m_LOFlagsArray[i].CopyData(&(pDevArray->m_LOFlagsArray[i]));
		m_LONotesArray[i].CopyData(&(pDevArray->m_LONotesArray[i]));
	}

	for (i = 0; i < MAX_DOTMATRIX; i++) {
		m_DotMXArray[i].CopyData(&(pDevArray->m_DotMXArray[i]));
		m_DotMXFlagsArray[i].CopyData(&(pDevArray->m_DotMXFlagsArray[i]));
		m_DotMXNotesArray[i].CopyData(&(pDevArray->m_DotMXNotesArray[i]));
	}

	for (i = 0; i < MAX_STEPPER; i++) {
		m_STEPArray[i].CopyData(&(pDevArray->m_STEPArray[i]));
		m_STEPFlagsArray[i].CopyData(&(pDevArray->m_STEPFlagsArray[i]));
		m_STEPNotesArray[i].CopyData(&(pDevArray->m_STEPNotesArray[i]));
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_PortLabel[i] = pDevArray->m_PortLabel[i];
	}
}

void CSaveDevice::CopyDeviceParam(DeviceParam *DevParam)
{
	m_DeviceParam.ConfigFlag = DevParam->ConfigFlag;
	memset(m_DeviceParam.DeviceName, atoi(" "), MAX_CHAR);
	memcpy(m_DeviceParam.DeviceName, DevParam->DeviceName, MAX_CHAR);
	m_DeviceParam.DeviceNameLength = DevParam->DeviceNameLength;
	memset(m_DeviceParam.DevicePath, atoi(" "), MAX_CHAR);
	memcpy(m_DeviceParam.DevicePath, DevParam->DevicePath, MAX_CHAR);
	m_DeviceParam.PathLength = DevParam->PathLength;
	m_DeviceParam.DevIndex = DevParam->DevIndex;
	m_DeviceParam.DeviceHandle = DevParam->DeviceHandle;
	memset(m_DeviceParam.ManufName, atoi(" "), MAX_CHAR);
	memcpy(m_DeviceParam.ManufName, DevParam->ManufName, MAX_CHAR);
	m_DeviceParam.ManufNameLength = DevParam->ManufNameLength;
	m_DeviceParam.NumberInputIndices = DevParam->NumberInputIndices;
	m_DeviceParam.NumberOutputIndices = DevParam->NumberOutputIndices;
	m_DeviceParam.NumberPortIndices = DevParam->NumberPortIndices;
	m_DeviceParam.NumberAnalogIndices = DevParam->NumberAnalogIndices;
	m_DeviceParam.NumberHATIndices = DevParam->NumberHATIndices;
	m_DeviceParam.NumberRotaryIndices = DevParam->NumberRotaryIndices;
	m_DeviceParam.NumberCRTRWRIndices = DevParam->NumberCRTRWRIndices;
	m_DeviceParam.NumberPWMIndices = DevParam->NumberPWMIndices;
	m_DeviceParam.NumberLCDIndices = DevParam->NumberLCDIndices;
	m_DeviceParam.NumberGLCDIndices = DevParam->NumberGLCDIndices;
	m_DeviceParam.NumberMuxDisplayIndices = DevParam->NumberMuxDisplayIndices;
	m_DeviceParam.NumberAlphanumericIndices = DevParam->NumberAlphanumericIndices;
	m_DeviceParam.NumberSPIIndices = DevParam->NumberSPIIndices;
	m_DeviceParam.NumberLatchedIndices = DevParam->NumberLatchedIndices;
	m_DeviceParam.NumberDotMatrixIndices = DevParam->NumberDotMatrixIndices;
	m_DeviceParam.NumberStepperIndices = DevParam->NumberStepperIndices;
	
	m_DeviceParam.ProductID = DevParam->ProductID ;
	m_DeviceParam.DeviceHandle = DevParam->DeviceHandle ;
	memset(m_DeviceParam.SerialNum, atoi(" "), 3);
	m_DeviceParam.SerialNum[3] = NULL;
	memcpy(m_DeviceParam.SerialNum, DevParam->SerialNum, 4);
	m_DeviceParam.SerialNumLength = DevParam->SerialNumLength;
	m_DeviceParam.VendorID = DevParam->VendorID;
	m_DeviceParam.VersionNumber = DevParam->VersionNumber;
	m_DeviceParam.FlashVersion = DevParam->FlashVersion;
	m_DeviceParam.ProgramFlag = DevParam->ProgramFlag;

	m_DeviceParam.InputReportByteLength = DevParam->InputReportByteLength;
	m_DeviceParam.OutputReportByteLength = DevParam->OutputReportByteLength;
	m_DeviceParam.Usage = DevParam->Usage;
	m_DeviceParam.UsagePage = DevParam->UsagePage;
}

void CSaveDevice::CopyDeviceStatus(DeviceStatus *DevStatus)
{
	memcpy(m_DeviceStatus.InputActive, DevStatus->InputActive, MAX_INPUTS);
	memcpy(m_DeviceStatus.OutputActive, DevStatus->OutputActive, MAX_OUTPUTS);
	memcpy(m_DeviceStatus.PortActive, DevStatus->PortActive, MAX_PORTS);
	memcpy(m_DeviceStatus.PortIOMode, DevStatus->PortIOMode, MAX_PORTS);
	memcpy(m_DeviceStatus.PortMode, DevStatus->PortMode, MAX_PORTS);
	memcpy(m_DeviceStatus.OutputMode, DevStatus->OutputMode, MAX_PORTS);
	memcpy(m_DeviceStatus.AnalogActive, DevStatus->AnalogActive, MAX_ANALOG);
	memcpy(m_DeviceStatus.HATActive, DevStatus->HATActive, MAX_HATS);
	memcpy(m_DeviceStatus.RotaryActive, DevStatus->RotaryActive, MAX_ROTARY);
	memcpy(m_DeviceStatus.CRTRWRActive, DevStatus->CRTRWRActive, MAX_RWR);
	memcpy(m_DeviceStatus.PWMActive, DevStatus->PWMActive, MAX_PWM);
	memcpy(m_DeviceStatus.LCDActive, DevStatus->LCDActive, MAX_LCDS);
	memcpy(m_DeviceStatus.GLCDActive, DevStatus->GLCDActive, MAX_GLCDS);
	memcpy(m_DeviceStatus.MuxDisplayActive, DevStatus->MuxDisplayActive, MAX_MUXDISPLAYS);
	memcpy(m_DeviceStatus.AlphanumericActive, DevStatus->AlphanumericActive, MAX_ALPHANUMERICS);
	memcpy(m_DeviceStatus.SPIActive, DevStatus->SPIActive, MAX_SPI);
	memcpy(m_DeviceStatus.LatchedActive, DevStatus->LatchedActive, MAX_LATCHED);
	memcpy(m_DeviceStatus.DotMatrixActive, DevStatus->DotMatrixActive, MAX_DOTMATRIX);
	memcpy(m_DeviceStatus.StepperActive, DevStatus->StepperActive, MAX_STEPPER);
}

void CSaveDevice::ClearDeviceData()
{
	int i;

	for (i = 0; i <MAX_INPUTS; i++) {
		m_NotesArray[i].Clear();
		m_SendKeysArray[i].Clear();
		m_VariablesArray[i].Clear();
	}

	for (i = 0; i < MAX_OUTPUTS; i++) {
		m_OutputArray[i].Clear();
		m_OutputFlagsArray[i].Clear();
		m_OutputNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_MUXDISPLAYS; i++) {
		m_MuxArray[i].Clear();
		m_MuxFlagsArray[i].Clear();
		m_MuxNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_DisplayArray[i].Clear();
		m_DisplayFlagsArray[i].Clear();
		m_DispNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_CrtRwrArray[i].Clear();
		m_CrtRwrFlagsArray[i].Clear();
		m_CrtRwrNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_CLCDArray[i].Clear();
		m_CLCDFlagsArray[i].Clear();
		m_CLCDNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_GLCDArray[i].Clear();
		m_GLCDFlagsArray[i].Clear();
		m_GLCDNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_SPI; i++) {
		m_SPIArray[i].Clear();
		m_SPIFlagsArray[i].Clear();
		m_SPINotesArray[i].Clear();
	}

	for (i = 0; i < MAX_LATCHED; i++) {
		m_LOArray[i].Clear();
		m_LOFlagsArray[i].Clear();
		m_LONotesArray[i].Clear();
	}

	for (i = 0; i < MAX_DOTMATRIX; i++) {
		m_DotMXArray[i].Clear();
		m_DotMXFlagsArray[i].Clear();
		m_DotMXNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_STEPPER; i++) {
		m_STEPArray[i].Clear();
		m_STEPFlagsArray[i].Clear();
		m_STEPNotesArray[i].Clear();
	}

	for (i = 0; i < MAX_PORTS; i++) {
		m_PortLabel[i].Empty();
	}
}

void CSaveDevice::ClearItemData(UINT ItemType, UINT ItemIndex)
{
	switch (ItemType) {
		case ID_INPUT:
			m_NotesArray[ItemIndex].Clear();
			m_SendKeysArray[ItemIndex].Clear();
			m_VariablesArray[ItemIndex].Clear();
			break;

		case ID_OUTPUT:
			m_OutputArray[ItemIndex].Clear();
			m_OutputFlagsArray[ItemIndex].Clear();
			m_OutputNotesArray[ItemIndex].Clear();
			break;

		case ID_MULTIPLEXED:
			m_DisplayArray[ItemIndex].Clear();
			m_DisplayFlagsArray[ItemIndex].Clear();
			m_DispNotesArray[ItemIndex].Clear();
			break;

		case ID_MUX:
			m_MuxArray[ItemIndex].Clear();
			m_MuxFlagsArray[ItemIndex].Clear();
			m_MuxNotesArray[ItemIndex].Clear();
			break;

		case ID_RWR:
			m_CrtRwrArray[ItemIndex].Clear();
			m_CrtRwrFlagsArray[ItemIndex].Clear();
			m_CrtRwrNotesArray[ItemIndex].Clear();
			break;

		case ID_CLCD:
			m_CLCDArray[ItemIndex].Clear();
			m_CLCDFlagsArray[ItemIndex].Clear();
			m_CLCDNotesArray[ItemIndex].Clear();
			break;

		case ID_GLCD:
			m_GLCDArray[ItemIndex].Clear();
			m_GLCDFlagsArray[ItemIndex].Clear();
			m_GLCDNotesArray[ItemIndex].Clear();
			break;

		case ID_SPI:
			m_SPIArray[ItemIndex].Clear();
			m_SPIFlagsArray[ItemIndex].Clear();
			m_SPINotesArray[ItemIndex].Clear();
			break;

		case ID_LO:
			m_LOArray[ItemIndex].Clear();
			m_LOFlagsArray[ItemIndex].Clear();
			m_LONotesArray[ItemIndex].Clear();
			break;

		case ID_DMX:
			m_DotMXArray[ItemIndex].Clear();
			m_DotMXFlagsArray[ItemIndex].Clear();
			m_DotMXNotesArray[ItemIndex].Clear();
			break;

		case ID_STEP:
			m_STEPArray[ItemIndex].Clear();
			m_STEPFlagsArray[ItemIndex].Clear();
			m_STEPNotesArray[ItemIndex].Clear();
			break;
	}
}

void CSaveDevice::CopyItemData(CSaveDevice *pDevArray, UINT ItemType, UINT ItemIndex)
{
	switch (ItemType) {
		case ID_INPUT:
			m_Notes.CopyData(&pDevArray->m_NotesArray[ItemIndex]);
			m_SendKeys.CopyData(&pDevArray->m_SendKeysArray[ItemIndex]);
			m_Variables.CopyData(&pDevArray->m_VariablesArray[ItemIndex]);
			break;

		case ID_OUTPUT:
			m_Output.CopyData(&pDevArray->m_OutputArray[ItemIndex]);
			m_OutputFlags.CopyData(&pDevArray->m_OutputFlagsArray[ItemIndex]);
			m_OutputNotes.CopyData(&pDevArray->m_OutputNotesArray[ItemIndex]);
			break;

		case ID_MULTIPLEXED:
			m_Display.CopyData(&pDevArray->m_DisplayArray[ItemIndex]);
			m_DisplayFlags.CopyData(&pDevArray->m_DisplayFlagsArray[ItemIndex]);
			m_DispNotes.CopyData(&pDevArray->m_DispNotesArray[ItemIndex]);
			break;

		case ID_MUX:
			m_MuxItem.CopyData(&pDevArray->m_MuxArray[ItemIndex]);
			m_MuxFlags.CopyData(&pDevArray->m_MuxFlagsArray[ItemIndex]);
			m_MuxNotes.CopyData(&pDevArray->m_MuxNotesArray[ItemIndex]);
			break;

		case ID_RWR:
			m_CrtRwr.CopyData(&pDevArray->m_CrtRwrArray[ItemIndex]);
			m_CrtRwrFlags.CopyData(&pDevArray->m_CrtRwrFlagsArray[ItemIndex]);
			m_CrtRwrNotes.CopyData(&pDevArray->m_CrtRwrNotesArray[ItemIndex]);
			break;

		case ID_CLCD:
			m_CLCD.CopyData(&pDevArray->m_CLCDArray[ItemIndex]);
			m_CLCDFlags.CopyData(&pDevArray->m_CLCDFlagsArray[ItemIndex]);
			m_CLCDNotes.CopyData(&pDevArray->m_CLCDNotesArray[ItemIndex]);
			break;

		case ID_GLCD:
			m_GLCD.CopyData(&pDevArray->m_GLCDArray[ItemIndex]);
			m_GLCDFlags.CopyData(&pDevArray->m_GLCDFlagsArray[ItemIndex]);
			m_GLCDNotes.CopyData(&pDevArray->m_GLCDNotesArray[ItemIndex]);
			break;

		case ID_SPI:
			m_SPIItem.CopyData(&pDevArray->m_SPIArray[ItemIndex]);
			m_SPIFlags.CopyData(&pDevArray->m_SPIFlagsArray[ItemIndex]);
			m_SPINotes.CopyData(&pDevArray->m_SPINotesArray[ItemIndex]);
			break;

		case ID_LO:
			m_LOItem.CopyData(&pDevArray->m_LOArray[ItemIndex]);
			m_LOFlags.CopyData(&pDevArray->m_LOFlagsArray[ItemIndex]);
			m_LONotes.CopyData(&pDevArray->m_LONotesArray[ItemIndex]);
			break;

		case ID_DMX:
			m_DotMX.CopyData(&pDevArray->m_DotMXArray[ItemIndex]);
			m_DotMXFlags.CopyData(&pDevArray->m_DotMXFlagsArray[ItemIndex]);
			m_DotMXNotes.CopyData(&pDevArray->m_DotMXNotesArray[ItemIndex]);
			break;

		case ID_STEP:
			m_STEPItem.CopyData(&pDevArray->m_STEPArray[ItemIndex]);
			m_STEPFlags.CopyData(&pDevArray->m_STEPFlagsArray[ItemIndex]);
			m_STEPNotes.CopyData(&pDevArray->m_STEPNotesArray[ItemIndex]);
			break;
	}
}

void CSaveDevice::PasteItemData(CSaveDevice *pDevArray, UINT ItemType, UINT ItemIndex)
{
	switch (ItemType) {
		case ID_INPUT:
			m_Notes.PasteData(&pDevArray->m_NotesArray[ItemIndex]);
			m_SendKeys.PasteData(&pDevArray->m_SendKeysArray[ItemIndex]);
			m_Variables.PasteData(&pDevArray->m_VariablesArray[ItemIndex]);
			break;

		case ID_OUTPUT:
			m_Output.PasteData(&pDevArray->m_OutputArray[ItemIndex]);
			m_OutputFlags.PasteData(&pDevArray->m_OutputFlagsArray[ItemIndex]);
			m_OutputNotes.PasteData(&pDevArray->m_OutputNotesArray[ItemIndex]);
			break;

		case ID_MULTIPLEXED:
			m_Display.PasteData(&pDevArray->m_DisplayArray[ItemIndex]);
			m_DisplayFlags.PasteData(&pDevArray->m_DisplayFlagsArray[ItemIndex]);
			m_DispNotes.PasteData(&pDevArray->m_DispNotesArray[ItemIndex]);
			break;

		case ID_MUX:
			m_MuxItem.PasteData(&pDevArray->m_MuxArray[ItemIndex]);
			m_MuxFlags.PasteData(&pDevArray->m_MuxFlagsArray[ItemIndex]);
			m_MuxNotes.PasteData(&pDevArray->m_MuxNotesArray[ItemIndex]);
			break;

		case ID_RWR:
			m_CrtRwr.PasteData(&pDevArray->m_CrtRwrArray[ItemIndex]);
			m_CrtRwrFlags.PasteData(&pDevArray->m_CrtRwrFlagsArray[ItemIndex]);
			m_CrtRwrNotes.PasteData(&pDevArray->m_CrtRwrNotesArray[ItemIndex]);
			break;

		case ID_CLCD:
			m_CLCD.PasteData(&pDevArray->m_CLCDArray[ItemIndex]);
			m_CLCDFlags.PasteData(&pDevArray->m_CLCDFlagsArray[ItemIndex]);
			m_CLCDNotes.PasteData(&pDevArray->m_CLCDNotesArray[ItemIndex]);
			break;

		case ID_GLCD:
			m_GLCD.PasteData(&pDevArray->m_GLCDArray[ItemIndex]);
			m_GLCDFlags.PasteData(&pDevArray->m_GLCDFlagsArray[ItemIndex]);
			m_GLCDNotes.PasteData(&pDevArray->m_GLCDNotesArray[ItemIndex]);
			break;

		case ID_SPI:
			m_SPIItem.PasteData(&pDevArray->m_SPIArray[ItemIndex]);
			m_SPIFlags.PasteData(&pDevArray->m_SPIFlagsArray[ItemIndex]);
			m_SPINotes.PasteData(&pDevArray->m_SPINotesArray[ItemIndex]);
			break;

		case ID_LO:
			m_LOItem.PasteData(&pDevArray->m_LOArray[ItemIndex]);
			m_LOFlags.PasteData(&pDevArray->m_LOFlagsArray[ItemIndex]);
			m_LONotes.PasteData(&pDevArray->m_LONotesArray[ItemIndex]);
			break;

		case ID_DMX:
			m_DotMX.PasteData(&pDevArray->m_DotMXArray[ItemIndex]);
			m_DotMXFlags.PasteData(&pDevArray->m_DotMXFlagsArray[ItemIndex]);
			m_DotMXNotes.PasteData(&pDevArray->m_DotMXNotesArray[ItemIndex]);
			break;

		case ID_STEP:
			m_STEPItem.PasteData(&pDevArray->m_STEPArray[ItemIndex]);
			m_STEPFlags.PasteData(&pDevArray->m_STEPFlagsArray[ItemIndex]);
			m_STEPNotes.PasteData(&pDevArray->m_STEPNotesArray[ItemIndex]);
			break;
	}
}

BOOL CSaveDevice::IsSet()
{
	int i;

	for (i = 0; i <MAX_INPUTS; i++) {
		if (m_NotesArray[i].IsSet() ||
			m_SendKeysArray[i].IsSet() ||
			m_VariablesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_OUTPUTS; i++) {
		if (m_OutputArray[i].IsSet() ||
			m_OutputFlagsArray[i].IsSet() ||
			m_OutputNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_MUXDISPLAYS; i++) {
		if (m_MuxArray[i].IsSet() ||
			m_MuxFlagsArray[i].IsSet() ||
			m_MuxNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_PORTS; i++) {
		if (m_DisplayArray[i].IsSet() ||
			m_DisplayFlagsArray[i].IsSet() ||
			m_DispNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_PORTS; i++) {
		if (m_CrtRwrArray[i].IsSet() ||
			m_CrtRwrFlagsArray[i].IsSet() ||
			m_CrtRwrNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_PORTS; i++) {
		if (m_CLCDArray[i].IsSet() ||
			m_CLCDFlagsArray[i].IsSet() ||
			m_CLCDNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_PORTS; i++) {
		if (m_GLCDArray[i].IsSet() ||
			m_GLCDFlagsArray[i].IsSet() ||
			m_GLCDNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_SPI; i++) {
		if (m_SPIArray[i].IsSet() ||
			m_SPIFlagsArray[i].IsSet() ||
			m_SPINotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_LATCHED; i++) {
		if (m_LOArray[i].IsSet() ||
			m_LOFlagsArray[i].IsSet() ||
			m_LONotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_DOTMATRIX; i++) {
		if (m_DotMXArray[i].IsSet() ||
			m_DotMXFlagsArray[i].IsSet() ||
			m_DotMXNotesArray[i].IsSet())
			return true;
	}

	for (i = 0; i < MAX_STEPPER; i++) {
		if (m_STEPArray[i].IsSet() ||
			m_STEPFlagsArray[i].IsSet() ||
			m_STEPNotesArray[i].IsSet())
			return true;
	}
	return false;
}

