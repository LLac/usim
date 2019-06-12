/****************************************************************************

    PROGRAM: Biusb.cpp

    PURPOSE: Contains library routines for detecting and reading data 
			from HIDs

    FUNCTIONS:

*******************************************************************************/

#include "stdafx.h"

#include "intel_hex.h"
#include "EFeeprom.h"
#include "PLeeprom.h"
#include "FUeeprom.h"
#include "biusb.h"
#include "biusb_dev.h"
#include "bidllapi.h"

FILE* stream;

/****************************************************************************
   FUNCTION: DllMain(HANDLE, DWORD, LPVOID)

   PURPOSE:  DllMain is called by Windows when
             the DLL is initialized, Thread Attached, and other times.
             Refer to SDK documentation as to the different ways this
             may be called.
             
             The DllMain function should perform additional initialization
             tasks required by the DLL.  In this example, no initialization
             tasks are required.  DllMain should return a value of 1 if
             the initialization is successful.
           
*******************************************************************************/
#ifndef BIHID_LIB
BOOL APIENTRY DllMain(HANDLE /*hModule*/, 
                      DWORD  ul_reason_for_call, 
                      LPVOID /*lpReserved*/)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif

HID_API BOOL APIENTRY DetectDevice(UINT *oDev_Cnt, DeviceParam *DeviceList, UINT DeviceID, UINT Flash_Flag)
{
	//Use a series of API calls to find a HID with a matching Vendor and Product ID.
	HIDD_ATTRIBUTES				Attributes;
	SP_DEVICE_INTERFACE_DATA	devInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA	*DetailData;
	BOOL						LastDevice = FALSE;
	BOOL						MyDeviceDetected = FALSE;
	int							MemberIndex = 0;
	LONG						Result;
	ULONG						Length = 0;
	ULONG						Required;
	HANDLE						hDevInfo;
	static GUID					HidGuid;
	HANDLE						FileHandle;
	int							Dev_Cnt;

	Dev_Cnt = 0;
	FileHandle = NULL;
	DetailData = NULL;

	switch (Flash_Flag) {
		case DT_FLASH:
			stream = fopen("biusb_flash.log", "w");
			break;
		case DT_DEVICES:
			stream = fopen("biusb_dev.log", "w");
			break;
		default:
			stream = fopen("biusb_all.log", "w");
			break;
	}
	fprintf(stream, "\n\nBIUSB initialization!\n");
	fprintf(stream, "\nFlash : %d\n", Flash_Flag);

	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&HidGuid);	
	
	/*
	API function: SetupDiGetClassDevs
	Returns: a handle to a device information set for all installed devices.
	Requires: the GUID returned by GetHidGuid.
	*/
	
	hDevInfo=SetupDiGetClassDevs 
		(&HidGuid, 
		NULL, 
		NULL, 
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);

	if (hDevInfo == INVALID_HANDLE_VALUE) {
		fprintf(stream, "hDevInfo : INVALID_HANDLE_VALUE\n");

		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		// Display the string.
		fprintf(stream, "Error ; %d - %s\n", GetLastError(), (LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );

		fclose(stream);
		return FALSE;
	}

	devInfoData.cbSize = sizeof(devInfoData);

	//Step through the available devices looking for the ones we want. 
	//Quit on checking all available devices.
	MemberIndex = 0;
	Dev_Cnt = 0;
	LastDevice = FALSE;

	do
	{
		/*
		API function: SetupDiEnumDeviceInterfaces
		On return, MyDeviceInterfaceData contains the handle to a
		SP_DEVICE_INTERFACE_DATA structure for a detected device.
		Requires:
		The DeviceInfoSet returned in SetupDiGetClassDevs.
		The HidGuid returned in GetHidGuid.
		An index to specify a device.
		*/

		Result=SetupDiEnumDeviceInterfaces 
			(hDevInfo, 
			0, 
			&HidGuid, 
			MemberIndex, 
			&devInfoData);

		if (Result != 0)
		{
			//A device has been detected, so get more information about it.

			/*
			API function: SetupDiGetDeviceInterfaceDetail
			Returns: an SP_DEVICE_INTERFACE_DETAIL_DATA structure
			containing information about a device.
			To retrieve the information, call this function twice.
			The first time returns the size of the structure in Length.
			The second time returns a pointer to the data in DeviceInfoSet.
			Requires:
			A DeviceInfoSet returned by SetupDiGetClassDevs
			The SP_DEVICE_INTERFACE_DATA structure returned by SetupDiEnumDeviceInterfaces.
			
			The final parameter is an optional pointer to an SP_DEV_INFO_DATA structure.
			This application doesn't retrieve or use the structure.			
			If retrieving the structure, set 
			MyDeviceInfoData.cbSize = length of MyDeviceInfoData.
			and pass the structure's address.
			*/
			
			//Get the Length value.
			//The call will return with a "buffer too small" error which can be ignored.
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				NULL, 
				0, 
				&Length, 
				NULL);

			//Allocate memory for the hDevInfo structure, using the returned Length.
			DetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);

			//Set cbSize in the detailData structure.
			DetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				DetailData, 
				Length, 
				&Required, 
				NULL);

			if (Result == 0) {
				fprintf(stream, "SetupDiGetDeviceInterfaceDetail : FALSE\n");

				MemberIndex = MemberIndex + 1;

				if (DetailData != NULL) {
					free(DetailData);
					DetailData = NULL;
				}

				continue;
			}

			//Open a handle to the device.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			FileHandle = CreateFile 
				(DetailData->DevicePath,
				GENERIC_READ|GENERIC_WRITE, 
				FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 
				//FILE_ATTRIBUTE_NORMAL, 
				FILE_FLAG_OVERLAPPED,
				NULL);
			
			if (FileHandle == INVALID_HANDLE_VALUE) {
				fprintf(stream, "%s", DetailData->DevicePath);	
				fprintf(stream, "CreateFile : INVALID_HANDLE_VALUE\n");

				LPVOID lpMsgBuf;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
				);
				// Display the string.
				fprintf(stream, "Error ; %d - %s\n", GetLastError(), (LPCTSTR)lpMsgBuf);
				// Free the buffer.
				LocalFree( lpMsgBuf );

				MemberIndex = MemberIndex + 1;

				if (DetailData != NULL) {
					free(DetailData);
					DetailData = NULL;
				}

				continue;
			}
			
			//Result = HidD_SetNumInputBuffers (FileHandle, 0);

			/*
			API function: HidD_GetAttributes
			Requests information from the device.
			Requires: the handle returned by CreateFile.
			Returns: a HIDD_ATTRIBUTES structure containing
			the Vendor ID, Product ID, and Product Version Number.
			Use this information to decide if the detected device is
			the one we're looking for.
			*/

			//Set the Size to the number of bytes in the structure.
			Attributes.Size = sizeof(Attributes);

			Result = HidD_GetAttributes 
				(FileHandle, 
				&Attributes);
			
			if (Result == FALSE) {
				fprintf(stream, "nHidD_GetAttributes : FALSE\n");

				MemberIndex = MemberIndex + 1;

				if (DetailData != NULL) {
					free(DetailData);
					DetailData = NULL;
				}

				continue;
			}

			// Is it the desired device?		
			if ( (Attributes.VendorID == VENDOR_ID_1 || Attributes.VendorID == VENDOR_ID_2) &&
				 (DeviceID == 0 || Attributes.ProductID == DeviceID) ) {

				fprintf(stream, "\nFound VendorID 0x%x\n", Attributes.VendorID);
				fprintf(stream, "Found matching ProductID 0x%x\n", Attributes.ProductID);
				fprintf(stream, "Flash_Flag = %d\n", Flash_Flag);

				switch (Flash_Flag) {
					case DT_FLASH:
						switch (Attributes.ProductID) {
							case PIDF_PLASMA_LITE_V2:
							case PIDF_GAMMARAY_V2:
							case PIDF_GAMMARAY_V3:
							case PIDF_ELECTRONFLUX:
							case PIDF_NITRO_SLG:
							case PIDF_NITRO_XG:
							case PIDF_GAMMATRON:
							case PIDF_PLASMA_MM2:
							case PIDF_FUSION:
							case PIDF_USIMV1:
							case PIDF_DRO:
							case 0x9999:	// AU
								DeviceList[Dev_Cnt].DevIndex = (USHORT)Dev_Cnt;
								DeviceList[Dev_Cnt].DeviceHandle = FileHandle;

								if (!GetDevParam(&Attributes, &DeviceList[Dev_Cnt], DetailData, DT_FLASH)) {
									CloseHandle(FileHandle);
									fprintf(stream, "DT_FLASH - GetDevParam return FALSE\n");
								} else {
									MyDeviceDetected = TRUE;
									Dev_Cnt++;
								}
								break;

							default:
								//The Product ID doesn't match.
								fprintf(stream, "DT_FLASH - not found!\n");
								CloseHandle(FileHandle);
						} // switch

						break;

					case DT_DEVICES:
						switch (Attributes.ProductID) {
							case PIDC_PLASMA_LITE_V2:
							case PIDC_GAMMARAY_V2:
							case PIDC_GAMMARAY_V3:
							case PIDC_GAMMATRON:
							case PIDC_PLASMA_MM2:
							case PIDC_FUSION:
								DeviceList[Dev_Cnt].DevIndex = (USHORT)Dev_Cnt;
								DeviceList[Dev_Cnt].DeviceHandle = FileHandle;

								if (!GetDevParam(&Attributes, &DeviceList[Dev_Cnt], DetailData, DT_DEVICES)) {
									CloseHandle(FileHandle);
									fprintf(stream, "DT_DEVICES - GetDevParam return FALSE\n");
								} else {
									MyDeviceDetected = TRUE;
									Dev_Cnt++;
								}
								break;
							default:
								//The Product ID doesn't match.
								fprintf(stream, "DT_DEVICES - not found!\n");
								CloseHandle(FileHandle);
						}

						break;

					case DT_REPORT:
						switch (Attributes.ProductID) {
							case PIDC_ELECTRONFLUX:
							case PIDC_NITRO_SLG:
							case PIDC_NITRO_XG:
							case PIDC_DRO:
							case 0x9998:	// AU
								DeviceList[Dev_Cnt].DevIndex = (USHORT)Dev_Cnt;
								DeviceList[Dev_Cnt].DeviceHandle = FileHandle;

								if (!GetDevParam(&Attributes, &DeviceList[Dev_Cnt], DetailData, DT_REPORT)) {
									CloseHandle(FileHandle);
									fprintf(stream, "DT_REPORT - GetDevParam return FALSE\n");
								} else {
									MyDeviceDetected = TRUE;
									Dev_Cnt++;
								}
								break;
							default:
								//The Product ID doesn't match.
								fprintf(stream, "DT_REPORT - not found!\n");
								CloseHandle(FileHandle);
						}

						break;

					case DT_SECURITY:
						switch (Attributes.ProductID) {
							case PID_USIM_V1:
								DeviceList[Dev_Cnt].DevIndex = (USHORT)Dev_Cnt;
								DeviceList[Dev_Cnt].DeviceHandle = FileHandle;

								if (!GetDevParam(&Attributes, &DeviceList[Dev_Cnt], DetailData, DT_SECURITY)) {
									CloseHandle(FileHandle);
									//fprintf(stream, "DT_SECURITY - GetDevParam return FALSE\n");
								} else {
									//fprintf(stream, "DT_SECURITY - GetDevParam return TRUE\n");
									MyDeviceDetected = TRUE;
									Dev_Cnt++;
								}
								break;
							default:
								//The Product ID doesn't match.
								//fprintf(stream, "DT_SECURITY - not found!\n");
								CloseHandle(FileHandle);
						}

						break;

					case DT_HID:
						switch (Attributes.ProductID) {
							case PID_0X64B:
							case PID_GAMMARAY:
							case PID_GAMMARAY64:
							case PID_ELECTRONFLUX:
							case PID_NITRO_SLG:
							case PID_NITRO_XG:
							case PID_GAMMARAY_V2:
							case PID_GAMMARAY_V3:
							case PID_GAMMATRON:
							case PID_USIMV1:
							case PID_DRO:
							case 0x9998:	// AU
								DeviceList[Dev_Cnt].DevIndex = (USHORT)Dev_Cnt;
								DeviceList[Dev_Cnt].DeviceHandle = FileHandle;

								if (!GetDevParam(&Attributes, &DeviceList[Dev_Cnt], DetailData, DT_STANDARD)) {
									CloseHandle(FileHandle);
									fprintf(stream, "ALL_DEVICES - GetDevParam return FALSE\n");
								} else {
									MyDeviceDetected = TRUE;
									Dev_Cnt++;
								}

								break;

							default:
								//The Product ID doesn't match.
								fprintf(stream, "ALL_DEVICES - not found!\n");
								CloseHandle(FileHandle);
						} // switch

						break;

					default:
						switch (Attributes.ProductID) {
							case PID_0X64B:
							case PID_GAMMARAY:
							case PID_GAMMARAY64:
							case PID_ELECTRONFLUX:
							case PID_NITRO_SLG:
							case PID_NITRO_XG:
							case PID_GAMMARAY_V2:
							case PID_GAMMARAY_V3:
							case PID_GAMMATRON:
							case PID_4X24BH:
							case PID_6X16B:
							case PID_6X13B:
							case PID_5X18BH:
							case PID_PLASMA:
							case PID_PLASMA_HOTAS:
							case PID_PLASMA_DUAL:
							case PID_PLASMA_HOTAS_DUAL:
							case PID_PLASMA_LITE:
							case PID_PLASMA_LITE_V2:
							case PID_PLASMA_MM2:
							case PID_FUSION:
							case PID_DRO:
								DeviceList[Dev_Cnt].DevIndex = (USHORT)Dev_Cnt;
								DeviceList[Dev_Cnt].DeviceHandle = FileHandle;

								if (!GetDevParam(&Attributes, &DeviceList[Dev_Cnt], DetailData, DT_STANDARD)) {
									CloseHandle(FileHandle);
									fprintf(stream, "ALL_DEVICES - GetDevParam return FALSE\n");
								} else {
									MyDeviceDetected = TRUE;
									Dev_Cnt++;
								}
								break;

							default:
								//The Product ID doesn't match.
								fprintf(stream, "ALL_DEVICES - not found!\n");
								CloseHandle(FileHandle);
						} // switch
				} // switch (Flash_Flag)
			} else {
				//The Vendor ID doesn't match.
				CloseHandle(FileHandle);
			} // if (Attributes.VendorID == VENDOR_ID_1 || Attributes.VendorID == VENDOR_ID_2)

		}  //if (Result != 0)
		else
		{
			//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
			LastDevice = TRUE;
		}
		//If we haven't found the device yet, and haven't tried every available device,
		//try the next one.
		MemberIndex = MemberIndex + 1;

		if (DetailData != NULL) {
			free(DetailData);
			DetailData = NULL;
		}
	} //do 
	while (LastDevice == FALSE);

	if (Dev_Cnt <= 0) MyDeviceDetected = FALSE;
	if (MyDeviceDetected) {
		// Device detected
		*oDev_Cnt = Dev_Cnt;
	} else {
		*oDev_Cnt = 0;
	}

	fprintf(stream, "\nNum Devices Detected : %d\n", (UINT)*oDev_Cnt);
	fclose(stream);

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return MyDeviceDetected;
}

