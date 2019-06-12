// IL2_UDPThread.cpp: implementation of the CIL2_UDPThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USIM.h"
#include "IL2_UDPThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CIL2_UDPThread, CThread)

CIL2_UDPThread::CIL2_UDPThread(void* pOwnerObject, LPARAM lParam) 
	: CThread(pOwnerObject, lParam)
{
	// WORKER THREAD CLASS GENERATOR - Do not remove/edit this code! //////////
	// Selecting Thread Handler & Implement Thread Synchronization & Notification
	SUPPORT_THREAD_SYNCHRONIZATION(CIL2_UDPThread)
	SUPPORT_THREAD_NOTIFICATION
	///////////////////////////////////////////////////////////////////////////

	m_deltaT_IW = 0;
	m_Run = FALSE;
}

CIL2_UDPThread::~CIL2_UDPThread()
{
	m_deltaT_IW = 0;
}

///////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove/edit this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
///////////////////////////////////////////////////////////////////////////////
DWORD CIL2_UDPThread::ThreadHandler()
{
	BOOL bCanContinue = true;
	int nIncomingCommand;
	static UCHAR Dispatcher = 0;

#ifdef _DEBUG
		TRACE ("\nIL2_UDP Thread started!\n");
#endif

	do
	{
		WaitForNotification(nIncomingCommand, 5);

		switch (nIncomingCommand)
		{
		case CThread::CMD_TIMEOUT_ELAPSED:
			if (Dispatcher++ == 1) {
				Dispatcher = 0;
				RequestBuffer();
				m_Status = SendPacket();
			} else {
				m_Status = ReadPacket();
				AnswerBuffer();
			}
			if (!m_Status) {
				m_Status = InitIL2_UDP(); 
			}

			break;

		case CThread::CMD_INITIALIZE:
			m_Run = FALSE;
			StopIL2_UDP();
			if (InitIL2_UDP()) m_Status = true;
			HandleCommandImmediately(CThread::CMD_RUN);
			break;

		case CThread::CMD_RUN:
			m_Run = TRUE;
			SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
			break;

		case CThread::CMD_PAUSE:
			m_Run = FALSE;
			StopIL2_UDP();
			SetActivityStatus(CThread::THREAD_PAUSED);
			break;

		case CThread::CMD_STOP:
			m_Run = FALSE;
			bCanContinue = FALSE;
			StopIL2_UDP();
			break;

		default:
			break;
		};

	} while (bCanContinue);

#ifdef _DEBUG
		TRACE ("\nIL2_UDPThread killed!\n");
#endif

	return 11;	// ... if Thread task completition OK
}

void CIL2_UDPThread::RequestBuffer()
{
	UINT i;
	CString str;

	if (m_sock == NULL) return;

	Lock();

	m_RequestBuf = "R";
	// DataRefs List
	for (i=0; i<ArraySize(IL2DataArray); i++) {
		if (theApp.m_IL2FlightData.Data[i].Active) {
			if (IL2DataArray[i].VarID %2 == 0) {
				// Read values from IL2 server
				str = m_RequestBuf;
				m_RequestBuf.Format("%s/%d", str, IL2DataArray[i].VarID);
				if (strlen(IL2DataArray[i].Param) > 0) {
					str = m_RequestBuf;
					m_RequestBuf.Format("%s\\%s", str, IL2DataArray[i].Param);
				}
			} else {
				// Write values to IL2 server
				if (theApp.m_IL2FlightData.Data[i].Write) {
					str = m_RequestBuf;
					m_RequestBuf.Format("%s/%d", str, IL2DataArray[i].VarID);
					if (strlen(IL2DataArray[i].Param) > 0) {
						str = m_RequestBuf;
						m_RequestBuf.Format("%s\\%s", str, IL2DataArray[i].Param);
					}

					if (IL2DataArray[i].Writable) {
						str = m_RequestBuf;
						switch(IL2DataArray[i].VarType) {
							case xplmType_Int:
								m_RequestBuf.Format("%s\\%d", str, theApp.m_IL2FlightData.Data[i].dDataWrite);
								break;

							case xplmType_Float:
								m_RequestBuf.Format("%s\\%f", str, theApp.m_IL2FlightData.Data[i].dDataWrite);
								break;

							case xplmType_Double:
								m_RequestBuf.Format("%s\\%f", str, theApp.m_IL2FlightData.Data[i].dDataWrite);
								break;
						}
						
					}
					theApp.m_IL2FlightData.Data[i].Write = 0;
				}
			}
		}
	}

	// Commands List
	for (i=0; i<ArraySize(IL2CmdArray); i++) {
		if (theApp.m_IL2FlightData.Cmd[i].Active) {
			// Start command
			if (theApp.m_IL2FlightData.Cmd[i].Press) {
				str = m_RequestBuf;
				m_RequestBuf.Format("%s/%d", str, IL2CmdArray[i].VarID);
				if (strlen(IL2CmdArray[i].Param) > 0) {
					str = m_RequestBuf;
					m_RequestBuf.Format("%s\\%s", str, IL2CmdArray[i].Param);
				}
				theApp.m_IL2FlightData.Cmd[i].Press = 0;
			}

			if (theApp.m_IL2FlightData.Cmd[i].Release) {
				str = m_RequestBuf;
				m_RequestBuf.Format("%s/%d", str, IL2CmdArray[i].VarID);
				if (strlen(IL2CmdArray[i].Param) > 0) {
					str = m_RequestBuf;
					m_RequestBuf.Format("%s\\%s", str, IL2CmdArray[i].Param);
				}
				theApp.m_IL2FlightData.Cmd[i].Release = 0;
			}
		}
	}

	Unlock();
}

