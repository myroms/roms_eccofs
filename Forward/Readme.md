<img width="600" alt="image" src="https://github.com/myroms/roms_test/assets/23062912/ad6a7ef1-1fed-4b2e-96b9-9c53615b9333">

 ## NLM Kernel Benchmark: East Coast Community Ocean Forecast System (ECCOFS) 

 This directory contains instructions for configuring and benchmarking the **ROMS** nonlinear
 kernel driver with the East Coast Community Ocean Forecast System (**ECCOFS**) application. It
 uses the fine-resolution 3km grid (**ECCOFS3**). It extracts a **6km** grid trajectory using a
 decimation factor of **2**, sampling the solution every other horizontal grid point, level-by-level.
 This strategy is used in the **mixed-resolution** split **4D-Var** data assimilation algorithm
 to improve computational efficiency, as shown in the smaller
 [USEC application](https://github.com/myroms/roms_test/blob/main/USEC/RBL4DVAR_mixres/Readme.md).
 It also computes the model solution at the observation locations, **H(x)** operator, which can be used
 for verification purposes. The figure below shows the 3km bathymetry and location of the cross-sections.

<img width="800" alt="eccofs3km_bath" src="https://github.com/user-attachments/assets/05a4bdbf-d892-422e-b3f0-298ca2fe1113" />
 
### Important CPP options:

They are activated in the build scripts.

  ``` d
   ECCOFS                  East Coast Community Ocean Forecast System 4D-var Data Assimilation
   ATM_PRESS               Impose inverse barometer effect in the pressure gradient term
   BGQC                    Background quality control of observations
   BULK_FLUXES             Surface bulk fluxes parameterization, latest COARE 3.5
   DEFLATE                 Setting compression in output NetCDF-4/HDF5 files
   DELAYED_SYNC_NF90       Delaying file disk synchronization until calling nf90_close
   DELAYED_SYNC_PIO        Delaying file disk synchronization until calling PIO_closefile
   GRID_EXTRACT            Activates writing a decimated nonlinear trajectory for inner loops
   OMEGA_IMPLICIT          Adaptive, Courant-number-based implicit vertical advection, NLM kernel
   OUT_DOUBLE              Double precision output fields in NetCDF files
   OUT_NETCDF4             Creating output compressed Netcdf4/HDF5 files
   PIO_LIB                 Using Parallel-IO from the PIO library
   VERIFICATION            Process model solution at observation locations
   WTYPE_GRID              Spatially varying Jerlov water type index
  ```

### ROMS Input NetCDF files:

The input **NetCDF** files were changed on Feb 2026 to compressed **NetCDF4/HDF4** to reduce they size
by around 50% and improve downloading bandwidth from GitHub.

  ``` d
                       Grid File:  ../Data/GRD/eccofs3km_roms_grd.nc4
                                   ../Data/GRD/eccofs6km_roms_grd.nc4
                    Initial File:  ../Data/INI/eccofs3km_roms_ini_20190101.nc4
                   Boundary File:  ../Data/BRY/eccofs3km_bry_Mercator_monthly_2018_BiasCor.nc4
                                   ../Data/BRY/eccofs3km_bry_Mercator_monthly_2019_BiasCor.nc4
                Climatology File:  ../Data/CLM/eccofs3km_clm_Mercator_Dec2018.nc
                                   ../Data/CLM/eccofs3km_clm_Mercator_Jan2019.nc
       Nudging Coefficients File:  ../Data/CLM/eccofs3km_nudgcoef_10days.nc4
              River Forcing File:  ../Data/GRD/eccofs3km_rivers_GloFAS_2011-2024_v5.nc4
              Tidal Forcing File:  ../Data/GRD/tides_tpxo_eccofs3km_01.nc4

  ERA5 Atmospheric Forcing Files:  ../Data/FRC/frc_eccofs_ERA5_allflux_Jan2019.nc4
               Observations File:  ../Data/OBS/eccofs3km_roms_obs_20190101.nc4
  ```

### Configuration and input scripts:

  ``` d
  build_roms.csh                ROMS GNU Make compiling and linking CSH script
  build_roms.sh                 ROMS GNU Make compiling and linking BASH script
  rbl4dvar.in                   RBL4D-Var data assimilation script
  roms_eccofs3km                ROMS nonlinear model standard input script
  eccofs3.h                     ROMS header file, ECCOFS3 application
  ```
### How to Compile and Run ROMS:
   
To compile **ROMS** data assimilation executables for **4D-Var** outer and inner loops, use:
  ``` d
    build_roms.sh -pio -j 10                      creates executable romsM
  ```
Notice it asks to compile with the **PIO-NetCDF** library (**-pio** option) to speed up the
computations. Alternatively, you have the option of downloading the **ROMS** code, say, the branch
**develop**, with the build script:

  ```
    build_roms.sh -pio -j 10 -b develop
  ```
Please review the **build** script, as it includes **CPP** options for the **ROMS** executable.

To run **ROMS**, use:

  ```
  mpirun -n 64 romsM < roms_eccofs3km.in > & log.nl &
  ```
You may edit **`roms_eccofs3dl.in`** to change the parallel partition:
  
  ``` d
! Domain decomposition parameters for serial, distributed-memory, or
! shared-memory configurations used to determine tile horizontal range
! indices (Istr,Iend) and (Jstr,Jend), [1:Ngrids].

      NtileI == 8                                     ! I-direction partition
      NtileJ == 8                                     ! J-direction partition
  ```
Note that the total number of **MPI** processes is **NtileI * NtileJ**. 


---

### The output Files:

- Standard Output Files:
  ``` d
    log.nl                                        ECCOFS 3km standard output log file
  ```

- **ROMS** NetCDF Files:
  ``` d
   eccofs3km_roms_his_20190101.nc                 ECCOFS 3km NLM history trajectory
   eccofs3km_roms_mod_20190101.nc                 ECCOFS 3km model values at observation location
   eccofs3km_roms_qck_20190101.nc                 ECCOFS 3km QuickSave history trajectory
   eccofs3km_roms_rst_20190101.nc                 ECCOFS 3km NLM restart
   eccofs6km_roms_xtr_20190101.nc                 ECCOFS 6km NLM decimated history trajectory
  ```
---

### Results

- **NLM Solution**: Jan 1 - Jan 4, 2019, **ERA** forcing. Top-to-bottom figures showing **3km** and decimated **6km** fields for free surface, potential temperature, salinity, u-velocity, and v-velocity at 20m depth. Notice that higher- and lower-resolution increments are indistinguishable. They are plotted with the provided **plot_state.m** Matlab script.
  
| 3km Solution at z=20m     | Decimated 6km Solution at z=20m  |
:--------------------------:|:---------------------------------:
|<img width="600" alt="zeta_3km_20m" src="https://github.com/user-attachments/assets/75318489-9e0d-4d16-a923-2431a0e6ac03" /> | <img width="600" alt="zeta_6km_20m" src="https://github.com/user-attachments/assets/ef896ade-19d2-418f-8b2c-a587f815cc87" /> |
|<img width="600" alt="temp_3km_20m" src="https://github.com/user-attachments/assets/d30bda1e-67a7-4d7c-936e-0bcbe0bcfdfd" /> | <img width="600" alt="temp_6km_20m" src="https://github.com/user-attachments/assets/d881eba4-cd02-4617-bcd8-f03bbed03854" /> |
|<img width="600" alt="salt_3km_20m" src="https://github.com/user-attachments/assets/37bec9bf-8447-4a8f-afc5-597c120de180" /> | <img width="600" alt="salt_6km_20m" src="https://github.com/user-attachments/assets/0d9174b7-8bb3-4775-a79c-37fddcefb4f0" /> |
|<img width="600" alt="u_3km_20m" src="https://github.com/user-attachments/assets/da6fd2cf-ed04-44cd-a3d8-d66c8422ac88" /> | <img width="600" alt="u_6km_20m" src="https://github.com/user-attachments/assets/add6c0e4-2b14-482b-bf8d-f7697da5b0e2" /> |
|<img width="600" alt="v_3km_20m" src="https://github.com/user-attachments/assets/83fb4dd6-0a84-4c93-a3a5-82cc027b6d10" /> | <img width="600" alt="v_6km_20m" src="https://github.com/user-attachments/assets/94b2a1ca-d774-4822-a49c-936bce201c27" /> |

- **NLM Cross-Sections**: Potential temperature and salinity for Jan 4, 2019.

| 3km Potential Temperature | 3km Salinity  |
:--------------------------:|:---------------------------------:
|<img width="600" alt="temp_68W_sec" src="https://github.com/user-attachments/assets/5e1b1eb9-a376-4cf7-b698-ce54e6680e0e" /> | <img width="600" alt="salt_68W_sec" src="https://github.com/user-attachments/assets/f2aae14d-5161-4c52-aa7a-9aeb7c3ce71b" /> |
|<img width="600" alt="temp_35N_sec" src="https://github.com/user-attachments/assets/3e4e768a-26a2-40c6-b410-94ac2cd016ab" /> | <img width="600" alt="salt_35N_sec" src="https://github.com/user-attachments/assets/80eca752-0f35-49be-ae8b-5af0fe82d70a" /> |
|<img width="600" alt="temp_Loop_sec" src="https://github.com/user-attachments/assets/476be6e8-77b1-4e1a-b319-acfc25b367a1" /> | <img width="600" alt="salt_Loop_sec" src="https://github.com/user-attachments/assets/fecd7469-feb1-4a73-89cc-edaf6ba342f3" /> |