BOOL GetDevParam(HIDD_ATTRIBUTES *Attributes, 
				 DeviceParam *DeviceList, 
				 SP_DEVICE_INTERFACE_DETAIL_DATA *DetailData, 
				 int CFG_TYPE)
{
	size_t Result;
	int retry_cnt;
	wchar_t	mString[MAX_CHAR];

	DeviceList->ProductID = Attributes->ProductID;
	DeviceList->VendorID = Attributes->VendorID;
	DeviceList->VersionNumber = Attributes->VersionNumber;

	GetDeviceCapabilities(DeviceList->DeviceHandle, DeviceList);

	DeviceList->PathLength = 
		(strlen((LPCSTR)DetailData->DevicePath) > MAX_CHAR ? 
		MAX_CHAR : strlen((LPCSTR)DetailData->DevicePath));

	memset(DeviceList->DevicePath, atoi(" "), MAX_CHAR);
	memcpy(DeviceList->DevicePath, DetailData->DevicePath, DeviceList->PathLength);

	// Get Manufacturer Name
	memset(DeviceList->ManufName, atoi(" "), MAX_CHAR);
	memcpy(DeviceList->ManufName, "N/A", 3);
	DeviceList->ManufNameLength = 3;
	Result = 0;
	retry_cnt = 0;

	while (Result != 1 && retry_cnt < MAX_RETRY) {
		Result = HidD_GetManufacturerString (DeviceList->DeviceHandle, mString, sizeof(mString));
		retry_cnt++;
	}

	if ( Result ) {
		if ( wcstombs(DeviceList->ManufName, mString, MAX_CHAR) != -1 ) {
			DeviceList->ManufNameLength = Result;
		} else {
			fprintf(stream, "Manufacturer String - WCS ERROR! N/A\n");
			// Hard code value
			strncpy(DeviceList->ManufName, MANUF_NAME, strlen(MANUF_NAME));
			DeviceList->ManufNameLength = strlen(MANUF_NAME);
		}
	} else {
		fprintf(stream, "Manufacturer String - ERROR : %d! N/A\n", GetLastError());
		// Hard code value
		strncpy(DeviceList->ManufName, MANUF_NAME, strlen(MANUF_NAME));
		DeviceList->ManufNameLength = strlen(MANUF_NAME);
	}

	fprintf(stream, "Manufacturer Name : %s\n", DeviceList->ManufName);

	// Get Device Product Name
	memset(DeviceList->DeviceName, atoi(" "), MAX_CHAR);
	memcpy(DeviceList->DeviceName, "N/A", 3);
	DeviceList->DeviceNameLength = 3;

	Result = 0;
	retry_cnt = 0;
	while ( Result != 1 && retry_cnt < MAX_RETRY) {
		Result = HidD_GetProductString (DeviceList->DeviceHandle, mString, sizeof(mString));
		retry_cnt++;
	}

	if ( Result ) {
		if ( wcstombs(DeviceList->DeviceName, mString, MAX_CHAR) != -1 ) {
			fprintf(stream, "Device Product Name : %s\n", DeviceList->DeviceName);
		} else {
			fprintf(stream, "Device Name String - WCS ERROR! N/A\n");
			// Hard code value
			GetHIDName(Attributes, DeviceList);
		}
	} else {
		fprintf(stream, "Device Name String - ERROR : %d! N/A\n", GetLastError());
		// Hard code value
		GetHIDName(Attributes, DeviceList);
	}

	DeviceList->DeviceNameLength = strlen(DeviceList->DeviceName);
	switch (CFG_TYPE) {
		case DT_DEVICES:	// Look for CFG devices
			fprintf(stream, "CFG_TYPE : DT_DEVICES\n");

			GetHIDName(Attributes, DeviceList);
			DeviceList->DeviceNameLength = strlen(DeviceList->DeviceName);

			if (strstr(DeviceList->DevicePath, "mi_01") != NULL ||
				strstr(DeviceList->DevicePath, "MI_01") != NULL) {
				DeviceList->ConfigFlag = TRUE;
			} else {
				memset(DeviceList, 0, sizeof(DeviceParam));
				return FALSE;
			}
			break;

		case DT_FLASH:
			fprintf(stream, "CFG_TYPE : DT_FLASH\n");

			DeviceList->ConfigFlag = FALSE;
			break;

		case DT_REPORT:
		case DT_SECURITY:
			fprintf(stream, "CFG_TYPE : DT_REPORT\n");

			GetHIDName(Attributes, DeviceList);
			DeviceList->DeviceNameLength = strlen(DeviceList->DeviceName);

			DeviceList->ConfigFlag = TRUE;
			break;

		case DT_STANDARD:	// DT_HID
			fprintf(stream, "CFG_TYPE : DT_STANDARD\n");

			if (strstr(DeviceList->DevicePath, "mi_01") != NULL ||
				strstr(DeviceList->DevicePath, "MI_01") != NULL) {
				// Second Interface found. CFG type unless we are dealing with Plasma V2 dual mode
				// skip if Plasma DUAL modes
				if (DeviceList->ProductID != PID_PLASMA_DUAL &&
					DeviceList->ProductID != PID_PLASMA_HOTAS_DUAL) {
					memset(DeviceList, 0, sizeof(DeviceParam));
					return FALSE;			
				}
				DeviceList->ConfigFlag = FALSE;
			} else {
				DeviceList->ConfigFlag = FALSE;
			}
			break;
	}

	// Get Device Serial Number
	memcpy(DeviceList->SerialNum, ("N/A"), 3);
	DeviceList->SerialNumLength = 3;
	Result = 0;
	retry_cnt = 0;
	while ( Result != 1 && retry_cnt < MAX_RETRY) {
		Result = HidD_GetSerialNumberString (DeviceList->DeviceHandle, mString,	sizeof(mString));
		retry_cnt++;
	}

	if ( Result ) {
		if ( wcstombs(DeviceList->SerialNum, mString, 3) == -1 ) {
			memcpy(DeviceList->SerialNum, "N/A", 3);
			fprintf(stream, "Serial String - NON! N/A\n");
		}
	} else {
		fprintf(stream, "Device Serial String - ERROR : %d! N/A\n", GetLastError());
	}

	DeviceList->SerialNum[3] = NULL;	// Null terminate string

	return TRUE;
}