void CIL2_UDPThread::AnswerBuffer()
{
	UINT i, j, k, varID;
	double ansDbl;
	CString strSub;

	if (m_sock == NULL) return;

	Lock();

	// DataRefs List
	j = 1;
	while (AfxExtractSubString (strSub, m_AnswerBuf, j++, '/')) {
		varID = atoi(strSub);
		i = SearchVarID(varID);
		k = strSub.Find('\\');
		if (k < 0) continue;

		if (strlen(IL2DataArray[i].Param) == 0) {
			strSub = strSub.Right(strSub.GetLength() - k - 1);
		} else {
			strSub = strSub.Right(strSub.GetLength() - k - 3);
		}
		ansDbl = atof(strSub);
		if (theApp.m_IL2FlightData.Data[i].Active) {
			switch(IL2DataArray[i].VarType) {
				case xplmType_Int:
					theApp.m_IL2FlightData.Data[i].dDataRead = (int)ansDbl;
					break;

				case xplmType_Float:
					theApp.m_IL2FlightData.Data[i].dDataRead = (float)ansDbl;
					break;

				case xplmType_Double:
					theApp.m_IL2FlightData.Data[i].dDataRead = (double)ansDbl;
					break;
			}
		}
	}

	Unlock();
}

UINT CIL2_UDPThread::SearchVarID(UINT iID)
{
	for (int i = 0; i<ArraySize(IL2DataArray); i++) {
		if (iID == IL2DataArray[i].VarID) return i;
	}

	return 0;
}

void CIL2_UDPThread::MapVars()
{
	UINT i;
	
	for (i=0; i<ArraySize(IL2DataArray); i++) {
		theApp.m_IL2FlightData.Data[i].Active = 0;
		theApp.m_IL2FlightData.Data[i].Write = 0;
	}

	for (i=0; i<ArraySize(IL2CmdArray); i++) {
		theApp.m_IL2FlightData.Cmd[i].Active = 0;
		theApp.m_IL2FlightData.Cmd[i].Press = 0;
		theApp.m_IL2FlightData.Cmd[i].Release = 0;
	}
}

BOOL CIL2_UDPThread::InitIL2_UDP()
{
	WORD	wVersionRequested = MAKEWORD(1,1);
	WSADATA wsadata;

	m_sock = NULL;

	if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        TRACE ("Init IL2_UDP socket failed.\n");
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

	// setup the IL2_UDP server
	// Clear out server struct
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));

	/* Set family and port */
	server.sin_family = AF_INET;
	server.sin_port = htons ((unsigned short)m_conf.port);

	/* Set server address */
	server.sin_addr.s_addr = inet_addr(m_conf.ip);

	/* Clear out client struct */
	memset((void *)&client, '\0', sizeof(struct sockaddr_in));

	/* Set family and port */
	client.sin_family = AF_INET;
	client.sin_port = htons(0);

    TRACE ("Going to bind IL2_UDP...\n");
	if (bind(m_sock, (struct sockaddr *)&client, sizeof(struct sockaddr_in)) == -1) {
		TRACE ("StartIL2_UDP() error!\n");
		closesocket(m_sock);
        WSACleanup();
		return false;
    }

	MapVars();

	TRACE ("Listening for IL2_UDP packets...\n");
	return true;
}

void CIL2_UDPThread::StopIL2_UDP()
{
	if (m_sock == NULL) return;

	TRACE ("Stopping IL2_UDP\n");

	closesocket (m_sock);
    WSACleanup();
	m_sock = NULL;
	m_Status = false;
}

BOOL CIL2_UDPThread::SendPacket()
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

BOOL CIL2_UDPThread::ReadPacket()
{
	if (m_sock == NULL || !m_Status) return false;

	size_t len = 0;
	size_t buff_len = 0;
	char temp_buff[256];
	memset(temp_buff, NULL, sizeof(temp_buff));
	buff_len = sizeof(temp_buff);
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

	buff_len = m_AnswerBuf.GetLength();
	strcpy(temp_buff, m_AnswerBuf);
	buff_len = sizeof(temp_buff);
	len = recv(m_sock, temp_buff, buff_len, 0);
	if (len == SOCKET_ERROR) {
		TRACE ("Client: Error receiving from socket: Error %d\n",WSAGetLastError());

        WSACleanup();
		return false;
	}

	temp_buff[len] = '\0';
	buff_len = strlen(temp_buff);
	if ((buff_len == 0) || (len == SOCKET_ERROR)) {
		TRACE("set_read_buff returned FALSE in ReadMsg, or no data was read from the socket.\n");
		return false;
	}

	m_AnswerBuf = temp_buff;
	TRACE("readpacket = %s\n", m_AnswerBuf);
	return true;
}