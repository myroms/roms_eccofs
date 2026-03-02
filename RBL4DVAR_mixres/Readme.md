<img width="600" alt="image" src="https://github.com/myroms/roms_test/assets/23062912/ad6a7ef1-1fed-4b2e-96b9-9c53615b9333">

## 4D-Var Mixed-Resolution Benchmark: East Coast Community Ocean Forecast System (ECCOFS)

This directory contains instructions for configuring and benchmarking the ROMS split, 
mixed-resolution RBL4D-Var data assimilation algorithm. The mixed-resolution 4D-Var 
utilizes the East Coast Community Ocean Forecast System (ECCOFS) application. The 4D-Var 
outer loops, encompassing the Background and Analysis phases, operate at a 3 km grid 
resolution (ECCOFS3; 1667x1443x50). In contrast, the inner loops in the Increment 
(minimization) phase use a coarser 6 km grid (ECCOFS6; 834x722x50) to enhance 
computational efficiency. During the Analysis phase, 4D-Var increments generated on 
the coarse grid are interpolated to the finer grid using the roms_interp and roms2roms 
CLASS objects. For further information, consult the ROMS/Utility/roms_interp.F and 
ROMS/Utility/state_regrid.F modules.

| Hurricane Dorian | CFF-USEC3 and CFF-USEC6 Grids |
:-----------------:|:-------------------------:
|<img width="600" alt="image" src="https://github.com/user-attachments/assets/430d8ff1-431b-4ab1-933e-09a979d93190"> | <img width="600" alt="image" src="https://github.com/user-attachments/assets/a28d91f6-24ab-440b-bb91-0313361a25c1"> |

In the **Background** phase, the coarse grid trajectory needed to linearize the tangent
linear (**TLM**) and adjoint (**ADM**) model kernels is extracted by a decimation of the
3km grid solution using the CPP option **GRID_EXTRACT**. Grid decimation is only possible
if the parent grid (**CFF-USEC3**, **Lm=559** and **Mm=241**) size satisfies
**MOD(Lm+1, 2) = 0** and **MOD(Mm+1, 2) = 0**. Please check
https://github.com/myroms/roms/pull/32 for more information. Currently, we only
support **ExtractFlag=2** for decimation in the **mixed-resolution** split **4D-Var** scheme
because the land/sea masking complicates extraction at factors larger than two.

The **mixed-resolution** split **4D-Var** data assimilation strategy improves the
computational efficiency as shown in the following table for a single **3**-day **4D-Var**
data assimilation cycle from a desktop Linux box (16 CPUs and 1 GPU with 384 CUDA cores, NVIDIA)
on **12** CPUs with a **3x4** partition compiled with **ifort** (Spack-Stack 1.9). The
benchmarks were computed using the **ROMS** option to output **NetCDF3** 64-bit offset files
(with **`OUT_NETCDF4`** set to off).

<img width="940" alt="image" src="https://github.com/user-attachments/assets/aaf2ba95-1378-4636-a135-360dc3186c71" />

The **mixed resolution** algorithm improves this application's computational efficiency by
over **88** percent (**Case 8**) compared to the **3**km non-splitted **4D-Var** in double precision
(**Case 5**). In some cases, **mixed-precision** outer loops (**double**) and inner loops (**single**)
are possible but **not recommended** because they affect the stability of the tangent linear
and adjoint trajectories. It improves efficiency by an additional 3 percent. However,
stability of the solution takes precedence.

In this test case, data is provided for two **3**-day data assimilation cycles:

- **4D-Var Cycle 1**: Aug 27 - Aug 30, 2019 (execution creates sub-directory **2019.08.27**)
- **4D-Var Cycle 2**: Aug 30 - Sep 02, 2019 (execution creates sub-directory **2019.08.30**)

to demonstrate how to configure continuous data assimilation cycles that use the **Analysis**
file (**DAINAME**) to initialize the next **4D-Var** cycle. The atmospheric forcing is from
the **NAM** or the **ERA-5** fields. The solutions below are for the **NAM** forcing
configured with **Nouter=1** and **Ninner=16**.

