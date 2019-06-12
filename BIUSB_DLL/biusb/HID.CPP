#include "stdafx.h"
#include <stdio.h>

#define HID_EXPORTS
#include "biusb.h"

//extern void GetHIDName(HIDD_ATTRIBUTES *);

void GetHIDName(HIDD_ATTRIBUTES *Attributes)
{
	switch(Attributes->ProductID) {
		// HID Devices
		case PRODUCT_ID_1:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_1);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_1);
			break;

		case PRODUCT_ID_2:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_2);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_2);
			break;

		case PRODUCT_ID_3:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_3);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_3);
			break;

		case PRODUCT_ID_4:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_4);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_4);
			break;

		case PRODUCT_ID_5: // Configuration device
//		case CFG_ID_2:
			// Search for bInterfaceNumber "MM_xx"
			if (strstr(DeviceList[(Dev_Cnt - 1)]->DevicePath, "MM_01") != NULL) {
				// CFG device found
				DeviceList[(Dev_Cnt - 1)]->ConfigFlag = TRUE;
				strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, CFG_NAME_ID_2);
				DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(CFG_NAME_ID_2);
			} else {
				// Device found.
				DeviceList[(Dev_Cnt - 1)]->ConfigFlag = FALSE;
				strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_5);
				DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_5);
			}

			break;

		// Standard Devices
		case PRODUCT_ID_10:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_10);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_10);
			break;

		case PRODUCT_ID_11:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_11);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_11);
			break;

		case PRODUCT_ID_12:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_12);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_12);
			break;

		case PRODUCT_ID_13:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_13);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_13);
			break;

		case PRODUCT_ID_14:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_14);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_14);
			break;

		case PRODUCT_ID_15:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_15);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_15);
			break;

		case PRODUCT_ID_16:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_16);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_16);
			break;

		case PRODUCT_ID_17:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_17);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_17);
			break;

		case PRODUCT_ID_18:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_18);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_18);
			break;

		case PRODUCT_ID_19: // Configuration device
			// Search for bInterfaceNumber "MM_xx"
			if (strstr(DeviceList[(Dev_Cnt - 1)]->DevicePath, "MM_01") != NULL) {
				// CFG device found
				DeviceList[(Dev_Cnt - 1)]->ConfigFlag = TRUE;
				strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, CFG_NAME_ID_1);
				DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(CFG_NAME_ID_1);
			} else {
				// Device found.
				DeviceList[(Dev_Cnt - 1)]->ConfigFlag = FALSE;
				strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, PRODUCT_NAME_ID_19);
				DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(PRODUCT_NAME_ID_19);
			}

			break;

		// Flash Loader devices
		case FLASH_ID_1:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, FLASH_NAME_ID_1);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(FLASH_NAME_ID_1);
			break;

		case FLASH_ID_2:
			strcpy(DeviceList[(Dev_Cnt - 1)]->DeviceName, FLASH_NAME_ID_2);
			DeviceList[(Dev_Cnt - 1)]->DeviceNameLength = strlen(FLASH_NAME_ID_2);
			break;
	}
}