void GetDeviceCapabilities(HANDLE iDeviceHandle, DeviceParam *DeviceList)
{
	DevicePHID	DevicePHIDList;
	HIDP_CAPS	Capabilities;
	int Idx = 0;

	//Get the Capabilities structure for the device.

	HidD_GetPreparsedData (iDeviceHandle, &DevicePHIDList.PreparsedData);
	HidP_GetCaps (DevicePHIDList.PreparsedData, &Capabilities);

	DeviceList->InputReportByteLength = Capabilities.InputReportByteLength;
	DeviceList->OutputReportByteLength = Capabilities.OutputReportByteLength;
	DeviceList->Usage = Capabilities.Usage;
	DeviceList->UsagePage = Capabilities.UsagePage;

	DeviceList->NumberInputIndices = 0;
	DeviceList->NumberOutputIndices = 0;
	DeviceList->NumberPortIndices = 0;
	DeviceList->NumberAnalogIndices = 0;
	DeviceList->NumberHATIndices = 0;
	DeviceList->NumberAlphanumericIndices = 0;
	DeviceList->NumberLCDIndices = 0;
	DeviceList->NumberGLCDIndices = 0;
	DeviceList->NumberPWMIndices = 0;
	DeviceList->NumberRotaryIndices = 0;
	DeviceList->NumberMuxDisplayIndices = 0;
	DeviceList->NumberCRTRWRIndices = 0;
	DeviceList->NumberAlphanumericIndices = 0;
	DeviceList->NumberSPIIndices = 0;
	DeviceList->NumberLatchedIndices = 0;
	DeviceList->NumberDotMatrixIndices = 0;
	DeviceList->NumberStepperIndices = 0;

	if (Capabilities.Usage == 0x0004 && Capabilities.UsagePage == 0x0001) {
		// we have a standard joystick. Get joystick information

		HIDP_REPORT_TYPE    ReportType = HidP_Input;
		HIDP_BUTTON_CAPS    ButtonCaps[2]; //support max 2 button caps
		USHORT              ButtonCapsLength;


		ButtonCapsLength = Capabilities.NumberInputButtonCaps;

		HidP_GetButtonCaps( ReportType,
							ButtonCaps,
							&ButtonCapsLength,
							DevicePHIDList.PreparsedData);

		// ButtonCaps[x].Range.UsageMax returns number of button inputs
		for (int i=0; i < ButtonCapsLength; i++) {
			DeviceList->NumberInputIndices = (USHORT)(DeviceList->NumberInputIndices + ButtonCaps[i].Range.UsageMax);
//			DeviceList[Index]->ButtonIndexMin[i] = ButtonCaps[i].Range.DataIndexMin;
//			DeviceList[Index]->ButtonIndexMax[i] = ButtonCaps[i].Range.DataIndexMax;
		}
	} else {
		// Standard HID device found
		switch (DeviceList->ProductID) {
			case PID_0X64B:
				Idx = 1;
				break;
			case PID_GAMMARAY:
				Idx = 2;
				break;
			case PID_GAMMARAY64:
				Idx = 3;
				break;
			case PID_ELECTRONFLUX:	// EFLUX
				Idx = 4;
				break;
			case PID_GAMMARAY_V2:
				Idx = 5;
				break;
			case PID_GAMMARAY_V3:
				Idx = 10;
				break;
			case PID_GAMMATRON:
				Idx = 6;
				break;
			case PID_PLASMA_LITE_V2:	// PLV2
				Idx = 7;
				break;
			case PID_NITRO_SLG:
				Idx = 8;
				break;
			case PID_NITRO_XG:
				Idx = 11;
				break;
			case PID_PLASMA_MM2:
				Idx = 12;
				break;
			case PID_DRO:
				Idx = 13;
				break;
			default:
				Idx = 0; 
		}

		DeviceList->NumberInputIndices = DevArray[Idx].NumberInputIndices;
		DeviceList->NumberOutputIndices = DevArray[Idx].NumberOutputIndices;
		DeviceList->NumberPortIndices = DevArray[Idx].NumberPortIndices;
		DeviceList->NumberAnalogIndices = DevArray[Idx].NumberAnalogIndices;
		DeviceList->NumberHATIndices = DevArray[Idx].NumberHATIndices;
		DeviceList->NumberAlphanumericIndices = DevArray[Idx].NumberAlphanumericIndices;
		DeviceList->NumberLCDIndices = DevArray[Idx].NumberLCDIndices;
		DeviceList->NumberGLCDIndices = DevArray[Idx].NumberGLCDIndices;
		DeviceList->NumberPWMIndices = DevArray[Idx].NumberPWMIndices;
		DeviceList->NumberRotaryIndices = DevArray[Idx].NumberRotaryIndices;
		DeviceList->NumberMuxDisplayIndices = DevArray[Idx].NumberMuxDisplayIndices;
		DeviceList->NumberCRTRWRIndices = DevArray[Idx].NumberCRTRWRIndices;
		DeviceList->NumberAlphanumericIndices = DevArray[Idx].NumberAlphanumericIndices;
		DeviceList->NumberSPIIndices = DevArray[Idx].NumberSPIIndices;
		DeviceList->NumberLatchedIndices = DevArray[Idx].NumberLatchedIndices;
		DeviceList->NumberDotMatrixIndices = DevArray[Idx].NumberDotMatrixIndices;
		DeviceList->NumberStepperIndices = DevArray[Idx].NumberStepperIndices;
	}

	if (DevicePHIDList.PreparsedData != NULL)
		HidD_FreePreparsedData(DevicePHIDList.PreparsedData);
}