---

### Important CPP options:

They are activated in the build scripts.

  ``` d
   USEC                    ROMS application CPP option
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
                    Initial File:  ../Data/INI/eccofs3km_roms_ini_20190827.nc4
                                   ../Data/INI/eccofs6km_roms_ini.nc4 (generic)
                   Boundary File:  ../Data/BRY/eccofs3km_roms_bry.nc4
                                   ../Data/BRY/eccofs6km_roms_bry_empty.nc4
                Climatology File:  ../Data/CLM/eccofs3km_roms_mercator_clm.nc4
       Nudging Coefficients File:  ../Data/GRD/eccofs3km_roms_nudgcoef.nc4
              River Forcing File:  ../Data/GRD/eccofs3km_roms_rivers.nc4
              Tidal Forcing File:  ../Data/GRD/eccofs3km_roms_tides.nc4

  ERA5 Atmospheric Forcing Files:  ../Data/FRC/era5_0825_0909_2019.nc4

   NAM Atmospheric Forcing Files:  ../Data/FRC/lwrad_down_nam_0815_0915_2019.nc4
                                   ../Data/FRC/swrad_daily_nam_0815_0915_2019.nc4
                                   ../Data/FRC/Pair_nam_0815_0915_2019.nc4
                                   ../Data/FRC/Qair_nam_0815_0915_2019.nc4
                                   ../Data/FRC/Tair_nam_0815_0915_2019.nc4
                                   ../Data/FRC/rain_nam_0815_0915_2019.nc4
                                   ../Data/FRC/Uwind_nam_0815_0915_2019.nc4
                                   ../Data/FRC/Vwind_nam_0815_0915_2019.nc4

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

    Initial Conditions Norm File:  ../Data/NRM/eccofs3km_roms_nrm_i.nc4
                                   ../Data/NRM/eccofs6km_roms_nrm_i.nc4
   Boundary Conditions Norm File:  ../Data/NRM/eccofs3km_roms_nrm_b.nc4
                                   ../Data/NRM/eccofs6km_roms_nrm_b.nc4
       Surface Forcing Norm File:  ../Data/NRM/eccofs3km_roms_nrm_f.nc4
                                   ../Data/NRM/eccofs6km_roms_nrm_f.nc4
               Observations File:  ../Data/OBS/eccofs3km_roms_obs_20190827.nc4
                                   ../Data/OBS/eccofs3km_roms_obs_20190830.nc4
                                   ../Data/OBS/eccofs6km_roms_obs_20190827.nc4
                                   ../Data/OBS/eccofs6km_roms_obs_20190830.nc4
  ```

### Configuration and input scripts:

  ``` d
  build_split.csh               ROMS GNU Make compiling and linking CSH script
  build_split.sh                ROMS GNU Make compiling and linking BASH script
  plot_inc.m                    Matlab plotting script for 4D-Var increments and control vectors 
  roms_da_usec_era5.tmpl        ROMS data assimilation standard input template, ERA-5 forcing
  roms_nl_usec_era5.tmpl        ROMS nonlinear model standard input template, ERA-5 forcing
  roms_da_usec_nam.tmpl         ROMS data assimilation standard input template, NAM forcing
  roms_nl_usec_nam.tmpl         ROMS nonlinear model standard input template, NAM forcing
  s4dvar.in                     RBL4D-Var data assimilation template
  submit_mixres_rbl4dvar.sh     Job submission bash script
  usec.h                        ROMS header file, USEC application
  ```
