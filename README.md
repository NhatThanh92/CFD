This projet using Coupling WRF/FLUENT to simulate the complex terrain

1. Using Weather Research and Forecasting (WRF)
=======
the coupling between the meteorological model, Weather Research and Forecasting (WRF), and a
commercial computational fluid dynamics (CFD) software, FLUENT, is developed. 
The WRF is used to provide velocity profile inlet for unsteady boundary conditions for FLUENT. 
In FLUENT with higher resolutions and techniques such as Computer Aided Design (CAD), Finite Volume Method, and
several turbulent models as Large eddy simulation (LES) and Detached eddy simulation (DES) combined with the appropriate boundary condition
could provide wind field simulation results in more accuracy.
1. Run wrf4_3D_wrf_contour_CBL.cpp

2. Run unsteady unsteady_wrf4_ch_3D-boundary 10min-(0-10) udf.cpp
References
[1] [Trieu Nhat-Thanh NGUYEN, Jiun-Jih MIAU, Yng-Ru CHEN, "Coupling WRF/FLUENT to simulate the complex terrain" 2018 AASRC Conference · Tainan · Dec 8, 2018](https://drive.google.com/file/d/1SAY42wGYLURPYZRgFv9WgAZ3PBjTuARZ/view?usp=sharing)
[2] [Trieu Nhat-Thanh Nguyen, MIAU Jiun-Jih, S.Y.Lin,"Application of numerical simulation to verify measurement data of wind field in offshore domain"
12th International Symposium on Advanced Science and Technology in Experimental Mechanics, 1-4 November, 2017, Kanazawa, Japan](https://drive.google.com/file/d/1WWlXbe0GVR_II1cRplhunu_vwX2RlO85/view?usp=sharing)
