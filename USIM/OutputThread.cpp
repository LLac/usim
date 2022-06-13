// OutputThread.cpp: implementation of the COutputThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include <math.h>
#include <afx.h>
#include "SimplexParser\FormelParser.h"

#include "OutputThread.h"


extern "C" void TriggerEvent (ULONG, LONG);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(COutputThread, CThread)

#define RADTODEG(x) ((x) * 57.29578)
#define DEGTORAD(y) ((y) * 0.017453)

#define TCHAR_ARG   TCHAR
#define WCHAR_ARG   WCHAR
#define CHAR_ARG    char

#ifdef _X86_
#define DOUBLE_ARG  double //{sizeof(double)}
#else
	#define DOUBLE_ARG  double
#endif

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

void Format(CString& str, LPCTSTR lpszFormat, ...);
//extern _XPDATA_LIST const XPDataArray[ArraySize(XPDataArray)];

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
COutputThread::COutputThread(void* pOwnerObject /*= NULL*/, LPARAM lParam /*= 0L*/)
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(COutputThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_delta_T = 0;
	m_RWRcnt = 0;
	m_GLCDcnt = 1;
	m_CLCDcnt = 1;
	m_SPIcnt = 1;
	m_STEPcnt = 0;

	m_Run = FALSE;
}

COutputThread::~COutputThread()
{
	m_delta_T = 0;
	m_RWRcnt = 0;
	m_GLCDcnt = 1;
	m_CLCDcnt = 1;
	m_SPIcnt = 1;
	m_STEPcnt = 0;
}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD COutputThread::ThreadHandler()
{
	BOOL bCanContinue = TRUE;
	int nIncomingCommand;

#ifdef _DEBUG
		TRACE ("\nOutput Thread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 10);

#ifdef _DEBUG
//	TRACE("DoInput Delta = %d\n", (timeGetTime() - m_delta_T));
#endif
		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
//			if (theApp.m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
				DoOutputWorker();
			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
//			if (theApp.m_KeyCheckThread.GetActivityStatus() == CThread::THREAD_RUNNING)
				DoOutputWorker();

			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_STOP:
			m_Run = FALSE;
			bCanContinue = FALSE;
			InitOLED(GLCD_SHUTDOWN);
			break;

		default:
			break;
		};

	} while (bCanContinue);

#ifdef _DEBUG
		TRACE ("\nDoOutput Thread killed!\n");
#endif

	return 9;	// ... if Thread task completition OK
}

void COutputThread::InitOLED(UCHAR PowerFlag)
{
	UINT dIndex, nPort;
	POSITION pos;

	// write to HID devices
	pos = theApp.m_DevArrayList.GetHeadPosition();
	while (pos != NULL) {
		dIndex = theApp.m_DevArrayList.GetNext(pos);

		if (theApp.m_pDevArray[dIndex]->m_Connected) {
			// send data to EF devices
			for (nPort = 0; nPort < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices; nPort++) {
				// if node ports active and mode output
				if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[nPort] && 
					theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[nPort] == IO_OUTPUT) {
			
					if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[nPort] == MODE_GLCD) {
						CmdGLCD (&theApp.m_pDevArray[dIndex]->m_DeviceParam, (UCHAR)nPort, (UCHAR)0, PowerFlag);
					}
				}
			}
		}
	} // while (pos != NULL)
}

void COutputThread::DoOutputWorker()
{
	UINT dIndex;
	POSITION pos;

	if (!m_Run)
		return;
	if ((timeGetTime() - m_delta_T) < 10) 
		return;

	// 10 ms elapsed
#ifdef _DEBUG
//	TRACE("DoInput Delta = %d\n", (timeGetTime() - m_delta_T));
#endif
	m_delta_T = timeGetTime();
	
	if (theApp.m_SimulationModel == SIM_NONE || theApp.m_SimulationModel == SIM_KBS) return;

	Lock();

	// update vars every 10 ms
	theApp.m_F4SharedMem.UpdateSharedMem();
	theApp.m_SimBinSharedMem.UpdateSharedMem();
	theApp.m_RFSharedMem.UpdateSharedMem();
	theApp.m_GPXSharedMem.UpdateSharedMem();

	// write to HID devices
	pos = theApp.m_DevArrayList.GetHeadPosition();
	while (pos != NULL) {
		dIndex = theApp.m_DevArrayList.GetNext(pos);

		if (theApp.m_pDevArray[dIndex]->m_Connected) {
			HIDOutput(dIndex);
		}
	} // while (pos != NULL)

	if (m_RWRcnt++ == 5)
		m_RWRcnt = 0;
	if (m_GLCDcnt++ == 3)
		m_GLCDcnt = 0;
	if (m_CLCDcnt++ == 5)
		m_CLCDcnt = 0;
	if (m_SPIcnt++ == 2) 
		m_SPIcnt = 0;
	if (m_STEPcnt++ == 2) 
		m_STEPcnt = 0;
	Unlock();
}

void COutputThread::HIDOutput(UINT dIndex)
{
	long retval = 0;
	UINT oIndex, nPort;
	int nIncomingCommand;
	int i;
	char mDotMX[4][4];

	// send data to Plasma devices
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_PLASMA_LITE_V2) {
		
		nPort = 0;
		if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[nPort] &&
			theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[nPort] == IO_OUTPUT &&
			theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[nPort] == MODE_MIXED) {

			ReadDirectOutputs(dIndex, 0);
			if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
				theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

				retval = WriteDirectOutput(&theApp.m_pDevArray[dIndex]->m_DeviceParam, 0, theApp.m_pDevArray[dIndex]->m_DirectOutputs);
			}

			ReadDisplayOutputs(dIndex, 0);
			if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
				theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

				retval = Write7Segment(&theApp.m_pDevArray[dIndex]->m_DeviceParam, (UCHAR *)(LPTSTR)(LPCTSTR)theApp.m_pDevArray[dIndex]->m_StrDisp[nPort]);
			}

			if (m_CLCDcnt == 5) {	// every 50 ms
				if (theApp.m_pDevArray[dIndex]->m_CLCDActive[nPort])
					UpdateLCD(&theApp.m_pDevArray[dIndex]->m_CLCDArray[nPort], dIndex, 0, MODE_CLCD);
			}

			if (m_SPIcnt == 2) {	// every 50 ms
				UpdateSPI(dIndex, nPort);
			}
		}
			
		return;
	}

	// send data to Nitro devices
	if (theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_NITRO_SLG ||
		theApp.m_pDevArray[dIndex]->m_DeviceParam.ProductID == PIDC_NITRO_XG) {
		
		nPort = 0;
		if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[nPort] &&
			theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[nPort] == IO_OUTPUT &&
			theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[nPort] == MODE_MIXED) {

			ReadDisplayOutputs(dIndex, nPort);
			if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
				theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

				retval = Write7Segment(&theApp.m_pDevArray[dIndex]->m_DeviceParam, (UCHAR *)(LPTSTR)(LPCTSTR)theApp.m_pDevArray[dIndex]->m_StrDisp[nPort]);
			}

			ReadLatchedOutputs(dIndex, nPort);
			if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
				theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

				oIndex = nPort*NUM_LATCHED_PER_XG;
				retval = WriteLatchedOutput(&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
											(UCHAR)nPort, 
											m_LatchedOutputs,
											theApp.m_pDevArray[dIndex]->m_LOLevel[nPort]);
			}

			if (m_GLCDcnt == 3) { // every 30 ms
				if (theApp.m_pDevArray[dIndex]->m_CLCDActive[nPort])
					UpdateLCD(&theApp.m_pDevArray[dIndex]->m_CLCDArray[nPort], dIndex, 0, MODE_CLCD);
			}

			if (m_SPIcnt == 2) { // every 50 ms
				UpdateSPI(dIndex, nPort);
			}

		}
		
		return;
	}

	// send data to EF devices
	for (nPort = 0; nPort < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberPortIndices; nPort++) {
		// since this is a long loop, check for incoming thread messages and exit loop
		WaitForNotification(nIncomingCommand, 0);
		if (nIncomingCommand != CThread::CMD_NONE && nIncomingCommand != CThread::CMD_TIMEOUT_ELAPSED) {
			HandleCommandImmediately(nIncomingCommand);
			Unlock();
			return;
		}

		// if node ports active and mode output
		if (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortActive[nPort] && 
			theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortIOMode[nPort] == IO_OUTPUT) {
	
			switch (theApp.m_pDevArray[dIndex]->m_DeviceStatus.PortMode[nPort]) {
				case MODE_DIRECT:
					ReadDirectOutputs(dIndex, nPort);
					if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
						theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

						oIndex = nPort*EEEF_NUM_OUTPUTS_PER_PORT;
						retval = WriteDirectOutput(&theApp.m_pDevArray[dIndex]->m_DeviceParam, (UCHAR)nPort, &theApp.m_pDevArray[dIndex]->m_DirectOutputs[oIndex]);
					}
					break;

				case MODE_MUX:
					if (theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OverrideOld == 1) {
						ReadDisplayOutputs(dIndex, nPort);
						if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
							theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

							retval = WriteDisplayOutput(&theApp.m_pDevArray[dIndex]->m_DeviceParam, (UCHAR)nPort, (LPTSTR)(LPCTSTR)theApp.m_pDevArray[dIndex]->m_StrDisp[nPort], theApp.m_pDevArray[dIndex]->m_DispLevel[nPort]);
						}
					} else {
						ReadMuxOutputs(dIndex, nPort);
						if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
							theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

							oIndex = nPort*NUM_MUX_PER_PORT;
							retval = WriteMuxOutput(&theApp.m_pDevArray[dIndex]->m_DeviceParam, (UCHAR)nPort, &theApp.m_pDevArray[dIndex]->m_MuxOutputs[oIndex]);
						}
					}
					break;

				case MODE_RWR:
					if (m_RWRcnt == 5) { // every 50 ms
						UpdateCrtRWR(dIndex, nPort);
					}
					break;

				case MODE_GLCD:
					if (m_GLCDcnt == 3) { // every 30 ms
						if (theApp.m_pDevArray[dIndex]->m_GLCDActive[nPort])
							UpdateLCD(&theApp.m_pDevArray[dIndex]->m_GLCDArray[nPort], dIndex, nPort, MODE_GLCD);
					}
					break;

				case MODE_LATCHED:
					ReadLatchedOutputs(dIndex, nPort);
					if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
						theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

						oIndex = nPort*NUM_LATCHED_PER_PORT;
						retval = WriteLatchedOutput(&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
													(UCHAR)nPort, 
													m_LatchedOutputs,
													theApp.m_pDevArray[dIndex]->m_LOLevel[nPort]);
					}
					break;

				case MODE_SPI:
				case MODE_DAC:
					if (m_SPIcnt == 2) { // every 50 ms
						UpdateSPI(dIndex, nPort);
					}
					break;

				case MODE_STEPPER:
					if (m_STEPcnt == 2) { // every 50 ms
						UpdateSTEP(dIndex, nPort);
					}
					break;

				case MODE_DOTMATRIX:
					ReadDotMatrixOutputs(dIndex, nPort);
					if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
						theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

						oIndex = nPort*NUM_DMX_PER_PORT;
						for (i = 0; i < 4; i++)
							memcpy (mDotMX[i], theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex+i], 4);
						retval = WriteDotMatrix(&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
												(UCHAR)nPort, 
												&mDotMX[0][0], 
												theApp.m_pDevArray[dIndex]->m_DotMXLevel[nPort]);
					}
					break;
			}

			if (retval == DEV_FAILED || retval == DEV_TIMEOUT)
				theApp.m_pDevArray[dIndex]->m_Connected = FALSE;
		}
	}
}

