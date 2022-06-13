// F4DataOut.cpp : implementation file
//

#include "stdafx.h"
#include "USIM.h"
#include "F4DataOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CF4DataOut dialog


CF4DataOut::CF4DataOut(CWnd* pParent /*=NULL*/)
	: CDialog(CF4DataOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CF4DataOut)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CF4DataOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CF4DataOut)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LIST1, m_DataList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CF4DataOut, CDialog)
	//{{AFX_MSG_MAP(CF4DataOut)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CF4DataOut message handlers

void CF4DataOut::OnStartTimer() 
{
	if (theApp.m_SimulationModel != SIM_NONE) {
		m_nTimer = SetTimer(1, 500, 0);
	}
}

void CF4DataOut::OnStopTimer() 
{
	KillTimer(m_nTimer);   
}

void CF4DataOut::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	DisplayData();

	CDialog::OnTimer(nIDEvent);
}

void CF4DataOut::DisplayData() 
{
	LVITEM lvi;
	CString strItem;

	// Set subitem 1
	lvi.mask = LVIF_TEXT;
	int i = 0;
	lvi.iSubItem = 1;

	lvi.iItem = i++;
	if (theApp.m_SimulationModel == SIM_F4BMS)
		strItem.Format(_T("BMS SM1 Ver %i : SM2 Ver %i"), theApp.m_F4FlightData.VersionNum, theApp.m_F4FlightData.VersionNum2);
	else if (theApp.m_SimulationModel == SIM_TEST)
		strItem.Format(_T("TEST %i"), theApp.m_F4FlightData.VersionNum);
	else if (theApp.m_SimulationModel == SIM_KBS)
		strItem.Format(_T("Keyboard Emulation %i"), theApp.m_F4FlightData.VersionNum);

	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.lightBits);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.lightBits2);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.lightBits3);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.hsiBits);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.altBits);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.powerBits);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.blinkBits);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("0x%Xh"), theApp.m_F4FlightData.miscBits);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	int j;
	for (j = 0; j < 5; j++) {
		lvi.iItem = i++;
		lvi.pszText = (LPTSTR)(LPCTSTR)(theApp.m_F4FlightData.DEDLines[j]);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		lvi.pszText = (LPTSTR)(LPCTSTR)(theApp.m_F4FlightData.Invert[j]);
		m_DataList.SetItem(&lvi);
	}

	for (j = 0; j < 5; j++) {
		lvi.iItem = i++;
		lvi.pszText = (LPTSTR)(LPCTSTR)(theApp.m_F4FlightData.PFLLines[j]);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		lvi.pszText = (LPTSTR)(LPCTSTR)(theApp.m_F4FlightData.PFLInvert[j]);
		m_DataList.SetItem(&lvi);
	}

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.x);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.y);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.z);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.xDot);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.yDot);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.zDot);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.alpha);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.beta);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.gamma);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.pitch);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.roll);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.yaw);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.mach);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.kias);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.vt);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.gs);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.windOffset);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.nozzlePos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.nozzlePos2);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.internalFuel);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.externalFuel);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.fuelFlow);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.fuelFlow2);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.rpm);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.rpm2);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.ftit);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.ftit2);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.gearPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.speedBrake);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.epuFuel);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.oilPressure);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.oilPressure2);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.ChaffCount);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.FlareCount);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.NoseGearPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.LeftGearPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.RightGearPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.AdiIlsHorPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.AdiIlsVerPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.courseState);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.headingState);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.totalStates);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.courseDeviation);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.desiredCourse);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.distanceToBeacon);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.bearingToBeacon);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.currentHeading);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.desiredHeading);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.deviationLimit);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.halfDeviationLimit);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.localizerCourse);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.airbaseX);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.airbaseY);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.TrimPitch);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.TrimRoll);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.TrimYaw);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.UFCTChan);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.AUXTChan);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.fwd);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.aft);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.total);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.MainPower);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.cabinAlt);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.hydPressureA);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.hydPressureB);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.navMode);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.AAUZ);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.tacanInfo[0]);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.tacanInfo[1]);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.AltCalReading);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.cmdsMode);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.BupUhfPreset);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.BupUhfFreq);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.currentTime);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.vehicleACD);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.lefPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.tefPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.vtolPos);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.PRKMag);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%.6f"), theApp.m_F4FlightData.LOX);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%s"), theApp.m_F4FlightData.O1);
	strItem = strItem.Left(4);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%s"), theApp.m_F4FlightData.O2);
	strItem = strItem.Left(4);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%s"), theApp.m_F4FlightData.O3Chaff);
	strItem = strItem.Left(4);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%s"), theApp.m_F4FlightData.O4Flare);
	strItem = strItem.Left(4);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	lvi.iItem = i++;
	strItem.Format(_T("%d"), theApp.m_F4FlightData.RwrObjectCount);
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.SetItem(&lvi);

	for (j=0; j<40; j++) {
		lvi.iItem = i++;
		strItem.Format(_T("%d"), theApp.m_F4FlightData.RWRsymbol[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("%.6f"), theApp.m_F4FlightData.bearing[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("%d"), theApp.m_F4FlightData.missileActivity[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("%d"), theApp.m_F4FlightData.missileLaunch[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("%d"), theApp.m_F4FlightData.selected[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("%.6f"), theApp.m_F4FlightData.lethality[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("%d"), theApp.m_F4FlightData.newDetection[j]);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.SetItem(&lvi);
	}
}

void CF4DataOut::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	OnStartTimer();
}

void CF4DataOut::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	OnStopTimer();
}

BOOL CF4DataOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	m_DataList.GetClientRect(&rect);
	int nColInterval = rect.Width()/10;

	m_DataList.DeleteAllItems();
	m_DataList.InsertColumn(0, _T(""), LVCFMT_LEFT, nColInterval*4);
	m_DataList.InsertColumn(1, _T(""), LVCFMT_LEFT, nColInterval*5);

	LVITEM lvi;
	CString strItem;

	// Set subitem 1
	lvi.mask = LVIF_TEXT;
	int i = 0;
	lvi.iSubItem = 0;

	lvi.iItem = i++;
	strItem = _T("Version");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("lightBits");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("lightBits2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("lightBits3");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("hsiBits");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("altBits");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);
	
	lvi.iItem = i++;
	strItem = _T("powerBits");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("blinkBits");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("miscBits");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	int j;
	for (j = 0; j < 5; j++) {
		lvi.iItem = i++;
		strItem.Format(_T("DED Line %d"), j + 1);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("DED Line %d Invert"), j + 1);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);
	}

	for (j = 0; j < 5; j++) {
		lvi.iItem = i++;
		strItem.Format(_T("PFL Line %d"), j + 1);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("PFL Line %d Invert"), j + 1);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);
	}

	lvi.iItem = i++;
	strItem = _T("x");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("y");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("z");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("xDot");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("yDot");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("zDot");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("alpha");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("beta");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("gamma");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("pitch");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("roll");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("yaw");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("mach");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("kias");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("vt");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("gs");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("windOffset");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("nozzlePos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("nozzlePos2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("internalFuel");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("externalFuel");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("fuelFlow");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("fuelFlow2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("rpm");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("rpm2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("ftit");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("ftit2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("gearPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("speedBrake");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("epuFuel");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("oilPressure");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("oilPressure2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("ChaffCount");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("FlareCount");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("NoseGearPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("LeftGearPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("RightGearPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("AdiIlsHorPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("AdiIlsVerPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("courseState");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("headingState");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("totalStates");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("courseDeviation");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("desiredCourse");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("distanceToBeacon");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("bearingToBeacon");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("currentHeading");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("desiredHeading");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("deviationLimit");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("halfDeviationLimit");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("localizerCourse");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("airbaseX");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("airbaseY");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("TrimPitch");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("TrimRoll");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("TrimYaw");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("UFCTChan");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("AUXTChan");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("fwd");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("aft");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("total");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("MainPower");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("cabinAlt");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("hydPressureA");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("hydPressureA");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("navMode");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("AAUZ");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("tacanInfo[0]");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("tacanInfo[1]");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("AltCalReading");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("cmdsMode");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("BupUhfPreset");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("BupUhfFreq");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("currentTime");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("vehicleACD");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("lefPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("tefPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("vtolPos");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("PRKMag");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("LOX");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("OSRAM O1");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("OSRAM O2");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("OSRAM O3 Chaff");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("OSRAM O4 Flare");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	lvi.iItem = i++;
	strItem = _T("RwrObjectCount");
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_DataList.InsertItem(&lvi);

	for (j=0; j<40; j++) {
		lvi.iItem = i++;
		strItem.Format(_T("RWRsymbol[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("bearing[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("missileActivity[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("missileLaunch[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("selected[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("lethality[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);

		lvi.iItem = i++;
		strItem.Format(_T("newDetection[%d]"), j);
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_DataList.InsertItem(&lvi);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
