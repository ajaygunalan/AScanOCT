# SOP for File Naming

## Metdata
1. Author: Ajay Gunalan
2. Last Update On: 8th JULY 2021
3. Project: Endosocpic OCT

## Folders Heirarchy

### Parent Folders

1. All folders should be either backed up via oneDrive or gitLab repository.
2. Folders will not contain any spaces or underscore or dash.
3. Name of the folder should be a combination of words describing the folder. Like AScanOCT, BScanOCT

### Sub-Folders 

#### Code Files

1. Source code will be stored in the folder `src`
2. Any dependencies will be stored in folder `Dependencies` with the appropriate name of the dependency as a subfolder. e.g., `AScanOCT/Dependencies/SpectralRadar/` where `SpectralRadar` is the folder that contains dependencies.
3. Binaries will be stored in the folder `bin` with the appropriate name of the platform as a subfolder. e.g., `AScanOCT/bin/Win32/Debug/` where `Win32` is the folder that contains binaries for Win32 platform.

#### Data Files

1. Data should be stored either in `AScanData\"date"` or `BScanData\"date"` folder. `date` shoulde of the format `8_july_21`.
2. files should name properly like `AScan_Probe_1_Processed.csv` or `AScan_Probe_1.raw`. Type Scan + Probe + Type of data.
3. README.md should contain info about the Probe_1.