HID_API void APIENTRY RetrieveStatus(DeviceParam *pDevice, DeviceStatus *pStatus)
{
	UCHAR nNode, nPort;
	int i;
	BYTE Data_Buffer[65];
	long retval;
	BOOL StatusFlag;
	STRUCT_HAT		HAT_Buffer[EE_HAT_CHANNELS];
	STRUCT_ACE		ACE_Buffer[EE_ACE_CHANNELS];
	EF_PORT_CONFIG	EEP_Buffer[2];
	OUTPUT_CONFIG	EEOUT_Buffer;
	SPI_CONFIG		EESPI_Buffer;
	STEPPER_CONFIG	EESTEPPER_Buffer;

	stream = fopen("biusb_dev.log", "a");

	// Apply DEFAULTS
	for (i = 0; i < EE_ANALOG_CHANNELS; i++) {
		pStatus->AnalogActive[i] = 1;
	}

	for (i = 0; i < MAX_INPUTS; i++) {
		pStatus->InputActive[i] = 1;
	}

	for (i = 0; i < MAX_PORTS; i++) {
		pStatus->PortActive[i] = 1;
		pStatus->PortIOMode[i] = 1;	// default to input
		pStatus->PortMode[i] = MODE_DIRECT;
	}

	for (i = 0; i < MAX_ROTARY; i++) {
		pStatus->RotaryActive[i] = 1;
	}

	for (i = 0; i < MAX_OUTPUTS; i++) {
		pStatus->OutputActive[i] = 1;
	}

	for (i = 0; i < MAX_SPI; i++) {
		pStatus->SPIActive[i] = 1;
	}

	for (i = 0; i < MAX_LATCHED; i++) {
		pStatus->LatchedActive[i] = 1;
	}

	for (i = 0; i < MAX_DOTMATRIX; i++) {
		pStatus->DotMatrixActive[i] = 1;
	}

	for (i = 0; i < MAX_STEPPER; i++) {
		pStatus->StepperActive[i] = 1;
	}

	switch(pDevice->ProductID) {
		case PID_PLASMA_LITE_V2:	// PLASMA-LITE V2
			if (!pDevice->ConfigFlag) break;
 
			// EE_HAT_CHANNELS
			Data_Buffer[REPORT_ID] = 0;	// Report ID
			Data_Buffer[REC_TYPE] = READ_EE_HAT;
			
			retval = WriteData(pDevice, (CHAR *)Data_Buffer);

			// read data
			StatusFlag = TRUE;
			retval = 0;
			retval = ReadData(pDevice, (CHAR *)Data_Buffer);

			if (retval == DEV_INPUT) {
				if (Data_Buffer[0] == READ_EE_HAT) {
					for (i=0; i<EE_HAT_CHANNELS; i++)
						memcpy(&HAT_Buffer[i], &Data_Buffer[i*EE_HAT_BYTES + 1], EE_HAT_BYTES);

					pStatus->PortActive[0] = HAT_Buffer[0].hat_on;

					if (HAT_Buffer[0].mode == 4) {
						pStatus->PortMode[0] = MODE_MIXED;	// Direct output mode
						pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
					} else 	if (HAT_Buffer[0].mode == 5 || HAT_Buffer[0].mode == 6) {
						pStatus->PortMode[0] = MODE_MIXED;	// DAC mode
						pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
					} else {
						pStatus->PortMode[0] = MODE_DIRECT;		
						pStatus->PortIOMode[0] = IO_INPUT;	// input mode
					}

					if (HAT_Buffer[1].mode == 2) {
						// SD9 mode
						pStatus->PortMode[0] = MODE_MIXED;	// Mux mode
						pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
					} else if (HAT_Buffer[1].mode == 4 || HAT_Buffer[1].mode == 5) {
						pStatus->PortMode[0] = MODE_MIXED;	// DAC mode
						pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
					} else {
						pStatus->PortMode[0] = MODE_DIRECT;	
						pStatus->PortIOMode[0] = IO_INPUT;	// input mode
					}
					
				} else {
					StatusFlag = FALSE;
				}
			} else {
				StatusFlag = FALSE;
			}

			if (!StatusFlag) {
				// something went wrong
				for (i = 0; i < 2; i++) {
					pStatus->PortActive[i] = 0;
					pStatus->PortIOMode[i] = 1;	// default to input
					pStatus->PortMode[i] = MODE_DIRECT;
				}
			}

			// EE_ACE_CHANNELS
			Data_Buffer[REPORT_ID] = 0;	// Report ID
			Data_Buffer[REC_TYPE] = READ_EE_ACE;
			
			retval = WriteData(pDevice, (CHAR *)Data_Buffer);

			// read data
			StatusFlag = TRUE;
			retval = 0;
			retval = ReadData(pDevice, (CHAR *)Data_Buffer);

			if (retval == DEV_INPUT) {
				if (Data_Buffer[0] == READ_EE_ACE) {
					for (i=0; i<EE_ACE_CHANNELS; i++)
						memcpy(&ACE_Buffer[i], &Data_Buffer[i*EE_ACE_BYTES + 1], EE_ACE_BYTES);

					pStatus->PortActive[0] = ACE_Buffer[0].ace_on;

					if (ACE_Buffer[0].ace_type == ACE_CLCD) {
						pStatus->PortMode[0] = MODE_MIXED;	// Direct output mode
						pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
					} else {
						pStatus->PortMode[0] = MODE_DIRECT;		
						pStatus->PortIOMode[0] = IO_INPUT;	// input mode
					}					
				} else {
					StatusFlag = FALSE;
				}
			} else {
				StatusFlag = FALSE;
			}

			if (!StatusFlag) {
				// something went wrong
				pStatus->PortActive[0] = 0;
				pStatus->PortIOMode[0] = 1;	// default to input
				pStatus->PortMode[0] = MODE_DIRECT;
			}
			break;

		case PID_NITRO_SLG:
			pStatus->PortMode[0] = MODE_MIXED;
			pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
			for (i=0; i<8; i++) {
				pStatus->PortActive[i] = 1;
			}
			break;

		case PID_NITRO_XG:
			pStatus->PortMode[0] = MODE_MIXED;
			pStatus->PortIOMode[0] = IO_OUTPUT;	// output mode
			for (i=0; i<10; i++) {
				pStatus->PortActive[i] = 1;
			}
			break;

		case PID_ELECTRONFLUX:	// ElectronFlux
			// Default ALL status data to failed.

			for (i = 0; i < MAX_ALPHANUMERICS; i++)
				pStatus->AlphanumericActive[i] = 0;	// Not supported

			for (i = 0; i < MAX_LCDS; i++)
				pStatus->LCDActive[i] = 0;			// Not supported
			
			for (i = 0; i < MAX_PORTS; i++) {
				pStatus->PortActive[i] = 0;
				pStatus->PortIOMode[i] = 1;
				pStatus->PortMode[i] = MODE_DIRECT;
			}

			for (i = 0; i < MAX_OUTPUTS; i++)
				pStatus->OutputActive[i] = 0;

			for (i = 0; i < MAX_MUXDISPLAYS; i++)
				pStatus->MuxDisplayActive[i] = 0;

			for (i = 0; i < MAX_RWR; i++)
				pStatus->CRTRWRActive[i] = 0;

			for (i = 0; i < MAX_GLCDS; i++)
				pStatus->GLCDActive[i] = 0;

			for (i = 0; i < MAX_SPI; i++)
				pStatus->SPIActive[i] = 0;

			for (i = 0; i < MAX_LATCHED; i++)
				pStatus->LatchedActive[i] = 0;

			for (i = 0; i < MAX_DOTMATRIX; i++)
				pStatus->DotMatrixActive[i] = 0;

			for (i = 0; i < MAX_STEPPER; i++)
				pStatus->StepperActive[i] = 0;

			// Read PORTS EEPROM from device
			for (nPort = 0; nPort < pDevice->NumberPortIndices; nPort+=2) {
				nNode = (UCHAR)(nPort>>1);

				// retrieve Port status
				Data_Buffer[REPORT_ID] = 0;	// Report ID
				Data_Buffer[REC_LEN] = 0;
				Data_Buffer[ADD_L] = (unsigned char)EEEF_PORT_INDEX;
				Data_Buffer[ADD_H] = (unsigned char)EEEF_PORT_BYTES;
				Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
				Data_Buffer[CMD_FLAGS] = 0;
				Data_Buffer[NODE_REC_TYPE] = (unsigned char)READ_EE_PORT;
				Data_Buffer[NODE_READ_LEN] = (unsigned char)EEEF_PORT_BYTES;
				Data_Buffer[NODE_ADDRESS] = nNode;

				retval = WriteData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\n\nNode = %d\nWrite Ports return = %d", nNode, retval);

				// read data
				StatusFlag = TRUE;
				retval = 0;
				retval = ReadData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\nRead Ports return = %d", retval);

				if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_PORT) {
					fprintf(stream, "\nRead Ports OK");
					memset(&EEP_Buffer, 0, EEEF_BYTES_PER_PORT);
					for (i = 0; i < 2; i++) {
						memcpy(&EEP_Buffer[i], &Data_Buffer[i*EEEF_BYTES_PER_PORT + 1], EEEF_BYTES_PER_PORT);
					}

					pStatus->PortActive[nPort] = EEP_Buffer[0].active;
					pStatus->PortIOMode[nPort] = EEP_Buffer[0].iomode;
					pStatus->PortMode[nPort] = EEP_Buffer[0].mode;

					if (nPort+1 < MAX_PORTS) {
						pStatus->PortActive[nPort+1] = EEP_Buffer[1].active;
						pStatus->PortIOMode[nPort+1] = EEP_Buffer[1].iomode;
						pStatus->PortMode[nPort+1] = EEP_Buffer[1].mode;
					}

					// Read OUTPUTS EEPROM from device
					Data_Buffer[REPORT_ID] = 0;	// Report ID
					Data_Buffer[REC_LEN] = 0;
					Data_Buffer[ADD_L] = (unsigned char)(EEEF_OUTPUT1_INDEX);
					Data_Buffer[ADD_H] = (unsigned char)(EEEF_OUTPUT1_BYTES+EEEF_OUTPUT2_BYTES);
					Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
					Data_Buffer[CMD_FLAGS] = 0;
					Data_Buffer[NODE_REC_TYPE] = (unsigned char)READ_EE_OUTPUT;
					Data_Buffer[NODE_READ_LEN] = (unsigned char)(EEEF_OUTPUT1_BYTES+EEEF_OUTPUT2_BYTES);
					Data_Buffer[NODE_ADDRESS] = nNode;

					retval = WriteData(pDevice, (CHAR *)Data_Buffer);

					fprintf(stream, "\n\nNode = %d\nWrite Outputs return = %d", nNode, retval);

					// read data
					StatusFlag = TRUE;
					retval = 0;
					retval = ReadData(pDevice, (CHAR *)Data_Buffer);

					fprintf(stream, "\nRead Outputs return = %d", retval);

					if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_OUTPUT) {
						fprintf(stream, "\nRead Outputs OK");
						for (i = 0; i < EEEF_NUM_OUTPUTS; i++) {
							memset(&EEOUT_Buffer, 0, EEEF_BYTES_PER_OUTPUT);
							memcpy((void *)&EEOUT_Buffer, &Data_Buffer[i*EEEF_BYTES_PER_OUTPUT + 1], EEEF_BYTES_PER_OUTPUT);
							pStatus->OutputActive[nNode*EEEF_NUM_OUTPUTS + i] = EEOUT_Buffer.active;
						}

						// Read Multiplexed EEPROM from device
						if (pStatus->PortActive[nPort] &&
							pStatus->PortIOMode[nPort] == IO_OUTPUT &&
							pStatus->PortMode[nPort] == MODE_MUX) {

							for (i = 0; i < NUM_MUX_PER_PORT; i++)
								pStatus->MuxDisplayActive[nPort*NUM_MUX_PER_PORT + i] = 1;
						} else {
							for (i = 0; i < NUM_MUX_PER_PORT; i++)
								pStatus->MuxDisplayActive[nPort*NUM_MUX_PER_PORT + i] = 0;
						}

						if (nPort+1 < MAX_PORTS) {
							if (pStatus->PortActive[nPort+1] &&
								pStatus->PortIOMode[nPort+1] == IO_OUTPUT &&
								pStatus->PortMode[nPort+1] == MODE_MUX) {

								for (i = 0; i < NUM_MUX_PER_PORT; i++)
									pStatus->MuxDisplayActive[(nPort+1)*NUM_MUX_PER_PORT + i] = 1;
							} else {
								for (i = 0; i < NUM_MUX_PER_PORT; i++)
									pStatus->MuxDisplayActive[(nPort+1)*NUM_MUX_PER_PORT + i] = 0;
							}
						}

						// Read CRT RWR EEPROM from device
						if (pStatus->PortActive[nPort] &&
							pStatus->PortIOMode[nPort] == IO_OUTPUT &&
							pStatus->PortMode[nPort] == MODE_RWR) {

							pStatus->CRTRWRActive[nPort] = 1;
						} else {
							pStatus->CRTRWRActive[nPort] = 0;
						}

						if (nPort+1 < MAX_PORTS) {
							if (pStatus->PortActive[nPort+1] &&
								pStatus->PortIOMode[nPort+1] == IO_OUTPUT &&
								pStatus->PortMode[nPort+1] == MODE_RWR) {

								pStatus->CRTRWRActive[(nPort+1)] = 1;
							} else {
								pStatus->CRTRWRActive[(nPort+1)] = 0;
							}
						}

						// Read GLCD EEPROM from device
						if (pStatus->PortActive[nPort] &&
							pStatus->PortIOMode[nPort] == IO_OUTPUT &&
							pStatus->PortMode[nPort] == MODE_GLCD) {

							pStatus->GLCDActive[nPort] = 1;
						} else {
							pStatus->GLCDActive[nPort] = 0;
						}

						if (nPort+1 < MAX_PORTS) {
							if (pStatus->PortActive[nPort+1] &&
								pStatus->PortIOMode[nPort+1] == IO_OUTPUT &&
								pStatus->PortMode[nPort+1] == MODE_GLCD) {

								pStatus->GLCDActive[(nPort+1)] = 1;
							} else {
								pStatus->GLCDActive[(nPort+1)] = 0;
							}
						}

						// Read Latched EEPROM from device
						if (pStatus->PortActive[nPort] &&
							pStatus->PortIOMode[nPort] == IO_OUTPUT &&
							pStatus->PortMode[nPort] == MODE_LATCHED) {

							for (i = 0; i < NUM_LATCHED_PER_PORT; i++)
								pStatus->LatchedActive[nPort*NUM_LATCHED_PER_PORT + i] = 1;
						} else {
							for (i = 0; i < NUM_LATCHED_PER_PORT; i++)
								pStatus->LatchedActive[nPort*NUM_LATCHED_PER_PORT + i] = 0;
						}

						if (nPort+1 < MAX_PORTS) {
							if (pStatus->PortActive[nPort+1] &&
								pStatus->PortIOMode[nPort+1] == IO_OUTPUT &&
								pStatus->PortMode[nPort+1] == MODE_LATCHED) {

								for (i = 0; i < NUM_LATCHED_PER_PORT; i++)
									pStatus->LatchedActive[(nPort+1)*NUM_LATCHED_PER_PORT + i] = 1;
							} else {
								for (i = 0; i < NUM_LATCHED_PER_PORT; i++)
									pStatus->LatchedActive[(nPort+1)*NUM_LATCHED_PER_PORT + i] = 0;
							}
						}

						// Read Dot Matrix EEPROM from device
						if (pStatus->PortActive[nPort] &&
							pStatus->PortIOMode[nPort] == IO_OUTPUT &&
							pStatus->PortMode[nPort] == MODE_DOTMATRIX) {

							for (i = 0; i < NUM_DOTMATRIX_PER_PORT; i++)
								pStatus->DotMatrixActive[nPort*NUM_DOTMATRIX_PER_PORT + i] = 1;
						} else {
							for (i = 0; i < NUM_DOTMATRIX_PER_PORT; i++)
								pStatus->DotMatrixActive[nPort*NUM_DOTMATRIX_PER_PORT + i] = 0;
						}

						if (nPort+1 < MAX_PORTS) {
							if (pStatus->PortActive[nPort+1] &&
								pStatus->PortIOMode[nPort+1] == IO_OUTPUT &&
								pStatus->PortMode[nPort+1] == MODE_DOTMATRIX) {

								for (i = 0; i < NUM_DOTMATRIX_PER_PORT; i++)
									pStatus->DotMatrixActive[(nPort+1)*NUM_DOTMATRIX_PER_PORT + i] = 1;
							} else {
								for (i = 0; i < NUM_DOTMATRIX_PER_PORT; i++)
									pStatus->DotMatrixActive[(nPort+1)*NUM_DOTMATRIX_PER_PORT + i] = 0;
							}
						}
					} // if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_OUTPUT) {
				} // if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_PORT) {

				// Read SPI EEPROM from device
				Data_Buffer[REPORT_ID] = 0;	// Report ID
				Data_Buffer[REC_LEN] = 0;
				Data_Buffer[ADD_L] = (unsigned char)(EEEF_SPI1_INDEX);
				Data_Buffer[ADD_H] = (unsigned char)(EEEF_SPI1_BYTES+EEEF_SPI2_BYTES);
				Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
				Data_Buffer[CMD_FLAGS] = 0;
				Data_Buffer[NODE_REC_TYPE] = (unsigned char)READ_EE_SPI;
				Data_Buffer[NODE_READ_LEN] = (unsigned char)(EEEF_SPI1_BYTES+EEEF_SPI2_BYTES);
				Data_Buffer[NODE_ADDRESS] = nNode;

				retval = WriteData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\n\nNode = %d\nWrite SPI return = %d", nNode, retval);

				// read data
				StatusFlag = TRUE;
				retval = 0;
				retval = ReadData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\nRead SPI return = %d", retval);

				if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_SPI) {
					fprintf(stream, "\nRead SPI OK");
					for (i = 0; i < EEEF_NUM_SPI; i++) {
						memset(&EESPI_Buffer, 0, EEEF_BYTES_PER_SPI);
						memcpy((void *)&EESPI_Buffer, &Data_Buffer[i*EEEF_BYTES_PER_SPI + 1], EEEF_BYTES_PER_SPI);
						pStatus->SPIActive[nNode*EEEF_NUM_SPI + i] = EESPI_Buffer.active;
					}

				} // if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_SPI) {

				// Read Stepper EEPROM from device
				Data_Buffer[REPORT_ID] = 0;	// Report ID
				Data_Buffer[REC_LEN] = 0;
				Data_Buffer[ADD_L] = (unsigned char)(EEEF_STEPPER1_INDEX);
				Data_Buffer[ADD_H] = (unsigned char)EEEF_STEPPER1_BYTES;
				Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
				Data_Buffer[CMD_FLAGS] = 0;
				Data_Buffer[NODE_REC_TYPE] = (unsigned char)READ_EE_STEPPER;
				Data_Buffer[NODE_READ_LEN] = (unsigned char)EEEF_STEPPER1_BYTES;
				Data_Buffer[NODE_ADDRESS] = nNode;

				retval = WriteData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\n\nNode = %d\nWrite Stepper 1 return = %d", nNode, retval);

				// read data
				StatusFlag = TRUE;
				retval = 0;
				retval = ReadData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\nRead Stepper 1 return = %d", retval);

				if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_STEPPER) {
					fprintf(stream, "\nRead Stepper 1 OK");
					for (i = 0; i < EEEF_NUM_STEPPER_PER_PORT; i++) {
						memset(&EESTEPPER_Buffer, 0, EEEF_BYTES_PER_STEPPER);
						memcpy((void *)&EESTEPPER_Buffer, &Data_Buffer[i*EEEF_BYTES_PER_STEPPER + 1], EEEF_BYTES_PER_STEPPER);
						pStatus->StepperActive[nPort*NUM_STEPPER_PER_PORT + i] = EESTEPPER_Buffer.active;
					}
				}

				Data_Buffer[REPORT_ID] = 0;	// Report ID
				Data_Buffer[REC_LEN] = 0;
				Data_Buffer[ADD_L] = (unsigned char)(EEEF_STEPPER2_INDEX);
				Data_Buffer[ADD_H] = (unsigned char)EEEF_STEPPER2_BYTES;
				Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
				Data_Buffer[CMD_FLAGS] = 0;
				Data_Buffer[NODE_REC_TYPE] = (unsigned char)READ_EE_STEPPER;
				Data_Buffer[NODE_READ_LEN] = (unsigned char)EEEF_STEPPER2_BYTES;
				Data_Buffer[NODE_ADDRESS] = nNode;

				retval = WriteData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\n\nNode = %d\nWrite Stepper 2 return = %d", nNode, retval);

				// read data
				StatusFlag = TRUE;
				retval = 0;
				retval = ReadData(pDevice, (CHAR *)Data_Buffer);

				fprintf(stream, "\nRead Stepper 2 return = %d", retval);

				if (retval == DEV_INPUT && Data_Buffer[0] == READ_EE_STEPPER) {
					fprintf(stream, "\nRead Stepper 2 OK");
					for (i = 0; i < EEEF_NUM_STEPPER_PER_PORT; i++) {
						memset(&EESTEPPER_Buffer, 0, EEEF_BYTES_PER_STEPPER);
						memcpy((void *)&EESTEPPER_Buffer, &Data_Buffer[i*EEEF_BYTES_PER_STEPPER + 1], EEEF_BYTES_PER_STEPPER);
						pStatus->StepperActive[(nPort+1)*NUM_STEPPER_PER_PORT + i] = EESTEPPER_Buffer.active;
					}
				}
			} // for (nPort = 0; nPort < pDevice->NumberPortIndices; nPort+=2) {

			break;
	}

	fclose(stream);
}

