/**
*** INTEL CONFIDENTIAL
***
*** Copyright (March 2011) (March 2011) Intel Corporation All Rights Reserved.
*** The source code contained or described herein and all documents related to the
*** source code ("Material") are owned by Intel Corporation or its suppliers or
*** licensors. Title to the Material remains with Intel Corporation or its
*** suppliers and licensors. The Material contains trade secrets and proprietary
*** and confidential information of Intel or its suppliers and licensors.
*** The Material is protected by worldwide copyright and trade secret laws
*** and treaty provisions. No part of the Material may be used, copied,
*** reproduced, modified, published, uploaded, posted, transmitted, distributed,
*** or disclosed in any way without Intel's prior express written permission.
***
*** No license under any patent, copyright, trade secret or other intellectual
*** property right is granted to or conferred upon you by disclosure or delivery
*** of the Materials, either expressly, by implication, inducement, estoppel or
*** otherwise. Any license under such intellectual property rights must be
*** express and approved by Intel in writing.
**/

//-----------------------------------------------------------------------------
// Headers inclusions.
//-----------------------------------------------------------------------------
#include <assert.h>
#include <windows.h>
#include <powerbase.h>
#include <winnt.h>
#include "mouse_input.h"
#include <Powrprof.h> // https://stackoverflow.com/questions/41978314/fresh-project-with-callntpowerinformation-from-powrprof-h-having-linker-errors
#pragma comment(lib, "Powrprof.lib")
#include <setupapi.h>
#pragma comment(lib, "SetupAPI.lib")
#include <BatClass.h>


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

BOOL  status;
NTSTATUS batteryInfo;
/*-----------------------------------------------------------------------------
	Variable: powerStatus
	From: https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-system_power_status
-----------------------------------------------------------------------------*/
SYSTEM_POWER_STATUS powerStatus = {
255,
255,
255,
0,
-1,
-1
};
/*-----------------------------------------------------------------------------
	Variable: batteryState
	From: https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-system_battery_state
-----------------------------------------------------------------------------*/
SYSTEM_BATTERY_STATE batteryState = {0};

