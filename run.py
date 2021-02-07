#!/usr/bin/env python
import os
import sys
import json
import pandas as pd
from IPython.display import display
import warnings
warnings.filterwarnings('ignore')


sys.path.insert(0, 'src/data')
from Loading_Data import * 
sys.path.insert(0, 'src/eda')
from feature_selection import * 
sys.path.insert(0, 'src/model')
from hypothesis_testing import * 



def main(targets):
    '''
    Runs the main project pipeline logic, given the targets.
    targets must contain: 'data', 'analysis', 'model'.

    `main` runs the targets in order of data=>analysis=>model.
    '''

    if 'data' in targets:
        with open('config/data-params.json') as fh:
            data_cfg = json.load(fh)
        # make the data target
        device_use = load_device(data_cfg["DEVICE_OUTFP1"], data_cfg["DEVICE_OUTFP2"])
        battery_event = load_battery_event(data_cfg["BATTERY_EVENT_OUTFP"])
        battery_info = load_battery_info(data_cfg["BATTERY_INFO_OUTFP"])
        process = load_process(data_cfg["PROCESS_OUTFP1"], data_cfg["PROCESS_OUTFP2"])
        cpu = load_cpu(data_cfg["HW_OUTFP1"], data_cfg["HW_OUTFP2"])
        
        display(device_use)
        display(battery_event)
        display(battery_info)
        display(process)
        display(cpu)

        
    if 'eda' in targets:
        ## Data Preprocessing Part
        
        with open('config/data-params.json') as fh:
            data_cfg = json.load(fh)
        try:
            device_use
        except:
            device_use = load_device(data_cfg["DEVICE_OUTFP1"], data_cfg["DEVICE_OUTFP2"])
        try:
            battery_event
        except:
            battery_event = load_battery_event(data_cfg["BATTERY_EVENT_OUTFP"])
        try:
            battery_info 
        except:
            battery_info = load_battery_info(data_cfg["BATTERY_INFO_OUTFP"])
        try:
            process
        except:
            process = load_process(data_cfg["PROCESS_OUTFP1"], data_cfg["PROCESS_OUTFP2"])
        try:
            cpu
        except:
            cpu = load_cpu(data_cfg["HW_OUTFP1"], data_cfg["HW_OUTFP2"])
        
        num_dev = num_dev_feature(battery_event, device_use)
        num_proc = num_proc_feature(battery_event, process)
        page_faults = page_faults_feature(battery_event, process)
        avg_memory = avg_memory_feature(battery_event, process)
        cpu_sec = cpu_sec_feature(battery_event, process)
        capacity = capacity_feature(battery_event, battery_info)
        cpu_percent = cpu_percent_feature(cpu, battery_event)
        cpu_temp = cpu_temp_feature(cpu, battery_event)

    
    if 'model' in targets:
        with open('config/data-params.json') as fh:
            data_cfg = json.load(fh)
        try:
            device_use
        except:
            device_use = load_device(data_cfg["DEVICE_OUTFP1"], data_cfg["DEVICE_OUTFP2"])
        try:
            battery_event
        except:
            battery_event = load_battery_event(data_cfg["BATTERY_EVENT_OUTFP"])
        try:
            battery_info 
        except:
            battery_info = load_battery_info(data_cfg["BATTERY_INFO_OUTFP"])
        try:
            process
        except:
            process = load_process(data_cfg["PROCESS_OUTFP1"], data_cfg["PROCESS_OUTFP2"])
        try:
            cpu
        except:
            cpu = load_cpu(data_cfg["HW_OUTFP1"], data_cfg["HW_OUTFP2"])
            
        try:
            num_dev
        except:
            num_dev = num_dev_feature(battery_event, device_use)
        try:
            num_proc
        except:
            num_proc = num_proc_feature(battery_event, process)
        try:
            page_faults 
        except:
            page_faults = page_faults_feature(battery_event, process)
        try:
            avg_memory
        except:
            avg_memory = avg_memory_feature(battery_event, process)
        try:
            cpu_sec
        except:
            cpu_sec = cpu_sec_feature(battery_event, process)
        try:
            capacity 
        except:
            capacity = capacity_feature(battery_event, battery_info)
        try:
            cpu_percent
        except:
            cpu_percent = cpu_percent_feature(cpu, battery_event)
        try:
            cpu_temp
        except:
            cpu_temp = cpu_temp_feature(cpu, battery_event)
            
        linear_train, linear_test = linear_reg()
        svm_train, svm_test = svm()
        dt_train, dt_test = dtr()
        
        hypo1()
        hypo2()
           
    
    if 'test' in targets:
        with open('config/data-params.json') as fh:
            data_cfg = json.load(fh)
        # make the data target
        device_use = load_device(data_cfg["DEVICE_OUTFP1"], data_cfg["DEVICE_OUTFP2"])
        battery_event = load_battery_event(data_cfg["BATTERY_EVENT_OUTFP"])
        battery_info = load_battery_info(data_cfg["BATTERY_INFO_OUTFP"])
        process = load_process(data_cfg["PROCESS_OUTFP1"], data_cfg["PROCESS_OUTFP2"])
        cpu = load_cpu(data_cfg["HW_OUTFP1"], data_cfg["HW_OUTFP2"])
        
        print(device_use)
        print(battery_event)
        print(battery_info)
        print(process)
        print(cpu)
        
        num_dev = num_dev_feature(battery_event, device_use)
        num_proc = num_proc_feature(battery_event, process)
        page_faults = page_faults_feature(battery_event, process)
        avg_memory = avg_memory_feature(battery_event, process)
        cpu_sec = cpu_sec_feature(battery_event, process)
        capacity = capacity_feature(battery_event, battery_info)
        cpu_percent = cpu_percent_feature(cpu, battery_event)
        cpu_temp = cpu_temp_feature(cpu, battery_event)
        
        linear_train, linear_test = linear_reg()
        svm_train, svm_test = svm()
        dt_train, dt_test = dtr()
        
        hypo1()
        hypo2()


if __name__ == '__main__':
    # run via:
    # python main.py data model
    targets = sys.argv[1:]
    main(targets)