HID_API LONG APIENTRY ReadInputData(DeviceParam *oDeviceList, CHAR *oDataBuffer, UINT Raw_Flag)
{
	DWORD		Result;
	DWORD		lNumberOfBytes = 0;
	LONG		return_flag;
	OVERLAPPED	HidOverlapped;
	CHAR		InputReport[MAX_INPUT_REPORT_BYTES];
	HANDLE		EventObject = NULL;

	//Read a report from the device.

	// Create a new event for report capture
	EventObject = CreateEvent(NULL, false, false, NULL);

	// fill the HidOverlapped structure so that Windows knows which
	// event to cause when the device sends an IN report
	HidOverlapped.hEvent = EventObject;
	HidOverlapped.Offset = 0;
	HidOverlapped.OffsetHigh = 0;

	Result = ReadFile(	oDeviceList->DeviceHandle,					// handle to device
						InputReport,								// IN report buffer to fill
						oDeviceList->InputReportByteLength,			// input buffer size 
						&lNumberOfBytes,							// returned buffer size 
						(LPOVERLAPPED) &HidOverlapped );			// long pointer to an OVERLAPPED structure

	Result = WAIT_TIMEOUT;
	Result = WaitForSingleObject(EventObject, 1000);

	switch (Result) {
		case WAIT_OBJECT_0:
			// ReadFile Completed
			StoreInputReport(oDeviceList, oDataBuffer, InputReport, Raw_Flag);
			return_flag = DEV_INPUT;
			break;

		case WAIT_ABANDONED:	// Can't read from device			
			//A timeout may mean that the device has been removed. 
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_FAILED;
			break;

		case WAIT_TIMEOUT:
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_TIMEOUT;
			break;

		case WAIT_FAILED:
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_FAILED;
			break;

		default:
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_FAILED;
	} // switch (Result)

	return return_flag;
}

HID_API LONG APIENTRY ReadData(DeviceParam *oDeviceList, CHAR *oDataBuffer)
{
	DWORD		Result;
	DWORD		lNumberOfBytes = 0;
	LONG		return_flag;
	OVERLAPPED	HidOverlapped;
	CHAR		InputReport[MAX_INPUT_REPORT_BYTES];
	HANDLE		EventObject = NULL;

	//Read a report from the device.

	// Create a new event for report capture
	EventObject = CreateEvent(NULL, false, false, NULL);

	// fill the HidOverlapped structure so that Windows knows which
	// event to cause when the device sends an IN report
	HidOverlapped.hEvent = EventObject;
	HidOverlapped.Offset = 0;
	HidOverlapped.OffsetHigh = 0;

	Result = ReadFile(	oDeviceList->DeviceHandle,					// handle to device
						InputReport,								// IN report buffer to fill
						oDeviceList->InputReportByteLength,			// input buffer size 
						&lNumberOfBytes,							// returned buffer size 
						(LPOVERLAPPED) &HidOverlapped );			// long pointer to an OVERLAPPED structure

	Result = WAIT_TIMEOUT;
	Result = WaitForSingleObject(EventObject, 1000);

	switch (Result) {
		case WAIT_OBJECT_0:
			// ReadFile Completed
			StoreInputReport(oDeviceList, oDataBuffer, InputReport, true);
			return_flag = DEV_INPUT;
			break;

		case WAIT_ABANDONED:	// Can't read from device			
			//A timeout may mean that the device has been removed. 
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_FAILED;
			break;

		case WAIT_TIMEOUT:
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_TIMEOUT;
			break;

		case WAIT_FAILED:
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_FAILED;
			break;

		default:
			CancelIo(&oDeviceList->DeviceHandle);
			return_flag = DEV_FAILED;
	} // switch (Result)

	return return_flag;
}

HID_API LONG APIENTRY WriteData(DeviceParam *oDeviceList, CHAR *oDataBuffer)
{
	//Send a report to the device.
	LONG	return_flag = DEV_WAIT;
	DWORD	Result;
	DWORD	BytesWritten;
	HANDLE		ReportEvent;
	OVERLAPPED	HidOverlapped;

	// Create a new event for report capture
	ReportEvent = CreateEvent(NULL, false, false, NULL);

	// fill the HidOverlapped structure so that Windows knows which
	// event to cause when the device sends an IN report
	HidOverlapped.hEvent = ReportEvent;
	HidOverlapped.Offset = 0;
	HidOverlapped.OffsetHigh = 0;

	Result = WriteFile 
		(oDeviceList->DeviceHandle, 
		oDataBuffer, 
		oDeviceList->OutputReportByteLength, 
		&BytesWritten, 
		(LPOVERLAPPED) &HidOverlapped);

	Result = WaitForSingleObject(ReportEvent, 1000);

	//return_flag = BytesWritten;
	// if the transaction timed out, then we have to manually cancel the request
	if (Result != WAIT_OBJECT_0) {
		ResetEvent(ReportEvent);
		CancelIo(&oDeviceList->DeviceHandle);
		return_flag = 0;
	} else {
		return_flag = oDeviceList->OutputReportByteLength;
	}

	//Return the result of the API call and the report bytes.	
	return return_flag;
}

