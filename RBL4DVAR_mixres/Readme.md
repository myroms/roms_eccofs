<img width="600" alt="image" src="https://github.com/myroms/roms_test/assets/23062912/ad6a7ef1-1fed-4b2e-96b9-9c53615b9333">

 ## 4D-Var Benchmark: East Coast Community Ocean Forecast System (ECCOFS) 

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
   DIURNAL_SRFLUX          Modulates shortwave radiation by the local diurnal cycle, if NAM forcing
   GRID_EXTRACT            Activates writing a decimated nonlinear trajectory for inner loops
   OMEGA_IMPLICIT          Adaptive, Courant-number-based implicit vertical advection, NLM kernel
   OUT_DOUBLE              Double precision output fields in NetCDF files
   OUT_NETCDF4             Creating output compressed Netcdf4/HDF5 files
   PIO_LIB                 Using Parallel-IO from the PIO library
   RPCG                    Restricted B-preconditioned Lanczos minimization
   SPLIT_EXECUTABLE        Split 4D-Var executable for background/analysis phases
   SPLIT_RBL4DVAR          Split RBL4D-Var algorithm driver
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
                                   ../Data/INI/eccofs6km_roms_ini.nc4 (generic)
                   Boundary File:  ../Data/BRY/eccofs3km_bry_Mercator_monthly_2018_BiasCor.nc4
                                   ../Data/BRY/eccofs3km_bry_Mercator_monthly_2019_BiasCor.nc4
                                   ../Data/BRY/eccofs6km_bry_empty.nc4
                Climatology File:  ../Data/CLM/eccofs3km_clm_Mercator_Dec2018.nc
                                   ../Data/CLM/eccofs3km_clm_Mercator_Jan2019.nc
       Nudging Coefficients File:  ../Data/CLM/eccofs3km_nudgcoef_10days.nc4
                                   ../Data/CLM/eccofs6km_nudgcoef_10days.nc4
              River Forcing File:  ../Data/GRD/eccofs3km_rivers_GloFAS_2011-2024_v5.nc4
              Tidal Forcing File:  ../Data/GRD/tides_tpxo_eccofs3km_01.nc4
                                   ../Data/GRD/tides_tpxo_eccofs6km_01.nc4

  ERA5 Atmospheric Forcing Files:  ../Data/FRC/frc_eccofs_ERA5_allflux_Jan2019.nc4

     Initial Conditions STD File:  ../Data/STD/eccofs3km_roms_std_i_20190827.nc4
                                   ../Data/STD/eccofs3km_roms_std_i_20190830.nc4
                                   ../Data/STD/eccofs6km_roms_std_i_20190827.nc4
                                   ../Data/STD/eccofs6km_roms_std_i_20190830.nc4
    Boundary Conditions STD File:  ../Data/STD/eccofs3km_roms_std_b_20190827.nc4
                                   ../Data/STD/eccofs3km_roms_std_b_20190830.nc4
                                   ../Data/STD/eccofs6km_roms_std_b_20190827.nc4
                                   ../Data/STD/eccofs6km_roms_std_b_20190830.nc4
        Surface Forcing STD File:  ../Data/STD/eccofs3km_roms_std_f_20190827.nc4
                                   ../Data/STD/eccofs3km_roms_std_f_20190830.nc4
                                   ../Data/STD/eccofs6km_roms_std_f_20190827.nc4
                                   ../Data/STD/eccofs6km_roms_std_f_20190830.nc4

    Initial Conditions Norm File:  ../Data/NRM/eccofs3km_roms_std_i.nc4
                                   ../Data/NRM/eccofs6km_roms_std_i.nc4
   Boundary Conditions Norm File:  ../Data/NRM/eccofs3km_roms_std_b.nc4
                                   ../Data/NRM/eccofs6km_roms_std_b.nc4

               Observations File:  ../Data/OBS/eccofs3km_roms_obs_20190101.nc4
                                   ../Data/OBS/eccofs6km_roms_obs_20190101.nc4
  ```

### Configuration and input scripts:

  ``` d
  build_split.csh               ROMS GNU Make compiling and linking CSH script
  build_split.sh                ROMS GNU Make compiling and linking BASH script
  plot_inc.m                    Matlab plotting script for 4D-Var increments and control vectors 
  roms_da_usec_era5.tmpl        ROMS data assimilation standard input template, ERA-5 forcing
  roms_nl_usec_era5.tmpl        ROMS nonlinear model standard input template, ERA-5 forcing
  s4dvar.in                     RBL4D-Var data assimilation template
  submit_mixres_rbl4dvar.sh     Job submission bash script
  eccofs.h                      ROMS header file, USEC application
  ```
The **submit_mixres_rbl4dvar.sh** script is complex.  Please study it carefully. You can modify
for your appropriate computer environment and **RBL4D-Var** running parameters. There is a
user-customizable section above the following heading:

  ``` d
  ##---------------------------------------------------------------------
  ## User tunable parameters. If you follow recommendations, this is
  ## the only section that you need to customize..
  ##---------------------------------------------------------------------

      ROMS_APP="ECCOFS"                # ROMS Application CPP
  ...
  ```

It uses the following **Perl** scripts: **substitute** and **dates**, which are distributed in
**ROMS** source code. They are located in **${ROMS_ROOT}/ROMS/Bin**. Thus, you must set
**ROMS_ROOT** to the appropriate path in the user-customizable section.


### How to Compile ROMS:
   
To compile **ROMS** data assimilation executables for **4D-Var** outer and inner loops, use:
  ``` d
    build_split.sh -nl -pio -j 10                 creates executable romsM_nl
    build_split.sh -da -pio -j 10                 creates executable romsM_da
  ```
Notice it asks to compile with the **PIO-NetCDF** library (**-pio** option) to speed up the
computations. Also, the **4D-Var split** scheme uses two different executables for **ROMS**:
  ``` d
      romsM_nl       Nonlinear driver for RBL4D-Var Background and Analysis phases, outer loops

      romsM_da       Data assimilation driver for RBL4D-Var Increment phase, inner loops
  ```
Please review the **build** script, as it includes **CPP** options for each executable. Also, it
sets standard **CPP** options for both executables. This strategy is preferable to having a **ROMS**
header file for the 3km and 6km grids.

To submit the job on 64 CPUs via SLURM or not, use:
  ``` d
    sbatch submit_mixres_rbl4dvar.sh        or
    submit_mixres_rbl4dvar.sh > & log &
  ```
Users may modify the number of processors to use in the **submit_mixres_rbl4dvar.sh** script.

---

### ROMS Execution Sequence:

 The **submit_mixres_rbl4dvar.sh** script creates the **2019.08.27** and **2019.08.30**
 sub-directories for **RBL4D-Var** Cycle **1** and Cycle **2**, respectively. It includes
 all the required input scripts to run the **mixed-resolution RBL4D-Var** system. The input
 scripts are generated from the templates.  The **submit_mixres_rbl4dvar.sh** script is
 designed to run sequential data assimilation 3-day cycles. It reports the execution sequence
 with detailed information. It can be executed in dry run mode (**DRYRUN=1**) to print its
 configuration without running. Please do it before submitting a large job, since
 it will take some computer resources and time.

``` d
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 ROMS Split RBL4D-Var Data Assimilation: ECCOFS
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

                    ROMS Root: /home/arango/ocean/repository/git/roms
            ROMS Executable A: romsM_nl  (background, analysis)
            ROMS Executable B: romsM_da  (increment, post_error)

      RBL4D-Var Starting Date: 2019-01-01  datenum = 737426
   First RBL4D-Var Cycle Date: 2019-01-01  datenum = 737426
    Last RBL4D-Var Cycle Date: 2019-01-01  datenum = 737426
          ROMS Reference Date: 2011-01-01  datenum = 734504
       RBL4D-Var Cycle Window: 3 days
      Number of parallel PETs: 12  (3x4)
   Current Starting Directory: /home/arango/ROMS/Projects/ECCOFS/RBL4DVAR_mixres
         ROMS Application CPP: ECCOFS
      Descriptor in filenames: eccofs3km  (outer loops grid)
      Descriptor in filenames: eccofs6km  (inner loops grid)