void COutputThread::UpdateSTEP(UINT dIndex, UINT nPort)
{
	static int nHeading = 0;
	static BOOL	DirFlag = true;
	UINT StartSTEP = (MAX_STEPPER/MAX_PORTS)*nPort;
	UINT EndSTEP = (MAX_STEPPER/MAX_PORTS)*(nPort+1);

	theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

	if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_STEPFlagsArray[nPort])) return;

	// loop through dacs
	for (UINT nSTEP = StartSTEP; nSTEP < EndSTEP; nSTEP++) {
		// Single STEP control
		UpdateSingleSTEP(&theApp.m_pDevArray[dIndex]->m_STEPArray[nSTEP],
						dIndex, 
						nPort, 
						nSTEP);
	}

	if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
		// write to STEPPERs
		WriteStepper(	&theApp.m_pDevArray[dIndex]->m_DeviceParam,
						(UCHAR)nPort, 
						&theApp.m_pDevArray[dIndex]->m_STEP[StartSTEP]);
	}
}

void COutputThread::UpdateSingleSTEP(CSTEPPage *pSTEPArray, UINT dIndex, UINT nPort, UINT nSTEP)
{
	static BOOL	DirFlag = true;
	static long nHeading = 0;
	double divisor;
	long sFactor;
	long m_STEPMin, m_STEPMax;

	// Single SPI control
	if (pSTEPArray->m_STEPCalibrate) {
		// Calibrate SPI
		theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = pSTEPArray->m_STEPTemp;

		if (theApp.m_pDevArray[dIndex]->m_STEPOld[nSTEP] != theApp.m_pDevArray[dIndex]->m_STEP[nSTEP]) {
			theApp.m_pDevArray[dIndex]->m_STEPOld[nSTEP] = theApp.m_pDevArray[dIndex]->m_STEP[nSTEP];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}
	} else if (pSTEPArray->m_STEPTest && pSTEPArray->m_Sections > 0) {
		// Test STEPs by looping value 360 degrees

		if (pSTEPArray->m_STEPMin[0] > pSTEPArray->m_STEPMax[pSTEPArray->m_Sections-1]) {
			m_STEPMax = pSTEPArray->m_STEPMin[0];
			m_STEPMin = pSTEPArray->m_STEPMax[pSTEPArray->m_Sections-1];
		} else {
			m_STEPMax = pSTEPArray->m_STEPMax[pSTEPArray->m_Sections-1];
			m_STEPMin = pSTEPArray->m_STEPMin[0];
		}

		if (DirFlag) nHeading += 10;
		else nHeading -= 10;

		long Dactemp = 0;

		for (int i=0; i<pSTEPArray->m_Sections-1; i++) {
			if (pSTEPArray->m_STEPMax[i] > pSTEPArray->m_STEPMin[i+1]) {
				if (nHeading > pSTEPArray->m_STEPMax[i] && nHeading < pSTEPArray->m_STEPMin[i+1]) {
					Dactemp = pSTEPArray->m_STEPMax[i];
					break;
				}
			} else {
				if (nHeading < pSTEPArray->m_STEPMax[i] && nHeading > pSTEPArray->m_STEPMin[i+1]) {
					Dactemp = pSTEPArray->m_STEPMax[i];
					break;
				}
			}
		}

		if (nHeading < m_STEPMin) {
			nHeading = m_STEPMin;
			DirFlag = true;
		}

		if (nHeading > m_STEPMax) {
			nHeading = m_STEPMax;
			DirFlag = false;
		}

		if (Dactemp != 0)
			theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = Dactemp;
		else
			theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = nHeading;
		
		theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] += pSTEPArray->m_STEPOffset[0];
		theApp.m_pDevArray[dIndex]->m_STEPOld[nSTEP] = theApp.m_pDevArray[dIndex]->m_STEP[nSTEP];
		theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;

//		CString str;
//		str.Format("nStep = %d : %d\n", nSTEP, theApp.m_pDevArray[dIndex]->m_STEP[nSTEP]);
//		TRACE (str);

	} else if (!pSTEPArray->m_VarTokenName.IsEmpty()) {
		// display sim data
		if (pSTEPArray->m_Sections < 1) return;

		double SimValue = 0;
		UINT vIndex = pSTEPArray->m_VarTokenIndex;
		UINT vSimType = pSTEPArray->m_SimType;

		switch (vSimType) {
			case SIM_XPLANE:
				if (vIndex == 0 || theApp.m_pXPFlightData == NULL)
					break;
				theApp.m_pXPFlightData->Data[vIndex].Active = true;
				SimValue = theApp.m_pXPFlightData->Data[vIndex].dDataRead;
			break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				if (vIndex == 0)
					break;
				theApp.m_IL2FlightData.Data[vIndex].Active = true;
				SimValue = theApp.m_IL2FlightData.Data[vIndex].dDataRead;
			break;

			case SIM_F4BMS:
			case SIM_F4USIM:
				SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
														theApp.m_pF4VarToken[vIndex]);
			break;

			case SIM_GTR:
			case SIM_GTR2:
			case SIM_EVO:
			case SIM_GTL:
			case SIM_RACE:
			case SIM_RACE07:
			case SIM_RACEON:
			case SIM_VOLVO:
			case SIM_SIMBIN:
				SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
														theApp.m_pSimBinVarToken[vIndex]);
			break;

			case SIM_RF:
				SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
														theApp.m_pRFVarToken[vIndex]);
			break;

			case SIM_GPX:
				SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
														theApp.m_pGPXVarToken[vIndex]);
			break;

			case SIM_LFS:
				SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
														theApp.m_pLFSVarToken[vIndex]);
			break;

			case USIM_INPUT_FLAGS:
				SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
			break;
		}

		int ErrorPosition;
		CString strErrortext;
		strErrortext.Empty();
		CFormulaParser FormulaParser;
		CString sExpression;

		for (int i=0; i<pSTEPArray->m_Sections; i++) {
			sExpression = pSTEPArray->m_Formula[i];
			if (!sExpression.IsEmpty()) { 
				SimValue = FormulaParser.Calculation(	sExpression, 
														SimValue, 
														ErrorPosition, 
														strErrortext);
			}

			if (SimValue >= pSTEPArray->m_MinValue[i] && SimValue <= pSTEPArray->m_MaxValue[i]) {
				divisor = pSTEPArray->m_MaxValue[i] - pSTEPArray->m_MinValue[i];
				SimValue -= pSTEPArray->m_MinValue[i];
				if (divisor != 0) {

					sFactor = pSTEPArray->m_STEPMax[i] - pSTEPArray->m_STEPMin[i];

					theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = (USHORT)((SimValue / divisor)*sFactor);
					if (pSTEPArray->m_MinValue[i] < 0)
						theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = (USHORT)(theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] - pSTEPArray->m_STEPMin[i]);
					else
						theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = (USHORT)(theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] + pSTEPArray->m_STEPMin[i]);
				}
				break;
			}
		}

		if (SimValue < pSTEPArray->m_MinValue[0]) {
			theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = (USHORT)pSTEPArray->m_STEPMin[0];
		} else if (SimValue > pSTEPArray->m_MaxValue[pSTEPArray->m_Sections-1]) {
			theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] = (USHORT)pSTEPArray->m_STEPMax[pSTEPArray->m_Sections-1];
		}

		theApp.m_pDevArray[dIndex]->m_STEP[nSTEP] += pSTEPArray->m_STEPOffset[0];
		if (theApp.m_pDevArray[dIndex]->m_STEPOld[nSTEP] != theApp.m_pDevArray[dIndex]->m_STEP[nSTEP]) {
			theApp.m_pDevArray[dIndex]->m_STEPOld[nSTEP] = theApp.m_pDevArray[dIndex]->m_STEP[nSTEP];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
			strErrortext.Format("STEP - in %.2f out %.2f\n",(float)SimValue, (float)theApp.m_pDevArray[dIndex]->m_STEP[nSTEP]);
			TRACE (strErrortext);
		}

	}
}

void COutputThread::UpdateSPI(UINT dIndex, UINT nPort)
{
	static int nHeading = 0;
	static BOOL	DirFlag = true;
	UINT StartSPI = (MAX_SPI/MAX_PORTS)*nPort;
	UINT EndSPI = (MAX_SPI/MAX_PORTS)*(nPort+1);

	theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

	if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_SPIFlagsArray[nPort])) return;

	// loop through dacs
	for (UINT nSPI = StartSPI; nSPI < EndSPI; nSPI++) {
		// Single SPI control
		UpdateSingleSPI(&theApp.m_pDevArray[dIndex]->m_SPIArray[nSPI], 
						dIndex, 
						nPort, 
						nSPI);
	}

	if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
		// write to DACs
		WriteSPIDAC(	&theApp.m_pDevArray[dIndex]->m_DeviceParam,
						(UCHAR)nPort, 
						(DAC_OUTPUTS *)&theApp.m_pDevArray[dIndex]->m_SPI[StartSPI],
						7);
	}
}

