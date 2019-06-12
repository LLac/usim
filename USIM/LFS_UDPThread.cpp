// LFS_UDPThread.cpp: implementation of the CLFS_UDPThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "LFS_UDPThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CLFS_UDPThread, CThread)

CLFS_UDPThread::CLFS_UDPThread(void* pOwnerObject, LPARAM lParam) 
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(CLFS_UDPThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_deltaT_IW = 0;
	m_Run = FALSE;
}

CLFS_UDPThread::~CLFS_UDPThread()
{
	m_deltaT_IW = 0;
}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD CLFS_UDPThread::ThreadHandler()
{
	BOOL bCanContinue = true;
	int nIncomingCommand;
	static UCHAR Dispatcher = 0;

#ifdef _DEBUG
		TRACE ("\nLFS_UDP Thread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 10);

		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
			ReadPacket();
			if (!m_Status) {
				m_Status = InitLFS_UDP(); 
			}

			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			StopLFS_UDP();
			if (InitLFS_UDP()) 
				m_Status = true;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			StopLFS_UDP();
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_STOP:
			m_Run = FALSE;
			bCanContinue = FALSE;
			StopLFS_UDP();
			break;

		default:
			break;
		};

	} while (bCanContinue);

#ifdef _DEBUG
		TRACE ("\nLFS_UDPThread killed!\n");
#endif

	return 11;	// ... if Thread task completition OK
}

void CLFS_UDPThread::RequestBuffer()
{

}

void CLFS_UDPThread::AnswerBuffer()
{

}

void CLFS_UDPThread::MapVars()
{
	int i = 0;

	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.SpareB;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Time;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Car;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Flags;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Gear;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Speed;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.RPM;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Turbo;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.EngTemp;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Fuel;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.OilPressure;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.OilTemp;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.DashLights;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.ShowLights;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Throttle;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Brake;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Clutch;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Display1;
	theApp.m_pLFSVarToken[i++] = (double *)&theApp.m_LFSSimData.Data.Display2;
}

BOOL CLFS_UDPThread::InitLFS_UDP()
{
//	int retval = 0;
	WORD	wVersionRequested = MAKEWORD(1,1);
	WSADATA wsadata;

	m_sock = NULL;

	if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        TRACE ("Init LFS_UDP socket failed.\n");
        return false;
    }

	TCHAR IPDefault[20];
	GetProfileString("Options", "IP Address", "127.0.0.1", IPDefault, 20);
	m_conf.ip = (LPTSTR)(LPCTSTR)IPDefault;
	UINT simIndex = 0;
	for (int i = 0; i < ArraySize(SimTypes); i++) {
		if (SimTypes[i].SimType == theApp.m_SimulationModel) {
			simIndex = i;
			break;
		}
	}

	m_conf.port = theApp.GetProfileInt("Options", "Port", theApp.SimList[simIndex].Port);
//	m_conf.port = theApp.GetProfileInt("Options", "Port", 5033);
	localhost = gethostbyname (m_conf.ip);
	if (localhost == NULL) {
		TRACE ("Can't resolve IP address\n");
        WSACleanup();
		return false;
	}

	m_sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_sock == INVALID_SOCKET) {
		TRACE ("Create socket failed.\n");
        WSACleanup();
		m_sock = NULL;
		return false;
	}
	
	FD_SET(m_sock, &m_ActiveReadFds);

	// setup the LFS_UDP server
	// Clear out server struct
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));

	/* Set family and port */
	server.sin_family = AF_INET;
	server.sin_port = htons ((unsigned short)m_conf.port);

	/* Set server address */
	server.sin_addr.s_addr = inet_addr(m_conf.ip);

	/* Set family and port */
    memcpy(&(server.sin_addr), localhost->h_addr, localhost->h_length);
    server.sin_port = htons ((unsigned short)m_conf.port);
    memcpy(&(server.sin_addr), localhost->h_addr, localhost->h_length);

    TRACE ("Going to bind LFS_UDP...\n");
	if (bind(m_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1) {
		TRACE ("StartLFS_UDP() error!\n");
		closesocket(m_sock);
        WSACleanup();
		return false;
    }

	MapVars();

	TRACE ("Listening for LFS_UDP packets...\n");
	return true;
}

void CLFS_UDPThread::StopLFS_UDP()
{
	if (m_sock == NULL) return;

	TRACE ("Stopping LFS_UDP\n");

	closesocket (m_sock);
    WSACleanup();
	m_sock = NULL;
	m_Status = false;
}

BOOL CLFS_UDPThread::SendPacket()
{	
	if (m_sock == NULL || !m_Status) return false;

	int retval = 0;
	
	int len = m_RequestBuf.GetLength();
	if (len == 1) return true;

	retval = sendto(m_sock, m_RequestBuf, len, 0, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	if (retval == SOCKET_ERROR) {
		TRACE ("Error in SendMSg. Error %d\n", WSAGetLastError());

		closesocket(m_sock);
        WSACleanup();
		return false;
	}

	TRACE("sendpacket = %s\n", m_RequestBuf);
	return true;
}

BOOL CLFS_UDPThread::ReadPacket()
{
	if (m_sock == NULL || !m_Status) return false;

	size_t len = 0;
	size_t buff_len = 0;

	memset(&m_AnswerBuf, NULL, sizeof(m_AnswerBuf));
	buff_len = sizeof(m_AnswerBuf);

	memset(&m_ActiveReadFds, 0x0, sizeof(fd_set));
	FD_SET(m_sock, &m_ActiveReadFds);
	struct timeval tv;
	memset(&tv, 0x0, sizeof(struct timeval));
	tv.tv_usec = 250000; // wait no more than 5ms

	int chk = select(0, &m_ActiveReadFds, NULL, NULL, &tv);
	if (chk <= 0) {
//		TRACE("No data to read in ReadMSg()\n");
		return true;
	} 

	buff_len = sizeof(OutGaugePack);
	len = recv(m_sock, (char *)&m_AnswerBuf, buff_len, 0);
	if (len == SOCKET_ERROR) {
		TRACE ("Client: Error receiving from socket: Error %d\n",WSAGetLastError());

        WSACleanup();
		return false;
	}

	buff_len = sizeof(m_AnswerBuf);
	if ((buff_len == 0) || (len == SOCKET_ERROR)) {
		TRACE("set_read_buff returned FALSE in ReadMsg, or no data was read from the socket.\n");
		return false;
	}

	memcpy(&theApp.m_LFSSimData, &m_AnswerBuf, buff_len);
	theApp.m_LFSSimData.Data.Gear -= 1;
	return true;
}