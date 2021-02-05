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

#ifndef __INCLUDE_STATIC_STANDARD_INPUT__
#define __INCLUDE_STATIC_STANDARD_INPUT__

//-----------------------------------------------------------------------------
// Headers inclusions.
//-----------------------------------------------------------------------------
#ifdef __PL_WINDOWS__
#include <windows.h>
#include <powerbase.h>
#include <winnt.h>
#endif // __PL_WINDOWS__
#include "pub_intel_modeler.h"
#include <Powrprof.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	/*--------------------------------------------------------------------------*/

	//-----------------------------------------------------------------------------
	// Defines.
	//-----------------------------------------------------------------------------
#define INPUTS_COUNT 14
	
//from SYSTEM_POWER_STATUS structure
#define BATTERY_LIFE_INPUT_INDEX 0
#define BATTERY_FLAG_INPUT_INDEX 1
#define BATTERY_LIFE_TIME_INPUT_INDEX 2

//from SYSTEM_BATTERY_STATE structure
#define BATTERY_CHARGING_INDEX 3
#define BATTERY_DISCHARGING_INDEX 4
#define BATTERY_MAXCAPACITY_INDEX 5
#define BATTERY_ESTIMATEDTIME_INDEX 6

// from BATTERY_QUERY_INFORMATION STRUCTURE
#define BATTERY_TAG_INDEX 7

//from BATTERY_INFORMATION structure
#define BATTERY_DESIGNED_CAPACITY_INDEX 8
#define BATTERY_FULLCHARED_CAPACITY_INDEX 9
#define BATTERY_CYCLE_COUNT_INDEX 10

//from BATTERY_STATUS STRUCTURE
#define BATTERY_CURRENT_CAPACITY_INDEX 11
#define BATTERY_VOLTAGE_INDEX 12
#define BATTERY_RATE_INDEX 13

#define INTPUT_BATTERY_NAME_STRING "BATTERY"
#define INPUT_DESCRIPTION_STRINGS \
	"Battery Left (%).",\
	"Battery Flag",\
	"Battery Life Time (Seconds).",\
	"Battery Charging.",\
	"Battery Discharging.",\
	"Battery MaxCapacity (mWh).",\
	"Battery estimatedTime (Seconds).",\
	"Battery Tag",\
	"Battery Designed Capacity (mWh) ",\
	"Battery Full Charged Capacity (mWh)",\
	"Battery Cycle Count",\
	"Battery Current Capacity (mWh)",\
	"Battery Voltage (mv)",\
	"Battery Rate (mW)"
//(ull)Battery Left (%): The percentage of full battery charge remaining. This member can be a value in the range 0 to 100, or 255 if status is unknown.
//(ull)Battery Flag: The battery charge status; 
	//1 -- High; battery capacity > 66%
	//2 -- low; battery capacity <33%
	//4 -- Critical; battery capacity < 5%
	//8 -- charging
	//128 -- no system battery
	//255 -- unable to read
//(ll)Battery Life Time (Seconds):The number of seconds of battery life remaining, or –1 if remaining seconds are unknown or if the device is connected to AC power.
//(ull)Battery Charging: True / False
//(ull)Battery Discharging: True / False
//(ull)Battery MaxCapacity (mWh):The theoretical capacity of the battery when new.
//(ull)Battery estimatedTime (Seconds): The estimated time remaining on the battery, in seconds.
//(STRING) Battery Tag: current battery tag for the battery
//(ull)/Battery Designed Capacity: theoretical capacity of the battery when new
//(ull)Battery Full Charged Capacity: battery's current fully charged capacity in mWh
//(ull)Battery Cycle Count: number of charge/discharge cycles the battery has experienced
//(ull)Battery Current Capacity: The current battery capacity, in mWh (or relative). This value can be used to generate a "gas gauge" display by dividing it by FullChargedCapacity
//(ull)Battery Voltage: The current battery voltage across the battery terminals, in millivolts (mv).
//(ll)Battery Rate: The current rate of battery charge or discharge.

#define INPUT_TYPES \
ULL_COUNTER, \
ULL_COUNTER,\
LL_COUNTER,\
ULL_COUNTER, \
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
ULL_COUNTER,\
LL_COUNTER


//-----------------------------------------------------------------------------
// Function prototypes.
//-----------------------------------------------------------------------------
	ESRV_API ESRV_STATUS modeler_init_inputs(
		unsigned int*,
		int*,
		int*,
		char*,
		size_t
	);
	ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE);
	/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INCLUDE_STATIC_STANDARD_INPUT__