void COutputThread::UpdateSingleSPI(CSPIPage *pSPIArray, UINT dIndex, UINT nPort, UINT nSPI)
{
	static BOOL	DirFlag = true;
	static int nHeading = 0;
	double divisor;
	long sFactor;
	USHORT m_SPIMin, m_SPIMax;

	// Single SPI control
	if (pSPIArray->m_SPICalibrate) {
		// Calibrate SPI
		theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = pSPIArray->m_SPITemp;

		if (theApp.m_pDevArray[dIndex]->m_SPIOld[nSPI] != theApp.m_pDevArray[dIndex]->m_SPI[nSPI]) {
			theApp.m_pDevArray[dIndex]->m_SPIOld[nSPI] = theApp.m_pDevArray[dIndex]->m_SPI[nSPI];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}
	} else if (pSPIArray->m_SPITest && pSPIArray->m_Sections > 0) {
		// Test SPIs by looping value 360 degrees

		if (DirFlag) nHeading += 10;
		else nHeading -= 10;

		int Dactemp = 0;

		if (pSPIArray->m_SPIMin[0] > pSPIArray->m_SPIMax[pSPIArray->m_Sections-1]) {
			m_SPIMax = (USHORT)pSPIArray->m_SPIMin[0];
			m_SPIMin = (USHORT)pSPIArray->m_SPIMax[pSPIArray->m_Sections-1];
		} else {
			m_SPIMax = (USHORT)pSPIArray->m_SPIMax[pSPIArray->m_Sections-1];
			m_SPIMin = (USHORT)pSPIArray->m_SPIMin[0];
		}

		for (int i=0; i<pSPIArray->m_Sections-1; i++) {
			if (pSPIArray->m_SPIMax[i] > pSPIArray->m_SPIMin[i+1]) {
				if (nHeading > (int)pSPIArray->m_SPIMax[i] && nHeading < (int)pSPIArray->m_SPIMin[i+1]) {
					Dactemp = pSPIArray->m_SPIMax[i];
					break;
				}
			} else {
				if (nHeading < (int)pSPIArray->m_SPIMax[i] && nHeading > (int)pSPIArray->m_SPIMin[i+1]) {
					Dactemp = pSPIArray->m_SPIMax[i];
					break;
				}
			}
		}

		if (nHeading < (int)m_SPIMin) {
			nHeading = m_SPIMin;
			DirFlag = true;
		}

		if (nHeading > (int)m_SPIMax) {
			nHeading = m_SPIMax;
			DirFlag = false;
		}

		if (Dactemp != 0)
			theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)Dactemp;
		else
			theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)nHeading;
		
		theApp.m_pDevArray[dIndex]->m_SPI[nSPI] += pSPIArray->m_SPIOffset[0];
		theApp.m_pDevArray[dIndex]->m_SPIOld[nSPI] = theApp.m_pDevArray[dIndex]->m_SPI[nSPI];
		theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;

	} else if (!pSPIArray->m_VarTokenName.IsEmpty()) {
		// display sim data
		if (pSPIArray->m_Sections < 1) return;

		double SimValue = 0;
		UINT vIndex = pSPIArray->m_VarTokenIndex;
		UINT vSimType = pSPIArray->m_SimType;

		switch (vSimType) {
			case SIM_XPLANE:
				if (vIndex == 0 || theApp.m_pXPFlightData == NULL)
					break;
				theApp.m_pXPFlightData->Data[vIndex].Active = true;
				SimValue = theApp.m_pXPFlightData->Data[vIndex].dDataRead;
			break;

			case SIM_IL2:
			case SIM_IL2FB:
			case SIM_IL2PF:
				if (vIndex == 0)
					break;
				theApp.m_IL2FlightData.Data[vIndex].Active = true;
				SimValue = theApp.m_IL2FlightData.Data[vIndex].dDataRead;
			break;

			case SIM_F4BMS:
			case SIM_F4USIM:
				SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
														theApp.m_pF4VarToken[vIndex]);
			break;

			case SIM_GTR:
			case SIM_GTR2:
			case SIM_EVO:
			case SIM_GTL:
			case SIM_RACE:
			case SIM_RACE07:
			case SIM_RACEON:
			case SIM_VOLVO:
			case SIM_SIMBIN:
				SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
														theApp.m_pSimBinVarToken[vIndex]);
			break;

			case SIM_RF:
				SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
														theApp.m_pRFVarToken[vIndex]);
			break;

			case SIM_GPX:
				SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
														theApp.m_pGPXVarToken[vIndex]);
			break;

			case SIM_LFS:
				SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
														theApp.m_pLFSVarToken[vIndex]);
			break;

			case USIM_INPUT_FLAGS:
				SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
			break;
		}

		int ErrorPosition;
		CString strErrortext;
		strErrortext.Empty();
		CFormulaParser FormulaParser;
		CString sExpression;

		for (int i=0; i<pSPIArray->m_Sections; i++) {
			sExpression = pSPIArray->m_Formula[i];
			if (!sExpression.IsEmpty()) { 
				SimValue = FormulaParser.Calculation(	sExpression, 
														SimValue, 
														ErrorPosition, 
														strErrortext);
			}

			if (SimValue >= pSPIArray->m_MinValue[i] && SimValue <= pSPIArray->m_MaxValue[i]) {
				divisor = pSPIArray->m_MaxValue[i] - pSPIArray->m_MinValue[i];
				SimValue -= pSPIArray->m_MinValue[i];
				if (divisor != 0) {

					sFactor = pSPIArray->m_SPIMax[i] - pSPIArray->m_SPIMin[i];

					theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)((SimValue / divisor)*sFactor);
					if (pSPIArray->m_MinValue[i] < 0)
						theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)(theApp.m_pDevArray[dIndex]->m_SPI[nSPI] - pSPIArray->m_SPIMin[i]);
					else
						theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)(theApp.m_pDevArray[dIndex]->m_SPI[nSPI] + pSPIArray->m_SPIMin[i]);
				}
				break;
			}
		}

		if (SimValue < pSPIArray->m_MinValue[0]) {
			theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)pSPIArray->m_SPIMin[0];
		} else if (SimValue > pSPIArray->m_MaxValue[pSPIArray->m_Sections-1]) {
			theApp.m_pDevArray[dIndex]->m_SPI[nSPI] = (USHORT)pSPIArray->m_SPIMax[pSPIArray->m_Sections-1];
		}

		theApp.m_pDevArray[dIndex]->m_SPI[nSPI] += pSPIArray->m_SPIOffset[0];
		if (theApp.m_pDevArray[dIndex]->m_SPIOld[nSPI] != theApp.m_pDevArray[dIndex]->m_SPI[nSPI]) {
			theApp.m_pDevArray[dIndex]->m_SPIOld[nSPI] = theApp.m_pDevArray[dIndex]->m_SPI[nSPI];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
			strErrortext.Format("SPI - in %.2f out %.2f\n",(float)SimValue, (float)theApp.m_pDevArray[dIndex]->m_SPI[nSPI]);
			TRACE (strErrortext);
		}

	}
}
/*
void COutputThread::Update12BITDAC(UINT dIndex, UINT nPort)
{
	static int nHeading = 0;
	static BOOL	DirFlag = true;
	DAC_OUTPUTS DACData;

	UINT StartDAC = (MAX_DACS/MAX_PORTS)*nPort;
	UINT EndDAC = (MAX_DACS/MAX_PORTS)*(nPort+1);

	theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

	if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_DACFlagsArray[nPort])) return;

	// loop through dacs
	for (UINT nDAC = StartDAC; nDAC < EndDAC; nDAC++) {
		// Single DAC control
		UpdateSingleSPI(&theApp.m_pDevArray[dIndex]->m_DACArray[nDAC], 
						dIndex, 
						nPort, 
						nDAC);
	}

	DACData.DAC_1 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[0]<<4);
	DACData.DAC_2 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[1]<<4);
	DACData.DAC_3 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[2]<<4);
	DACData.DAC_4 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[3]<<4);
	DACData.DAC_5 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[4]<<4);
	DACData.DAC_6 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[5]<<4);
	DACData.DAC_7 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[6]<<4);
	DACData.DAC_8 = (USHORT)(theApp.m_pDevArray[dIndex]->m_DACs[7]<<4);

	if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
		// write to DACs
		
		WriteSPIDAC(&theApp.m_pDevArray[dIndex]->m_DeviceParam,
					(UCHAR)nPort, 
					&DACData,
					7);
	}
}
*/
void COutputThread::UpdateCrtRWR(UINT dIndex, UINT nPort)
{
	int i, j, k;
	float angle_rad, radius;
	double xPos, yPos;
	RWROUTPUT rwrArray[MAX_RWR_SYMBOLS];
	BOOL FoundFlag;
	unsigned char RWR_Page;

	theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

	if (!theApp.m_pDevArray[dIndex]->m_CrtRwrActive[nPort]) return;
	// if active && set
	RWR_Page = RWRPAGE_NORMAL;
	if (theApp.m_pDevArray[dIndex]->m_CrtRwrArray[nPort].m_RWREnable) {
		if (theApp.m_pDevArray[dIndex]->m_CrtRwrArray[nPort].m_RWRTest == 1) {
			theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt = 1;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].symbol = RWRSYM_TEST;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].xPos = 127;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].yPos = 127;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].missileActivity = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].missileLaunch = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].newguy = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].selected = 0;
			RWR_Page = RWRPAGE_TEST;
		} else if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_CrtRwrFlagsArray[nPort])) {
			theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt = 0;
			RWR_Page = RWRPAGE_BLANK;
		} else if (theApp.m_SimulationModel == SIM_F4BMS) {
			theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt = (UCHAR)theApp.m_F4FlightData.RwrObjectCount;
			for (i=0; i<theApp.m_F4FlightData.RwrObjectCount; i++) {
				theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].symbol = (UCHAR)theApp.m_F4FlightData.RWRsymbol[i];
				theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].missileActivity = theApp.m_F4FlightData.missileActivity[i]&0x01;
				theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].missileLaunch = theApp.m_F4FlightData.missileLaunch[i]&0x01;
				//theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].newguy = 0;
				theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].selected = theApp.m_F4FlightData.selected[i]&0x01;

				angle_rad = theApp.m_F4FlightData.bearing[i] - theApp.m_F4FlightData.yaw;

				if (theApp.m_F4FlightData.lethality[i] > 1.0)
					// 0.95 keeps things from leaking off the edge of the display
					radius = (2.0F - theApp.m_F4FlightData.lethality[i]) * 0.95f;    
				else
					// 0.95 keeps things from leaking off the edge of the display
					radius = (1.0F - theApp.m_F4FlightData.lethality[i]) * 0.95f;

				xPos  = radius * sin(angle_rad);
				 // Flip this display if we are upside down
				xPos *=  ((RADTODEG(fabs(theApp.m_F4FlightData.roll)) > 120.0F) ? -1 : 1);
				xPos +=  1.0;
				yPos  = radius * cos(angle_rad) + 1.0;

				theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].xPos = (UCHAR)(xPos*127);
				theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i].yPos = (UCHAR)(yPos*127);
			}
		} else {
			RWR_Page = RWRPAGE_BLANK;
			theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].symbol = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].xPos = 127;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].yPos = 127;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].missileActivity = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].missileLaunch = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].newguy = 0;
			theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].selected = 0;
		}
	} else {
		RWR_Page = RWRPAGE_BLANK;
		theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt = 0;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].symbol = 0;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].xPos = 127;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].yPos = 127;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].missileActivity = 0;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].missileLaunch = 0;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].newguy = 0;
		theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[0].selected = 0;
	}

	// if changed
	for (i=0; i<theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt; i++) {
		if (memcmp(	&theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i],
					&theApp.m_pDevArray[dIndex]->m_CrtRwrOld[i],
					EF_BYTES_PER_RWR) != 0) {
			// Changed. Save old value

			if (theApp.m_SimulationModel == SIM_F4BMS) {
				// Look for and set newguy data

				for (k=0; k<MAX_RWR_SYMBOLS; k++) {
					rwrArray[k].symbol = 0;
					rwrArray[k].cnt = 0;
				}

				// loop through all old contacts and compile list into table
				for (k=0; k<theApp.m_pDevArray[dIndex]->m_RWRSymbolCntOld; k++) {
					for (j=0; j<MAX_RWR_SYMBOLS; j++) {
						if (rwrArray[j].symbol != 0) {
							if (theApp.m_pDevArray[dIndex]->m_CrtRwrOld[k].symbol == rwrArray[j].symbol) {
								rwrArray[j].cnt++;
								break;
							}
						} else {
							// symbol not found, add new symbol to table
							rwrArray[j].symbol = theApp.m_pDevArray[dIndex]->m_CrtRwrOld[k].symbol;
							rwrArray[j].cnt++;
							break;
						}
					}
				}

				// loop through all new contacts and find newguy contacts
				for (k=0; k<theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt; k++) {
					FoundFlag = false;
					for (j=0; j<MAX_RWR_SYMBOLS; j++) {
						if (rwrArray[j].symbol == 0) break;
						if (theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[k].symbol == rwrArray[j].symbol) {
							if (rwrArray[j].cnt > 0) {
								rwrArray[j].cnt--;
								FoundFlag = true;
							}
							break;
						}
					}

					if (!FoundFlag) { 					
						// symbol not found, add newguy contact
						theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[k].newguy = 1;
					}
				}

			}

			memcpy(	&theApp.m_pDevArray[dIndex]->m_CrtRwrOld[i],
					&theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs[i],
					EF_BYTES_PER_RWR);
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}
	}

	if (theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt != theApp.m_pDevArray[dIndex]->m_RWRSymbolCntOld) {
		theApp.m_pDevArray[dIndex]->m_RWRSymbolCntOld = theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt;
		theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
	}

	if (theApp.m_pDevArray[dIndex]->m_PortChange[nPort]) {
		WriteCRTRWR(&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
					(UCHAR)nPort,
					RWR_Page,									// RWR Page
					theApp.m_pDevArray[dIndex]->m_RWRSymbolCnt,	// Total number of contacts
					theApp.m_pDevArray[dIndex]->m_CrtRwrOutputs);
	}
}

