C $Header: /u/gcmpack/MITgcm_contrib/gael/verification/global_oce_llc90/code/PROFILES_SIZE.h_itXX,v 1.2 2015/08/08 18:37:49 gforget Exp $
C $Name:  $

C============================================================
C NOBSGLOB            :: maximum number of profiles per file and tile
C NFILESPROFMAX       :: maximum number of files
C NVARMAX             :: maximum number of variables 
C NLEVELMAX           :: maximum number of vertical levels 
C NUM_INTERP_POINTS   :: number of points used in horizontal averaging
C============================================================

      INTEGER  NOBSGLOB
      PARAMETER ( NOBSGLOB = 200000  )
      INTEGER NFILESPROFMAX
C      PARAMETER ( NFILESPROFMAX=20 )
      PARAMETER ( NFILESPROFMAX=40 )
      INTEGER NVARMAX
      PARAMETER ( NVARMAX=6 )
      INTEGER NLEVELMAX
      PARAMETER ( NLEVELMAX=110 )
      INTEGER NUM_INTERP_POINTS
      PARAMETER (NUM_INTERP_POINTS = 4)
#ifdef ALLOW_PROFILES_SAMPLESPLIT_COST
      INTEGER NLEVELCOMBMAX
      PARAMETER ( NLEVELCOMBMAX=105 )
      INTEGER NAVGBINMAX
      PARAMETER ( NAVGBINMAX=10245 )
#endif
