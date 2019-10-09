C $Header: /u/gcmpack/MITgcm_contrib/atnguyen/llc_270/aste_270x450x180/code_ad/tamc.h,v 1.2 2014/04/20 01:05:12 atn Exp $
C $Name:  $

#include "PACKAGES_CONFIG.h"

c     ==================================================================
c     HEADER TAMC
c     ==================================================================
c
c     o Header for the use of the Tangent Linear and Adjoint Model
c       Compiler (TAMC).
c
c     started: Christian Eckert eckert@mit.edu  04-Feb-1999
c
c     changed: Patrick Heimbach heimbach@mit.edu 06-Jun-2000
c              - New parameter nlevchk_0 for dimensionalising
c                common blocks in the undef ALLOW_TAMC_CHECKPOINTING case
c              - nhreads_chkpt was declared at the wrong place
c
c
c     ==================================================================
c     HEADER TAMC
c     ==================================================================


c     TAMC checkpointing parameters:
c     ==============================
c
c     The checkpointing parameters have to be consistent with other model
c     parameters and variables. This has to be checked before the model is
c     run.
c
c     nyears_chkpt   - Number of calendar years affected by the assimilation
c                      experiment; nyears_chkpt has to be at least equal to
c                      the result of cal_IntYears(mythid).
c     nmonths_chkpt  - Number of months per year; nmonth_chkpt has to be at
c                      least equal to nmonthyear.
c     ndays_chkpt    - Number of days per month; nday_chkpt has to be at least
c                      equal to nmaxdaymonth.
c     nsteps_chkpt   - Number of steps per day; nsteps_chkpt has to be at
c                      least equal to cal_nStepDay(mythid)
c     ncheck_chkpt   - Number of innermost checkpoints.
c
c     ngeom_chkpt    - Geometry factor.
c     nthreads_chkpt - Number of threads to be used; nth_chkpt .eq. nTx*nTy

      integer nyears_chkpt
      integer nmonths_chkpt
      integer ndays_chkpt
      integer ngeom_chkpt
      integer ncheck_chkpt
      integer nthreads_chkpt

      parameter (nyears_chkpt   =          1 )
      parameter (nmonths_chkpt  =         12 )
      parameter (ndays_chkpt    =         31 )
      parameter (ngeom_chkpt    = nr*nsx*nsy )
      parameter (ncheck_chkpt   =          6 )
      parameter ( nthreads_chkpt = 1 )

#ifdef ALLOW_TAMC_CHECKPOINTING

      integer    nchklev_1
      integer    nchklev_2
      integer    nchklev_3
C-- calculate numbers below as follows:
c-- take ivy-bridge as example, 3.GB/node -> nchklev_1=24
c-- then take nchklev_[2,3]=sqrt(nTimesteps / 24), round UP
C==== 5 years ------------------------------------------
c change to this for 5-yr run: 2.4GB/node for neh
c      parameter( nchklev_1      =  20 )
c      parameter( nchklev_2      =  84 )
c      parameter( nchklev_3      =  84 )
c change to this for 5-yr run: 2.7GB/node for ivybridge
c      parameter( nchklev_1      =  22 )
c      parameter( nchklev_2      =  80 )
c      parameter( nchklev_3      =  80 )
c change to this for 5-yr run: 3.GB/node for ivybridge
C      parameter( nchklev_1      =  24 )
C      parameter( nchklev_2      =  77 )
C      parameter( nchklev_3      =  77 )
C==== 20 years ------------------------------------------
c change to this for 20-yr run: 2.4GB/node for neh
c      parameter( nchklev_1      =  20 )
c      parameter( nchklev_2      =  163 )
c      parameter( nchklev_3      =  163 )
c change to this for 20-yr run: 2.7GB/node for ivybridge
c      parameter( nchklev_1      =  22 )
c      parameter( nchklev_2      =  155 )
c      parameter( nchklev_3      =  155 )
c change to this for 20-yr run: 3.GB/node for ivybridge
c      parameter( nchklev_1      =  24 )
c      parameter( nchklev_2      =  148 )
c      parameter( nchklev_3      =  148 )
c change to this for 15-yr run: 3.GB/node for ivybridge
c      parameter( nchklev_1      =  24 )
c      parameter( nchklev_2      =  129 )
c      parameter( nchklev_3      =  129 )
c change to this for 25-yr run: 3.GB/node for ivybridge
      parameter( nchklev_1      =  24 )
      parameter( nchklev_2      =  200 )
      parameter( nchklev_3      =  200 )