BOOL COutputThread::CheckLCDPageFlags(CFlagsPage *pFlagsArray)
{
	for (int i=0; i<MAX_INPUT_FLAGS; i++) {
		if (pFlagsArray->Active[i] && 
			!theApp.m_UserVarsArray.m_IOFlags[i].FlagValue ) 
			return false;
	}

	return true;
}

void COutputThread::UpdateLCD(CCLCDPage *LCDArray, UINT dIndex, UINT nPort, UINT nMode)
{
	CFlagsPage *pFlagsArray;

	theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = false;

	if (LCDArray->m_LCDTest != LCDArray->m_LCDTestOld || LCDArray->m_LCDInvert != LCDArray->m_LCDInvertOld) {
		LCDArray->m_LCDTestOld = LCDArray->m_LCDTest;
		LCDArray->m_LCDInvertOld = LCDArray->m_LCDInvert;
		if (LCDArray->m_LCDTest) {
			if (nMode == MODE_CLCD) {
				CmdLCD( &theApp.m_pDevArray[dIndex]->m_DeviceParam,
						0, 
						0, 
						&LCDArray->m_LCDLinesOld[0][0],
						LCD_TEST);
			} else if (nMode == MODE_GLCD) {
				CmdGLCD(&theApp.m_pDevArray[dIndex]->m_DeviceParam,
						(UCHAR)nPort, 
						LCDArray->m_LCDInvert, 
						GLCD_TEST);
			}
			return;
		} else {
			// clear in order to force restoring page after test
			memset(&LCDArray->m_LCDLinesOld[0][0], 0xFF, 40);
			memset(&LCDArray->m_LCDLinesOld[1][0], 0xFF, 40);
			memset(&LCDArray->m_LCDLinesOld[2][0], 0xFF, 40);
			memset(&LCDArray->m_LCDLinesOld[3][0], 0xFF, 40);
			memset(&LCDArray->m_LCDLinesOld[4][0], 0xFF, 40);
		}
	} else if (LCDArray->m_LCDTest) return;

	if (theApp.m_SimulationModel < SIM_XPLANE) {
		memset(theApp.m_F4FlightData.DEDLines, 0x20, 5*26);
		memset(theApp.m_F4FlightData.Invert, 0, 5*26); 
		memset(theApp.m_F4FlightData.PFLLines, 0x20, 5*26);
		memset(theApp.m_F4FlightData.PFLInvert, 0, 5*26);
	}

	if (nMode == MODE_CLCD)
		pFlagsArray = &theApp.m_pDevArray[dIndex]->m_CLCDFlagsArray[nPort];
	else
		pFlagsArray = &theApp.m_pDevArray[dIndex]->m_GLCDFlagsArray[nPort];

	if (!CheckFlags(pFlagsArray)) {
		// Blank LCD
		memset(&LCDArray->m_LCDLines[0][0], 0x20, 40);
		memset(&LCDArray->m_LCDLines[1][0], 0x20, 40);
		memset(&LCDArray->m_LCDLines[2][0], 0x20, 40);
		memset(&LCDArray->m_LCDLines[3][0], 0x20, 40);
		memset(&LCDArray->m_LCDLines[4][0], 0x20, 40);

		// check if any changes occured and write output to LCD if changed
		long retval = 0;
		for (UCHAR nLine=0; nLine<GLCD_MAX_LINES; nLine++) {
			if (memcmp((void *)LCDArray->m_LCDLines[nLine], (void *)LCDArray->m_LCDLinesOld[nLine], 40) != 0) {
				// buffer has changed
				if (nMode == MODE_CLCD && nLine < LCD_MAX_LINES) {
					// write line to LCD
					retval = WriteLCD (	&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
										0, 
										UCHAR(nLine + 1), 
										&LCDArray->m_LCDLines[nLine][0],
										0);
				} else if (nMode == MODE_GLCD) {
					retval = WriteGLCD(	&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
										(UCHAR)nPort, 
										nLine,
										1, 
										LCDArray->m_LCDInvert, 
										&LCDArray->m_LCDLines[nLine][0]);
				}

				if (retval != 0) {
					memcpy(&LCDArray->m_LCDLinesOld[nLine][0], &LCDArray->m_LCDLines[nLine][0], 40);
				}
			}
		}
	} else {
		FormatLCD(LCDArray, dIndex, nPort, nMode);
	}
}

void COutputThread::FormatLCD(CCLCDPage *LCDArray, UINT dIndex, UINT nPort, UINT nMode)
{
	UINT Idx0;
	UCHAR nLine, nPage;
	CString tmpStr;
	UCHAR FlagPrev, FlagNext;
	char LCDCurrentPage;
	long retval;

	// determine which page we need to check
	LCDCurrentPage = LCDArray->m_LCDCurrentPage;
	if (LCDArray->m_LCDPageEnable[LCDCurrentPage]) {
		// Page enabled & set

		// check if any flags have toggled
		if (LCDArray->m_LCDRadio == 0) {
			// using prev/next flags
			FlagPrev = 0;
			if (LCDArray->m_LCDPagePrev > 0)
				FlagPrev = theApp.m_UserVarsArray.m_IOFlags[(LCDArray->m_LCDPagePrev - 1)].FlagValue;
			FlagNext = 0;
			if (LCDArray->m_LCDPageNext > 0)
				FlagNext = theApp.m_UserVarsArray.m_IOFlags[(LCDArray->m_LCDPageNext - 1)].FlagValue;

			if (LCDArray->m_FlagNextOld != FlagNext && FlagNext) {
				// find next enabled page
				LCDCurrentPage += 1;
				for (nPage = LCDCurrentPage; nPage < LCD_MAX_PAGES; nPage++) {
					if (LCDArray->m_LCDPageEnable[nPage] == 1) {
						LCDCurrentPage = nPage;
						LCDArray->m_LCDCurrentPage = LCDCurrentPage;
						break;
					}
				}

				if (LCDCurrentPage != LCDArray->m_LCDCurrentPage) {
					// next enabled page not found. Loop and start from first.
					for (nPage = 0; nPage <= LCDCurrentPage; nPage++) {
						if (LCDArray->m_LCDPageEnable[nPage] == 1) {
							LCDCurrentPage = nPage;
							LCDArray->m_LCDCurrentPage = LCDCurrentPage;
							break;
						}
					}
				}
			} else if (LCDArray->m_FlagPrevOld != FlagPrev && FlagPrev) {
				// find previous enabled page
				LCDCurrentPage -= 1;
				for (nPage = LCDCurrentPage; nPage >= 0; nPage--) {
					if (LCDArray->m_LCDPageEnable[nPage] == 1) {
						LCDCurrentPage = nPage;
						LCDArray->m_LCDCurrentPage = LCDCurrentPage;
						break;
					}
				}

				if (LCDCurrentPage != LCDArray->m_LCDCurrentPage) {
					// prev enabled page not found. Loop and start from end.
					for (nPage = LCD_MAX_PAGES; nPage >= LCDCurrentPage; nPage--) {
						if (LCDArray->m_LCDPageEnable[nPage] == 1) {
							LCDCurrentPage = nPage;
							LCDArray->m_LCDCurrentPage = LCDCurrentPage;
							break;
						}
					}
				}
			}

			LCDArray->m_FlagNextOld = FlagNext;
			LCDArray->m_FlagPrevOld = FlagPrev;
		} else {
			// using set page flags
			// loop though pages and find which flag toggled.
			for (nPage = 0; nPage < LCD_MAX_PAGES; nPage++) {
				// set current page to toggled flag page.
				if (LCDArray->m_LCDPageEnable[nPage] == 1) {
					if (LCDArray->m_LCDPageDisplay[nPage] > 0 &&
						theApp.m_UserVarsArray.m_IOFlags[(LCDArray->m_LCDPageDisplay[nPage] - 1)].FlagValue == 1) {
						LCDCurrentPage = LCDArray->m_LCDCurrentPage = nPage;
						break;
					}
				}
			}
		}
	} else {
		// find first available page that is enabled
		LCDCurrentPage = LCDArray->m_LCDCurrentPage = 0;
		for (nPage = 0; nPage < LCD_MAX_PAGES; nPage++) {
			if (LCDArray->m_LCDPageEnable[nPage] == 1) {
				LCDCurrentPage = LCDArray->m_LCDCurrentPage = nPage;
				break;
			}
		}
	}

	if (LCDCurrentPage > 9 || LCDCurrentPage < 0) LCDCurrentPage = 0;

	// parse LCD lines and format output
	if (LCDArray->m_LCDPageEnable[LCDCurrentPage] == 1) {
		for (nLine = 0; nLine < GLCD_MAX_LINES; nLine++) {
			tmpStr = ParseLCDText(LCDArray->m_LCDPages[LCDCurrentPage][nLine], LCDArray->m_DEDEnable);

			memcpy(&LCDArray->m_LCDLines[nLine][0], tmpStr, 40);
			Idx0 = strlen(LCDArray->m_LCDLines[nLine]);
			if (Idx0 > 40) Idx0 = 40;
			memset(&LCDArray->m_LCDLines[nLine][Idx0], 0x20, 40 - Idx0);

			// check if any changes occured and write output to LCD if changed
			if (memcmp(&LCDArray->m_LCDLines[nLine][0], &LCDArray->m_LCDLinesOld[nLine][0], 40) != 0) {

				// buffer has changed
				retval = 0;
				if (nMode == MODE_CLCD && nLine < LCD_MAX_LINES) {
					// write line to LCD
					retval = WriteLCD (	&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
								0, 
								UCHAR(nLine + 1), 
								&LCDArray->m_LCDLines[nLine][0],
								0xFF);
				} else if (nMode == MODE_GLCD) {
					retval = WriteGLCD(	&theApp.m_pDevArray[dIndex]->m_DeviceParam, 
										(UCHAR)nPort, 
										nLine,
										1, 
										LCDArray->m_LCDInvert, 
										&LCDArray->m_LCDLines[nLine][0]);
				}
				if (retval != 0) {
					memcpy(&LCDArray->m_LCDLinesOld[nLine][0], &LCDArray->m_LCDLines[nLine][0], 40);
				}
			}
		}
	}
}

