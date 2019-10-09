C $Header: /u/gcmpack/MITgcm_contrib/atnguyen/llc_270/aste_270x450x180/code_ad/SALT_PLUME_OPTIONS.h,v 1.3 2014/05/23 22:10:43 atn Exp $
C $Name:  $

C CPP options file for salt_plume package
C Use this file for selecting options within the salt_plume package

#ifndef SALT_PLUME_OPTIONS_H
#define SALT_PLUME_OPTIONS_H
#include "PACKAGES_CONFIG.h"
#include "CPP_OPTIONS.h"

#ifdef ALLOW_SALT_PLUME
C Place CPP define/undef flag here

C SALT_PLUME_IN_LEADS
C   Motivation: As ice concentration AREA -> 1, leads occur -> ice
C     production is no longer uniform in grid box -> assumptions
C     which motivate KPP no longer holds -> treat overturn more
C     realistic with this flag.
C   if defined: Activate pkg/salt_plume only when seaice AREA exceeds
C               a certain value representative of lead opening AND only
C               if seaice growth dh is from atmospheric cooling.
C   if undefined: Activate pkg/salt_plume whenever seaice forms.
C                 This is the default of pkg/salt_plume.
#define SALT_PLUME_IN_LEADS
C note that SALT_PLUME_SPLIT_BASIN does not close salt budget yet
#undef SALT_PLUME_SPLIT_BASIN
#undef SALT_PLUME_VOLUME
#endif /* ALLOW_SALT_PLUME */
#endif /* SALT_PLUME_OPTIONS_H */