c Haswell 20-yr run: limit 5.GB/core, 3.2percent or 4.0g per core
c      parameter( nchklev_1      =  52 )
c      parameter( nchklev_2      =  88 )
c      parameter( nchklev_3      =  88 )
c Haswell 1992/01/01 to 2014/12/31 run: limit 5.GB/core, 3.2percent or 4.0g per core
c      parameter( nchklev_1      =  52 )
c      parameter( nchklev_2      =  110 )
c      parameter( nchklev_3      =  110 )
c Haswell 2002/01/01 to 2017/12/31 run: limit 5.GB/core, 3.2percent or 4.0g per core
c      parameter( nchklev_1      =  52 )
c      parameter( nchklev_2      =  130 )
c      parameter( nchklev_3      =  130 )
c  change to these values for very short test runs:
cph      parameter( nchklev_1      =  24 )
cph      parameter( nchklev_2      =  4 )
cph      parameter( nchklev_3      =  3 )
cC Stampede2 SKX 1992/01/01 to 2014/12/31 run: limit 4.GB/core, ?.0g per core, 800GB tapes
c      parameter( nchklev_1      =  6 )
c      parameter( nchklev_2      =  326 )
c      parameter( nchklev_3      =  326 )
C Stampede2 SKX 1992/01/01 to 2014/12/31 run: limit 4.GB/core, ?.0g per core, 422GB tapes, 74GB free
C      parameter( nchklev_1      =  24 )
C      parameter( nchklev_2      =  166 )
C      parameter( nchklev_3      =  166 )
C Stampede2 SKX 1992/01/01 to 2014/12/31 run: limit 4.GB/core, ?.0g per core, 351G tapes
c      parameter( nchklev_1      =  32 )
c      parameter( nchklev_2      =  144 )
c      parameter( nchklev_3      =  144 )
C Stampede2 SKX 1992/01/01 to 2014/12/31 run: limit 4.GB/core, ?.0g per core, 321G tapes
c      parameter( nchklev_1      =  40 )
c      parameter( nchklev_2      =  129 )
c      parameter( nchklev_3      =  129 )
C Stampede2 SKX 1992/01/01 to 2014/12/31 run: limit 4.GB/core, ?.0g per core
c      parameter( nchklev_1      =  44 )
c      parameter( nchklev_2      =  123 )
c      parameter( nchklev_3      =  123 )

c--   Note always check for the correct sizes of the common blocks!

#else /* ALLOW_TAMC_CHECKPOINTING undefined */

      integer    nchklev_0
      parameter( nchklev_0      =  48 )
      integer    nchklev_1
      parameter( nchklev_1      =    5 )

#endif /* ALLOW_TAMC_CHECKPOINTING */

c     TAMC keys:
c     ==========
c
c     The keys are used for storing and reading data of the reference
c     trajectory.
c
c     The convention used here is:
c                                    ikey_<name>
c
c     which means that this key is used in routine <name> for reading
c     and writing data.

      common /tamc_keys_i/
     &                     ikey_dynamics,
     &                     ikey_yearly,
     &                     ikey_daily_1,
     &                     ikey_daily_2,
     &                     iloop_daily

      integer ikey_dynamics
      integer ikey_yearly
      integer ikey_daily_1
      integer ikey_daily_2
      integer iloop_daily

      INTEGER    isbyte
      PARAMETER( isbyte      = 4 )
      INTEGER    maximpl
      PARAMETER( maximpl     = 6 )
      INTEGER    maxpass
      PARAMETER( maxpass     = 6 )
      INTEGER    maxcube
      PARAMETER( maxcube     = 3 )

      INTEGER act0, act1, act2, act3, act4
      INTEGER max0, max1, max2, max3
      INTEGER iikey, kkey, passkey, igadkey,
     &        itdkey, idynkey, igmkey

c     ==================================================================
c     END OF HEADER TAMC
c     ==================================================================