CString COutputThread::ParseLCDText(CString lpszSource, UCHAR nDEDMode)
{
	UINT vIndex;
	int Idx0, Idx1, Idx2, vartype;
	CString strSub1, strSub2, strFmt, strSimName, lpszParsed, sSpecifier;
	CString sExpression;
	double SimValue = 0;
	char cSimValue[LCD_MAX_CHARS];

	lpszParsed.Empty();
	Idx0 = 0;
	Idx1 = 0;
	if (lpszSource.IsEmpty())
		Idx0 = -1;

	while (Idx0 != -1) {
		sExpression.Empty(); 
		vartype = 0;
		Idx0 = lpszSource.Find('$', Idx0);
		if (Idx0 != -1) {
			// 1st $ found
			strSub1 = lpszSource.Mid(Idx1, Idx0 - Idx1);

			// Extract Format from substring
			Idx0 += 1;
			Idx1 = lpszSource.Find(',', Idx0);
			if (Idx1 != -1) {
				// ' ' found
				strFmt = lpszSource.Mid(Idx0, Idx1 - Idx0);
				Idx0 = Idx1 + 1;

				// extract math function if any.
				Idx1 = 0;
				Idx1 = strFmt.Find('(');
				if (Idx1 != -1) {
					// '(' found
					Idx2 = strFmt.ReverseFind(')');
					if (Idx2 != -1) {
						// ')' found
						sExpression = strFmt.Mid(Idx1+1, Idx2-Idx1-1);
					}
					strFmt = strFmt.Mid(0, Idx1);
				}
			}

			// Extract Variable from substring
			Idx1 = lpszSource.Find('$', Idx0);
			if (Idx1 != -1) {
				// 2nd $ found
				strSub2 = lpszSource.Mid(Idx0, Idx1 - Idx0);
				Idx1 += 1;
				Idx0 = Idx1;
				
				// Find strSub2 from variable lists
				// extrac sim type
				SimValue = 0;
				memset(cSimValue, 0, LCD_MAX_CHARS);
				if (AfxExtractSubString(strSimName, strSub2, 0, '/')) {
					if (strSimName == "sim") {
						for (vIndex = 0; vIndex < ArraySize(XPDataArray); vIndex++) {
							if (XPDataArray[vIndex].VarTokenName == strSub2 && theApp.m_pXPFlightData != NULL) {
								theApp.m_pXPFlightData->Data[vIndex].Active = true;
								vartype = XPDataArray[vIndex].VarType;
								SimValue = VarTypeConversion (	XPDataArray[vIndex].VarType,
																&theApp.m_pXPFlightData->Data[vIndex].dDataRead);
								break;
							}
						}
					} else if (strSimName == "IL2") {
						for (vIndex = 0; vIndex < ArraySize(IL2DataArray); vIndex++) {
							if (IL2DataArray[vIndex].VarTokenName == strSub2) {
								theApp.m_IL2FlightData.Data[vIndex].Active = true;
								vartype = IL2DataArray[vIndex].VarType;
								SimValue = VarTypeConversion (	IL2DataArray[vIndex].VarType,
																&theApp.m_IL2FlightData.Data[vIndex].dDataRead);
								break;
							}
						}
					} else if (strSimName == "Falcon_4") {
						for (vIndex = 0; vIndex < ArraySize(F4DataArray); vIndex++) {
							if (F4DataArray[vIndex].VarTokenName == strSub2 && theApp.m_pF4VarToken[vIndex] != NULL) {
								vartype = F4DataArray[vIndex].VarType;
								memset (cSimValue, 0, LCD_MAX_CHARS);
								strncpy(cSimValue, (char *)theApp.m_pF4VarToken[vIndex], 25);
								SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
																theApp.m_pF4VarToken[vIndex]);

								if (nDEDMode) {
									UINT nLine = 5;
									if (strstr(F4DataArray[vIndex].VarTokenName, "DED_Line_1"))
										nLine = 0;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "DED_Line_2"))
										nLine = 1;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "DED_Line_3"))
										nLine = 2;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "DED_Line_4"))
										nLine = 3;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "DED_Line_5"))
										nLine = 4;

									if (nLine < 5) {
										for (UINT j = 1; j < 25; j++) {
											if (theApp.m_F4FlightData.Invert[nLine][j] == 0x02) {
												cSimValue[j-1] = (char)(0x80 | cSimValue[j]);
											} else {
												cSimValue[j-1] = cSimValue[j];
											}
										}
										break;
									}

									if (strstr(F4DataArray[vIndex].VarTokenName, "PFL_Line_1"))
										nLine = 0;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "PFL_Line_2"))
										nLine = 1;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "PFL_Line_3"))
										nLine = 2;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "PFL_Line_4"))
										nLine = 3;
									else if (strstr(F4DataArray[vIndex].VarTokenName, "PFL_Line_5"))
										nLine = 4;

									if (nLine < 5) {
										for (UINT j = 1; j < 25; j++) {
											if (theApp.m_F4FlightData.PFLInvert[nLine][j] == 0x02) {
												cSimValue[j-1] = (char)(0x80 | cSimValue[j]);
											} else {
												cSimValue[j-1] = cSimValue[j];
											}
										}
									}
								}

								break;
							}
						}
					} else if (strSimName == "SimBin") {
						for (vIndex = 0; vIndex < ArraySize(SimBinDataArray); vIndex++) {
							if (SimBinDataArray[vIndex].VarTokenName == strSub2 && theApp.m_pSimBinVarToken[vIndex] != NULL) {
								vartype = SimBinDataArray[vIndex].VarType;
								strncpy(cSimValue, (char *)theApp.m_pSimBinVarToken[vIndex], LCD_MAX_CHARS);
								SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
																theApp.m_pSimBinVarToken[vIndex]);
								break;
							}
						}
					} else if (strSimName == "rFactor") {
						for (vIndex = 0; vIndex < ArraySize(RFDataArray); vIndex++) {
							if (RFDataArray[vIndex].VarTokenName == strSub2 && theApp.m_pRFVarToken[vIndex] != NULL) {
								vartype = RFDataArray[vIndex].VarType;
								strncpy(cSimValue, (char *)theApp.m_pRFVarToken[vIndex], LCD_MAX_CHARS);
								SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
																theApp.m_pRFVarToken[vIndex]);
								break;
							}
						}
					} else if (strSimName == "GPx") {
						for (vIndex = 0; vIndex < ArraySize(GPXDataArray); vIndex++) {
							if (GPXDataArray[vIndex].VarTokenName == strSub2 && theApp.m_pGPXVarToken[vIndex] != NULL) {
								vartype = GPXDataArray[vIndex].VarType;
								strncpy(cSimValue, (char *)theApp.m_pGPXVarToken[vIndex], LCD_MAX_CHARS);
								SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
																theApp.m_pGPXVarToken[vIndex]);
								break;
							}
						}
					} else if (strSimName == "LFS") {
						for (vIndex = 0; vIndex < ArraySize(LFSDataArray); vIndex++) {
							if (LFSDataArray[vIndex].VarTokenName == strSub2 && theApp.m_pLFSVarToken[vIndex] != NULL) {
								vartype = LFSDataArray[vIndex].VarType;
								strncpy(cSimValue, (char *)theApp.m_pLFSVarToken[vIndex], LCD_MAX_CHARS);
								SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
																theApp.m_pLFSVarToken[vIndex]);
								break;
							}
						}
					} else if (strSimName == "Input_Flags") {
						AfxExtractSubString(strSub2, strSub2, 1, '/');
						for (vIndex = 0; vIndex < MAX_INPUT_FLAGS; vIndex++) {
							if (theApp.m_UserVarsArray.m_IOFlags[vIndex].VarTokenString == strSub2) {
								vartype = xplmType_Byte;
								SimValue = VarTypeConversion(	xplmType_Byte,
																(double *)&theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue);
								break;
							}
						}
					}
				}
			}
		} else {
			// Check to see if we have text at end of line after last #var#
			strSub1 = lpszSource.Mid(Idx1, lpszSource.GetLength());
		}

		// Create final formated text
		lpszParsed += strSub1;
		sSpecifier = strFmt.Right(1);
//		strFmt = lpszParsed + strFmt;

		int ErrorPosition;
		CString strErrortext;
		strErrortext.Empty();
		CFormulaParser FormulaParser;

		if (!sExpression.IsEmpty()) { 
			SimValue = FormulaParser.Calculation(	sExpression, 
													SimValue, 
													ErrorPosition, 
													strErrortext);
		}

		if (sSpecifier == "c")
			Format(strSub1, strFmt, (char)SimValue);
		else if (sSpecifier == "d" || sSpecifier == "i")
			Format(strSub1, strFmt, (int)SimValue);
		else if (sSpecifier == "e" || sSpecifier == "E")
			Format(strSub1, strFmt, SimValue);
		else if (sSpecifier == "f")
			Format(strSub1, strFmt, SimValue);
		else if (sSpecifier == "g" || sSpecifier == "G")
			Format(strSub1, strFmt, SimValue);
		else if (sSpecifier == "o")
			Format(strSub1, strFmt, (long)SimValue);
		else if (sSpecifier == "s")
			Format(strSub1, strFmt, cSimValue);
		else if (sSpecifier == "u")
			Format(strSub1, strFmt,(DWORD)SimValue);
		else if (sSpecifier == "x" || sSpecifier == "X")
			Format(strSub1, strFmt,(DWORD)SimValue);
		else
			Format(strSub1, strFmt, SimValue);

		lpszParsed += strSub1;
		strFmt.Empty();
	} // while (Idx0 != -1)

	return lpszParsed;
}

void COutputThread::ReadDirectOutputs(UINT dIndex, UINT nPort)
{
	UINT	oIndex, vIndex, vIndex2, vSimType, start_index, end_index;
	double	SimValue, SimValue2;

	start_index = nPort*EEEF_NUM_OUTPUTS_PER_PORT;
	end_index = nPort*EEEF_NUM_OUTPUTS_PER_PORT + EEEF_NUM_OUTPUTS_PER_PORT;

	for (oIndex = start_index; oIndex < end_index; oIndex++) {	
		// if active && set
		if (theApp.m_pDevArray[dIndex]->m_OutputActive[oIndex] && 
			theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].IsSet() &&
			theApp.m_SimulationModel != SIM_NONE) {

			SimValue = 0;
			SimValue2 = 0;

			vIndex = theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_VarTokenIndex;
			vIndex2 = theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_VarBlinkTokenIndex;
			vSimType = theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_SimType;

			switch (vSimType) {
				case SIM_XPLANE:
					if (vIndex == 0 || theApp.m_pXPFlightData == NULL)
						break;
					theApp.m_pXPFlightData->Data[vIndex].Active = true;
					SimValue = VarTypeConversion (	XPDataArray[vIndex].VarType,
													&theApp.m_pXPFlightData->Data[vIndex].dDataRead);
					if (vIndex2 == 0)
						break;
					theApp.m_pXPFlightData->Data[vIndex2].Active = true;
					SimValue2 = VarTypeConversion(XPDataArray[vIndex2].VarType,
						&theApp.m_pXPFlightData->Data[vIndex2].dDataRead);
				break;

				case SIM_IL2:
				case SIM_IL2FB:
				case SIM_IL2PF:
					if (vIndex == 0)
						break;
					theApp.m_IL2FlightData.Data[vIndex].Active = true;
					SimValue = VarTypeConversion (	IL2DataArray[vIndex].VarType,
													&theApp.m_IL2FlightData.Data[vIndex].dDataRead);
					if (vIndex2 == 0)
						break;
					theApp.m_IL2FlightData.Data[vIndex2].Active = true;
					SimValue2 = VarTypeConversion(IL2DataArray[vIndex2].VarType,
						&theApp.m_IL2FlightData.Data[vIndex2].dDataRead);
				break;

				case SIM_F4BMS:
				case SIM_F4USIM:
					if (theApp.m_pF4VarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
													theApp.m_pF4VarToken[vIndex]);
					if (theApp.m_pF4VarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(F4DataArray[vIndex2].VarType,
						theApp.m_pF4VarToken[vIndex2]);
				break;

				case SIM_GTR:
				case SIM_GTR2:
				case SIM_EVO:
				case SIM_GTL:
				case SIM_RACE:
				case SIM_RACE07:
				case SIM_RACEON:
				case SIM_VOLVO:
				case SIM_SIMBIN:
					if (theApp.m_pSimBinVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
													theApp.m_pSimBinVarToken[vIndex]);
					if (theApp.m_pSimBinVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(SimBinDataArray[vIndex2].VarType,
						theApp.m_pSimBinVarToken[vIndex2]);
				break;

				case SIM_RF:
					if (theApp.m_pRFVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
													theApp.m_pRFVarToken[vIndex]);
					if (theApp.m_pRFVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(RFDataArray[vIndex2].VarType,
						theApp.m_pRFVarToken[vIndex2]);
				break;

				case SIM_GPX:
					if (theApp.m_pGPXVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
													theApp.m_pGPXVarToken[vIndex]);
					if (theApp.m_pGPXVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(GPXDataArray[vIndex2].VarType,
						theApp.m_pGPXVarToken[vIndex2]);
				break;

				case SIM_LFS:
					if (theApp.m_pLFSVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
													theApp.m_pLFSVarToken[vIndex]);
					if (theApp.m_pLFSVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(LFSDataArray[vIndex2].VarType,
						theApp.m_pLFSVarToken[vIndex2]);
				break;

				case USIM_INPUT_FLAGS:
					SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
					SimValue2 = theApp.m_UserVarsArray.m_IOFlags[vIndex2].FlagValue;
				break;
			}

			if (CheckConditions(SimValue, SimValue2, &theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex])) {
				// bit ON
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = 1;
			} else {
				// bit OFF
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = 0;
			}

			if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_OutputFlagsArray[oIndex]))
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = 0;

			if (theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_Invert)
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = (BYTE)((~theApp.m_pDevArray[dIndex]->m_Outputs[oIndex])&0x01);

			if (theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_LampTest)
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = 1;
		} else {
			if (theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_Invert)
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = (BYTE)((~theApp.m_pDevArray[dIndex]->m_Outputs[oIndex])&0x01);

			if (theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].m_LampTest)
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = 1;
			else
				theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] = 0;
		} // if (theApp.m_pDevArray[dIndex]->m_OutputActive[oIndex] && theApp.m_pDevArray[dIndex]->m_OutputArray[oIndex].IsSet() )

		// if toggled
		if (theApp.m_pDevArray[dIndex]->m_Outputs[oIndex] != theApp.m_pDevArray[dIndex]->m_OutputsOld[oIndex] &&
			!theApp.m_pDevArray[dIndex]->m_OutputDisabled[oIndex]) {
			// Toggled. Save old value
			theApp.m_pDevArray[dIndex]->m_OutputsOld[oIndex] = theApp.m_pDevArray[dIndex]->m_Outputs[oIndex];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}

		// store in output structure
		if (!theApp.m_pDevArray[dIndex]->m_OutputDisabled[oIndex])
			theApp.m_pDevArray[dIndex]->m_DirectOutputs[oIndex].state = theApp.m_pDevArray[dIndex]->m_Outputs[oIndex];

	} // for (oIndex = 0; oIndex < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberOutputIndices; oIndex++)
}

