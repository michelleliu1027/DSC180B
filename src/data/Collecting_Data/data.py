import subprocess
import os

def get_data(year, location, group, outpath=None):
    '''
    Retrieve data for year/location/group from the internet
    and return data (or write data to file, if `outpath` is
    not `None`).
    '''
    
    path = os.path.join( "src", "data", "example.bat")
    subprocess.run([path, 'S'])
    return