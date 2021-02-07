import pandas as pd
import gzip
import numpy as np
import matplotlib.pyplot as plt
import warnings
from datetime import datetime

warnings.filterwarnings('ignore')

# Parameters
DATES = pd.date_range('2020-06', '2021-01', freq='D', closed='left') ## only focus on data of past half year

DEVICE_INFP1 = 'devuse_4known_device.csv000.gz'
DEVICE_OUTFP1 = 'sample_device_1.csv'

DEVICE_INFP2 = 'devuse_4known_device.csv001.gz'
DEVICE_OUTFP2 = 'sample_device_2.csv'

BATTERY_EVENT_INFP = 'batt_acdc_events.csv000.gz'
BATTERY_EVENT_OUTFP = 'sample_events.csv'

BATTERY_INFO_INFP = 'batt_info.csv000.gz'
BATTERY_INFO_OUTFP = 'sample_info.csv'

PROCESS_INFP1 = 'plist_process_resource_util_13wks.csv000.gz'
PROCESS_OUTFP1 = 'sample_process_1.csv'

PROCESS_INFP2 = 'plist_process_resource_util_13wks.csv001.gz'
PROCESS_OUTFP2 = 'sample_process_2.csv'

HW_INFP1 = 'hw_metric_histo.csv000.gz'
HW_OUTFP1 = 'sample_hw_1.csv'

HW_INFP2 = 'hw_metric_histo.csv001.gz'
HW_OUTFP2 = 'sample_hw_2.csv'
    
# # Helper Methods:
# def find_seperator(infp):
#     with gzip.open(infp,'rt') as f:
#         count = 0
#         for line in f:
#             if count == 0:
#                 toreplace = line[7] 
#                 count+=1
#             else:
#                 break
#     return toreplace

# def device_filtered(dates, infp, outfp):
#     seperator = find_seperator(infp)
    
#     pd.read_csv(infp, compression='gzip'\
#             ,sep=seperator, nrows=0).to_csv(outfp, index=False) # read column names


#     for counter, chunk in enumerate(pd.read_csv(infp,
#                             compression='gzip', sep=seperator,dtype=str, chunksize=1000000,error_bad_lines=False)):
#         print(counter)
#         device_use = chunk.loc[chunk.device == 'GUID_DEVICE_BATTERY']
#         device_use.dt = pd.to_datetime(device_use.dt, errors='coerce')
#         needed_time = set(dates).intersection(set(device_use.dt))
#         filtered = device_use.set_index('dt').loc[needed_time].reset_index()
#         filtered.to_csv(outfp, index=False, header=False, mode='a')
#     return 


# def battery_filtered(needed_guid, dates, infp, outfp):
#     seperator = find_seperator(infp)
#     pd.read_csv(infp, compression='gzip'\
#                 ,sep=seperator, nrows=0).to_csv(outfp, index=False) # read column names

    
#     if 'process'in infp:
#         dt = 'start_dt'
#     else:
#         dt = 'dt'
    
#     if 'hw' in infp:
#         flag = True
#     else:
#         flag = False
        
#     for counter, chunk in enumerate(pd.read_csv(infp, \
#                             compression='gzip', sep=seperator,dtype=str, \
#                                                 chunksize=1000000,error_bad_lines = False)):
#         print(counter)
#         needed = set(needed_guids).intersection(set(chunk.guid))
#         chunk = chunk.set_index('guid').loc[needed].reset_index()
#         chunk[dt] = pd.to_datetime(chunk[dt],errors='coerce')
#         needed_time = set(dates).intersection(set(chunk[dt]))
#         filtered = chunk.set_index(dt).loc[needed_time].reset_index()
#         if flag:
#             cpu_percent = filtered.name == 'HW::CORE:C0:PERCENT:'
#             temperature = filtered.name == 'HW::CORE:TEMPERATURE:CENTIGRADE:'
#             filtered = filtered.loc[cpu_percent | temperature ]
        
#         filtered.to_csv(outfp, index=True, header=False, mode='a')
#     return 
        

def load_device(DEVICE_OUTFP1, DEVICE_OUTFP2):
    
#     #Read first device file
#     device_filtered(dates, DEVICE_INFP1, DEVICE_OUTFP1)
    
#     #read second device file
#     device_filtered(DATES, DEVICE_INFP2, DEVICE_OUTFP2)
    
    df3 = pd.read_csv(DEVICE_OUTFP1)
    df4 = pd.read_csv(DEVICE_OUTFP2)
    device_use = df4.append(df3)
    device_use = device_use[device_use.columns[1:]]
    newcol = ['dt', 'load_ts', 'batch_id', 'audit_zip', 'audit_internal_path', 'guid',
           'interval_start_utc', 'interval_end_utc', 'interval_local_start',
           'interval_local_end', 'ts','device', 'hw_name', 'name',
           'duration', 'status']
    device_use.columns = newcol
    
    return device_use
    