void COutputThread::ReadLatchedOutputs(UINT dIndex, UINT nPort)
{
	UINT	oIndex, vIndex, vIndex2, vSimType, start_index, end_index;
	double	SimValue, SimValue2;

	start_index = nPort*NUM_LATCHED_PER_PORT;
	end_index = nPort*NUM_LATCHED_PER_PORT + NUM_LATCHED_PER_PORT;

	memset(m_LatchedOutputs, 0, 8);	// clear byte

	for (oIndex = start_index; oIndex < end_index; oIndex++) {	
		// if active && set
		if (theApp.m_pDevArray[dIndex]->m_LOActive[oIndex] && 
			theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].IsSet() &&
			theApp.m_SimulationModel != SIM_NONE) {

			SimValue = 0;
			SimValue2 = 0;

			vIndex = theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_VarTokenIndex;
			vIndex2 = theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_VarBlinkTokenIndex;
			vSimType = theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_SimType;

			switch (vSimType) {
				case SIM_XPLANE:
					if (vIndex == 0 || theApp.m_pXPFlightData == NULL)
						break;
					theApp.m_pXPFlightData->Data[vIndex].Active = true;
					SimValue = VarTypeConversion (	XPDataArray[vIndex].VarType,
													&theApp.m_pXPFlightData->Data[vIndex].dDataRead);

					if (vIndex2 == 0)
						break;
					theApp.m_pXPFlightData->Data[vIndex2].Active = true;
					SimValue2 = VarTypeConversion(XPDataArray[vIndex2].VarType,
						&theApp.m_pXPFlightData->Data[vIndex2].dDataRead);
				break;

				case SIM_IL2:
				case SIM_IL2FB:
				case SIM_IL2PF:
					if (vIndex == 0)
						break;
					theApp.m_IL2FlightData.Data[vIndex].Active = true;
					SimValue = VarTypeConversion (	IL2DataArray[vIndex].VarType,
													&theApp.m_IL2FlightData.Data[vIndex].dDataRead);
					if (vIndex2 == 0)
						break;
					theApp.m_IL2FlightData.Data[vIndex2].Active = true;
					SimValue2 = VarTypeConversion(IL2DataArray[vIndex2].VarType,
						&theApp.m_IL2FlightData.Data[vIndex2].dDataRead);
				break;

				case SIM_F4BMS:
				case SIM_F4USIM:
					if (theApp.m_pF4VarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
													theApp.m_pF4VarToken[vIndex]);

					if (theApp.m_pF4VarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(F4DataArray[vIndex2].VarType,
						theApp.m_pF4VarToken[vIndex2]);
				break;

				case SIM_GTR:
				case SIM_GTR2:
				case SIM_EVO:
				case SIM_GTL:
				case SIM_RACE:
				case SIM_RACE07:
				case SIM_RACEON:
				case SIM_VOLVO:
				case SIM_SIMBIN:
					if (theApp.m_pSimBinVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
													theApp.m_pSimBinVarToken[vIndex]);

					if (theApp.m_pSimBinVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(SimBinDataArray[vIndex2].VarType,
						theApp.m_pSimBinVarToken[vIndex2]);
				break;

				case SIM_RF:
					if (theApp.m_pRFVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
													theApp.m_pRFVarToken[vIndex]);

					if (theApp.m_pRFVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(RFDataArray[vIndex2].VarType,
						theApp.m_pRFVarToken[vIndex2]);
				break;

				case SIM_GPX:
					if (theApp.m_pGPXVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
													theApp.m_pGPXVarToken[vIndex]);

					if (theApp.m_pGPXVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(GPXDataArray[vIndex2].VarType,
						theApp.m_pGPXVarToken[vIndex2]);
				break;

				case SIM_LFS:
					if (theApp.m_pLFSVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
													theApp.m_pLFSVarToken[vIndex]);

					if (theApp.m_pLFSVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(LFSDataArray[vIndex2].VarType,
						theApp.m_pLFSVarToken[vIndex2]);
				break;

				case USIM_INPUT_FLAGS:
					SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
					SimValue2 = theApp.m_UserVarsArray.m_IOFlags[vIndex2].FlagValue;
				break;
			}

			if (CheckConditions(SimValue, SimValue2, &theApp.m_pDevArray[dIndex]->m_LOArray[oIndex])) {
				// bit ON
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = 1;
			} else {
				// bit OFF
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = 0;
			}

			if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_LOFlagsArray[oIndex]))
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = 0;

			if (theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_Invert)
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = (BYTE)((~theApp.m_pDevArray[dIndex]->m_LO[oIndex]) & 0x01);

			if (theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_LampTest)
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = 1;
		} else {
			if (theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_Invert)
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = (BYTE)((~theApp.m_pDevArray[dIndex]->m_LO[oIndex]) & 0x01);

			if (theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].m_LampTest)
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = 1;
			else
				theApp.m_pDevArray[dIndex]->m_LO[oIndex] = 0;
		} // if (theApp.m_pDevArray[dIndex]->m_LOActive[oIndex] && theApp.m_pDevArray[dIndex]->m_LOArray[oIndex].IsSet() )

		// if toggled
		if (theApp.m_pDevArray[dIndex]->m_LO[oIndex] != theApp.m_pDevArray[dIndex]->m_LOOld[oIndex] &&
			!theApp.m_pDevArray[dIndex]->m_LODisabled[oIndex]) {

			// Toggled. Save old value
			theApp.m_pDevArray[dIndex]->m_LOOld[oIndex] = theApp.m_pDevArray[dIndex]->m_LO[oIndex];
			theApp.m_pDevArray[dIndex]->m_LOLevelOld[nPort] = theApp.m_pDevArray[dIndex]->m_LOLevel[nPort];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}

		// store in output structure
		if (!theApp.m_pDevArray[dIndex]->m_LODisabled[oIndex])
			m_LatchedOutputs[(oIndex&0x3F)] = theApp.m_pDevArray[dIndex]->m_LO[oIndex];

	} // for (oIndex = 0; oIndex < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberLOIndices; oIndex++)

	if (theApp.m_pDevArray[dIndex]->m_LOLevel[nPort] != theApp.m_pDevArray[dIndex]->m_LOLevelOld[nPort] &&
		!theApp.m_pDevArray[dIndex]->m_LODisabled[oIndex]) {

		theApp.m_pDevArray[dIndex]->m_LOLevelOld[nPort] = theApp.m_pDevArray[dIndex]->m_LOLevel[nPort];
		theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
	}
}

void COutputThread::ReadMuxOutputs(UINT dIndex, UINT nPort)
{
	UINT	oIndex, vIndex, vIndex2, vSimType, start_index, end_index;
	double	SimValue, SimValue2;

	start_index = nPort*NUM_MUX_PER_PORT;	
	end_index = nPort*NUM_MUX_PER_PORT + NUM_MUX_PER_PORT;

	for (oIndex = start_index; oIndex < end_index; oIndex++) {
		// if active && set
		if (theApp.m_pDevArray[dIndex]->m_MuxActive[oIndex] && 
			theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].IsSet() &&
			theApp.m_SimulationModel != SIM_NONE) {

			SimValue = 0;
			SimValue2 = 0;

			vIndex = theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_VarTokenIndex;
			vIndex2 = theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_VarBlinkTokenIndex;
			vSimType = theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_SimType;

			switch (vSimType) {
				case SIM_XPLANE:
					if (vIndex == 0 || theApp.m_pXPFlightData == NULL)
						break;
					theApp.m_pXPFlightData->Data[vIndex].Active = true;
					SimValue = VarTypeConversion (	XPDataArray[vIndex].VarType, 
													&theApp.m_pXPFlightData->Data[vIndex].dDataRead);
					if (vIndex2 == 0)
						break;
					theApp.m_pXPFlightData->Data[vIndex2].Active = true;
					SimValue = VarTypeConversion(XPDataArray[vIndex2].VarType,
						&theApp.m_pXPFlightData->Data[vIndex2].dDataRead);
				break;

				case SIM_IL2:
				case SIM_IL2FB:
				case SIM_IL2PF:
					if (vIndex == 0)
						break;
					theApp.m_IL2FlightData.Data[vIndex].Active = true;
					SimValue = VarTypeConversion (	IL2DataArray[vIndex].VarType, 
													&theApp.m_IL2FlightData.Data[vIndex].dDataRead);
					if (vIndex2 == 0)
						break;
					theApp.m_IL2FlightData.Data[vIndex2].Active = true;
					SimValue2 = VarTypeConversion(IL2DataArray[vIndex2].VarType,
						&theApp.m_IL2FlightData.Data[vIndex2].dDataRead);
				break;

				case SIM_F4BMS:
				case SIM_F4USIM:
					if (theApp.m_pF4VarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion (	F4DataArray[vIndex].VarType,
													theApp.m_pF4VarToken[vIndex]);
					if (theApp.m_pF4VarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(F4DataArray[vIndex2].VarType,
						theApp.m_pF4VarToken[vIndex2]);
				break;

				case SIM_GTR:
				case SIM_GTR2:
				case SIM_EVO:
				case SIM_GTL:
				case SIM_RACE:
				case SIM_RACE07:
				case SIM_RACEON:
				case SIM_VOLVO:
				case SIM_SIMBIN:
					if (theApp.m_pSimBinVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion (	SimBinDataArray[vIndex].VarType,
													theApp.m_pSimBinVarToken[vIndex]);
					if (theApp.m_pSimBinVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(SimBinDataArray[vIndex2].VarType,
						theApp.m_pSimBinVarToken[vIndex2]);
				break;

				case SIM_RF:
					if (theApp.m_pRFVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion (	RFDataArray[vIndex].VarType,
													theApp.m_pRFVarToken[vIndex]);
					if (theApp.m_pRFVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(RFDataArray[vIndex2].VarType,
						theApp.m_pRFVarToken[vIndex2]);
				break;

				case SIM_GPX:
					if (theApp.m_pGPXVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion (	GPXDataArray[vIndex].VarType,
													theApp.m_pGPXVarToken[vIndex]);
					if (theApp.m_pGPXVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(GPXDataArray[vIndex2].VarType,
						theApp.m_pGPXVarToken[vIndex2]);
				break;

				case SIM_LFS:
					if (theApp.m_pLFSVarToken[vIndex] == NULL)
						break;
					SimValue = VarTypeConversion (	LFSDataArray[vIndex].VarType,
													theApp.m_pLFSVarToken[vIndex]);
					if (theApp.m_pLFSVarToken[vIndex2] == NULL)
						break;
					SimValue2 = VarTypeConversion(LFSDataArray[vIndex2].VarType,
						theApp.m_pLFSVarToken[vIndex2]);
				break;

				case USIM_INPUT_FLAGS:
					SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
					SimValue2 = theApp.m_UserVarsArray.m_IOFlags[vIndex2].FlagValue;
				break;
			}

			if (CheckConditions(SimValue, SimValue2, &theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex])) {
				// bit ON
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 1;
			} else {
				// bit OFF
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 0;
			}

			if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_MuxFlagsArray[oIndex]))
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 0;

			if (theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_Invert)
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = (BYTE)((!theApp.m_pDevArray[dIndex]->m_Mux[oIndex]) & 0x01);

			if (theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_LampTest)
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 1;
		} else {
			if (theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_Invert)
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = (BYTE)((!theApp.m_pDevArray[dIndex]->m_Mux[oIndex]) & 0x01);

			if (theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_LampTest)
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 1;
			else
				theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 0;
		}

		if (theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Override != 
			theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OverrideOld) {
			theApp.m_pDevArray[dIndex]->m_Mux[oIndex] = 0;
			theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OverrideOld = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Override;
		}

		// set output level if changed
		if (theApp.m_pDevArray[dIndex]->m_MuxOutputs[oIndex].level != theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_OBLevel) {
			theApp.m_pDevArray[dIndex]->m_MuxOutputs[oIndex].level = theApp.m_pDevArray[dIndex]->m_MuxArray[oIndex].m_OBLevel;
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}

		// if toggled
		if (theApp.m_pDevArray[dIndex]->m_Mux[oIndex] != theApp.m_pDevArray[dIndex]->m_MuxOld[oIndex] &&
			!theApp.m_pDevArray[dIndex]->m_MuxDisabled[oIndex]) {
			// Toggled. Save old value
			theApp.m_pDevArray[dIndex]->m_MuxOld[oIndex] = theApp.m_pDevArray[dIndex]->m_Mux[oIndex];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}

		// store in output structure
		if (!theApp.m_pDevArray[dIndex]->m_MuxDisabled[oIndex])
			theApp.m_pDevArray[dIndex]->m_MuxOutputs[oIndex].state = theApp.m_pDevArray[dIndex]->m_Mux[oIndex];

	} // for (oIndex = 0; oIndex < theApp.m_pDevArray[dIndex]->m_DeviceParam.NumberOutputIndices; oIndex++)
}

void COutputThread::ReadDisplayOutputs(UINT dIndex, UINT nPort)
{
	UINT vIndex, nDigits, nGrouping, disp_cnt, i, nChars;
	CString sdigits, sgrouping, StrValue;
	CString chr;
	int nDecimal, nRight, nLeft;
	double nMax, nMin, SimValue;
	UINT vSimType;
	double nFactor, nConstant;
	BOOL nLeadingZero, nRaw;
	BOOL VarTypeString;

	if (theApp.m_SimulationModel != SIM_NONE) {
		// set output level if changed
		if (theApp.m_pDevArray[dIndex]->m_DispLevel[nPort] != theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OBLevel) {
			theApp.m_pDevArray[dIndex]->m_DispLevel[nPort] = (UCHAR)theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OBLevel;
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}

		disp_cnt = 0;
		theApp.m_pDevArray[dIndex]->m_StrDisp[nPort] = "";
		if (theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_LampTest == 1) {
			if (theApp.m_pDevArray[dIndex]->m_StrDispOld[nPort] != "8888888")
				theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
			theApp.m_pDevArray[dIndex]->m_StrDisp[nPort] = "8888888";
		} else if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_DisplayFlagsArray[nPort])) {
			theApp.m_pDevArray[dIndex]->m_StrDisp[nPort] = "       ";
		} else if (!theApp.m_pDevArray[dIndex]->m_DispDisabled[nPort])  {
			nChars = 0;
			for (i = 0; i < 7; i++) {
				if (theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Grouping == 0 &&
					theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Digits == 0) {
					break;
				}

				nFactor = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Factor;
				nConstant = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Constant;
				nLeadingZero = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].LeadingZero;
				nDigits = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Digits;
				nGrouping = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Grouping;
				nMax = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Maximum;
				nMin = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Minimum;
				nRaw = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].Raw;
				
				SimValue = 0;
				VarTypeString = false;
				if (!theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].VarTokenName.IsEmpty()) {
					vIndex = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].VarTokenIndex;
					vSimType = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].SimType;

					switch (vSimType) {
						case SIM_XPLANE:
							if (vIndex == 0 || theApp.m_pXPFlightData == NULL)
								break;
							theApp.m_pXPFlightData->Data[vIndex].Active = true;
							SimValue = VarTypeConversion (	XPDataArray[vIndex].VarType,
															&theApp.m_pXPFlightData->Data[vIndex].dDataRead);
						break;

						case SIM_IL2:
						case SIM_IL2FB:
						case SIM_IL2PF:
							if (vIndex == 0)
								break;
							theApp.m_IL2FlightData.Data[vIndex].Active = true;
							SimValue = VarTypeConversion (	IL2DataArray[vIndex].VarType,
															&theApp.m_IL2FlightData.Data[vIndex].dDataRead);
						break;

						case SIM_F4BMS:
						case SIM_F4USIM:
							if (theApp.m_pF4VarToken[vIndex] != NULL)
								SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
																theApp.m_pF4VarToken[vIndex]);
						break;

						case SIM_GTR:
						case SIM_GTR2:
						case SIM_EVO:
						case SIM_GTL:
						case SIM_RACE:
						case SIM_RACE07:
						case SIM_RACEON:
						case SIM_VOLVO:
						case SIM_SIMBIN:
							if (theApp.m_pSimBinVarToken[vIndex] != NULL)
								SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
																theApp.m_pSimBinVarToken[vIndex]);
						break;

						case SIM_RF:
							if (theApp.m_pRFVarToken[vIndex] != NULL)
								SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
																theApp.m_pRFVarToken[vIndex]);
						break;

						case SIM_GPX:
							if (theApp.m_pGPXVarToken[vIndex] != NULL)
								SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
																theApp.m_pGPXVarToken[vIndex]);
						break;

						case SIM_LFS:
							if (theApp.m_pLFSVarToken[vIndex] != NULL)
								SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
																theApp.m_pLFSVarToken[vIndex]);
						break;

						case USIM_INPUT_FLAGS:
							SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
						break;
					}
				} else {
					BOOL bNumeric = TRUE;
					LPCSTR szText = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].UserValue;; // Saves CString's array access overheads

					while (bNumeric && *szText)	{
						if (!isdigit(*szText) && *szText != '-' && *szText != '.')
							bNumeric = FALSE;
						szText++;
					}

					if (bNumeric) {
						SimValue = atof(theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].UserValue);
					} else {
						// Not a number
						VarTypeString = true;
					}
				}

				if (!VarTypeString) {
					SimValue *= nFactor;
					SimValue += nConstant;
					if (SimValue > nMax) SimValue = nMax;
					if (SimValue < nMin) SimValue = nMin;
					
					if (nRaw) {
						union foo {
						  int port;
						  char asc[ sizeof(int) ];
						} a;
						a.port = (int)SimValue;
						theApp.m_pDevArray[dIndex]->m_StrDisp[nPort].Format("%s", a.asc);
					} else {
						if (nLeadingZero)
							StrValue.Format("0000000%.7f", SimValue);
						else
							StrValue.Format("       %.7f", SimValue);

						nDecimal = StrValue.Find(".", 0);
						nLeft = nGrouping;
						nRight = nDecimal - nGrouping;

						if (nRight < 0) {
							nLeft += nRight;
							nRight = 0;
						}

						if (nDigits > 0) nDigits++;
						StrValue = StrValue.Mid(nRight, nLeft + nDigits);
						theApp.m_pDevArray[dIndex]->m_StrDisp[nPort] = StrValue + theApp.m_pDevArray[dIndex]->m_StrDisp[nPort];
					}
				} else {
					StrValue = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Displays[i].UserValue.Right(nGrouping);
					theApp.m_pDevArray[dIndex]->m_StrDisp[nPort] = StrValue + theApp.m_pDevArray[dIndex]->m_StrDisp[nPort];
				}
			} // for (i = 0; i < 7; i++)
		}
	} 
	
	if (theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Override != 
		theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OverrideOld) {
		theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_OverrideOld = theApp.m_pDevArray[dIndex]->m_DisplayArray[nPort].m_Override;
		theApp.m_pDevArray[dIndex]->m_StrDisp[nPort] = "";
	}

	if (theApp.m_pDevArray[dIndex]->m_StrDispOld[nPort] != theApp.m_pDevArray[dIndex]->m_StrDisp[nPort]) {
		theApp.m_pDevArray[dIndex]->m_StrDispOld[nPort] = theApp.m_pDevArray[dIndex]->m_StrDisp[nPort];
		theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
	}
}

