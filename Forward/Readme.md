<img width="600" alt="image" src="https://github.com/myroms/roms_test/assets/23062912/ad6a7ef1-1fed-4b2e-96b9-9c53615b9333">

 ## NLM Kernel Benchmark: East Coast Community Ocean Forecast System (ECCOFS) 

 This directory contains instructions for configuring and benchmarking the **ROMS** split,
 mixed-resolution **RBL4D-Var** data assimilation algorithm. The mixed-resolution **4D-Var** 
 utilizes the East Coast Community Ocean Forecast System (**ECCOFS**) application. The **4D-Var** 
 outer loops, encompassing the **Background** and **Analysis** phases, operate at a **3** km grid
 resolution (**ECCOFS3**; **1667x1443x50**). In contrast, the inner loops in the **Increment** 
 (minimization) phase use a coarser **6** km grid (**ECCOFS6**; **834x722x50**) to enhance
 computational efficiency. During the **Analysis** phase, **4D-Var** increments generated on
 the coarse grid are interpolated to the finer grid using the **roms_interp** and **roms2roms** 
 `CLASS objects`. For further information, consult the modules **ROMS/Utility/roms_interp.F** and
 **ROMS/Utility/state_regrid.F**.

 <img width = "648" height = "542" alt = "image" src = "https://github.com/user-attachments/assets/645a4d40-f694-4f81-b1c6-f02b78886383"> 

 In the **Background** phase, the coarse grid trajectory needed to linearize the tangent
 linear (**TLM**) and adjoint (**ADM**) model kernels is extracted by a decimation of the
 3km grid solution using the CPP option **GRID_EXTRACT**. Grid decimation is only possible
 if the parent grid (**ECCOFS3** , **Lm=1665** and **Mm=1441**) size satisfies
 **MOD(Lm+1, 2) = 0** and **MOD(Mm+1, 2) = 0** . Please check
 https://github.com/myroms/roms/pull/32 for more information. Currently, we only
 support **ExtractFlag=2** for decimation in the **mixed-resolution** split **4D-Var** scheme
 because land/sea masking complicates extraction at factors larger than two.

 The **mixed-resolution** split **4D-Var** data assimilation strategy improves the
 computational efficiency, as shown for the smaller
 [USEC application](https://github.com/myroms/roms_test/blob/main/USEC/RBL4DVAR_mixres/Readme.md), 
 where the computational efficiency can be improved over **88** percent (**Case 8**) compared
 to the **3** km non-splitted **4D-Var** in double precision (**Case 5**).

 In this benchmark, data are provided for a **3**-day data assimilation cycle:

 - **4D-Var Cycle**: Jan 1 - Jan 4, 2019 (execution creates sub-directory **2019.01.01**).

 The **4D-Var** algorithm can be configured with **1** outer loop and **16** inner loops or
 **2** outer loops and **8** inner loops (default). The user can make the appropriate changes to
 the **`submit_mixres_rbl4dvar.sh`** script.
 
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
   VERIFICATION            Proccess model solution at observation locations
   WTYPE_GRID              Spatially varying Jerlov water type index
  ```

The CPP option **SPLIT_EXECUTABLE** avoids allocating the control vectors and adjoint
state arrays in the high-resolution **Background** and **Analysis** phases to reduce memory
requirements. The pointers for such variables are available but unallocated since they are
unused. Otherwise, it would limit the running of larger applications because they do not
fit into the computer's memory. Notice that the **outer loop** grid is twice as large as
the **inner loop** grid.

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
  roms_eccofs3km.in             ROMS nonlinear model standard input script
  eccofs.h                      ROMS header file, ECCOFS3 application
  ```
### How to Compile ROMS:
   
To compile **ROMS** data assimilation executables for **4D-Var** outer and inner loops, use:
  ``` d
    build_roms.sh -pio -j 10                      creates executable romsM
  ```
Notice it asks to compile with the **PIO-NetCDF** library (**-pio** option) to speed up the
computations.

Please review the **build** script, as it includes **CPP** options for ROMS executable.

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

