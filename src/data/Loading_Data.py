import os
import pandas as pd

def load_data():
    '''
    Retrieve data for year/location/group from the internet
    and return data (or write data to file, if `outpath` is
    not `None`).
    '''
    path = os.path.join( "V8DEMOANALYZERTASK_20201208153257.V8")
    battery_info = pd.read_csv(path, sep = '',header=None)[[5,6,7]]
    battery_info.columns = ['Features', 'Time','Value']
    battery_info = battery_info.pivot(index = 'Time', columns = 'Features')
    battery_info.columns = battery_info.columns.droplevel(0)
    battery_info = battery_info.reset_index().rename_axis(None, axis=1)
    battery4 = battery_info.groupby('Battery Tag').get_group(4)
    battery5 = battery_info.groupby('Battery Tag').get_group(5)
    
    battery5.to_csv('./src/data/battery_data.csv')
    return battery5