void StoreInputReport(DeviceParam *oDeviceList, CHAR *oDataBuffer, CHAR *InputReport, BOOL Raw_Flag)
{
	USHORT	ByteNumber;
	USHORT	BitNumber;
	CHAR	ReceivedByte;
	DevicePHID	DevicePHIDList;
	
	if (!HidD_GetPreparsedData (oDeviceList->DeviceHandle, &DevicePHIDList.PreparsedData)) return;

	if (oDeviceList->Usage == 0x0004 && 
		oDeviceList->UsagePage == 0x0001) {
		// we have a standard joystick. Get joystick information
		HIDP_REPORT_TYPE    ReportType = HidP_Input;
		HIDP_BUTTON_CAPS    ButtonCaps[2]; //support max 2 button caps
		USHORT              ButtonCapsLength = 2;
		HIDP_DATA			DataList[256];
		ULONG				DataLength = 256;

		HidP_GetButtonCaps( ReportType,
							ButtonCaps,
							&ButtonCapsLength,
							DevicePHIDList.PreparsedData);

		HidP_GetData(	ReportType,
						DataList,
						&DataLength,
						DevicePHIDList.PreparsedData,
						InputReport,
						oDeviceList->InputReportByteLength);

		memset(oDataBuffer, 0, MAX_INPUTS); 

		// ButtonCaps[x].Range.UsageMax returns number of button inputs
		for (UINT i = 0; i < DataLength; i++) {
			if (DataList[i].DataIndex >= ButtonCaps[0].Range.DataIndexMin &&
				DataList[i].DataIndex <= ButtonCaps[0].Range.DataIndexMax) {
				if (DataList[i].On) {
					oDataBuffer[(DataList[i].DataIndex - ButtonCaps[0].Range.DataIndexMin)] = 1;
				}
			} else if (	ButtonCapsLength > 1) {
				if (DataList[i].DataIndex >= ButtonCaps[1].Range.DataIndexMin &&
					DataList[i].DataIndex <= ButtonCaps[1].Range.DataIndexMax) {
					if (DataList[i].On) {
						oDataBuffer[(DataList[i].DataIndex - ButtonCaps[0].Range.DataIndexMin)] = 1;
					}
				}
			}
		}

	} else {
		//Step through the received bytes and store each.
		for (ByteNumber=0; ByteNumber < oDeviceList->InputReportByteLength - 1; ByteNumber++) {
			//Get a byte.
			//oDataBuffer[ByteNumber] = InputReport[oDeviceList->DevIndex][ByteNumber+1];
			ReceivedByte = InputReport[ByteNumber+1];

			if (Raw_Flag) {
				oDataBuffer[ByteNumber] = ReceivedByte;
			} else {
				// store byte to bits in buffer
				for (BitNumber=0; BitNumber < 8; BitNumber++)
					oDataBuffer[(ByteNumber*8+BitNumber)] = (char)((ReceivedByte >> BitNumber) & 0x0001);
			}
		}
	}

	HidD_FlushQueue(oDeviceList->DeviceHandle);

	if (DevicePHIDList.PreparsedData != NULL)
		HidD_FreePreparsedData(DevicePHIDList.PreparsedData);
}

HID_API LONG APIENTRY ReadInputs(DeviceParam *iDeviceList, char *iDataBuffer)
{
	long retval;

	// Read Rotary Channels
	iDataBuffer[REPORT_ID] = 0;	// Report ID
	iDataBuffer[REC_TYPE] = READ_INPUTS;
	
	retval = WriteData(iDeviceList, iDataBuffer);

	   // read data
    retval = 0;
	retval = ReadInputData(iDeviceList, iDataBuffer, false);

	//Return the result of the API call and the report bytes.
	return retval;
}

