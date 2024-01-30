**This projet using Coupling WRF/FLUENT to simulate the complex terrain**

**1. Coupling WRF and FLUENT:**

The project involves the coupling of the WRF meteorological model with the FLUENT CFD software.
WRF is used to provide the velocity profile at the inlet for unsteady boundary conditions in FLUENT.

**2. Simulation Techniques in FLUENT:**

FLUENT utilizes higher resolutions and various techniques such as Computer-Aided Design (CAD), Finite Volume Method, and multiple turbulent models, including Large Eddy Simulation (LES) and Detached Eddy Simulation (DES).
These techniques, combined with appropriate boundary conditions, aim to enhance the accuracy of wind field simulation results.

**3. Specific Files to Run:**

1.wrf4_3D_wrf_contour_CBL.cpp: This file appears to be related to the WRF simulation, possibly for generating contours in a complex boundary layer.

2. unsteady_wrf4_ch_3D-boundary 10min-(0-10) udf.cpp: This file seems to be associated with the unsteady WRF simulation with a specific time range and user-defined functions (UDF) in FLUENT.

Click [Here]()for more info.
**References**

[1] Trieu Nhat-Thanh NGUYEN, Jiun-Jih MIAU, Yng-Ru CHEN, ["Coupling WRF/FLUENT to simulate the complex terrain"](https://drive.google.com/file/d/1SAY42wGYLURPYZRgFv9WgAZ3PBjTuARZ/view?usp=sharing), 2018 AASRC Conference · Tainan · Dec 8, 2018

[2] Trieu Nhat-Thanh Nguyen, MIAU Jiun-Jih, S.Y.Lin, ["Application of numerical simulation to verify measurement data of wind field in offshore domain"
](https://drive.google.com/file/d/1WWlXbe0GVR_II1cRplhunu_vwX2RlO85/view?usp=sharing), 12th International Symposium on Advanced Science and Technology in Experimental Mechanics, 1-4 November, 2017, Kanazawa, Japan
