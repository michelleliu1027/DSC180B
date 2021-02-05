#!/usr/bin/env python
import os
import sys
import json
import pandas as pd
from IPython.display import display
import warnings
warnings.filterwarnings('ignore')


sys.path.insert(0, 'src/data')
from Loading_Data import load_data
sys.path.insert(0, 'src/analysis')
import analysis 


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
        battery5 = load_data()
        display(battery5)
#         return battery_information
    
    if 'analysis' in targets:
        ## Data Preprocessing Part
        
        try:
            battery5
        except:
            battery5 = pd.read_csv('./src/data/battery_data.csv')
        
        analysis.plot_correlation(battery5)
        analysis.corr_matrix(battery5)
        ridge_score = analysis.ridge(battery5)
        logistic_score = analysis.logistic(battery5)
        
        return ridge_score, logistic_score 
    
    if 'test' in targets:
        with open('config/data-params.json') as fh:
            data_cfg = json.load(fh)
        # make the data target
        battery5 = load_data()
        display(battery5)
        
        analysis.plot_correlation(battery5)
        analysis.corr_matrix(battery5)
        
        ridge_score = analysis.ridge(battery5)
        logistic_score = analysis.logistic(battery5)
        
        return ridge_score, logistic_score


if __name__ == '__main__':
    # run via:
    # python main.py data model
    targets = sys.argv[1:]
    main(targets)