void COutputThread::ReadDotMatrixOutputs(UINT dIndex, UINT nPort)
{
	UINT vIndex, vSimType, nDigits, nGrouping, disp_cnt, i, nChars;
	UINT oIndex, start_index, end_index;
	CString sdigits, sgrouping, StrValue, StrDotMX;
	CString chr;
	int nDecimal, nRight, nLeft;
	double nMax, nMin, SimValue;
	double nFactor, nConstant;
	BOOL nLeadingZero;
	BOOL VarTypeString;

	if (theApp.m_SimulationModel != SIM_NONE) {
		// set output level if changed
		if (theApp.m_pDevArray[dIndex]->m_DotMXLevelOld[nPort] != theApp.m_pDevArray[dIndex]->m_DotMXLevel[nPort]) {
			theApp.m_pDevArray[dIndex]->m_DotMXLevelOld[nPort] = theApp.m_pDevArray[dIndex]->m_DotMXLevel[nPort];
			theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
		}
	
		start_index = nPort*NUM_DMX_PER_PORT;
		end_index = nPort*NUM_DMX_PER_PORT + NUM_DMX_PER_PORT;
		for (oIndex = start_index; oIndex < end_index; oIndex++) {
			disp_cnt = 0;
			theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = "    ";
			if (theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_LampTest == 1) {
				if (theApp.m_pDevArray[dIndex]->m_StrDotMXOld[oIndex] != "8888")
					theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
				theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = "8888";
			} else if (!CheckFlags(&theApp.m_pDevArray[dIndex]->m_DotMXFlagsArray[oIndex])) {
				theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = "    ";
			} else if (!theApp.m_pDevArray[dIndex]->m_DotMXDisabled[oIndex])  {
				nChars = 0;
				StrDotMX.Empty();
				for (i = 0; i < 4; i++) {
					if (theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Grouping == 0 &&
						theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Digits == 0) {
						break;
					}

					nFactor = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Factor;
					nConstant = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Constant;
					nLeadingZero = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].LeadingZero;
					nDigits = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Digits;
					nGrouping = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Grouping;
					nMax = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Maximum;
					nMin = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].Minimum;
					nChars += (nGrouping + nDigits);
					if (nChars > 4) nChars = 4;

					SimValue = 0;
					VarTypeString = false;
					if (!theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].VarTokenName.IsEmpty()) {
						vIndex = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].VarTokenIndex;
						vSimType = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].SimType;

						switch (vSimType) {
							case SIM_XPLANE:
								if (vIndex == 0)
									break;
								theApp.m_pXPFlightData->Data[vIndex].Active = true;
								if (theApp.m_pXPFlightData != NULL)
									SimValue = VarTypeConversion (	XPDataArray[vIndex].VarType,
																	&theApp.m_pXPFlightData->Data[vIndex].dDataRead);
							break;

							case SIM_IL2:
							case SIM_IL2FB:
							case SIM_IL2PF:
								if (vIndex == 0)
									break;
								theApp.m_IL2FlightData.Data[vIndex].Active = true;
								SimValue = VarTypeConversion (	IL2DataArray[vIndex].VarType,
																&theApp.m_IL2FlightData.Data[vIndex].dDataRead);
							break;

							case SIM_F4BMS:
							case SIM_F4USIM:
								if (theApp.m_pF4VarToken[vIndex] != NULL)
									SimValue = VarTypeConversion(	F4DataArray[vIndex].VarType,
																	theApp.m_pF4VarToken[vIndex]);

								if (strcmp(F4DataArray[vIndex].VarTokenName, "Falcon_4/cockpit/displays/OSRAM_Display_O1") == 0) {
									StrDotMX = theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = theApp.m_F4FlightData.O1;
									VarTypeString = true;
								} else if (strcmp(F4DataArray[vIndex].VarTokenName, "Falcon_4/cockpit/displays/OSRAM_Display_O2") == 0) {
									StrDotMX = theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = theApp.m_F4FlightData.O2;
									VarTypeString = true;
								} else if (strcmp(F4DataArray[vIndex].VarTokenName, "Falcon_4/cockpit/displays/OSRAM_Display_O3-Chaff") == 0) {
									StrDotMX = theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = theApp.m_F4FlightData.O3Chaff;
									VarTypeString = true;
								} else if (strcmp(F4DataArray[vIndex].VarTokenName, "Falcon_4/cockpit/displays/OSRAM_Display_O4-Flare") == 0) {
									StrDotMX = theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = theApp.m_F4FlightData.O4Flare;
									VarTypeString = true;
								}
							break;

							case SIM_GTR:
							case SIM_GTR2:
							case SIM_EVO:
							case SIM_GTL:
							case SIM_RACE:
							case SIM_RACE07:
							case SIM_RACEON:
							case SIM_VOLVO:
							case SIM_SIMBIN:
								if (theApp.m_pSimBinVarToken[vIndex] != NULL)
									SimValue = VarTypeConversion(	SimBinDataArray[vIndex].VarType,
																	theApp.m_pSimBinVarToken[vIndex]);
							break;

							case SIM_RF:
								if (theApp.m_pRFVarToken[vIndex] != NULL)
									SimValue = VarTypeConversion(	RFDataArray[vIndex].VarType,
																	theApp.m_pRFVarToken[vIndex]);
							break;

							case SIM_GPX:
								if (theApp.m_pGPXVarToken[vIndex] != NULL)
									SimValue = VarTypeConversion(	GPXDataArray[vIndex].VarType,
																	theApp.m_pGPXVarToken[vIndex]);
							break;

							case SIM_LFS:
								if (theApp.m_pLFSVarToken[vIndex] != NULL)
									SimValue = VarTypeConversion(	LFSDataArray[vIndex].VarType,
																	theApp.m_pLFSVarToken[vIndex]);
							break;

							case USIM_INPUT_FLAGS:
								SimValue = theApp.m_UserVarsArray.m_IOFlags[vIndex].FlagValue;
							break;
						}
					} else {
						BOOL bNumeric = TRUE;
						LPCSTR szText = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].UserValue;; // Saves CString's array access overheads

						while (bNumeric && *szText)	{
							if (!isdigit(*szText) && *szText != '-' && *szText != '.')
								bNumeric = FALSE;
							szText++;
						}

						if (bNumeric) {
							SimValue = atof(theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].UserValue);
						} else {
							// Not a number
							VarTypeString = true;
						}
					}

					if (!VarTypeString) {
						SimValue *= nFactor;
						SimValue += nConstant;
						if (SimValue > nMax) SimValue = nMax;
						if (SimValue < nMin) SimValue = nMin;
						if (nLeadingZero)
							StrValue.Format("000%.4f", SimValue);
						else
							StrValue.Format("   %.4f", SimValue);

						nDecimal = StrValue.Find(".", 0);
						nLeft = nGrouping;
						nRight = nDecimal - nGrouping;

						if (nRight < 0) {
							nLeft += nRight;
							nRight = 0;
						}

						if (nDigits > 0) nDigits++;
						StrDotMX = StrValue.Mid(nRight, nLeft + nDigits) + StrDotMX;
					} else {
						StrDotMX = theApp.m_pDevArray[dIndex]->m_DotMXArray[oIndex].m_Displays[i].UserValue.Right(nGrouping) + StrDotMX;
					}
				} // for (i = 0; i < 4; i++)

				UINT cnt = StrDotMX.GetLength();
				if (cnt > 4) cnt = 4;
				for (UINT j=0; j < 4 - cnt; j++)
					StrDotMX = " " + StrDotMX;
				theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex] = StrDotMX;
			}

			if (theApp.m_pDevArray[dIndex]->m_StrDotMXOld[oIndex] != theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex]) {
				theApp.m_pDevArray[dIndex]->m_StrDotMXOld[oIndex] = theApp.m_pDevArray[dIndex]->m_StrDotMX[oIndex];
				theApp.m_pDevArray[dIndex]->m_PortChange[nPort] = true;
			}
		}
	}
}