DWORD GetBatteryState()
{
	//From: https://docs.microsoft.com/en-us/windows/win32/power/enumerating-battery-devices
#define GBS_HASBATTERY 0x1
#define GBS_ONBATTERY  0x2
	// Returned value includes GBS_HASBATTERY if the system has a 
	// non-UPS battery, and GBS_ONBATTERY if the system is running on 
	// a battery.
	//
	// dwResult & GBS_ONBATTERY means we have not yet found AC power.
	// dwResult & GBS_HASBATTERY means we have found a non-UPS battery.

	DWORD dwResult = GBS_ONBATTERY;

	// IOCTL_BATTERY_QUERY_INFORMATION,
	// enumerate the batteries and ask each one for information.

	HDEVINFO hdev =
		SetupDiGetClassDevs(&GUID_DEVICE_BATTERY,
			0,
			0,
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (INVALID_HANDLE_VALUE != hdev)
	{
		// Limit search to 100 batteries max
		for (int idev = 0; idev < 100; idev++)
		{
			SP_DEVICE_INTERFACE_DATA did = { 0 };
			did.cbSize = sizeof(did);

			if (SetupDiEnumDeviceInterfaces(hdev,
				0,
				&GUID_DEVICE_BATTERY,
				idev,
				&did))
			{
				DWORD cbRequired = 0;

				SetupDiGetDeviceInterfaceDetail(hdev,
					&did,
					0,
					0,
					&cbRequired,
					0);
				if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
				{
					PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd =
						(PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR,
							cbRequired);
					if (pdidd)
					{
						pdidd->cbSize = sizeof(*pdidd);
						if (SetupDiGetDeviceInterfaceDetail(hdev,
							&did,
							pdidd,
							cbRequired,
							&cbRequired,
							0))
						{
							// Enumerated a battery.  Ask it for information.
							HANDLE hBattery =
								CreateFile(pdidd->DevicePath,
									GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
							if (INVALID_HANDLE_VALUE != hBattery)
							{
								// Ask the battery for its tag.
								BATTERY_QUERY_INFORMATION bqi = { 0 };
								//BATTERY_QUERY_INFORMATION bqi2 = { 0 };
								DWORD dwWait = 0;
								//DWORD dwWait2 = 0;
								DWORD dwOut;
								//DWORD dwout2;

								if (DeviceIoControl(
									hBattery,
									IOCTL_BATTERY_QUERY_TAG,
									&dwWait,
									sizeof(dwWait),
									&bqi.BatteryTag,
									sizeof(bqi.BatteryTag),
									&dwOut,
									NULL)
									&& bqi.BatteryTag)
								{
									// With the tag, you can query the battery info.
									BATTERY_INFORMATION bi = { 0 };
									bqi.InformationLevel = BatteryInformation; //https://docs.microsoft.com/en-us/windows/win32/power/battery-query-information-str

									if (DeviceIoControl(
										hBattery,
										IOCTL_BATTERY_QUERY_INFORMATION,
										&bqi,
										sizeof(bqi),
										&bi,
										sizeof(bi),
										&dwOut,
										NULL))
									{
										// Only non-UPS system batteries count
										if (bi.Capabilities & BATTERY_SYSTEM_BATTERY)
										{
											if (!(bi.Capabilities & BATTERY_IS_SHORT_TERM))
											{
												dwResult |= GBS_HASBATTERY;
											}

											// Query the battery status.
											BATTERY_WAIT_STATUS bws = { 0 };
											bws.BatteryTag = bqi.BatteryTag;
											BATTERY_STATUS bs;
											if (DeviceIoControl(hBattery,
												IOCTL_BATTERY_QUERY_STATUS,
												&bws,
												sizeof(bws),
												&bs,
												sizeof(bs),
												&dwOut,
												NULL))
											{
												if (bs.PowerState & BATTERY_POWER_ON_LINE)
												{
													dwResult &= ~GBS_ONBATTERY;
												}
											}

											//Query the battery temperature -- needs to be done... unable to do it....; not sure how to do two battery_query_information
										}
									}
								}
								CloseHandle(hBattery);
							}
						}
						LocalFree(pdidd);
					}
				}
			}
			else  if (ERROR_NO_MORE_ITEMS == GetLastError())
			{
				break;  // Enumeration failed - perhaps we're out of items
			}
		}
		SetupDiDestroyDeviceInfoList(hdev);
	}

	//  Final cleanup:  If we didn't find a battery, then presume that we
	//  are on AC power.

	if (!(dwResult & GBS_HASBATTERY))
		dwResult &= ~GBS_ONBATTERY;
	return dwResult;
}



/*-----------------------------------------------------------------------------
Function: modeler_init_inputs
Purpose : return the inputs count.
In      : pointer to an unsigned integer.
Out     : modified unsigned integer.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_init_inputs(
	unsigned int* p,
	int* pfd,
	int* pfe,
	char* po,
	size_t so
) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);
	assert(pfd != NULL);
	assert(pfe != NULL);
	SET_INPUTS_COUNT(INPUTS_COUNT);

	//Getting Battery information
	//status = GetSystemPowerStatus(&powerStatus);
	////checking get info or not
	//if (status == 0) {
	//	//goto modeler_init_inputs_error;
	//	printf("THERE IS A ERROR, THE STATUS IS 0");
	//}

	//
	// getting power infomation, from another api: 
	//
	//batteryInfo = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));
	//printf(batteryState.MaxCapacity);


	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}

/*-----------------------------------------------------------------------------
Function: modeler_open_inputs
Purpose : open inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Input descriptions.
	//-------------------------------------------------------------------------
	static char* descriptions[INPUTS_COUNT] = {
		INPUT_DESCRIPTION_STRINGS
	};

	static INTEL_MODELER_INPUT_TYPES types[INPUTS_COUNT] = {
		INPUT_TYPES
	};

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	//-------------------------------------------------------------------------
	// Set input information.
	//-------------------------------------------------------------------------
	SET_INPUTS_NAME(INTPUT_BATTERY_NAME_STRING);

	int count = 0;
	for (count = 0; count < INPUTS_COUNT; count++) {
		SET_INPUT_DESCRIPTION(
			count,
			descriptions[count]
		);
		SET_INPUT_TYPE(
			count,
			types[count]
		);
	} 

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_close_inputs
Purpose : close inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING
		
	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_read_inputs
Purpose : collect all inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	//BOOL bret = FALSE;
	//POINT point = { 0 };

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);
	GetSystemPowerStatus(&powerStatus);
	CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));


	//---------------------------------------------------------------------------------------------------------------------
	//
	// Get battery query information part:
	// https://docs.microsoft.com/en-us/windows/win32/power/enumerating-battery-devices
	//
	// --------------------------------------------------------------------------------------------------------------------
	#define GBS_HASBATTERY 0x1
	#define GBS_ONBATTERY  0x2
	// Returned value includes GBS_HASBATTERY if the system has a 
	// non-UPS battery, and GBS_ONBATTERY if the system is running on 
	// a battery.
	//
	// dwResult & GBS_ONBATTERY means we have not yet found AC power.
	// dwResult & GBS_HASBATTERY means we have found a non-UPS battery.
	DWORD dwResult = GBS_ONBATTERY;


	// IOCTL_BATTERY_QUERY_INFORMATION,
	// enumerate the batteries and ask each one for information.
	HDEVINFO hdev =
		SetupDiGetClassDevs(&GUID_DEVICE_BATTERY,
			0,
			0,
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (INVALID_HANDLE_VALUE != hdev)
	{
		// Limit search to 100 batteries max
		for (int idev = 0; idev < 100; idev++)
		{
			SP_DEVICE_INTERFACE_DATA did = { 0 };
			did.cbSize = sizeof(did);

			if (SetupDiEnumDeviceInterfaces(hdev,
				0,
				&GUID_DEVICE_BATTERY,
				idev,
				&did))
			{
				DWORD cbRequired = 0;

				SetupDiGetDeviceInterfaceDetail(hdev,
					&did,
					0,
					0,
					&cbRequired,
					0);
				if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
				{
					PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd =
						(PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR,
							cbRequired);
					if (pdidd)
					{
						pdidd->cbSize = sizeof(*pdidd);
						if (SetupDiGetDeviceInterfaceDetail(hdev,
							&did,
							pdidd,
							cbRequired,
							&cbRequired,
							0))
						{
							// Enumerated a battery.  Ask it for information.
							HANDLE hBattery =
								CreateFile(pdidd->DevicePath,
									GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
							if (INVALID_HANDLE_VALUE != hBattery)
							{
								// Ask the battery for its tag.
								BATTERY_QUERY_INFORMATION bqi = { 0 };
								//BATTERY_QUERY_INFORMATION bqi2 = { 0 };
								DWORD dwWait = 0;
								//DWORD dwWait2 = 0;
								DWORD dwOut;
								//DWORD dwout2;

								if (DeviceIoControl(
									hBattery,
									IOCTL_BATTERY_QUERY_TAG,
									&dwWait,
									sizeof(dwWait),
									&bqi.BatteryTag,
									sizeof(bqi.BatteryTag),
									&dwOut,
									NULL)
									&& bqi.BatteryTag)
								{
									// With the tag, you can query the battery info.
									BATTERY_INFORMATION bi = { 0 };
									bqi.InformationLevel = BatteryInformation; //https://docs.microsoft.com/en-us/windows/win32/power/battery-query-information-str

									if (DeviceIoControl(
										hBattery,
										IOCTL_BATTERY_QUERY_INFORMATION,
										&bqi,
										sizeof(bqi),
										&bi,
										sizeof(bi),
										&dwOut,
										NULL))
									{
										 // set information from bi or bqi into our database;
										SET_INPUT_ULL_VALUE(
											BATTERY_TAG_INDEX,
											bqi.BatteryTag
										);
										SET_INPUT_ULL_VALUE(
											BATTERY_DESIGNED_CAPACITY_INDEX,
											bi.DesignedCapacity
										);
										SET_INPUT_ULL_VALUE(
											BATTERY_FULLCHARED_CAPACITY_INDEX,
											bi.FullChargedCapacity
										);
										SET_INPUT_ULL_VALUE(
											BATTERY_CYCLE_COUNT_INDEX,
											bi.CycleCount
										);

										// Only non-UPS system batteries count
										if (bi.Capabilities & BATTERY_SYSTEM_BATTERY)
										{
											if (!(bi.Capabilities & BATTERY_IS_SHORT_TERM))
											{
												dwResult |= GBS_HASBATTERY;
											}

											// Query the battery status.
											BATTERY_WAIT_STATUS bws = { 0 };
											bws.BatteryTag = bqi.BatteryTag;
											BATTERY_STATUS bs;
											if (DeviceIoControl(hBattery,
												IOCTL_BATTERY_QUERY_STATUS,
												&bws,
												sizeof(bws),
												&bs,
												sizeof(bs),
												&dwOut,
												NULL))
											{
												// set information from bs into our database;
												SET_INPUT_ULL_VALUE(
													BATTERY_CURRENT_CAPACITY_INDEX,
													bs.Capacity
												);
												SET_INPUT_ULL_VALUE(
													BATTERY_VOLTAGE_INDEX,
													bs.Voltage
												);
												SET_INPUT_LL_VALUE(
													BATTERY_RATE_INDEX,
													bs.Rate
												);
												if (bs.PowerState & BATTERY_POWER_ON_LINE)
												{
													dwResult &= ~GBS_ONBATTERY;
												}

											}

											//Query the battery temperature -- needs to be done... unable to do it....; not sure how to do two battery_query_information
										}
									}
								}
								CloseHandle(hBattery);
							}
						}
						LocalFree(pdidd);
					}
				}
			}
			else  if (ERROR_NO_MORE_ITEMS == GetLastError())
			{
				break;  // Enumeration failed - perhaps we're out of items
			}
		}
		SetupDiDestroyDeviceInfoList(hdev);
	}
	//  Final cleanup:  If we didn't find a battery, then presume that we
	//  are on AC power.

	if (!(dwResult & GBS_HASBATTERY))
		dwResult &= ~GBS_ONBATTERY;


	//-------------------------------------------------------------------------
	// Set input values.
	//-------------------------------------------------------------------------
	SET_INPUT_ULL_VALUE(
		BATTERY_LIFE_INPUT_INDEX,
		powerStatus.BatteryLifePercent
	);
	//-------------------------------------------------------------------------
	SET_INPUT_ULL_VALUE(
		BATTERY_FLAG_INPUT_INDEX,
		powerStatus.BatteryFlag
	);
	SET_INPUT_LL_VALUE(
		BATTERY_LIFE_TIME_INPUT_INDEX,
		powerStatus.BatteryLifeTime
	);
	SET_INPUT_ULL_VALUE(
		BATTERY_CHARGING_INDEX,
		batteryState.Charging
	);
	//-------------------------------------------------------------------------
	SET_INPUT_ULL_VALUE(
		BATTERY_DISCHARGING_INDEX,
		batteryState.Discharging
	);
	SET_INPUT_ULL_VALUE(
		BATTERY_MAXCAPACITY_INDEX,
		batteryState.MaxCapacity
	);

	//-------------------------------------------------------------------------
	SET_INPUT_ULL_VALUE(
		BATTERY_ESTIMATEDTIME_INDEX,
		batteryState.EstimatedTime
	);


	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------

modeler_read_inputs_error:

	return(ESRV_FAILURE);
	INPUT_END_EXCEPTIONS_HANDLING(p)


}

/*-----------------------------------------------------------------------------
Function: modeler_listen_inputs
Purpose : listen for all inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_process_dctl
Purpose : process DCTL commands on DCTL interrupt notification.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_process_lctl
Purpose : process LCTL commands on LCTL interrupt notification.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}