HID_API LONG APIENTRY ResetRotary(DeviceParam *iDeviceList)
{
	DWORD	Result;
	BYTE	Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	// EE_HAT_CHANNELS
	Data_Buffer[REPORT_ID] = 0;	// Report ID
	Data_Buffer[REC_TYPE] = RESET_ROTARY;
	
	Result = WriteData(iDeviceList, (char *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY ReadRotary(DeviceParam *iDeviceList, char *iDataBuffer)
{
	long retval;

	// Read Rotary Channels
	iDataBuffer[REPORT_ID] = 0;	// Report ID
	iDataBuffer[REC_TYPE] = READ_ROTARY;
	
	retval = WriteData(iDeviceList, iDataBuffer);

	   // read data
    retval = 0;
	retval = ReadData(iDeviceList, iDataBuffer);

	//Return the result of the API call and the report bytes.
	return retval;
}

HID_API LONG APIENTRY WriteStepper(DeviceParam *iDeviceList, UCHAR nPort, long *iDataBuffer)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = EEEF_BYTES_STEPPER_PER_PORT;			// bytes to write
	Data_Buffer[ADD_L] = (UCHAR)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = 0;
	Data_Buffer[REC_TYPE] = NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = NODE_NO_REPLY;		// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = NODE_WRITE_STEPPER;
	Data_Buffer[NODE_ADDRESS] = (UCHAR)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;				// return bytes to read
	
	memcpy(&Data_Buffer[NODE_SOD], iDataBuffer, EEEF_BYTES_STEPPER_PER_PORT);

	Result = WriteData(iDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteSPIDAC(DeviceParam *iDeviceList, UCHAR nPort, DAC_OUTPUTS *iDataBuffer, UCHAR nLevel)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 40;			// bytes to write
	Data_Buffer[ADD_L] = (UCHAR)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = nLevel;
	Data_Buffer[REC_TYPE] = NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = NODE_NO_REPLY;		// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = NODE_WRITE_DAC;
	Data_Buffer[NODE_ADDRESS] = (UCHAR)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;				// return bytes to read
	
	memcpy(&Data_Buffer[NODE_SOD], iDataBuffer, 40);

	Result = WriteData(iDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY Write7Segment(DeviceParam *oDeviceList, UCHAR *oData)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 0;			// bytes to write
	Data_Buffer[ADD_L] = 0;
	Data_Buffer[ADD_H] = 0;
	Data_Buffer[REC_TYPE] = WRITE_SHIFTER;
	Data_Buffer[CMD_FLAGS] = 0;
	Data_Buffer[SOD] = oData[0];	// SHIFT data
	Data_Buffer[SOD+1] = oData[1];	// SHIFT LIGHT data

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY ClearLCD(DeviceParam *oDeviceList, UCHAR nLCD)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	nLCD;

	Data_Buffer[REPORT_ID] = 0;	// Report ID
	Data_Buffer[REC_LEN] = 0;
	Data_Buffer[ADD_L] = 0;
	Data_Buffer[ADD_H] = 0;
	Data_Buffer[REC_TYPE] = WRITE_CLCD;
	Data_Buffer[CMD_FLAGS] = LCD_CLEAR;

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteLCD(DeviceParam *oDeviceList, UCHAR nLCD, UCHAR nLine, char *oDataBuffer, UCHAR nLevel)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	nLCD;

	Data_Buffer[REPORT_ID] = 0;	// Report ID
	Data_Buffer[REC_LEN] = 0;
	Data_Buffer[ADD_L] = 0;
	Data_Buffer[ADD_H] = nLevel;
	Data_Buffer[REC_TYPE] = WRITE_CLCD;
	Data_Buffer[CMD_FLAGS] = LCD_PAGE1;
	Data_Buffer[SOD] = 1;		// num of lines
	Data_Buffer[SOD+1] = nLine;		// line number
	Data_Buffer[SOD+2] = 20;	// num of chars
	memset(&Data_Buffer[SOD+3], 0x20, 20);
	if (oDataBuffer != NULL)
		memcpy(&Data_Buffer[SOD+3], oDataBuffer, 20);

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY CmdLCD(DeviceParam *oDeviceList, UCHAR nLCD, UCHAR nChar, char *oDataBuffer, UCHAR Cmd)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	nLCD;

	Data_Buffer[REPORT_ID] = 0;	// Report ID
	Data_Buffer[REC_LEN] = 0;
	Data_Buffer[ADD_L] = 0;
	Data_Buffer[ADD_H] = 0;
	Data_Buffer[REC_TYPE] = WRITE_CLCD;
	Data_Buffer[CMD_FLAGS] = Cmd;
	Data_Buffer[SOD] = nChar;		// char address
	Data_Buffer[SOD+1] = 0;			// line number
	Data_Buffer[SOD+2] = 0;			// num of chars
	memset(&Data_Buffer[SOD+3], 0x20, 8);
	if (oDataBuffer != NULL)
		memcpy(&Data_Buffer[SOD+3], oDataBuffer, 8);

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY CmdGLCD(DeviceParam *oDeviceList, UCHAR nPort, UCHAR nBGColor, UCHAR Cmd)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;					// Report ID
	Data_Buffer[REC_LEN] = 1;					// bytes to write
	Data_Buffer[ADD_L] = (UCHAR)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = nBGColor;
	Data_Buffer[REC_TYPE] = NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = NODE_NO_REPLY;		// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = NODE_WRITE_GLCD;
	Data_Buffer[NODE_ADDRESS] = (UCHAR)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;				// return bytes to read
	Data_Buffer[NODE_SOD] = Cmd;

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}


HID_API LONG APIENTRY WriteGLCD(DeviceParam *oDeviceList, UCHAR nPort, UCHAR nLine, UCHAR nNumLines, UCHAR nBGColor, char *oDataBuffer)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	if (nNumLines > 2) nNumLines = 2;

	memset(&Data_Buffer[NODE_SOD+3], 0x20, 24*nNumLines);
	memcpy(&Data_Buffer[NODE_SOD+3], oDataBuffer, 24*nNumLines);

	Data_Buffer[REPORT_ID] = 0;							// Report ID
	Data_Buffer[REC_LEN] = (UCHAR)((nNumLines*24) + 3);	// bytes to write
	Data_Buffer[ADD_L] = (UCHAR)(nPort&0x01);			// port output index
	Data_Buffer[ADD_H] = nBGColor;
	Data_Buffer[REC_TYPE] = NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = NODE_NO_REPLY;				// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = NODE_WRITE_GLCD;
	Data_Buffer[NODE_ADDRESS] = (UCHAR)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;						// return bytes to read
	Data_Buffer[NODE_SOD] = GLCD_DISPLAY;
	Data_Buffer[NODE_SOD+1] = nLine;
	Data_Buffer[NODE_SOD+2] = nNumLines;

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}


HID_API LONG APIENTRY WriteDirectOutput(DeviceParam *oDeviceList, UCHAR nPort, DIRECT_OUTPUT *oDataBuffer)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 15;			// bytes to write
	Data_Buffer[ADD_L] = (unsigned char)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = 0;
	Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;	// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_PORT;
	Data_Buffer[NODE_ADDRESS] = (UCHAR)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;	// return bytes to read

	for (int i = 0; i<Data_Buffer[REC_LEN]; i++)
		Data_Buffer[NODE_SOD + i] = *((UCHAR *)(&oDataBuffer[i]));

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteLatchedOutput(DeviceParam *oDeviceList, UCHAR nPort, UCHAR *oDataBuffer, UCHAR nLevel)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 8;			// bytes to write
	Data_Buffer[ADD_L] = (unsigned char)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = nLevel;
	Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;	// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_PORT;
	Data_Buffer[NODE_ADDRESS] = (UCHAR)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;	// return bytes to read

	memset(&Data_Buffer[NODE_SOD], 0, 8);
	for (int i = 0; i<64; i++) {
		Data_Buffer[NODE_SOD + (i>>3)] |= (oDataBuffer[i]<<(i&0x07));
	}

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteDisplayOutput(DeviceParam *oDeviceList, UCHAR nPort, char *oDataBuffer, UCHAR nLevel)
{
						// 0	 1		2	 3		4	 5		6	 7		8	 9		-	BLANK
	BYTE DispSegment[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40, 0x00};
	char		chr, str[] = ".- ";
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];
	int			idx, i;

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 8;			// bytes to write
	Data_Buffer[ADD_L] = (unsigned char)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = nLevel; 
	Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;	// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_DISPLAY;
	Data_Buffer[NODE_ADDRESS] = (unsigned char)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;	// bytes to read

	idx = 0;
	i = strlen(oDataBuffer);
	memset(&Data_Buffer[NODE_SOD], 0, 7);

	while (i-- > 0) {
		if (oDataBuffer[i] == str[0]) {
			if (idx > 0) {
				Data_Buffer[NODE_SOD + idx] |= 0x80;	// add decimal
				idx--;
			}
		} else if (oDataBuffer[i] == str[1]) {
			Data_Buffer[NODE_SOD + idx] = DispSegment[10]; // add "-"
		} else if (oDataBuffer[i] == str[2]) {
			Data_Buffer[NODE_SOD + idx] = DispSegment[11]; // add BLANK
		} else {
			chr = oDataBuffer[i];
			Data_Buffer[NODE_SOD + idx] |= DispSegment[atoi((const char *)&chr)];
		}
		if (++idx > 6) break;
	}

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);
				
	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteDotMatrix(DeviceParam *oDeviceList, UCHAR nPort, char *oDataBuffer, UCHAR nLevel)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 17;			// bytes to write
	Data_Buffer[ADD_L] = (unsigned char)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = nLevel;
	Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;	// No Reply Required
	Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_DOTMATRIX;
	Data_Buffer[NODE_ADDRESS] = (unsigned char)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;	// bytes to read

	memset(&Data_Buffer[NODE_SOD], 0, 17);
	memcpy(&Data_Buffer[NODE_SOD], &oDataBuffer[0], 16);
	
	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);
				
	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteMuxOutput(DeviceParam *oDeviceList, UCHAR nPort, DIRECT_OUTPUT *oDataBuffer)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	if (nPort > PORT_Y) return 0;

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 56;			// bytes to write
	Data_Buffer[ADD_L] = (unsigned char)(nPort&0x01);	// port output index
	Data_Buffer[ADD_H] = 0;
	Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
	Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;	// No Reply Required;
	Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_PORT;
	Data_Buffer[NODE_ADDRESS] = (unsigned char)(nPort>>1);
	Data_Buffer[NODE_READ_LEN] = 0;	// bytes to read

	for (int i = 0; i<Data_Buffer[REC_LEN]; i++)
		Data_Buffer[NODE_SOD + i] = *((UCHAR *)(&oDataBuffer[i]));

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteCRTRWR(DeviceParam *oDeviceList, UCHAR nPort, UCHAR nPage, UCHAR nSymbolCnt, CRTRWR_OUTPUT *oRWRBuffer)
{
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];
	int			i;
	UCHAR		nContactsCnt, nContactIndex;
	char		nContactsDone;

	if (nPort > PORT_Y) return 0;

	if (nSymbolCnt > MAX_RWR_CONTACTS) nSymbolCnt = MAX_RWR_CONTACTS;

	Data_Buffer[REPORT_ID] = 0;				// Report ID
	nContactsDone = nSymbolCnt;
	nContactIndex = 0;

	if (nPort == PORT_Y) {
		// Send to Port Y in 56 byte packets (14 contacts * 4 bytes)
		while (nContactsDone >= 0) {
			if (nContactsDone > 14) {
				nContactsCnt = 14;
				nContactsDone -= 14;
			} else {
				nContactsCnt = nContactsDone;
				nContactsDone = -1;
			}

			Data_Buffer[REC_LEN] = nSymbolCnt;		// Total number of symbols
			Data_Buffer[ADD_L] = nContactsCnt;		// Number of contacts in this buffer
			Data_Buffer[ADD_H] = nContactIndex;		// nIndex of first contact in symbols buffer
			Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
			Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;		// No Reply Required;
			Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_RWR;	// packet for Port Y
			Data_Buffer[NODE_ADDRESS] = (unsigned char)PORTY_ADDRESS;
			Data_Buffer[NODE_READ_LEN] = nPage;		// Page to display if any

			for (i = 0; i<nContactsCnt; i++)
				memcpy(&Data_Buffer[NODE_SOD + i*EEEF_BYTES_PER_RWR], &oRWRBuffer[nContactIndex + i], EEEF_BYTES_PER_RWR);

			nContactIndex += nContactsCnt;

			Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);
			if (Result == 0)
				break;	// Write failed
		}
	} else {
		// Send to Node Port in 56 byte packets (13 contacts * 4 bytes)
		while (nContactsDone >= 0) {
			if (nContactsDone > 13) {
				nContactsCnt = 13;
				nContactsDone -= 13;
			} else {
				nContactsCnt = nContactsDone;
				nContactsDone = -1;
			}

			Data_Buffer[REC_LEN] = 56;				// bytes to write
			Data_Buffer[ADD_L] = nContactsCnt;		// Number of contacts in this buffer
			Data_Buffer[ADD_H] = nContactIndex;		// nIndex of first contact in symbols buffer
			Data_Buffer[REC_TYPE] = (unsigned char)NODE_COMMAND;
			Data_Buffer[CMD_FLAGS] = (unsigned char)NODE_NO_REPLY;	// No Reply Required;
			Data_Buffer[NODE_REC_TYPE] = (unsigned char)NODE_WRITE_RWR;
			Data_Buffer[NODE_ADDRESS] = (unsigned char)(nPort>>1);	// NODE
			Data_Buffer[NODE_READ_LEN] = nPage;		// Page to display if any
			Data_Buffer[NODE_SOD] = nSymbolCnt;		// Total number of symbols

			for (i = 0; i<nSymbolCnt; i++)
				memcpy(&Data_Buffer[NODE_SOD + i*EEEF_BYTES_PER_RWR + 1], &oRWRBuffer[nContactIndex + i], EEEF_BYTES_PER_RWR);

			nContactIndex += nContactsCnt;

			Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);
			if (Result == 0)
				break;	// Write failed
		}
	}

	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API LONG APIENTRY WriteFUSION(DeviceParam *oDeviceList, FUSION_OUTPUT *oDataBuffer, UCHAR nLevel)
{
						// 0	 1		2	 3		4	 5		6	 7		8	 9		-	  L	    P	 =	  BLANK	  r
	BYTE DispSegment[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x01, 0x0E, 0x67, 0x49, 0x00, 0x05};
	char		chr, str[] = ".- LP=r";
	int			idx, i;
	DWORD		Result;
	BYTE		Data_Buffer[MAX_OUTPUT_REPORT_BYTES + 1];

	Data_Buffer[REPORT_ID] = 0;			// Report ID
	Data_Buffer[REC_LEN] = 0;			// bytes to write
	Data_Buffer[ADD_L] = 0;				// port output index
	Data_Buffer[ADD_H] = nLevel;
	Data_Buffer[REC_TYPE] = (unsigned char)WRITE_FUSION;
	Data_Buffer[CMD_FLAGS] = 0;

	i = strlen(oDataBuffer->text_a);
	memset(&Data_Buffer[SOD], 0, 4);
	idx = 0;

	while (i-- > 0) {
		chr = oDataBuffer->text_a[i];
		if (oDataBuffer->text_a[i] == str[0]) {
			if (idx >= 0) {
				Data_Buffer[SOD + idx] |= 0x80;		// add decimal
				idx--;
			}
		} else if (oDataBuffer->text_a[i] == str[1]) {
			Data_Buffer[SOD + idx] |= DispSegment[10]; // add "-"
		} else if (oDataBuffer->text_a[i] == str[2]) {
			Data_Buffer[SOD + idx] |= DispSegment[14]; // add BLANK
		} else if (oDataBuffer->text_a[i] == str[3]) {
			Data_Buffer[SOD + idx] |= DispSegment[11]; // L
		} else if (oDataBuffer->text_a[i] == str[4]) {
			Data_Buffer[SOD + idx] |= DispSegment[12]; // P
		} else if (oDataBuffer->text_a[i] == str[5]) {
			Data_Buffer[SOD + idx] |= DispSegment[13]; // =
		} else if (oDataBuffer->text_a[i] == str[6]) {
			Data_Buffer[SOD + idx] |= DispSegment[15]; // r
		} else if (oDataBuffer->text_a[i] <= 57) {
			Data_Buffer[SOD + idx] |= DispSegment[atoi((const char *)&chr)];
		} else {
			Data_Buffer[SOD + idx] |= DispSegment[14]; // add BLANK
		}
		if (++idx > 3) break;
	}

	i = strlen(oDataBuffer->text_b);
	memset(&Data_Buffer[SOD + 4], 0, 4);
	idx = 0;

	while (i-- > 0) {
		chr = oDataBuffer->text_b[i];
		if (oDataBuffer->text_b[i] == str[0]) {
			if (idx >= 0) {
				Data_Buffer[SOD + 4 + idx] |= 0x80;	// add decimal
				idx--;
			}
		} else if (oDataBuffer->text_b[i] == str[1]) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[10]; // add "-"
		} else if (oDataBuffer->text_b[i] == str[2]) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[14]; // add BLANK
		} else if (oDataBuffer->text_b[i] == str[3]) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[11]; // L
		} else if (oDataBuffer->text_b[i] == str[4]) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[12]; // P
		} else if (oDataBuffer->text_b[i] == str[5]) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[13]; // =
		} else if (oDataBuffer->text_b[i] == str[6]) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[15]; // r
		} else if (oDataBuffer->text_b[i] <= 57) {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[atoi((const char *)&chr)];
		} else {
			Data_Buffer[SOD + 4 + idx] |= DispSegment[14]; // add BLANK
		}
		if (++idx > 3) break;
	}

	Data_Buffer[SOD + 8] = DispSegment[oDataBuffer->gear];

	Data_Buffer[SOD + 9] = UCHAR(oDataBuffer->LEDs);
	Data_Buffer[SOD + 10] = UCHAR(oDataBuffer->LEDs>>8);

	Result = WriteData(oDeviceList, (CHAR *)Data_Buffer);
				
	//Return the result of the API call and the report bytes.
	return Result;
}