def load_battery_event(BATTERY_EVENT_OUTFP):
#     #Read first device file
#     device_filtered(dates, DEVICE_INFP1, DEVICE_OUTFP1)
    
#     #read second device file
#     device_filtered(DATES, DEVICE_INFP2, DEVICE_OUTFP2)
    
#     df3 = pd.read_csv(DEVICE_OUTFP1)
#     df4 = pd.read_csv(DEVICE_OUTFP2)
#     device_use = df4.append(df3)
#     newcol = ['dt', 'load_ts', 'batch_id', 'audit_zip', 'audit_internal_path', 'guid',
#            'interval_start_utc', 'interval_end_utc', 'interval_local_start',
#            'interval_local_end', 'ts','device', 'hw_name', 'name',
#            'duration', 'status']
#     device_use.columns = newcol
    
#     needed_guids = set(device_use.guid)
    
#     battery_filtered(needed_guids, DATES, HW_INFP2, HW_OUTFP2)
    
    battery_event = pd.read_csv(BATTERY_EVENT_OUTFP)
    battery_event = battery_event[battery_event.columns[1:]]
    newcol = ['dt', 'guid','load_ts','batch_id','audit_zip','audit_internal_path',
           'interval_start_utc', 'interval_end_utc', 'interval_local_start',
           'interval_local_end', 'ts','system_power_state', 'event_type',
           'duration', 'battery_percent_remaining', 'battery_minutes_remaining']
    battery_event.columns = newcol
    
    return battery_event

def load_battery_info(BATTERY_INFO_OUTFP):
#     #Read first device file
#     device_filtered(dates, DEVICE_INFP1, DEVICE_OUTFP1)
    
#     #read second device file
#     device_filtered(DATES, DEVICE_INFP2, DEVICE_OUTFP2)
    
#     df3 = pd.read_csv(DEVICE_OUTFP1)
#     df4 = pd.read_csv(DEVICE_OUTFP2)
#     device_use = df4.append(df3)
#     newcol = ['dt', 'load_ts', 'batch_id', 'audit_zip', 'audit_internal_path', 'guid',
#            'interval_start_utc', 'interval_end_utc', 'interval_local_start',
#            'interval_local_end', 'ts','device', 'hw_name', 'name',
#            'duration', 'status']
#     device_use.columns = newcol
    
#     needed_guids = set(device_use.guid)
    
#     battery_filtered(needed_guids, DATES, HW_INFP2, HW_OUTFP2)
    
    battery_info = pd.read_csv(BATTERY_INFO_OUTFP)
    battery_info = battery_info[battery_info.columns[1:]]
    newcol = ['dt', 'guid','load_ts','batch_id','audit_zip','audit_internal_path',
           'interval_start_utc', 'interval_end_utc', 'interval_local_start',
           'interval_local_end', 'ts','battery_enum', 'chemistry',
           'designed_capacity', 'full_charge_capacity', 'battery_count']
    battery_info.columns = newcol
    
    return battery_info

def load_process(PROCESS_OUTFP1, PROCESS_OUTFP2):
    process1 = pd.read_csv(PROCESS_OUTFP1)
    process2 = pd.read_csv(PROCESS_OUTFP2)
    process = process1.append(process2)
    process = process[process.columns[1:]]

    newcol = ['dt', 'guid','load_ts','batch_id','audit_zip','audit_internal_path','interval_start_utc', 
     'interval_end_utc', 'interval_local_start',
           'interval_local_end', 'ts','proc_name', 'exe_hash',
           'num_runs', 'ttl_run_tm_in_ms', 'cpu_user_sec', 'cpu_kernel_sec',
           'io_bytes_read', 'io_bytes_write', 'io_bytes_other', 'page_faults',
           'hard_page_faults', 'disk_read_iobytes', 'disk_write_iobytes',
           'tcpip_sendbytes', 'tcpip_receivebytes', 'udpip_sendbytes',
           'udpip_receivebytes', 'avg_memory', 'peak_memory']
    process.columns = newcol
    
    return process

def load_cpu(HW_OUTFP1, HW_OUTFP2):
    cpu1 = pd.read_csv(HW_OUTFP1)
    cpu2 = pd.read_csv(HW_OUTFP2)
    cpu = cpu1.append(cpu2)
    cpu = cpu[cpu.columns[1:]]

    newcol = ['dt', 'guid','load_ts','batch_id','audit_zip','audit_internal_path',
           'interval_start_utc', 'interval_end_utc', 'interval_local_start',
           'interval_local_end', 'name', 'instance', 'nrs', 'mean',
           'histogram_min', 'histogram_max', 'bin_1', 'bin_2', 'bin_3', 'bin_4',
           'bin_5', 'bin_6', 'bin_7', 'bin_8', 'bin_9', 'bin_10',
           'metric_max_val']
    cpu.columns = newcol
    
    return cpu
 