BOOL COutputThread::CheckFlags(CFlagsPage *pFlagsArray)
{
	if (pFlagsArray->m_PowerFlag && !theApp.m_F4FlightData.MainPower) 
		return false;

	for (int i=0; i<MAX_INPUT_FLAGS; i++) {
		if (pFlagsArray->Active[i] && 
			!theApp.m_UserVarsArray.m_IOFlags[i].FlagValue ) 
			return false;
	}
	return true;
}

BOOL COutputThread::CheckConditions(double SimValue, double SimValue2, COutputPage *pOutputArray)
{
	BOOL ResultFlag1 = false;
	BOOL ResultFlag2 = false;

	switch (pOutputArray->m_Condition1) {
		case 0:		// >
			if (SimValue > pOutputArray->m_OPValue1)
				ResultFlag1 = true;
		break;

		case 1:		// >=
			if (SimValue >= pOutputArray->m_OPValue1)
				ResultFlag1 = true;
		break;

		case 2:		// <
			if (SimValue < pOutputArray->m_OPValue1)
				ResultFlag1 = true;
		break;

		case 3:		// <=
			if (SimValue <= pOutputArray->m_OPValue1)
				ResultFlag1 = true;
		break;

		case 4:		// =
			if (SimValue == pOutputArray->m_OPValue1)
				ResultFlag1 = true;
		break;

		case 5:		// !=
			if (SimValue != pOutputArray->m_OPValue1)
				ResultFlag1 = true;
		break;

		case 6:		// & BITMASK
			if (((UINT)SimValue & pOutputArray->m_BitMask) != 0)
				ResultFlag1 = true;
		break;
	}

	if (pOutputArray->m_Condition2 != 0) {
		switch (pOutputArray->m_Condition3) {
		case 0:		// >
			if (SimValue > pOutputArray->m_OPValue2)
				ResultFlag2 = true;
			break;

		case 1:		// >=
			if (SimValue >= pOutputArray->m_OPValue2)
				ResultFlag2 = true;
			break;

		case 2:		// <
			if (SimValue < pOutputArray->m_OPValue2)
				ResultFlag2 = true;
			break;

		case 3:		// <=
			if (SimValue <= pOutputArray->m_OPValue2)
				ResultFlag2 = true;
			break;

		case 4:		// =
			if (SimValue == pOutputArray->m_OPValue2)
				ResultFlag2 = true;
			break;

		case 5:		// !=
			if (SimValue != pOutputArray->m_OPValue2)
				ResultFlag2 = true;
			break;
		}

		if (pOutputArray->m_Condition2 == 1) {
			// AND
			if (ResultFlag1 && ResultFlag2)	ResultFlag1 = true;
			else ResultFlag1 = false;
		}
		else {
			// OR
			if (ResultFlag1 || ResultFlag2)	ResultFlag1 = true;
			else ResultFlag1 = false;
		}
	}

	// check blinking condition
	if (ResultFlag1) {
		if (((UINT)SimValue2 & pOutputArray->m_BlinkMask) != 0) {
			if (pOutputArray->m_BlinkOnFlag) {
				if (pOutputArray->m_BlinkOnTimeCnt++ >= pOutputArray->m_BlinkOnTime*5) {		// multiples of 50ms
					pOutputArray->m_BlinkOnTimeCnt = 0;
					pOutputArray->m_BlinkOffTimeCnt = 0;
					ResultFlag1 = 0; // toggle output
					pOutputArray->m_BlinkOnFlag = false;
				}
			} else if (pOutputArray->m_BlinkOffTimeCnt++ >= pOutputArray->m_BlinkOffTime*5) {	// multiples of 50ms
				pOutputArray->m_BlinkOffTimeCnt = 0;
				pOutputArray->m_BlinkOnTimeCnt = 0;
				ResultFlag1 = 1; // toggle output
				pOutputArray->m_BlinkOnFlag = true;
			} else ResultFlag1 = 0;
		}
	}

	return ResultFlag1;
}

// formatting (using wsprintf style formatting)
void Format(CString& str, LPCTSTR lpszFormat, ...)
{
//	ASSERT(AfxIsValidString(lpszFormat));
	if (!AfxIsValidString(lpszFormat)) {
		str = "Syntax Error";
		return;
	}

//	LPTSTR m_pchData;   // pointer to ref counted string data
	va_list argList;
	va_start(argList, lpszFormat);
	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%')
		{
			nMaxLen += _tclen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}
//		ASSERT(nWidth >= 0);
		if (nWidth < 0) {
			str = "Syntax Error";
			return;
		}

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else
			{
				nPrecision = _ttoi(lpsz);
				for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
//			ASSERT(nPrecision >= 0);
			if (nPrecision < 0) {
				str = "Syntax Error";
				return;
			}
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			ASSERT(FALSE);
#endif
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _tcsinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

		// strings
		case 's':
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = lstrlen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
#ifndef _UNICODE
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
#else
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
#endif
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					double f;
					LPTSTR pszTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312+nPrecision+6));

					f = va_arg(argList, double);
					_stprintf( pszTemp, _T( "%*.*f" ), nWidth, nPrecision+6, f );
					nItemLen = _tcslen(pszTemp);
				}
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
//				ASSERT(FALSE);  // unknown formatting option
				str = "Syntax Error";
				return;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	str.GetBuffer(nMaxLen);
	VERIFY(_vstprintf(str.GetBuffer(1), lpszFormat, argListSave) <= str.GetAllocLength());
	str.ReleaseBuffer();

	va_end(argListSave);
}