HID_API BOOL APIENTRY CloseDevices(UINT iDev_Cnt, DeviceParam *oDeviceList)
{
	BOOL	RetResult = TRUE;
	long retval;

	for (UINT i=0; i < iDev_Cnt; i++) {
		if (oDeviceList[i].DeviceHandle != NULL) {
			retval = CloseHandle(oDeviceList[i].DeviceHandle);
			oDeviceList[i].DeviceHandle = NULL;
		}
	}
	
	return RetResult;
}

void GetHIDName(HIDD_ATTRIBUTES *Attributes, DeviceParam *DeviceList)
{
	switch(Attributes->ProductID) {
		// HID Devices
		case PID_0X64B:
			strcpy(DeviceList->DeviceName, PIDN_0X64B);
			DeviceList->DeviceNameLength = strlen(PIDN_0X64B);
			break;

		case PID_GAMMARAY:
			strcpy(DeviceList->DeviceName, PIDN_GAMMARAY);
			DeviceList->DeviceNameLength = strlen(PIDN_GAMMARAY);
			break;

		case PID_GAMMARAY64:
			strcpy(DeviceList->DeviceName, PIDN_GAMMARAY64);
			DeviceList->DeviceNameLength = strlen(PIDN_GAMMARAY64);
			break;

		case PID_ELECTRONFLUX:
			strcpy(DeviceList->DeviceName, PIDN_ELECTRONFLUX);
			DeviceList->DeviceNameLength = strlen(PIDN_ELECTRONFLUX);
			break;

		case PID_NITRO_SLG:
			strcpy(DeviceList->DeviceName, PIDN_NITRO_SLG);
			DeviceList->DeviceNameLength = strlen(PIDN_NITRO_SLG);
			break;

		case PID_NITRO_XG:
			strcpy(DeviceList->DeviceName, PIDN_NITRO_XG);
			DeviceList->DeviceNameLength = strlen(PIDN_NITRO_XG);
			break;

		case PID_GAMMARAY_V2: // Configuration device
			// Search for bInterfaceNumber "MI_xx"
			if (strstr(DeviceList->DevicePath, "MI_01") != NULL || 
				strstr(DeviceList->DevicePath, "mi_01") != NULL) {
				// CFG device found
				DeviceList->ConfigFlag = TRUE;
				strcpy(DeviceList->DeviceName, PIDNC_GAMMARAY_V2);
				DeviceList->DeviceNameLength = strlen(PIDNC_GAMMARAY_V2);
			} else {
				// Device found.
				DeviceList->ConfigFlag = FALSE;
				strcpy(DeviceList->DeviceName, PIDN_GAMMARAY_V2);
				DeviceList->DeviceNameLength = strlen(PIDN_GAMMARAY_V2);
			}

			break;

		case PID_GAMMARAY_V3: // Configuration device
			// Search for bInterfaceNumber "MI_xx"
			if (strstr(DeviceList->DevicePath, "MI_01") != NULL || 
				strstr(DeviceList->DevicePath, "mi_01") != NULL) {
				// CFG device found
				DeviceList->ConfigFlag = TRUE;
				strcpy(DeviceList->DeviceName, PIDNC_GAMMARAY_V3);
				DeviceList->DeviceNameLength = strlen(PIDNC_GAMMARAY_V3);
			} else {
				// Device found.
				DeviceList->ConfigFlag = FALSE;
				strcpy(DeviceList->DeviceName, PIDN_GAMMARAY_V3);
				DeviceList->DeviceNameLength = strlen(PIDN_GAMMARAY_V3);
			}

			break;

		case PID_GAMMATRON: // Configuration device
			// Search for bInterfaceNumber "MI_xx"
			if (strstr(DeviceList->DevicePath, "MI_01") != NULL || 
				strstr(DeviceList->DevicePath, "mi_01") != NULL) {
				// CFG device found
				DeviceList->ConfigFlag = TRUE;
				strcpy(DeviceList->DeviceName, PIDNC_GAMMATRON);
				DeviceList->DeviceNameLength = strlen(PIDNC_GAMMATRON);
			} else {
				// Device found.
				DeviceList->ConfigFlag = FALSE;
				strcpy(DeviceList->DeviceName, PIDN_GAMMATRON);
				DeviceList->DeviceNameLength = strlen(PIDN_GAMMATRON);
			}

			break;

		// Standard Devices
		case PID_4X24BH:
			strcpy(DeviceList->DeviceName, PIDN_4X24BH);
			DeviceList->DeviceNameLength = strlen(PIDN_4X24BH);
			break;

		case PID_6X16B:
			strcpy(DeviceList->DeviceName, PIDN_6X16B);
			DeviceList->DeviceNameLength = strlen(PIDN_6X16B);
			break;

		case PID_6X13B:
			strcpy(DeviceList->DeviceName, PIDN_6X13B);
			DeviceList->DeviceNameLength = strlen(PIDN_6X13B);
			break;

		case PID_5X18BH:
			strcpy(DeviceList->DeviceName, PIDN_5X18BH);
			DeviceList->DeviceNameLength = strlen(PIDN_5X18BH);
			break;

		case PID_PLASMA:
			strcpy(DeviceList->DeviceName, PIDN_PLASMA);
			DeviceList->DeviceNameLength = strlen(PIDN_PLASMA);
			break;

		case PID_PLASMA_HOTAS:
			strcpy(DeviceList->DeviceName, PIDN_PLASMA_HOTAS);
			DeviceList->DeviceNameLength = strlen(PIDN_PLASMA_HOTAS);
			break;

		case PID_PLASMA_DUAL:
			strcpy(DeviceList->DeviceName, PIDN_PLASMA_DUAL);
			DeviceList->DeviceNameLength = strlen(PIDN_PLASMA_DUAL);
			break;

		case PID_PLASMA_HOTAS_DUAL:
			strcpy(DeviceList->DeviceName, PIDN_PLASMA_HOTAS_DUAL);
			DeviceList->DeviceNameLength = strlen(PIDN_PLASMA_HOTAS_DUAL);
			break;

		case PID_PLASMA_LITE:
			strcpy(DeviceList->DeviceName, PIDN_PLASMA_LITE);
			DeviceList->DeviceNameLength = strlen(PIDN_PLASMA_LITE);
			break;

		case PID_PLASMA_LITE_V2: // Configuration device
			// Search for bInterfaceNumber "MM_xx"
			if (strstr(DeviceList->DevicePath, "MI_01") != NULL || 
				strstr(DeviceList->DevicePath, "mi_01") != NULL) {
				// CFG device found
				DeviceList->ConfigFlag = TRUE;
				strcpy(DeviceList->DeviceName, PIDNC_PLASMA_LITE_V2);
				DeviceList->DeviceNameLength = strlen(PIDNC_PLASMA_LITE_V2);
			} else {
				// Device found.
				DeviceList->ConfigFlag = FALSE;
				strcpy(DeviceList->DeviceName, PIDN_PLASMA_LITE_V2);
				DeviceList->DeviceNameLength = strlen(PIDN_PLASMA_LITE_V2);
			}
			break;

		case PID_PLASMA_MM2: // Configuration device
			// Search for bInterfaceNumber "MM_xx"
			if (strstr(DeviceList->DevicePath, "MI_01") != NULL || 
				strstr(DeviceList->DevicePath, "mi_01") != NULL) {
				// CFG device found
				DeviceList->ConfigFlag = TRUE;
				strcpy(DeviceList->DeviceName, PIDNC_PLASMA_MM2);
				DeviceList->DeviceNameLength = strlen(PIDNC_PLASMA_MM2);
			} else {
				// Device found.
				DeviceList->ConfigFlag = FALSE;
				strcpy(DeviceList->DeviceName, PIDN_PLASMA_MM2);
				DeviceList->DeviceNameLength = strlen(PIDN_PLASMA_MM2);
			}
			break;

		case PID_FUSION: // Configuration device
			// Search for bInterfaceNumber "MM_xx"
			if (strstr(DeviceList->DevicePath, "MI_01") != NULL || 
				strstr(DeviceList->DevicePath, "mi_01") != NULL) {
				// CFG device found
				DeviceList->ConfigFlag = TRUE;
				strcpy(DeviceList->DeviceName, PIDNC_FUSION);
				DeviceList->DeviceNameLength = strlen(PIDNC_FUSION);
			} else {
				// Device found.
				DeviceList->ConfigFlag = FALSE;
				strcpy(DeviceList->DeviceName, PIDN_FUSION);
				DeviceList->DeviceNameLength = strlen(PIDN_FUSION);
			}
			break;

		// Flash Loader devices
		case PIDF_PLASMA_LITE_V2:
			strcpy(DeviceList->DeviceName, PIDNF_PLASMA_LITE_V2);
			DeviceList->DeviceNameLength = strlen(PIDNF_PLASMA_LITE_V2);
			break;

		case PIDF_GAMMARAY_V2:
			strcpy(DeviceList->DeviceName, PIDNF_GAMMARAY_V2);
			DeviceList->DeviceNameLength = strlen(PIDNF_GAMMARAY_V2);
			break;

		case PIDF_GAMMARAY_V3:
			strcpy(DeviceList->DeviceName, PIDNF_GAMMARAY_V3);
			DeviceList->DeviceNameLength = strlen(PIDNF_GAMMARAY_V3);
			break;

		case PIDF_ELECTRONFLUX:
			strcpy(DeviceList->DeviceName, PIDNF_ELECTRONFLUX);
			DeviceList->DeviceNameLength = strlen(PIDNF_ELECTRONFLUX);
			break;

		case PIDF_NITRO_SLG:
			strcpy(DeviceList->DeviceName, PIDNF_NITRO_SLG);
			DeviceList->DeviceNameLength = strlen(PIDNF_NITRO_SLG);
			break;

		case PIDF_NITRO_XG:
			strcpy(DeviceList->DeviceName, PIDNF_NITRO_XG);
			DeviceList->DeviceNameLength = strlen(PIDNF_NITRO_XG);
			break;

		case PIDF_GAMMATRON:
			strcpy(DeviceList->DeviceName, PIDNF_GAMMATRON);
			DeviceList->DeviceNameLength = strlen(PIDNF_GAMMATRON);
			break;

		case PIDF_PLASMA_MM2:
			strcpy(DeviceList->DeviceName, PIDNF_PLASMA_MM2);
			DeviceList->DeviceNameLength = strlen(PIDNF_PLASMA_MM2);
			break;

		case PIDF_FUSION:
			strcpy(DeviceList->DeviceName, PIDNF_FUSION);
			DeviceList->DeviceNameLength = strlen(PIDNF_FUSION);
			break;

		case PIDF_USIMV1:
			strcpy(DeviceList->DeviceName, PIDNF_USIMV1);
			DeviceList->DeviceNameLength = strlen(PIDNF_USIMV1);
			break;
	}
}