The **submit_mixres_rbl4dvar.sh** script is complex.  Please study it carefully. You can modify
for your appropriate computer environment and **RBL4D-Var** running parameters. There is a
user-customizable section above the following heading:

  ``` sh
  #######################################################################
  ## Main body of script starts here. It is very unlikely that the USER
  ## needs to modify it.
  #######################################################################
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

To submit the job on 12 CPUs via SLURM or not, use:
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
 ROMS Split RBL4D-Var Data Assimilation: USEC
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

                    ROMS Root: /home/arango/ocean/repository/git/roms
            ROMS Executable A: romsM_nl  (background, analysis)
            ROMS Executable B: romsM_da  (increment, post_error)

      RBL4D-Var Starting Date: 2019-08-27  datenum = 737664
   First RBL4D-Var Cycle Date: 2019-08-27  datenum = 737664
    Last RBL4D-Var Cycle Date: 2019-08-30  datenum = 737667
          ROMS Reference Date: 2006-01-01  datenum = 732678
       RBL4D-Var Cycle Window: 3 days
      Number of parallel PETs: 12  (3x4)
   Current Starting Directory: /home/arango/ROMS/Projects/USEC/RBL4DVAR_mixres
         ROMS Application CPP: USEC
      Descriptor in filenames: eccofs3km  (outer loops grid)
      Descriptor in filenames: eccofs6km  (inner loops grid)

----------------------------------------------------------------------------------------------------

       RBL4D-Var Cycle Date: 2019-08-27 00  DayOfYear = 239  Cycle = 1
         Data sub-directory: ../../Data
          Run sub-directory: 2019.08.27
      Number of outer loops: 1
      Number of inner loops: 16
    NLM trajectory  writing: 30, 15 timesteps
    NLM quicksave   writing: 30, 15 timesteps
    NLM decimation  writing: 30, 15 timesteps
    TLM trajectory  writing: 30, 15 timesteps
    ADM trajectory  writing: 2160, 1080 timesteps
    SFF adjustment  writing: 30, 15 timesteps
    OBC adjustment  writing: 30, 15 timesteps
  NLM multi-file trajectory: 0, 0 timesteps
                ROMS DSTART: 4986.0d0
     Outer Loops Resolution: 3 km
     Inner Loops Resolution: 6 km
           I/O Files Prefix: eccofs3km  (outer loops grid)
           I/O Files Prefix: eccofs6km  (inner loops grid)
           I/O Files Suffix: 20190827
              ReferenceTime: 2006 01 01 00 00 00
        RBL4D-Var StartTime: 2019 08 27 00 00 00
        RBL4D-Var  StopTime: 2019 08 30 00 00 00
   ROMS outer loops Grid IC: ../../Data/INI/eccofs3km_roms_ini_20190827.nc4
   ROMS inner loops Grid IC: ../../Data/INI/eccofs6km_roms_ini_20190827.nc4
   NL Standard Input Script: roms_nl_usec_nam_20190827.in   (outer loops grid)
   DA Standard Input Script: roms_da_usec_nam_20190827.in   (inner loops grid)
  NL RBL4D-Var Input Script: rbl4dvar_nl.in  (outer loops grid)
  DA RBL4D-Var Input Script: rbl4dvar_da.in  (inner loops grid)

Cycle 1, Creating run sub-directory: 2019.08.27

Changing to directory: /home/arango/ROMS/Projects/USEC/RBL4DVAR_mixres/2019.08.27

   Creating NL ROMS Standard Input Script: roms_nl_usec_nam_20190827.in
   Creating DA ROMS Standard Input Script: roms_da_usec_nam_20190827.in
   Copying NLM IC file ../../Data/INI/eccofs3km_roms_ini_20190827.nc4  as  eccofs3km_roms_ini_20190827.nc4
   Copying NLM IC file ../../Data/INI/eccofs6km_roms_ini.nc4  as  eccofs6km_roms_ini_20190827.nc4
   Copying OBS    file ../../Data/OBS/eccofs3km_roms_obs_20190827.nc4  as  eccofs3km_roms_obs_20190827.nc4
   Copying OBS    file ../../Data/OBS/eccofs6km_roms_obs_20190827.nc4  as  eccofs6km_roms_obs_20190827.nc4

Running 4D-Var System:  Cycle = 1   Outer = 0   Phase = background

   Creating 4D-Var Input Script from Template: rbl4dvar_nl.in   Outer = 0  Phase = background
     (Resolution = 3 km, Fprefix = eccofs3km, Fsuffix = 20190827)

   mpirun -np 12 romsM_nl roms_nl_usec_nam_20190827.in

   Renaming NLM trajectory eccofs6km_roms_fwd_20190827.nc  to  eccofs6km_roms_fwd_20190827_outer0.nc

Running 4D-Var System:  Cycle = 1   Outer = 1   Phase = increment

   Creating 4D-Var Input Script from Template: rbl4dvar_da.in   Outer = 1  Phase = increment
     (Resolution = 6 km, Fprefix = eccofs6km, Fsuffix = 20190827)

   mpirun -np 12 romsM_da roms_da_usec_nam_20190827.in

Running 4D-Var System:  Cycle = 1   Outer = 1   Phase = analysis

   Creating 4D-Var Input Script from Template: rbl4dvar_nl.in   Outer = 1  Phase = analysis
     (Resolution = 3 km, Fprefix = eccofs3km, Fsuffix = 20190827)

   mpirun -np 12 romsM_nl roms_nl_usec_nam_20190827.in

   Renaming NLM trajectory eccofs6km_roms_fwd_20190827.nc  to  eccofs6km_roms_fwd_20190827_outer1.nc

Finished 4D-Var outer loops iterations

Finished RBL4D-Var Cycle 1,  Elapsed time = 02:38:49

Changing to directory: /home/arango/ROMS/Projects/USEC/RBL4DVAR_mixres

----------------------------------------------------------------------------------------------------

       RBL4D-Var Cycle Date: 2019-08-30 00  DayOfYear = 242  Cycle = 2
         Data sub-directory: ../../Data
          Run sub-directory: 2019.08.30
      Number of outer loops: 1
      Number of inner loops: 16
    NLM trajectory  writing: 30, 15 timesteps
    NLM quicksave   writing: 30, 15 timesteps
    NLM decimation  writing: 30, 15 timesteps
    TLM trajectory  writing: 30, 15 timesteps
    ADM trajectory  writing: 2160, 1080 timesteps
    SFF adjustment  writing: 30, 15 timesteps
    OBC adjustment  writing: 30, 15 timesteps
  NLM multi-file trajectory: 0, 0 timesteps
                ROMS DSTART: 4989.0d0
     Outer Loops Resolution: 3 km
     Inner Loops Resolution: 6 km
           I/O Files Prefix: eccofs3km  (outer loops grid)
           I/O Files Prefix: eccofs6km  (inner loops grid)
           I/O Files Suffix: 20190830
              ReferenceTime: 2006 01 01 00 00 00
        RBL4D-Var StartTime: 2019 08 30 00 00 00
        RBL4D-Var  StopTime: 2019 09 02 00 00 00
   ROMS outer loops Grid IC: ../2019.08.27/eccofs3km_roms_dai_20190827.nc
   ROMS inner loops Grid IC: ../2019.08.27/eccofs6km_roms_ini_20190830.nc
   NL Standard Input Script: roms_nl_usec_nam_20190830.in   (outer loops grid)
   DA Standard Input Script: roms_da_usec_nam_20190830.in   (inner loops grid)
  NL RBL4D-Var Input Script: rbl4dvar_nl.in  (outer loops grid)
  DA RBL4D-Var Input Script: rbl4dvar_da.in  (inner loops grid)

Cycle 2, Creating run sub-directory: 2019.08.30

Changing to directory: /home/arango/ROMS/Projects/USEC/RBL4DVAR_mixres/2019.08.30

   Creating NL ROMS Standard Input Script: roms_nl_usec_nam_20190830.in
   Creating DA ROMS Standard Input Script: roms_da_usec_nam_20190830.in
   Copying NLM IC file ../2019.08.27/eccofs3km_roms_dai_20190827.nc  as  eccofs3km_roms_dai_20190827.nc
   Copying NLM IC file ../../Data/INI/eccofs6km_roms_ini.nc4  as  eccofs6km_roms_ini_20190830.nc4
   Copying OBS    file ../../Data/OBS/eccofs3km_roms_obs_20190830.nc4  as  eccofs3km_roms_obs_20190830.nc4
   Copying OBS    file ../../Data/OBS/eccofs6km_roms_obs_20190830.nc4  as  eccofs6km_roms_obs_20190830.nc4

Running 4D-Var System:  Cycle = 2   Outer = 0   Phase = background

   Creating 4D-Var Input Script from Template: rbl4dvar_nl.in   Outer = 0  Phase = background
     (Resolution = 3 km, Fprefix = eccofs3km, Fsuffix = 20190830)

   mpirun -np 12 romsM_nl roms_nl_usec_nam_20190830.in

   Renaming NLM trajectory eccofs6km_roms_fwd_20190830.nc  to  eccofs6km_roms_fwd_20190830_outer0.nc

Running 4D-Var System:  Cycle = 2   Outer = 1   Phase = increment

   Creating 4D-Var Input Script from Template: rbl4dvar_da.in   Outer = 1  Phase = increment
     (Resolution = 6 km, Fprefix = eccofs6km, Fsuffix = 20190830)

   mpirun -np 12 romsM_da roms_da_usec_nam_20190830.in

Running 4D-Var System:  Cycle = 2   Outer = 1   Phase = analysis

   Creating 4D-Var Input Script from Template: rbl4dvar_nl.in   Outer = 1  Phase = analysis
     (Resolution = 3 km, Fprefix = eccofs3km, Fsuffix = 20190830)

   mpirun -np 12 romsM_nl roms_nl_usec_nam_20190830.in

   Renaming NLM trajectory eccofs6km_roms_fwd_20190830.nc  to  eccofs6km_roms_fwd_20190830_outer1.nc

Finished 4D-Var outer loops iterations

Finished RBL4D-Var Cycle 2,  Elapsed time = 02:40:10

Changing to directory: /home/arango/ROMS/Projects/USEC/RBL4DVAR_mixres

Finished computations, Total time = 05:18:59
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
   eccofs3km_roms_dai_20190827.nc                   Next DA cycle 3km initial state
   eccofs3km_roms_fwd_20190827_outer0.nc            Outer loop 0, 3km NLM trajectory
   eccofs3km_roms_fwd_20190827_outer1.nc            Outer loop 1, 3km NLM trajectory
   eccofs3km_roms_ini_20190827.nc                   NLM 3km initial conditions
   eccofs3km_roms_itl_20190827.nc                   TLM 3km increments
   eccofs3km_roms_mod_20190827.nc                   RBL4D-Var model/observations control vectors
   eccofs3km_roms_obs_20190827.nc                   3km observation vectors
   eccofs3km_roms_qck_20190827_outer0.nc            Outer loop 0, NLM QuickSave 3km history
   eccofs3km_roms_qck_20190827_outer1.nc            Outer loop 1, NLM QuickSave 3km history
   eccofs3km_roms_rst_20190827.nc                   3km NLM restart

   eccofs6km_roms_adj_20190827.nc                   ADM 6km gradients
   eccofs6km_roms_fwd_20190827_outer0.nc            Outer loop 0, NLM 6km decimated trajectory
   eccofs6km_roms_fwd_20190827_outer1.nc            Outer loop 1, NLM 6km decimated trajectory
   eccofs6km_roms_ini_20190827.nc                   Inner loops,  NLM 6km initial conditions and prior 
   eccofs6km_roms_itl_20190827.nc                   TLM 6km increments
   eccofs6km_roms_obs_20190827.nc                   6km observation vectors
   eccofs6km_roms_tlf_20190827.nc                   Adjoint impulse forcing, TLM forcing
  ```
