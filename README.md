# DSC180B-Capstone-Project
DSC Capstpne Project: A Prediction Model for Battery Remaining Time
## Usage Instructions
We provided 4 targets for your usage. They are `data`, `eda`, `model`, and `test`.

`test` would run our project on test data, which provides a miniature of what we have done on a smaller dataset. 

An exmple for running our project through terminal is `python run.py data`, which will show our main dataset. To run other branches, just replace `data` with `eda`, `model` or`test`.

## Description of Contents
```
PROJECT
├── config
    └── data-params.json
    └── inputs.json
├── notebooks
    └── cp2.ipynb
├── references
    └── README.md
└── src
    ├── analysis(empty folder for now)
    ├── data
        ├── Collecting_Data
                ├── data.py
                └── example.bat
                └── mouse_input.c
                └── mouse_input.h
                └── mouse_input.sln
                └── mouse_input.dll
        ├── battery_use-000025.db
        └── Loading_Data.py
    └── model(empty folder for now)
├── .gitignore
├── README.md
└── run.py
└── submission.json
```
### `config/`
* `data-params.json`: It contains the SQL query for analyzing `battery_use-000025`
* `inputs.json`: It contains the argument inputs for extracting battery and process information 

### `notebooks/`
* `cp2.ipynb`: EDA and preprocessing on collected data

### `references/`
* `README.md`: External sources (need to be done in the future)

### `src/`
* `analysis/`: Later for developing data analysis code there
* `data/Collecting_Data/data.py`: code for executing batch file (recommended to use only when you have _xlsdk downloaded and have access to it)
* `data/Collecting_Data/example.bat`: batch script for collecting data
* `data/Collecting_Data/mouse_input.c` /`data/Collecting_Data/mouse_input.h` / `data/Collecting_Data/mouse_input.sln` / `data/Collecting_Data/mouse_input.dll` : our input libraries for collecting data
* `battery_use-000025.db`: Pre-collected Database
* `Loading_Data.py`: Main code for loading and analyzing data

### `run.py`
* Main driver for this project. See cp2.ipynb for a basic analysis for Checkpoint2 on the battery table.