----------------------------------------------------------------------------------------------------

       RBL4D-Var Cycle Date: 2019-01-01 00  DayOfYear = 001  Cycle = 1
         Data sub-directory: ../../Data
          Run sub-directory: 2019.01.01
      Number of outer loops: 1
      Number of inner loops: 16
    NLM trajectory  writing: 60, 40 timesteps
    NLM quicksave   writing: 30, 40 timesteps
    NLM decimation  writing: 30, 20 timesteps
    TLM trajectory  writing: 60, 40 timesteps
    ADJ trajectory  writing: 2160, 1440 timesteps
    AVG trajectory  writing: 720, 480 timesteps
    DIA trajectory  writing: 720, 480 timesteps
    SFF adjustment  writing: 30, 20 timesteps
    OBC adjustment  writing: 30, 20 timesteps
  NLM multi-file trajectory: 0, 0 timesteps
                ROMS DSTART: 2922.0d0
     Outer Loops Resolution: 3 km
     Inner Loops Resolution: 6 km
           I/O Files Prefix: eccofs3km  (outer loops grid)
           I/O Files Prefix: eccofs6km  (inner loops grid)
           I/O Files Suffix: 20190101
              ReferenceTime: 2011 01 01 00 00 00
        RBL4D-Var StartTime: 2019 01 01 00 00 00
        RBL4D-Var  StopTime: 2019 01 04 00 00 00
   ROMS outer loops Grid IC: ../../Data/INI/eccofs3km_roms_ini_20190101.nc4
   ROMS inner loops Grid IC: ../../Data/INI/eccofs6km_roms_ini_20190101.nc4
   NL Standard Input Script: roms_nl_eccofs_era5_20190101.in   (outer loops grid)
   DA Standard Input Script: roms_da_eccofs_era5_20190101.in   (inner loops grid)
  NL RBL4D-Var Input Script: rbl4dvar_nl.in  (outer loops grid)
  DA RBL4D-Var Input Script: rbl4dvar_da.in  (inner loops grid)

