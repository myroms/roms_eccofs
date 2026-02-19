/*
** git $Id
*******************************************************************************
** Copyright (c) 2002-2026 The ROMS Group                                    **
**   Licensed under a MIT/X style license                                    **
**   See License_ROMS.md                                                     **
*******************************************************************************
**
** East Coast Community Ocean Forecast System: 3 km resolution
**
** Application flag:   ECCOFS3
** Input script:       roms_eccofs.in
**
*/

/* Basic physics options */

#define UV_ADV
#define UV_COR
#define UV_VIS2
#define MIX_S_UV
#define TS_DIF2
#define MIX_GEO_TS
#define SOLVE3D
#define SALINITY
#define NONLIN_EOS

#define ATM_PRESS
#define PRESS_COMPENSATE

/* Basic numerics options */

#define UV_U3HADVECTION
#define UV_C4VADVECTION
#define DJ_GRADPS
#define CURVGRID
#define MASKING
#define WTYPE_GRID

/* Surface and bottom boundary conditions */

#define UV_QDRAG
#define GLS_MIXING

#define CRAIG_BANNER
#define CHARNOK
#define LIMIT_STFLX_COOLING

#define SOLAR_SOURCE
#define ANA_BSFLUX
#define ANA_BTFLUX

/* Vertical subgridscale turbulence closure */

#ifdef MY25_MIXING
# define N2S2_HORAVG
# define KANTHA_CLAYSON
#endif
#ifdef GLS_MIXING
# define KANTHA_CLAYSON
# undef  CANUTO_A
# define N2S2_HORAVG
#endif

/* Open boundary condition settings */

#define SSH_TIDES
#define UV_TIDES
#define TIDE_GENERATING_FORCES

#ifdef SSH_TIDES
# undef RAMP_TIDES
# define ADD_FSOBC
#endif
#ifdef UV_TIDES
# define ADD_M2OBC
#endif

/*
**  Common options to all 4DVAR algorithms.
*/

#if defined ARRAY_MODES              || \
    defined CLIPPING                 || \
    defined I4DVAR                   || \
    defined I4DVAR_ANA_SENSITIVITY   || \
    defined RBL4DVAR                 || \
    defined RBL4DVAR_ANA_SENSITIVITY || \
    defined RBL4DVAR_FCT_SENSITIVITY || \
    defined R4DVAR                   || \
    defined R4DVAR_ANA_SENSITIVITY   || \
    defined SPLIT_I4DVAR             || \
    defined SPLIT_RBL4DVAR           || \
    defined SPLIT_R4DVAR
# define PRIOR_BULK_FLUXES
# define FORWARD_FLUXES
# define VCONVOLUTION
# define IMPLICIT_VCONV
# ifdef BALANCE_OPERATOR
#  define ZETA_ELLIPTIC
# endif
# define FORWARD_WRITE
# define FORWARD_READ
# define FORWARD_MIXING
#endif

/*
**  I/O files.
*/

#define NO_LBC_ATT
#undef  STATIONS

