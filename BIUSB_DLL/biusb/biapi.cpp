#include "stdafx.h"

//#define HID_EXPORTS
#include "biusb.h"
#define BIAPI_H
#include "biapi.h"

FILE*	FileStream;
//ULONG	Time_Out[MAX_DEVICES] = {0};

void StoreInputReport(DeviceParam *oDeviceList, CHAR *oDataBuffer, BOOL Raw_Flag)
{
	USHORT	ByteNumber;
	USHORT	BitNumber;
	CHAR	ReceivedByte;
	
	if (oDeviceList->Usage == 0x0004 && oDeviceList->UsagePage == 0x0001) {
		// joystick standard
		HIDP_REPORT_TYPE    ReportType = HidP_Input;
		HIDP_DATA  DataList[256];
		ULONG  DataLength = 256;

		HidP_GetData(	ReportType,
						DataList,
						&DataLength,
						oDeviceList->PreparsedData,
						InputReport[oDeviceList->DevIndex],
						oDeviceList->InputReportByteLength);

		memset(oDataBuffer, 0, oDeviceList->ButtonIndexMax); 
		for (UINT i = 0; i < DataLength; i++) {
			if (DataList[i].DataIndex >= oDeviceList->ButtonIndexMin) {
				if (DataList[i].DataIndex <= oDeviceList->ButtonIndexMax) {
					if (DataList[i].On)
						oDataBuffer[(DataList[i].DataIndex - oDeviceList->ButtonIndexMin)] = 1;
				} else {
					break;
				}
			}
		}
		return;
	}

	//Step through the received bytes and store each.
	for (ByteNumber=0; ByteNumber < oDeviceList->InputReportByteLength - 1; ByteNumber++)
	{
		//Get a byte.
		//oDataBuffer[ByteNumber] = InputReport[oDeviceList->DevIndex][ByteNumber+1];
		ReceivedByte = InputReport[oDeviceList->DevIndex][ByteNumber+1];

		if (Raw_Flag) {
			oDataBuffer[ByteNumber] = ReceivedByte;
		} else {
			// store byte to bits in buffer
			for (BitNumber=0; BitNumber < 8; BitNumber++)
				oDataBuffer[(ByteNumber*8+BitNumber)] = (char)((ReceivedByte >> BitNumber) & 0x0001);
		}
	}
}