Cycle 1, Creating run sub-directory: 2019.01.01

Changing to directory: /home/arango/ROMS/Projects/ECCOFS/RBL4DVAR_mixres/2019.01.01

   Creating NL ROMS Standard Input Script: roms_nl_eccofs_era5_20190101.in
   Creating DA ROMS Standard Input Script: roms_da_eccofs_era5_20190101.in
   Copying NLM IC file ../../Data/INI/eccofs3km_roms_ini_20190101.nc4  as  eccofs3km_roms_ini_20190101.nc4
   Copying NLM IC file ../../Data/INI/eccofs6km_roms_ini.nc4  as  eccofs6km_roms_ini_20190101.nc4
   Copying OBS    file ../../Data/OBS/eccofs3km_roms_obs_20190101.nc4  as  eccofs3km_roms_obs_20190101.nc4
   Copying OBS    file ../../Data/OBS/eccofs6km_roms_obs_20190101.nc4  as  eccofs6km_roms_obs_20190101.nc4

Running 4D-Var System:  Cycle = 1   Outer = 0   Phase = background

   Creating 4D-Var Input Script from Template: rbl4dvar_nl.in   Outer = 0  Phase = background
     (Resolution = 3 km, Fprefix = eccofs3km, Fsuffix = 20190101)

   mpirun -np 12 romsM_nl roms_nl_eccofs_era5_20190101.in

   Renaming NLM trajectory eccofs6km_roms_fwd_20190101.nc  to  eccofs6km_roms_fwd_20190101_outer0.nc

Running 4D-Var System:  Cycle = 1   Outer = 1   Phase = increment

   Creating 4D-Var Input Script from Template: rbl4dvar_da.in   Outer = 1  Phase = increment
     (Resolution = 6 km, Fprefix = eccofs6km, Fsuffix = 20190101)

   mpirun -np 12 romsM_da roms_da_eccofs_era5_20190101.in
```

---

### The output Files:

- Standard Output Files:
  ``` d
    log_outer0.nl                                 Outer loop 0, Background phase, NLM trajectory
    log_outer1.da                                 Outer loop 1, Increment phase, TLM/ADM inner loops
    log_outer1.nl                                 Outer loop 1, Analysis phase
  ```

- **ROMS** NetCDF Files:
  ``` d
   eccofs3km_roms_dai_20190101.nc                   Next DA cycle 3km initial state
   eccofs3km_roms_fwd_20190101_outer0.nc            Outer loop 0, 3km NLM trajectory
   eccofs3km_roms_fwd_20190101_outer1.nc            Outer loop 1, 3km NLM trajectory
   eccofs3km_roms_ini_20190101.nc                   NLM 3km initial conditions
   eccofs3km_roms_itl_20190101.nc                   TLM 3km increments
   eccofs3km_roms_mod_20190101.nc                   RBL4D-Var model/observations control vectors
   eccofs3km_roms_obs_20190101.nc                   3km observation vectors
   eccofs3km_roms_qck_20190101_outer0.nc            Outer loop 0, NLM QuickSave 3km history
   eccofs3km_roms_qck_20190101_outer1.nc            Outer loop 1, NLM QuickSave 3km history
   eccofs3km_roms_rst_20190101.nc                   3km NLM restart

   eccofs6km_roms_adj_20190101.nc                   ADM 6km gradients
   eccofs6km_roms_fwd_20190101_outer0.nc            Outer loop 0, NLM 6km decimated trajectory
   eccofs6km_roms_fwd_20190101_outer1.nc            Outer loop 1, NLM 6km decimated trajectory
   eccofs6km_roms_ini_20190101.nc                   Inner loops,  NLM 6km initial conditions and prior 
   eccofs6km_roms_itl_20190101.nc                   TLM 6km increments
   eccofs6km_roms_obs_20199101.nc                   6km observation vectors
   eccofs6km_roms_tlf_20190101.nc                   Adjoint impulse forcing, TLM forcing
  ```

---

  ### Results

- **4D-Var Cycle**: Jan 1 - Jan 4, 2019, **ERA** forcing. Top-to-bottom figures showing 3km and 6km increments for free surface, potential temperature, salinity, u-velocity, and v-velocity at 20m depth. Notice that higher- and lower-resolution increments are indistinguishable. They are plotted with the provided **plot_state.m** Matlab script.
  
| 3km Increments at z=20m   | 6km Increments at z=20m  |
:--------------------------:|:-------------------------:
|<img width="600" alt="zeta_3km" src="https://github.com/user-attachments/assets/e17a588b-ca82-4f28-b0e9-6a49c9cc20aa" /> | <img width="600" alt="zeta_6km" src="https://github.com/user-attachments/assets/7bb43685-a490-4da7-80c1-97cddb03d937" /> |
|<img width="600" alt="temp_3km_20m" src="https://github.com/user-attachments/assets/34b36583-da67-4442-947d-067b01fa3174" /> | <img width="600" alt="temp_6km_20m" src="https://github.com/user-attachments/assets/d48d4c24-3bc2-4327-a7d7-83dcefb96004" /> |










