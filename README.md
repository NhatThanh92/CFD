## Coupling WRF/FLUENT to simulate the complex terrain

**1. Coupling WRF and FLUENT:**

The project involves the coupling of the WRF meteorological model with the FLUENT CFD software.
WRF is used to provide the velocity profile at the inlet for unsteady boundary conditions in FLUENT.

**2. Simulation Techniques in FLUENT:**

FLUENT employs the Finite Volume Method along with various turbulent models, such as Reynolds-averaged Navier–Stokes (RANS), Large Eddy Simulation (LES) and Detached Eddy Simulation (DES). These turbulent models play a crucial role in addressing the challenge of capturing small-scale spatial variability in meteorological dynamics and land-atmosphere exchanges, particularly over complex terrain.

In this process, the velocity profile generated by WRF serves as input for FLUENT. The simulation runs through User Defined Functions (UDF) specifically developed for FLUENT, which are coded in C++. 
Linear and Bilinear interpolation techniques are utilized to interpolate spatial and temporal variables within the simulation.

**3. Specific Files to Run:**

1.WRF.cpp: This script is designed to read the output file generated by WRF simulation (wrfout_d03_2017-06-06_12). Its main function is to rearrange the vector velocity (u, v, w) according to (x,y,z), for example: the file "wrf_3D_cbl_1102_0hr0min-2.txt" with respect to time.

2.Create UDF.cpp: This script is crafted to generate UDF files containing the velocity profile obtained from the WRF model. First, it reads a sequence of files named "wrf_3D_cbl_1102_%dhr%dmin-2.txt" starting from step 1. 

Simultaneously, it incorporates the turbulence Large Eddy Simulation (LES) by employing the Smagorinsky-Lilly model. Finally, The script utilizes both Linear and Bilinear interpolation techniques to interpolate spatial and temporal variables during the simulation process. (example: 3D_ch_1216_0hr_udf.c)

Click [Here](https://drive.google.com/file/d/1ndAXF_V_vMwYsiWR3ACs11DyPRVYYA5g/view?usp=sharing) for more info.

**References**

[1] Trieu Nhat-Thanh NGUYEN, Jiun-Jih MIAU, Yng-Ru CHEN, "Coupling WRF/FLUENT to simulate the complex terrain", 2018 AASRC Conference · Tainan · Dec 8, 2018. (Click [Here](https://drive.google.com/file/d/1SAY42wGYLURPYZRgFv9WgAZ3PBjTuARZ/view?usp=sharing))

[2] Trieu Nhat-Thanh Nguyen, MIAU Jiun-Jih, S.Y.Lin, "Application of numerical simulation to verify measurement data of wind field in offshore domain", 12th International Symposium on Advanced Science and Technology in Experimental Mechanics, 1-4 November, 2017, Kanazawa, Japan. (Click [Here](https://drive.google.com/file/d/1WWlXbe0GVR_II1cRplhunu_vwX2RlO85/view?usp=sharing))
